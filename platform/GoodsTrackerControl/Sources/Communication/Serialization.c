#include <stdio.h>
#include <string.h>

#include "XF1.h"

#include "clock.h"
#include "Serialization.h"

void tlm2String(Telemetria* tlm,PayLoad* ans){

	if(tlm!=NULL && ans!= NULL){

		XF1_xsprintf(ans->Data,"%.8f%c%.8f%c%d%c%d%c%d%c%.3f%c%.3f%c%.2f%c%d%c%d%c%d%c%d",
				tlm->GPS.Lat, 				CHAR_SEPARATOR,
				tlm->GPS.Lng, 				CHAR_SEPARATOR,
				tlm->Accelerometer.x, 		CHAR_SEPARATOR,
				tlm->Accelerometer.y, 		CHAR_SEPARATOR,
				tlm->Accelerometer.z, 		CHAR_SEPARATOR,
				tlm->Accelerometer.x_g,		CHAR_SEPARATOR,
				tlm->Accelerometer.y_g,		CHAR_SEPARATOR,
				tlm->Accelerometer.z_g,		CHAR_SEPARATOR,
				tlm->GPS.Speed, 			CHAR_SEPARATOR,
				tlm->Tank.Level, 			CHAR_SEPARATOR,
				tlm->Tank.Lock, 			CHAR_SEPARATOR,
				strToTimeStamp(tlm->GPS.Date,tlm->GPS.Time_UTC));

		ans->Length = strlen(ans->Data);
	}
}
//------------------------------------------------------------------------

void header2String(Header* header,char* out){

	char resource[4];

	getResourceName(resource,header->resource);

	XF1_xsprintf(out,"%05d%c%05d%c%010d%c%s%c%s%c%03d%c",
			header->address, 		CHAR_SEPARATOR,
			header->dest, 			CHAR_SEPARATOR,
			header->time_stamp,		CHAR_SEPARATOR,
			header->operacao, 		CHAR_SEPARATOR,
			resource,				CHAR_SEPARATOR,
			header->lengthPayLoad,	CHAR_SEPARATOR
	);
}
//------------------------------------------------------------------------