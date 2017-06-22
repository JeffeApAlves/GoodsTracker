/*
 * protocol.h
 *
 *  Created on: 09/06/2017
 *      Author: Suporte
 */

#include "PE_Types.h"

#ifndef SOURCES_PROTOCOL_H_
#define SOURCES_PROTOCOL_H_

#define	ADDRESS			2

#define TIME_TX			5
#define TIME_RX			5

#define LEN_ADDRESS		5
#define LEN_ORIGEM		5
#define LEN_OPERATION	2
#define LEN_RESOURCE	3
#define LEN_SIZE_PL		3
#define SIZE_HEADER		(LEN_ADDRESS+LEN_ORIGEM+LEN_OPERATION+LEN_RESOURCE+LEN_SIZE_PL)


#define LEN_MIN_FRAME	18
#define LEN_MAX_FRAME	273
#define LEN_MAX_PAYLOAD	255

/* [LED01]\r\n*/
#define CHAR_CMD_START	'['
#define CHAR_CMD_END	']'
#define CHAR_CR			'\r'
#define CHAR_LF			'\n'
#define CHAR_SEPARATOR	':'
#define CHAR_NAK		0x15
#define CHAR_STR_END	'\0'

typedef enum {

	EXEC_UNSUCCESS	= -3,
	INVALID_CMD		= -2,
	INVALID_PARAM	= -1,
	EXEC_SUCCESS	= 0,

}ResultExec;

typedef enum {

	CMD_LED,
	CMD_ANALOG,
	CMD_PWM,
	CMD_TOUCH,
	CMD_ACC,
	CMD_TELEMETRIA,
	CMD_LOCK

}IdCmd;

typedef enum {

	CMD_INIT,
	CMD_INIT_OK,
	CMD_RX_START,
	CMD_RX_PAYLOAD,
	CMD_RX_END,
	CMD_RX_NL,
	CMD_RX_CR,
	CMD_DECODER,
	CMD_EXEC,
	CMD_ERROR,
	CMD_ACK,
	CMD_NAK,

} StatusRx;

/*
typedef struct {

	char	data[LEN_MAX_FRAME];
	unsigned char count;

} DataRx;*/

typedef struct{

	unsigned char	checksum_calc;
	unsigned char	checksum_rx;

	char	operacao[2];
	char	resource[LEN_RESOURCE];

	int		address;
	int		dest;
	int		value;

	int		sizePayLoad;
	int		sizeFrame;

	char	payload[LEN_MAX_PAYLOAD];
	char	frame[LEN_MAX_FRAME];

} DataFrame;


/**
 * Ponteiro para as call backs
 */
typedef ResultExec(*pCallBack)(DataFrame*);


/**
 *
 */
typedef struct{

	IdCmd		id_cmd;
	char 		resource[LEN_RESOURCE];
	pCallBack	cb;

} Cmd;


void rxStartCMD (void);
void receiveFrame (void);
void rxNL(void);
void rxCR(void);
void decoderCMD(void);
pCallBack getCallBack(void);
void initRxCMD(void);
void sendNAK(void);
void sendACK(void);
void sendResult(void);
void execCallBack();
void setStatusRx(StatusRx sts);
bool getRxData(char* ch);
bool putTxData(char data);
void sendString(const char* str);
void clearData(DataFrame* frame);
void errorRx(void);
bool decoderFrame(void);
void formatCMD(void);
bool decoderFrame2(void);

/*interface*/
void processProtocol(void);
void setEventCMD(IdCmd id_cmd,pCallBack c);
bool putRxData(char ch);
bool getTxData(char* ch);
bool hasTxData(void);
void startTX(void);
void setPayLoad(DataFrame* frame,char* str);
void buildHeader(DataFrame *frame);

extern unsigned int timeTx,timeRx;

#endif /* SOURCES_PROTOCOL_H_ */