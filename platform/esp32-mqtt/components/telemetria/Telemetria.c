/*
 * DataTLM.c
 *
 *  Created on: 01/07/2017
 *      Author: Jefferson
 */

#include "accelerometer.h"
#include "gps.h"
#include "tank.h"
#include "clock.h"
#include "consumer.h"
#include "Telemetria.h"

Telemetria	telemetria;

/**
 *
 * Atualiza a s informcoes de telemetria que estao na lista de menssagem xQueueDataTLM
 *
 */
void updateTLM(EventBits_t ulNotifiedValue){

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

		if (xQueueReceive(xQueueGPS, &telemetria.GPS, (TickType_t ) 1)) {

			if(telemetria.GPS.FixQuality>0){
				adjusteClock();
			}
		}
	}
}
//-------------------------------------------------------------------------

void updateDataAcce(void) {

	if (xQueueAcc != 0) {

		if (xQueueReceive(xQueueAcc, &telemetria.Accelerometer, (TickType_t ) 1)) {

		}
	}
}
//-------------------------------------------------------------------------

void updateDataLevel(void) {

	if (xQueueTank != 0) {

		if (xQueueReceive(xQueueTank, &telemetria.Tank, (TickType_t ) 1)) {

		}
	}
}
//-------------------------------------------------------------------------
