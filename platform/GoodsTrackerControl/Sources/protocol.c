#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "RingBuffer.h"
#include "utils.h"
#include "protocol.h"
#include "application.h"

unsigned int timeTx = TIME_TX;
unsigned int timeRx = TIME_RX;

StatusRx	statusRx = CMD_INIT;
RingBuffer	bufferRx,bufferTx;
DataFrame	dataFrame;

Cmd			ListCmd[]	= {	{.id_cmd=CMD_LED,		.resource = "LED",	.cb = onLED},
							{.id_cmd=CMD_ANALOG,	.resource = "AN",	.cb = onAnalog},
							{.id_cmd=CMD_PWM,		.resource = "PWM",	.cb = onPWM},
							{.id_cmd=CMD_ACC,		.resource = "ACC",	.cb = onAccel},
							{.id_cmd=CMD_TOUCH,		.resource = "TOU",	.cb = onTouch},
							{.id_cmd=CMD_TELEMETRIA,.resource = "TLM",	.cb = onTelemetry},
							{.id_cmd=CMD_LOCK,		.resource = "LCK",	.cb = onLock},
							};

const unsigned char SIZE_LIST_CMD = sizeof(ListCmd)/sizeof(Cmd);

void processProtocol(void) {

	switch(statusRx){

		default:
		case CMD_INIT:			initRxCMD();			break;
		case CMD_INIT_OK:		rxStartCMD();			break;
		case CMD_RX_START:		receiveFrame();			break;
		case CMD_RX_PAYLOAD:	receiveFrame();			break;
		case CMD_RX_END:		rxCR();					break;
		case CMD_RX_CR:			rxNL();					break;
		case CMD_RX_NL:			decoderCMD();			break;
		case CMD_DECODER:		execCallBack();			break;

		case CMD_ERROR:			errorRx();				break;
		case CMD_ACK:			sendACK();				break;
		case CMD_NAK:			sendNAK();				break;
		case CMD_EXEC:			sendResult();			break;
	}

	startTX();
}
//------------------------------------------------------------------------

void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CMD_START){

			clearData(&dataFrame);

			setStatusRx(CMD_RX_START);
		}
	}
}
//------------------------------------------------------------------------

void receiveFrame (void) {

	unsigned char ch;

	if(getRxData(&ch)) {

		if(ch==CHAR_CMD_START || dataFrame.sizeFrame>=LEN_MAX_FRAME) {
			setStatusRx(CMD_ERROR);
		}
		else
		  if(ch==CHAR_CMD_END) {

			 if(dataFrame.sizeFrame>=LEN_MIN_FRAME) {

				setStatusRx(CMD_RX_END);
			 }
			 else {
			   setStatusRx(CMD_ERROR);
			 }
		}
		else {

		  setStatusRx(CMD_RX_PAYLOAD);
		  dataFrame.frame[(dataFrame.sizeFrame++)%LEN_MAX_FRAME] = ch;
		}
	}
}
//------------------------------------------------------------------------

void rxNL(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_LF){

			setStatusRx(CMD_RX_NL);

		}else {

			setStatusRx(CMD_ERROR);
		}
	}
}
//------------------------------------------------------------------------

void rxCR(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CR){

			setStatusRx(CMD_RX_CR);

		}else{

			setStatusRx(CMD_ERROR);
		}
	}
}
//------------------------------------------------------------------------

void decoderCMD(void) {

	if(decoderFrame2()){

		setStatusRx(CMD_DECODER);

	}else{

		setStatusRx(CMD_ERROR);
	}
}
//------------------------------------------------------------------------


bool decoderFrame2(void) {

	bool ret = false;

	List	list;

	dataFrame.checksum_calc = calcChecksum(dataFrame.frame, dataFrame.sizeFrame - 2);

	str_split(&list, dataFrame.frame, CHAR_SEPARATOR);

	if(list.itens!=NULL) {


		/*Usamos strtol pois o �ltimo objeto da lista armazena o valor do checksum em HEXA*/
		dataFrame.checksum_rx = strtol(list.itens[list.size-1], NULL, 16);

		if(dataFrame.checksum_rx==dataFrame.checksum_calc) {

			char i;

			for(i=0;i < list.size;i++) {

				if(list.itens[i]!=NULL) {

					switch(i) {

						case 0:	dataFrame.address			= atoi(list.itens[0]);					break;
						case 1:	dataFrame.dest				= atoi(list.itens[1]);					break;
						case 2:	strncpy(dataFrame.operacao, list.itens[2], strlen(list.itens[2]));	break;
						case 3:	strncpy(dataFrame.resource,	list.itens[3], strlen(list.itens[3]));	break;
						case 4:	dataFrame.sizePayLoad		= atoi(list.itens[4]);					break;
						case 5:	memcpy(dataFrame.payload, 	list.itens[5], dataFrame.sizePayLoad);	break;
					}
				}
			}

			ret = true;
		}

		removeList(&list);
	}

	return ret;
}
//------------------------------------------------------------------------

void setEventCMD(IdCmd id_cmd,pCallBack c) {
	char i;
	for(i=0;i<SIZE_LIST_CMD;i++){

		if(ListCmd[i].id_cmd==id_cmd){

			ListCmd[i].cb = c;
			break;
		}
	}
}
//------------------------------------------------------------------------

