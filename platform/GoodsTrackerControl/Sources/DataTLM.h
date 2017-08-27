/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATATLM_H_
#define SOURCES_DATATLM_H_

#include <stdint.h>
#include <string.h>

enum {AXIS_X=0,AXIS_Y=1,AXIS_Z=2};

//#define	TIMEOUT_TLM		10

typedef struct{

		float		Lat;
		float		Lng;
		int			Axis[3];
		int			G[3];
		int			Speed ;
		int			Level;
		int			Lock;
		char		Time[11];
		char		Date[7];
} DataTLM;


void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);
void updateTLM(uint32_t ulNotifiedValue);

#define clearDataTLM(f) memset((void*)f,0,sizeof(DataTLM));

extern DataTLM			dataTLM;

#endif /* SOURCES_DATATLM_H_ */
