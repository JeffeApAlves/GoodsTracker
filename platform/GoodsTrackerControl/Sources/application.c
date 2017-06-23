/*
 * application.c
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#include <stdio.h>
#include "LED_B.h"
#include "LED_G.h"
#include "LED_R.h"
#include "AD1.h"
#include "MMA1.h"
#include "application.h"

volatile	bool AD_finished=FALSE;
uint16_t	AD_Values[AD1_CHANNEL_COUNT];
uint8_t		xyz[3];

void initCallBacks(){

	setEventCMD(CMD_LED,	onLED);
	setEventCMD(CMD_ANALOG,	onAnalog);
	setEventCMD(CMD_ACC,	onAccel);
	setEventCMD(CMD_TOUCH,	onTouch);
	setEventCMD(CMD_PWM,	onPWM);
}
//-------------------------------------------------------------------------

ResultExec onLED(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		buildHeader(frame);

		char *msg2send = "0.0000000,0.0000000";

		frame->sizePayLoad = strlen(msg2send);

		setPayLoad(frame, msg2send);

		buildPayload(frame);
		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
	//-------------------------------------------------------------------------

ResultExec onAnalog(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		buildHeader(frame);

		char *msg2send = "0.0000000,0.0000000";

		frame->sizePayLoad = strlen(msg2send);

		setPayLoad(frame, msg2send);

		buildPayload(frame);
		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onAccel(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		buildHeader(frame);

		char *msg2send = "0.0000000,0.0000000";

		frame->sizePayLoad = strlen(msg2send);

		setPayLoad(frame, msg2send);

		buildPayload(frame);
		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onTouch(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		buildHeader(frame);

		char *msg2send = "0.0000000,0.0000000";

		frame->sizePayLoad = strlen(msg2send);

		setPayLoad(frame, msg2send);

		buildPayload(frame);
		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//-------------------------------------------------------------------------

ResultExec onPWM(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		buildHeader(frame);

		char *msg2send = "0.0000000,0.0000000";

		frame->sizePayLoad = strlen(msg2send);

		setPayLoad(frame, msg2send);

		buildPayload(frame);
		res = CMD_RESULT_EXEC_SUCCESS;
	}


	return res;
}
//------------------------------------------------------------------------

ResultExec onTelemetry(DataFrame* frame){
	//ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;
	ResultExec res = CMD_RESULT_EXEC_SUCCESS;

	char *msg2send = "0.0000000,0.0000000";

#if 0
	if (frame) {

		                 /*    LAT*/
		char *msg2send = "-23.5912537:-46.6451776:0.1:9.8:0.4:1:2:3:60:1000:23/06/2017 19.52";
		doAnswer(msg2send);
		res = CMD_RESULT_EXEC_SUCCESS;
	}
#endif

<<<<<<< HEAD
=======
	doAnswer(msg2send);

>>>>>>> b5adea659d46635408b216ced18a345c77620c2d
	return res;
}
//------------------------------------------------------------------------

ResultExec onLock(DataFrame* frame){
	ResultExec res = CMD_RESULT_EXEC_UNSUCCESS;

	if (frame) {
		char *msg2send = "";

		LED_G_On();

		doAnswer(msg2send);
		res = CMD_RESULT_EXEC_SUCCESS;
	}

	return res;
}
//------------------------------------------------------------------------

void read_Channels_AD(){

//	if(AD_finished){
//		if(AD1_GetValue16(AD_Values)==ERR_OK){
			//TODO
//		}
//	}

	if(AD1_Measure(FALSE)==ERR_OK){

		AD_finished = FALSE;

		while (!AD_finished) {}

		if(AD1_GetValue16(AD_Values)==ERR_OK){
			//TODO
		}
	}
}
//------------------------------------------------------------------------

void read_accel() {

	MMA1_GetRaw8XYZ(xyz);
}
//------------------------------------------------------------------------

void initAccel(){

	MMA1_Init();

	MMA1_Enable();
}
//------------------------------------------------------------------------

void buildPayLoad(DataFrame* frame){

	char p[LEN_MAX_PAYLOAD];

	sprintf(p,"%s%d:%d:%d:%d",frame->resource,frame->address,xyz[0],xyz[1],xyz[2]);

	setPayLoad(frame,p);
}
//------------------------------------------------------------------------

