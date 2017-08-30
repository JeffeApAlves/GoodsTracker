#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "XF1.h"
#include "Array.h"
#include "AS1.h"
#include "Events.h"
#include "RingBuffer.h"
#include "AppQueues.h"
#include "protocol.h"

const char* OPERATION_AN = "AN";
const char* OPERATION_RD = "RD";
const char* OPERATION_WR = "WR";

static StatusRx		statusRx = CMD_INIT;
static RingBuffer	bufferRx,bufferTx;
static DataCom		dataComRx,AnsCmd;
static ArrayFrame	frameRx,frameTx;

Resource	ListCmd[]	= {	{.id = CMD_NONE,	.name = "---\0"},
							{.id = CMD_LED,		.name = "LED\0"},
							{.id = CMD_ANALOG,	.name = "ANL\0"},
							{.id = CMD_PWM,		.name = "PWM\0"},
							{.id = CMD_ACC,		.name = "ACC\0"},
							{.id = CMD_TOUCH,	.name = "TOU\0"},
							{.id = CMD_TLM,		.name = "TLM\0"},
							{.id = CMD_LOCK,	.name = "LCK\0"},
							{.id = CMD_LCD,		.name = "LCD\0"},
				};

const unsigned char SIZE_LIST_CMD = sizeof(ListCmd)/sizeof(Resource);

/*
 * Processa maquina de estado de recepção do protocolo de comunicação com o Host
 * Chamada pela task ''
 *
 */
void processRx(void){

	while(isAnyRxData()){

		switch(statusRx){

			default:
			case CMD_INIT:			initCommunication();	break;
			case CMD_INIT_OK:		rxStartCMD();			break;
			case CMD_RX_START:		receiveFrame();			break;
			case CMD_RX_FRAME:		receiveFrame();			break;
			case CMD_RX_END:		rxCR();					break;
			case CMD_RX_CR:			rxLF();					break;
			case CMD_RX_LF:			verifyFrame();			break;
			case CMD_FRAME_NOK:		errorRxFrame();			break;
		}
	}
}
//------------------------------------------------------------------------

/*
 *
 *
 */
void processTx(void){

	uint32_t ulNotifiedValue;

	xTaskNotifyWait( 0x0, BIT_TX,  &ulNotifiedValue, portMAX_DELAY );

	if(ulNotifiedValue & BIT_TX){

		while (xQueueReceive(xQueueAnswer, &AnsCmd, (TickType_t ) 1)) {

			doAnswer(&AnsCmd);
		}
	}
}
//------------------------------------------------------------------------

static void rxStartCMD (void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_START){

			clearArrayFrame(&frameRx);
			setStatusRx(CMD_RX_START);
		}
	}
}
//------------------------------------------------------------------------

static void receiveFrame (void) {

	char ch;

	if(getRxData(&ch)) {

		if(ch==CHAR_START || frameRx.Length>=ARRAY_LEN_FRAME) {

			errorRxFrame();
		}
		else
		  if(ch==CHAR_END) {

			 if(frameRx.Length>=SIZE_MIN_FRAME) {

				setStatusRx(CMD_RX_END);
			 }
			 else {

			   errorRxFrame();
			 }
		}
		else {

			putDataArray(&frameRx,ch);
			setStatusRx(CMD_RX_FRAME);
		}
	}
}
//------------------------------------------------------------------------

static void rxLF(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_LF){

			setStatusRx(CMD_RX_LF);

		}else {

			errorRxFrame();
		}
	}
}
//------------------------------------------------------------------------

static void rxCR(void) {

	char ch;

	if(getRxData(&ch)){

		if(ch==CHAR_CR){

			setStatusRx(CMD_RX_CR);

		}else{

			errorRxFrame();
		}
	}
}
//------------------------------------------------------------------------

static void verifyFrame(void) {

	if(decoderFrame()){

		acceptRxFrame();

	}else{

		errorRxFrame();
	}
}
//------------------------------------------------------------------------

static bool decoderFrame(void) {

	bool ret = false;

	List	list;

	str_split(&list, frameRx.Data, CHAR_SEPARATOR);

	clearData(&dataComRx);

	if(list.itens!=NULL) {

		// Minimo 8 itens
		if(list.count >= 8){

			//-2 para desconsiderar o checksum que esta no frame recebido
			unsigned int checksum_rx;
			unsigned int checksum_calc = calcChecksum(frameRx.Data, frameRx.Length - LEN_CHECKSUM);
			checksum_rx = ~checksum_calc;

			AsHex(&checksum_rx,&list,list.count-1);

			if(checksum_rx==checksum_calc) {

				AsInteger(&dataComRx.address,			&list,0);
				AsInteger(&dataComRx.dest,				&list,1);
				AsInteger(&dataComRx.countFrame,		&list,2);
				AsString(&dataComRx.operacao,			&list,3);
				AsString(&dataComRx.resource.name,		&list,4);
				AsInteger(&dataComRx.PayLoad.Length,	&list,5);
				AsString(&dataComRx.PayLoad.Data,		&list,6);

				ret = true;
			}
		}

		removeList(&list);
	}

	return ret;
}
//------------------------------------------------------------------------

