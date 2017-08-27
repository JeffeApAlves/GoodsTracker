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

typedef struct{

	float	Lat;
	float	Lng;
	int		Speed ;
	char	Time[11];
	char	Date[7];

} DataGPS;

typedef struct{

	int		Axis[3];
	int		G[3];

} DataAccelerometer;

typedef struct{

	int		Level;
	int		Lock;

} DataTank;

typedef struct{

	DataGPS				GPS;
	DataTank			Tank;
	DataAccelerometer	Accelerometer;

} DataTLM;


void updateDataLevel(void);
void updateDataAcce(void);
void updateDataGPS(void);
void updateTLM(uint32_t ulNotifiedValue);

#define clearDataTLM(t,f) memset((void*)f,0,sizeof(t));

extern DataTLM			dataTLM;

#endif /* SOURCES_DATATLM_H_ */
