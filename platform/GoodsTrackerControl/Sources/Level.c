/*
 * Level.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "DataTLM.h"
#include "AppQueues.h"
#include "Level.h"

static uint16_t	ADValues[AD1_CHANNEL_COUNT];
static DataTank		adInfo;

DataTank*	pInfo = &adInfo;

volatile	bool AD_finished = FALSE;

void runAnalog(void){

	if(AD1_Measure(TRUE)==ERR_OK){

		while (!AD_finished) {}

		if(AD1_GetValue16(ADValues)==ERR_OK){

			pInfo->Level = ADValues[2];

		    if(xQueueSendToBack( xQueueTank , ( void * ) &pInfo, ( TickType_t ) 1 ) ){

		    	xTaskNotify( xHandleCallBackTask, BIT_UPDATE_AD , eSetBits );
		    }
		}
	}
}
//------------------------------------------------------------------------
