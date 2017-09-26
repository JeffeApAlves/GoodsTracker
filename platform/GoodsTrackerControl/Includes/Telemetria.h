/*
 * Data.h
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_TELEMETRIA_H_
#define SOURCES_TELEMETRIA_H_

#include "Tank.h"
#include "GPS.h"
#include "Accelerometer.h"

typedef struct{

	GPS			GPS;
	Tank			Tank;
	Accelerometer	Accelerometer;

} Telemetria;

#define clearTelemetria(f) memset((void*)f,0,sizeof(Telemetria));

#endif /* SOURCES_DATATLM_H_ */