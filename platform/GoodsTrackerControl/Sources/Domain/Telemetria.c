/*
 * DataTLM.c
 *
 *  Created on: 01/07/2017
 *      Author: Jefferson
 */
#include <string.h>
#include "Accelerometer.h"
#include "AppQueues.h"
#include "Telemetria.h"

DataTLM		dataTLM;

/**
 *
 * Atualiza a s informcoes de telemetria que estao na lista de menssagem xQueueDataTLM
 *
 */
void updateTLM(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_UPDATE_GPS){

		updateDataGPS();
	}

	if(ulNotifiedValue & BIT_UPDATE_ACCE){

		updateDataAcce();
	}

	if(ulNotifiedValue & BIT_UPDATE_AD){

		updateDataLevel();
	}
}
//-------------------------------------------------------------------------

void updateDataGPS(void) {

	if (xQueueGPS != 0) {

		if (xQueueReceive(xQueueGPS, &dataTLM.GPS, (TickType_t ) 1)) {

		}
	}
}
//-------------------------------------------------------------------------

void updateDataAcce(void) {

	if (xQueueAcc != 0) {

		if (xQueueReceive(xQueueAcc, &dataTLM.Accelerometer, (TickType_t ) 1)) {

		}
	}
}
//-------------------------------------------------------------------------

void updateDataLevel(void) {

	if (xQueueTank != 0) {

		if (xQueueReceive(xQueueTank, &dataTLM.Tank, (TickType_t ) 1)) {

		}
	}
}
//-------------------------------------------------------------------------
