/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdlib.h>
#include <stdbool.h>
#include <Telemetria.h>

#include "AppQueues.h"
#include "clock.h"
#include "lock.h"
#include "NMEAFrame.h"
#include "Level.h"
#include "Accelerometer.h"
#include "Serialization.h"
#include "application.h"

static 	int			_lock;
static 	DataCom		Answer,Cmd;

void runApp(void){

	uint32_t ulNotifiedValue;

	xTaskNotifyWait( 0x0, BIT_RX_FRAME | BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,  &ulNotifiedValue, portMAX_DELAY);

	updateTLM(ulNotifiedValue);

	execCMD(ulNotifiedValue);
}
//-------------------------------------------------------------------------

void runMain(void){

	if (xQueueGPS != 0) {

		//DataGPS gps;

		//if (xQueuePeek(xQueueGPS, &(gps), (TickType_t ) 1)) {

			//setClockByString(gps.Time,gps.Date);
		//}
	}
}
//-------------------------------------------------------------------------

void execCMD(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_RX_FRAME){

		while (xQueueReceive(xQueueCom, &Cmd, (TickType_t ) 1)) {

			pCallBack cb = getCallBack(&Cmd.resource);

			if(cb!=NULL){

				//Preenche dados do Header para retorno
				setHeaderAnswer(&Cmd);

				if(cb(&Cmd) == CMD_RESULT_EXEC_SUCCESS) {

					// TODO Sucesso na execução
				}
				else {

					// TODO Sem sucesso na execução
				}
			}
		}
	}
}
//-------------------------------------------------------------------------

ResultExec onLED(DataCom* frame){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAnalog(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		answerTLM();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(DataCom* cmd){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (cmd) {

		decoderLockPayLoad(&cmd->PayLoad);

		if(_lock){

			lock();

		}else{

			unLock();
		}

		answerTime();

		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

void decoderLockPayLoad(ArrayPayLoad* payload){

	AsInteger(&_lock,payload->Data,0,CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

void answerTime(void){

	clearArrayPayLoad(&Answer.PayLoad);

	AppendPayLoad(&Answer.PayLoad,"1569695954");


	if(xQueueSendToBack( xQueueAnswer , &Answer, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleRunTxTask , BIT_TX , eSetBits );

	}else{

		//TODO erro na resposta
	}
}
//-------------------------------------------------------------------------

void answerTLM(void){

	tlm2String(&telemetria,&Answer.PayLoad);

	if(xQueueSendToBack( xQueueAnswer , &Answer, ( TickType_t ) 1 ) ){

    	xTaskNotify( xHandleRunTxTask , BIT_TX , eSetBits );

	}else{

		//TODO erro na resposta
	}
}
//-------------------------------------------------------------------------

/*
 *
 * Set endereco de origem e destino e o tipo da operacao
 *
 */
static void setHeaderAnswer(DataCom* data){

	strcpy(Answer.operacao,OPERATION_AN);
	Answer.resource		= data->resource;
	Answer.dest			= data->address;
	Answer.address		= ADDRESS;
	Answer.countFrame	= data->countFrame;
}
//------------------------------------------------------------------------

pCallBack getCallBack(Resource* r) {

	pCallBack	cb = NULL;

	switch(r->id){

		case CMD_LED:		cb = onLED;			break;
		case CMD_ANALOG:	cb = onAnalog;		break;
		case CMD_PWM:		cb = onPWM;			break;
		case CMD_ACC:		cb = onAccel;		break;
		case CMD_TOUCH:		cb = onTouch;		break;
		case CMD_TLM:		cb = onTelemetry;	break;
		case CMD_LOCK:		cb = onLock;		break;
		case CMD_LCD:		cb = NULL;			break;
	}

	return cb;
}
//------------------------------------------------------------------------

void initApp(void){

	clearTelemetria(&telemetria);
	clearData(&Answer);
}
//------------------------------------------------------------------------

