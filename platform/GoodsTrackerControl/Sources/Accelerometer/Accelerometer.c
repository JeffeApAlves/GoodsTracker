/*
 * Accelerometer.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <Telemetria.h>
#include "AppQueues.h"
#include "MMA8451.h"
#include "Accelerometer.h"

Accelerometer	acceInfo;

void runAccelerometer(void) {

	MMA845x_getValues(&acceInfo);

    if(xQueueSendToBack( xQueueAcc ,  &acceInfo, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleCallBackTask , BIT_UPDATE_ACCE , eSetBits );
    }
}
//------------------------------------------------------------------------

void initAccelerometer(void){

	clearAccelerometer(&acceInfo);
	MMA845x_init();
}
//------------------------------------------------------------------------

void deInitAccelerometer(void){

	MMA845x_deInit();
}
//------------------------------------------------------------------------
