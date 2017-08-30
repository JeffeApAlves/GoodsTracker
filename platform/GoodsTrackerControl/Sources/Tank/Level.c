/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include <Telemetria.h>
#include "AppQueues.h"
#include "Level.h"

static uint16_t	ADValues[AD1_CHANNEL_COUNT];
static Tank		adInfo;

volatile	bool AD_finished = FALSE;

void runAnalog(void){

	AD_finished = FALSE;

	if(AD1_Measure(TRUE)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(&ADValues[0])==ERR_OK){

			adInfo.Level = ADValues[0];

		    if(xQueueSendToBack( xQueueTank ,  &adInfo, ( TickType_t ) 1 ) ){

		    	xTaskNotify( xHandleCallBackTask, BIT_UPDATE_AD , eSetBits );
		    }
		}
	}
}
//------------------------------------------------------------------------
