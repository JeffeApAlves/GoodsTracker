/*
 * ihm.c
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#include <stdio.h>
#include <string.h>
#include "ihm.h"
#include "TSSin.h"
#include "lcd.h"
#include "XF1.h"
#include "clock.h"

char* functionsArray[] = { "OPTION 1", "OPTION 3", "OPTION 2" };

static ihmStruct ihmGlobal;

void runIHM() {

	TSS_Task(); /* call TSS library to process touches */

	ihm_process_events(&ihmGlobal);

	printClock(&Time);
}
//-----------------------------------------------------------------------------------------

void ihm_process_events(ihmStruct *ihm) {

	if (ihm) {
#if 0
		//TODO - ISSO � S� UMA VERS�O PRELIMINAR, O CERTO � TRATAR COMO RingBuffer
		UINT8 evmax = ihm->ihmEventBuffer.head;
		unsigned char i;
		for (i = 0; i < evmax; i++) {
			ihmEvent *ev = &ihm->ihmEventBuffer.event[i];
			ihm_process_event(ev);
		}
#endif
	}
}
//-----------------------------------------------------------------------------------------

static void ihm_process_event(ihmEvent *ev) {

	if (ev) {

		switch (ev->evType) {
		case IHM_EVENT_NONE:
			break;
		case IHM_EVENT_CHOOSE_LEFT:
			LCDClear();
			LCDWriteString("ESQUERDA");
			break;
		case IHM_EVENT_CHOOSE_RIGHT:
			LCDClear();
			LCDWriteString("DIREITA");
			break;
		case IHM_EVENT_CHOOSE_OK:
			LCDClear();
			LCDWriteString("OK");
			break;
		default:
			break;
		}

	}
}
//-----------------------------------------------------------------------------------------

//TODO - IMPLEMENTAR CHAMADA PARA RECEBER EVENTOS EM GERAL, ABSTRAIR MELHOR O HARDWARE
//TODO - POR ENQUANTO FICA ASSIM PARA IMPLEMENTA��O B�SICA
int ihm_put_slide_event(TSS_CSASlider *event) {

	if (event == NULL)
		return -1;

	if (TSSin_cKey0.DynamicStatus.Movement) {
		UINT8 evposit = ihmGlobal.ihmEventBuffer.head;
		if (evposit >= IHM_MAX_EVENTS)
			evposit = 0;
		else
			evposit++;

		ihmGlobal.ihmEventBuffer.head = evposit;

		ihmEvent *ev = &ihmGlobal.ihmEventBuffer.event[evposit];

		if (TSSin_cKey0.Events.Touch) {
			if (!(TSSin_cKey0.Events.InvalidPos)) {

//				LED_R_Put(0);
//				LED_G_Put(1);
//				LED_B_Put(0);

				ev->evType = IHM_EVENT_CHOOSE_OK;
			}
		}

		if (TSSin_cKey0.DynamicStatus.Displacement > (UINT8) 15) {
			if (TSSin_cKey0.DynamicStatus.Direction) {
//				LED_R_Put(1);
//				LED_G_Put(0);
//				LED_B_Put(0);

				ev->evType = IHM_EVENT_CHOOSE_LEFT;
			} else {
//				LED_R_Put(0);
//				LED_G_Put(0);
//				LED_B_Put(1);

				ev->evType = IHM_EVENT_CHOOSE_RIGHT;
			}
		}

	}

	return 0;
}
//-----------------------------------------------------------------------------------------

void deInitIHM() {

}
//-----------------------------------------------------------------------------------------

/*
 *
 * Inicializa LCD
 *
 */
void initIHM() {

	TSSin_Configure(); /* initialize TSS library */

	LCDInit();

	ihmGlobal.option = 0;

	initEvents();

	printLCD(1,2,".GOODSTRACKER.");
	printLCD(2,1,"V.0.2 -");
}
//-----------------------------------------------------------------------------------------

void printClock(LDD_RTC_TTime* time){

	char time_str[10];


	switch(statuc_clock){

		case CLOCK_INIT:
			XF1_xsprintf(time_str,"CLKINIT\n");

			break;
		case CLOCK_STARTED:
			XF1_xsprintf(time_str,"CLKSTART\n");
			break;
		case CLOCK_UPDATE:
			XF1_xsprintf(time_str,"CLKUPD\n");
			break;
		case CLOCK_ADJUSTED:
			XF1_xsprintf(time_str,"%02d.%02d.%4d %02d:%02d:%02d \n",time->Day,time->Month,time->Year,time->Hour+FUSO_HORARIO_BR, time->Minute, time->Second);
			break;
	}

//	printLCD(2,9,time_str);
	printLCD(2,1,time_str);

}
//-----------------------------------------------------------------------------------------

void printLCD(int linha,int col,char* str){

	LCDGotoXY(linha,col);
	LCDWriteString(str);
}
//-----------------------------------------------------------------------------------------

/*
 * Inicializa a estrutura de eventos
 *
 */
void initEvents(){

	unsigned char i;
	ihmGlobal.ihmEventBuffer.head = 0;
	ihmGlobal.ihmEventBuffer.tail = 0;
	for (i = 0; i < IHM_MAX_EVENTS; i++) {
		ihmGlobal.ihmEventBuffer.event[i].evType = IHM_EVENT_NONE;
		/*INICIALIZAMOS COMO J� TRATADO PARA N�O ENTRAR A 1a VEZ A TOA*/
		ihmGlobal.ihmEventBuffer.event[i].eventTreated = TRUE;
	}
}
//-----------------------------------------------------------------------------------------