pCallBack getCallBack(void) {

	pCallBack cb = NULL;
	char i;
	for(i = 0; i < SIZE_LIST_CMD; i++){

		if(strcmp(ListCmd[i].resource, dataFrame.resource) == 0){

			cb = ListCmd[i].cb;
			break;
		}
	}

	return cb;
}
//------------------------------------------------------------------------

void execCallBack(void) {

	pCallBack cb = getCallBack();

	if(cb!=NULL && cb(&dataFrame) == CMD_RESULT_EXEC_SUCCESS) {

			setStatusRx(CMD_EXEC);
	}
	else {

		setStatusRx(CMD_NAK);
	}
}
//------------------------------------------------------------------------

void initRxCMD(void) {

	clearData(&dataFrame);
	initBuffer(&bufferRx);
	initBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendACK(void) {

	sprintf(dataFrame.frame,"%s%d%d",dataFrame.resource,dataFrame.address,dataFrame.value);

	sendFrame(dataFrame.frame);

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendNAK(void) {

	sprintf(dataFrame.frame,"%s%d%d%s",dataFrame.resource,dataFrame.address,dataFrame.value,CHAR_NAK);

	sendFrame(dataFrame.frame);

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendResult(void){

	sendFrame(dataFrame.frame);

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void sendFrame(const char* str){

	putTxData(CHAR_CMD_START);

	sendString(str);

	putTxData(CHAR_CMD_END);
	putTxData(CHAR_CR);
	putTxData(CHAR_LF);
}
//------------------------------------------------------------------------

void sendString(const char* str){

	char* p = (char *)str;

	if(p!=NULL){

		while(*p!=CHAR_STR_END){

			putTxData(*p++);
		}
	}
}
//------------------------------------------------------------------------

inline void setStatusRx(StatusRx sts) {

	statusRx = sts;
}
//------------------------------------------------------------------------

inline bool putTxData(char data) {

	return putData(&bufferTx,data);
}
//------------------------------------------------------------------------

inline bool putRxData(char data) {

	return putData(&bufferRx,data);
}
//------------------------------------------------------------------------

inline bool getTxData(char* ch){

	return getData(&bufferTx,ch);
}
//------------------------------------------------------------------------

inline bool getRxData(char* ch){

	return getData(&bufferRx,ch);
}
//------------------------------------------------------------------------

void errorRx(void){

	/**/

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

void clearData(DataFrame* frame){

	frame->checksum_rx	= -1;
	frame->checksum_calc= 0;
	frame->address		= 0;
	frame->value		= 0;
	frame->sizeFrame	= 0;
	frame->sizePayLoad	= 0;
	frame->sizeHeader	= 0;

	int i;

	for(i=0;i<LEN_MAX_PAYLOAD;i++){

		frame->payload[i]	= CHAR_STR_END;
	}

	for(i=0;i<LEN_MAX_FRAME;i++){

		frame->frame[i]		= CHAR_STR_END;
	}

}
//------------------------------------------------------------------------

bool hasTxData(void){

	return hasData(&bufferTx);
}
//------------------------------------------------------------------------

/**
 * Verifica se o buffer de TX esta vazio. Se sim, chama a call back
 * da transmissao de caracter para iniciar a transmissao do buffer
 *
 */
void startTX(void){

	if(hasTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}
}
//------------------------------------------------------------------------

void buildHeader(DataFrame *frame) {

	sprintf(frame->frame,"%05d%c%05d%c%s%c%s%c",
				frame->address,
					CHAR_SEPARATOR,
						frame->dest,
							CHAR_SEPARATOR,
								frame->operacao,
									CHAR_SEPARATOR,
										frame->resource,
											CHAR_SEPARATOR);

	frame->sizeHeader	= strlen(frame->frame);
}
//------------------------------------------------------------------------

void buildFrame(DataFrame *frame) {

	char seperador[]= {CHAR_SEPARATOR,CHAR_STR_END};
	char checksum[4];

	buildHeader(frame);
	strncat(frame->frame, frame->payload,frame->payload);
	strcat(dataFrame.frame, seperador);

	frame->sizeFrame = strlen(frame->frame);

	sprintf(checksum, "%02X", calcChecksum (frame->frame,frame->sizeFrame));


	strcat(dataFrame.frame, checksum);
}
//------------------------------------------------------------------------

void setPayLoad(DataFrame* frame, char* str) {

	size_t size = strlen(str);

	sprintf(frame->payload,"%03d:",size);

	if(frame->sizePayLoad <= LEN_MAX_PAYLOAD){

		strcpy(frame->payload+4, str);
	}

	frame->sizePayLoad	= strlen(frame->payload);
}
//------------------------------------------------------------------------

void doAnswer(char *msg) {

	if (msg) {

		setPayLoad(&dataFrame, msg);

		strncpy(dataFrame.operacao, "AN", 2);
		dataFrame.dest		= dataFrame.address;
		dataFrame.address	= ADDRESS;

		buildFrame(&dataFrame);
	}
	else {
		/*O QUE RESPONDE EM CASO DE MENSAGEM NULA ???*/
	}
}
//------------------------------------------------------------------------

unsigned char calcChecksum(const char *buff, size_t sz) {

	unsigned char i=0;
	unsigned char chk = 0;

	if (buff) {

		for (i = 0; i < sz; i++)
			chk ^= buff[i];
	}

	return chk;
}
//------------------------------------------------------------------------
