/*
 * AppQueues.c
 *
 *  Created on: 29/06/2017
 *      Author: Jefferson
 */

#include "Array.h"
#include "DataTLM.h"
#include "DataFrame.h"
#include "AppQueues.h"

// Tasks Delay
const TickType_t xMainDelay				= (10000 / portTICK_PERIOD_MS);
const TickType_t xCommunicationDelay	= (10 / portTICK_PERIOD_MS);
const TickType_t xDataDelay				= (200 / portTICK_PERIOD_MS);
const TickType_t xIHMDelay				= (50 / portTICK_PERIOD_MS);
const TickType_t xGPSDelay				= (50 / portTICK_PERIOD_MS);
const TickType_t xAccelDelay			= (100 / portTICK_PERIOD_MS);

// Handles Tasks
TaskHandle_t 	xHandleMainTask		= NULL, xHandleCommunicationTask	= NULL,
				xHandleDataTask		= NULL, xHandleIHMTask				= NULL,
				xHandleGPSTask		= NULL,	xHandleAccelTask			= NULL,
				xHandleCallBackTask	= NULL,xHandleRunTxTask				= NULL;

// Queues
QueueHandle_t	xQueueCom, xQueueLCD, xQueueAnswer, xQueueDataTLM;

/**
 * Cria todas as filas de menssagens
 *
 */
void initQueues(void){

	xQueueCom		= xQueueCreate( 3, sizeof( DataCom* ));
	xQueueAnswer	= xQueueCreate( 3, sizeof( ArrayPayLoad* ));
	xQueueDataTLM	= xQueueCreate( 5, sizeof( DataTLM* ));
}
//-----------------------------------------------------------------------------
