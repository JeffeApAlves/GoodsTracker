/*
 * consumer.c
 *
 *  Created on: Sep 24, 2017
 *      Author: Jefferson
 */

#include "Tank.h"
#include "Clock.h"
#include "Accelerometer.h"
#include "gps.h"
#include "application.h"
#include "ihm.h"
#include "consumer.h"

void updateDataGPS(Telemetria* tlm) {

	if (xQueueReceive(xQueueGPS, &tlm->GPS, (TickType_t ) 1)) {

		// Configura o relógio através do GPS somente após indicar dados OK
		if(tlm->GPS.FixQuality>0){
			adjusteClock(tlm->GPS.Date,tlm->GPS.Time_UTC);
		}
	}
}
//-------------------------------------------------------------------------

void updateDataAcce(Telemetria* tlm) {

	if (xQueueReceive(xQueueAcc, &tlm->Accelerometer, (TickType_t ) 1)) {

	}
}
//-------------------------------------------------------------------------

void updateDataLevel(Telemetria* tlm) {

	if (xQueueReceive(xQueueTank, &tlm->Tank, (TickType_t ) 1)) {

	}
}
//-------------------------------------------------------------------------

/**
 *
 * Atualiza as informções de telemetria que estão na lista de menssagens
 *
 */
void updateTLM(Telemetria* tlm,uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_UPDATE_GPS){

		updateDataGPS(tlm);
	}

	if(ulNotifiedValue & BIT_UPDATE_ACCE){

		updateDataAcce(tlm);

		//Gera evento de atualização do LCD
		ihm_event_notify(BIT_UPDATE_LCD_XYZ);
	}

	if(ulNotifiedValue & BIT_UPDATE_AD){

		updateDataLevel(tlm);
	}
}
//-------------------------------------------------------------------------

