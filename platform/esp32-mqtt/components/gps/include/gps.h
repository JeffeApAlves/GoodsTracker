/*
 * gps.h
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_GPS_H_
#define SOURCES_GPS_H_

#include <stdbool.h>

#include "rtos.h"
#include "NMEAFrame.h"
#include "utils.h"


#define NMEA_CHAR_START		'$'
#define NMEA_CHAR_END		'*'
#define NMEA_CHAR_SEPARATOR	','
#define NMEA_CHAR_CR		'\r'
#define NMEA_CHAR_LF		'\n'
#define NMEA_CHAR_STR_END	'\0'

/**
 *
 * Maquina de estado para recebimento do frame
 */
typedef enum {

	NMEA_INIT,
	NMEA_INIT_OK,
	NMEA_RX_START,
	NMEA_RX_FRAME,
	NMEA_RX_END,
	NMEA_RX_CHECKSUM,
	NMEA_RX_LF,
	NMEA_RX_CR,
	NMEA_FRAME_NOK,
	NMEA_FRAME_OK,
	NMEA_EXEC,
	NMEA_EXEC_ERROR,

} StatusNMEA;

/*
 * Estrutura de dados do frame NMEA
 *
 */

typedef struct{

	char	Identifier[NMEA_LEN_ID];
	char	Time_UTC[NMEA_LEN_TIME];
	char	Date[NMEA_LEN_DATE];
	float	Lat;
	float	Lng;

	char	LatDirection;
	char	LngDirection;
	char	Status;
	char	SelectionMode;
	char	Mode;
	int		PRNNumber[12];

	int		Speed;
	int		FixQuality;
	int		NumberOfSatelites;
	float	HDOP;
	float	PDOP;
	float	VDOP;
	float	Altitude;
	float	HGeoid;
	float	MagVariation;

} DataGPS;

static void NMEA_rxStart(void);
static void NMEA_receiveFrame(void);
static void NMEA_receiveCheckSum(void);
static void NMEA_rxCR(void);
static void NMEA_rxLF(void);
static void NMEA_verifyFrame(void);
static void NMEA_acceptRxFrame(void);
static void NMEA_errorRxFrame(void);

static void setGPSStatus(StatusNMEA sts);
static bool NMEA_decoderFrame(void);
static void decoderGGA(char* frame,DataGPS* data);
static void decoderRMC(char* frame,DataGPS* data);
static void decoderGSA(char* frame,DataGPS* data);

//API
bool getGPSData(char* ch);
bool putGPSData(char data);
void gps_random_data(void);
void gps_task(void);
bool gps_isAnyData();
void gps_init(void);
void gps_sendResultQueue(void);
bool gps_getdata(void);

#define clearGPS(f) memset((void*)f,0,sizeof(DataGPS));

// Handles das Queues
extern QueueHandle_t	xQueueGPS;

#endif /* SOURCES_GPS_H_ */