static Resource getResource(char* name) {

	Resource	r = ListCmd[0];

	unsigned char i;

	for(i = 1; i < SIZE_LIST_CMD; i++){

		if(strcmp(ListCmd[i].name, name) == 0){

			r = ListCmd[i];
			break;
		}
	}

	return r;
}
//------------------------------------------------------------------------

/**
 *
 * Envia o frame
 *
 */
static void sendFrame(char* frame){

	putTxData(CHAR_START);					// Envia caracter de inicio

	putString(&bufferTx,frame);				// Envia o frame

	putTxData(CHAR_END);					// Envia o caracter de fim

	putTxData(CHAR_CR);						// Envia caracteres de controle
	putTxData(CHAR_LF);

	startTX();	// Envia 1 byte para iniciar a transmissao os demais serao via interrupcao TX

}
//-------------------1-----------------------------------------------------

static inline void setStatusRx(StatusRx sts) {

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

/*
 *
 * Pega um char do buffer circular da transmissao
 *
 */
inline bool getTxData(char* ch){

	return getData(&bufferTx,ch);
}
//------------------------------------------------------------------------

/*
 *
 * Pega um char do buffer circular da recepcao
 *
 */
inline bool getRxData(char* ch){

	return getData(&bufferRx,ch);
}
//------------------------------------------------------------------------

/**
 * Recepcao do frame OK
 *
 */
static void acceptRxFrame(void) {

	dataComRx.resource = getResource(dataComRx.resource.name);

	if(xQueueSendToBack( xQueueCom , &dataComRx, ( TickType_t ) 1 ) ){

		xTaskNotify( xHandleCallBackTask , BIT_RX_FRAME , eSetBits );
	}

	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 * Erro na recepcao do frame
 *
 */
static void errorRxFrame(void){

	setStatusRx(CMD_FRAME_NOK);
	clearData(&dataComRx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------

/**
 * Verifica se existe dados no buffer circular de transmiassa
 */
inline bool hasTxData(void){

	return hasData(&bufferTx);
}
//------------------------------------------------------------------------

/**
 *
 *  Preenche todo o frame  de envio
 *
 */
static void buildFrame(DataCom* data,ArrayFrame* frame) {

	clearArrayFrame(frame);

	copyHeaderToFrame(data,frame);
	copyPayLoadToFrame(data,frame);
	copyCheckSumToFrame(frame);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o cabecalho no frame
 *
 */
static void copyHeaderToFrame(DataCom* data,ArrayFrame* frame) {

	XF1_xsprintf(frame->Data,"%05d%c%05d%c%05d%c%s%c%s%c%03d%c",
				data->address, 			CHAR_SEPARATOR,
				data->dest, 			CHAR_SEPARATOR,
				data->countFrame,		CHAR_SEPARATOR,
				data->operacao, 		CHAR_SEPARATOR,
				data->resource.name,	CHAR_SEPARATOR,
				data->PayLoad.Length,	CHAR_SEPARATOR
				);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o Payload no Frame
 *
 */
inline static void copyPayLoadToFrame(DataCom* data,ArrayFrame* frame) {

	AppendFrame(frame,data->PayLoad.Data);
}
//------------------------------------------------------------------------

/**
 *
 * Adiciona o CheckSum no Frame
 *
 */
static void copyCheckSumToFrame(ArrayFrame* frame) {

	char separator[] = {CHAR_SEPARATOR,CHAR_STR_END};

	AppendFrame(frame,separator);
	XF1_xsprintf(frame->checksum, "%02X", calcChecksum (frame->Data,frame->Length));
	AppendFrame(frame,frame->checksum);
}
//------------------------------------------------------------------------

/**
 *
 * Funcao disponibilizada para aplicacao preencher o payload
 *
 */
void doAnswer(DataCom* ans) {

	if (ans) {

//		ArrayFrame	frameTx;

		buildFrame(ans,&frameTx);

		sendFrame(frameTx.Data);
	}
	else {

		//TODO O QUE RESPONDE EM CASO DE MENSAGEM NULA ???
	}
}
//------------------------------------------------------------------------

/**
 * Verifica se o buffer de TX esta vazio. Se sim, chama a call back
 * da transmissao de caracter para iniciar a transmissao do buffer
 *
 */
static void startTX(void){

	vPortEnterCritical();

	if(hasTxData() && AS1_GetCharsInTxBuf()==0){

		AS1_OnTxChar();
	}

	vPortExitCritical();
}
//------------------------------------------------------------------------

inline bool isAnyRxData(){

	return getCount(&bufferRx)>0;
}
//------------------------------------------------------------------------

/**
 *
 * Inicializa a comunicação
 *
 */
void initCommunication(void) {

	clearData(&dataComRx);
	clearArrayFrame(&frameRx);
	clearBuffer(&bufferRx);
	clearBuffer(&bufferTx);
	setStatusRx(CMD_INIT_OK);
}
//------------------------------------------------------------------------
