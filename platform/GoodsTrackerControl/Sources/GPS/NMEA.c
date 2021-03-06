/*
 * NMEA.c
 *
 *  Created on: Oct 19, 2017
 *      Author: Jefferson
 */

#include <uart_gps.h>
#include "gps.h"
#include "NMEA.h"

const char* GGA	=	"GGA";
const char* RMC	=	"RMC";
const char* GSA	=	"GSA";

#define		SOUTH	'S'
#define		WEST	'W'

static StatusNMEA	statusNMEA	= NMEA_INIT;
static Frame		frameNMEA;


static void NMEA_clear_all(void);

static inline void setGPSStatus(StatusNMEA sts) {

	statusNMEA = sts;
}
//------------------------------------------------------------------------

static void NMEA_acceptRxFrame(void){

	gps_publish();

    setGPSStatus(NMEA_INIT_OK);
}
//------------------------------------------------------------------------

static void NMEA_errorRxFrame(void){

	//TODO
	setGPSStatus(NMEA_INIT_OK);
}
//------------------------------------------------------------------------

static void NMEA_rxStart(void){

	char ch;

	if(getGPSData(&ch)){

		if(ch==NMEA_CHAR_START){

			clearFrame(&frameNMEA);

			setGPSStatus(NMEA_RX_START);
		}
	}
}
//------------------------------------------------------------------------

static void NMEA_receiveFrame(void){

	char ch;

	if(getGPSData(&ch)) {

		if(ch==NMEA_CHAR_START || frameNMEA.Length>=LEN_FRAME) {

			NMEA_errorRxFrame();
		}
		else
		  if(ch==NMEA_CHAR_END) {

			 if(frameNMEA.Length>=NMEA_SIZE_MIN_FRAME) {

				 setGPSStatus(NMEA_RX_END);
			 }
			 else {

				 NMEA_errorRxFrame();
			 }
		}
		else {

			putDataArray(&frameNMEA,ch);
			setGPSStatus(NMEA_RX_FRAME);
		}
	}
}
//------------------------------------------------------------------------

static void NMEA_receiveCheckSum(){

	char ch;

	if(getGPSData(&ch)){

		frameNMEA.checksum[(frameNMEA.CountCheckSum++)%NMEA_LEN_CHECKSUM] = ch;

		if(frameNMEA.CountCheckSum >= NMEA_LEN_CHECKSUM){

			frameNMEA.checksum[NMEA_LEN_CHECKSUM] = 0;

			setGPSStatus(NMEA_RX_CHECKSUM);
		}
	}
}
//------------------------------------------------------------------------

static void NMEA_rxCR(void){

	char ch;

	if(getGPSData(&ch)){

		if(ch==NMEA_CHAR_CR){

			setGPSStatus(NMEA_RX_CR);

		}else{

			NMEA_errorRxFrame();
		}
	}
}
//------------------------------------------------------------------------

static void NMEA_rxLF(void){

	char ch;

	if(getGPSData(&ch)){

		if(ch==NMEA_CHAR_LF){

			setGPSStatus(NMEA_RX_LF);

		}else {

			NMEA_errorRxFrame();
		}
	}
}
//------------------------------------------------------------------------

/*
       1         2       3 4        5 6 7  8    9  10 |  12 13 14   15
	   |         |       | |        | | |  |    |  |  |  |  |   |   |
$--GGA,hhmmss.ss,llll.ll,a,yyyyy.yy,a,x,xx,x.x,x.x,M,x.x,M,x.x,xxxx*hh

1) Time (UTC)
2) Latitude
3) N or S (North or South)
4) Longitude
5) E or W (East or West)
6) GPS Quality Indicator,
  0 - fix not available,
  1 - GPS fix,
  2 - Differential GPS fix
7) Number of satellites in view, 00 - 12
8) Horizontal Dilution of precision
9) Antenna Altitude above/below mean-sea-level (geoid)
10) Units of antenna altitude, meters
11) Geoidal separation, the difference between the WGS-84 earth
   ellipsoid and mean-sea-level (geoid), "-" means mean-sea-level below ellipsoid
12) Units of geoidal separation, meters
13) Age of differential GPS data, time in seconds since last SC104
type 1 or 9 update, null field when DGPS is not used
14) Differential reference station ID, 0000-1023
15) Checksum
*/
static void decoderGGA(char* frame,GPS* data){

	AsString(data->Identifier,			frame,0, NMEA_CHAR_SEPARATOR);
	AsString(data->Time_UTC,			frame,1, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->Lat,					frame,2, NMEA_CHAR_SEPARATOR);
	AsChar(&data->LatDirection,			frame,3, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->Lng,					frame,4, NMEA_CHAR_SEPARATOR);
	AsChar(&data->LngDirection,			frame,5, NMEA_CHAR_SEPARATOR);
	AsInteger(&data->FixQuality,		frame,6, NMEA_CHAR_SEPARATOR);
	AsInteger(&data->NumberOfSatelites,	frame,7, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->HDOP,				frame,8, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->Altitude,			frame,9, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->HGeoid,				frame,11,NMEA_CHAR_SEPARATOR);

	if(data->LatDirection==SOUTH){

		data->Lat *=-1;
	}

	if(data->LngDirection == WEST){

		data->Lng *=-1;
	}
}
//------------------------------------------------------------------------

