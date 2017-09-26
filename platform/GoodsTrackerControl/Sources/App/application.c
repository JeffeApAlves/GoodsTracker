/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>

#include "Tank.h"
#include "Telemetria.h"
#include "clock.h"
#include "lock.h"
#include "NMEAFrame.h"
#include "Accelerometer.h"
#include "Serialization.h"
#include "communication.h"
#include "consumer.h"
#include "application.h"

static 	int		_lock;
Telemetria		telemetria;
TaskHandle_t	xHandleCallBackTask;

void callback_task(void){

	uint32_t ulNotifiedValue;

	xTaskNotifyWait( 0x0, BIT_RX_FRAME | BIT_UPDATE_GPS | BIT_UPDATE_ACCE | BIT_UPDATE_AD,  &ulNotifiedValue, portMAX_DELAY);

	updateTLM(&telemetria,ulNotifiedValue);

	execCMD(ulNotifiedValue);
}
//-------------------------------------------------------------------------

void execCMD(uint32_t ulNotifiedValue){

	if(ulNotifiedValue & BIT_RX_FRAME){

		CommunicationPackage	package;

		while (xQueueReceive(xQueueRx, &package, (TickType_t ) 1)) {

			pCallBack cb = getCallBack(package.Header.resource);

			if(cb!=NULL){

				if(cb(&package) == CMD_RESULT_EXEC_SUCCESS) {

					 putPackageTx(&package);
				}
				else {

					execError(&package);
				}
			}
		}
	}
}
//-------------------------------------------------------------------------

ResultExec onLED(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAnalog(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	// ... Implementação da execução do cmd ...

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	tlm2String(&telemetria,&package->PayLoad);

	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(CommunicationPackage* package){

	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	decoderLockPayLoad(&package->PayLoad);

	if(_lock){

		lock();

	}else{

		unLock();
	}

	res = CMD_RESULT_EXEC_SUCCESS;

	initPackageAnswer(package);

	// Preenche payload com resultado
	AppendPayLoad(&package->PayLoad,"1569695954");;

	return res;
}
//------------------------------------------------------------------------

void execError(CommunicationPackage* package){

	initPackageAnswer(package);

	AppendPayLoad(&package->PayLoad,"EXEC ERROR");;
}
//------------------------------------------------------------------------

void decoderLockPayLoad(PayLoad* payload){

	AsInteger(&_lock,payload->Data,0,CHAR_SEPARATOR);
}
//------------------------------------------------------------------------

/*
 *
 * Set endereco de origem e destino e o tipo da operacao
 *
 */
static void initPackageAnswer(CommunicationPackage* package){

	if(package){

		// Payload do cmd apagado
		clearArrayPayLoad(&package->PayLoad);

		strcpy(package->Header.operacao,OPERATION_AN);
		package->Header.dest			= package->Header.address;
		package->Header.address			= ADDRESS;
		package->Header.time_stamp		= getCurrentTimeStamp();
	}
}
//------------------------------------------------------------------------

pCallBack getCallBack(Resource r) {

	pCallBack	cb = NULL;

	switch(r){

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

void app_init(void){

	clearTelemetria(&telemetria);
}
//------------------------------------------------------------------------