/*
       1         2 3       4 5        6 7   8   9    10  11 12
       |         | |       | |        | |   |   |    |   |  |
$--RMC,hhmmss.ss,A,llll.ll,a,yyyyy.yy,a,x.x,x.x,xxxx,x.x,a*hh
1) Time (UTC)
2) Status, V = Navigation receiver warning
3) Latitude
4) N or S
5) Longitude
6) E or W
7) Speed over ground, knots
8) Track made good, degrees true
9) Date, ddmmyy
10) Magnetic Variation, degrees
11) E or W
12) Checksum
*/
static void decoderRMC(char* frame,GPS* data){

	AsString(data->Identifier,		frame,0, NMEA_CHAR_SEPARATOR);
	AsString(data->Time_UTC,		frame,1, NMEA_CHAR_SEPARATOR);
	AsChar(&data->Status,			frame,2, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->Lat,				frame,3, NMEA_CHAR_SEPARATOR);
	AsChar(&data->LatDirection,		frame,4, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->Lng,				frame,5, NMEA_CHAR_SEPARATOR);
	AsChar(&data->LngDirection,		frame,6, NMEA_CHAR_SEPARATOR);
	AsInteger(&data->Speed,			frame,7, NMEA_CHAR_SEPARATOR);
	AsString(data->Date,			frame,9, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->MagVariation,	frame,10,NMEA_CHAR_SEPARATOR);

	if(data->LatDirection==SOUTH){

		data->Lat *=-1;
	}

	if(data->LngDirection == WEST){

		data->Lng *=-1;
	}
}
//------------------------------------------------------------------------

/*
GSA GPS DOP and active satellites
       1 2 3                         14 15  16  17 18
       | | |                         |  |   |   |  |
$--GSA,a,a,x,x,x,x,x,x,x,x,x,x,x,x,x,x,x.x,x.x,x.x*hh
 1) Selection mode
 2) Mode
 3) ID of 1st satellite used for fix
 4) ID of 2nd satellite used for fix
 ...
14) ID of 12th satellite used for fix
15) PDOP in meters
16) HDOP in meters
17) VDOP in meters
18) Checksum
*/
static void decoderGSA(char* frame,GPS* data){

	AsString(data->Identifier,		frame,0, NMEA_CHAR_SEPARATOR);
	AsChar(&data->SelectionMode,	frame,1, NMEA_CHAR_SEPARATOR);
	AsChar(&data->Mode,				frame,2, NMEA_CHAR_SEPARATOR);

	char i;
	for(i=0;i<12;i++){

		AsInteger(data->PRNNumber+i,frame,i+3, NMEA_CHAR_SEPARATOR);
	}

	AsFloat(&data->PDOP,			frame,15, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->HDOP,			frame,16, NMEA_CHAR_SEPARATOR);
	AsFloat(&data->VDOP,			frame,17, NMEA_CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

static bool NMEA_decoderFrame(GPS* gps){

	bool ret = false;
	uint16_t count = getNumField(frameNMEA.Data,NMEA_CHAR_SEPARATOR);

	if(count >= 5){

		uint16_t checksum_rx,checksum_calc;

		// Checksum
		checksum_calc = calcChecksum(frameNMEA.Data, frameNMEA.Length);
		// garante que estao diferentes antes de ler.
		checksum_rx = ~checksum_calc;
		// Converte de string para numero o checksum recebido
		checksum_rx = strtol(frameNMEA.checksum, NULL, 16);

		if(checksum_rx==checksum_calc) {

			ret = true;

			char id[NMEA_LEN_ID];

			AsString(id,frameNMEA.Data,0,NMEA_CHAR_SEPARATOR);

			if(strcmp(GGA,id+2)==0){		decoderGGA(frameNMEA.Data,gps);

			}else if(strcmp(RMC,id+2)==0){	decoderRMC(frameNMEA.Data,gps);

			}else if(strcmp(GSA,id+2)==0){	decoderGSA(frameNMEA.Data,gps);

			}else{

				ret = false;
			}
		}
	}

	return ret;
}
//------------------------------------------------------------------------

static void NMEA_verifyFrame(void){

	if(NMEA_decoderFrame(&gps)){

		NMEA_acceptRxFrame();

	}else{

		NMEA_errorRxFrame();
	}
}
//------------------------------------------------------------------------

void receiveNMEA(void) {

	while(isAnyGPSData()){

		switch(statusNMEA){
			default:
			case NMEA_INIT:			NMEA_clear_all();		break;
			case NMEA_INIT_OK:		NMEA_rxStart();			break;
			case NMEA_RX_START:		NMEA_receiveFrame();	break;
			case NMEA_RX_FRAME:		NMEA_receiveFrame();	break;
			case NMEA_RX_END:		NMEA_receiveCheckSum();	break;
			case NMEA_RX_CHECKSUM:	NMEA_rxCR();			break;
			case NMEA_RX_CR:		NMEA_rxLF();			break;
			case NMEA_RX_LF:		NMEA_verifyFrame();		break;
		}
	}
}
//------------------------------------------------------------------------

static void NMEA_clear_all(void){

	clearFrame(&frameNMEA);
	setGPSStatus(NMEA_INIT_OK);
}
//------------------------------------------------------------------------
