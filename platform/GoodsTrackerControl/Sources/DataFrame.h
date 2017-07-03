/*
 * DataFrame.h
 *
 *  Created on: 27/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_DATAFRAME_H_
#define SOURCES_DATAFRAME_H_

#include "Array.h"

#define LEN_ADDRESS		5
#define LEN_ORIGEM		5
#define LEN_COUNT		5
#define LEN_OPERATION	2
#define LEN_RESOURCE	3
#define LEN_SIZE_PL		3
#define LEN_CHECKSUM	2

#define SIZE_HEADER			(LEN_ADDRESS+LEN_ORIGEM+LEN_OPERATION+LEN_RESOURCE+LEN_SIZE_PL+4)	// 4 separadores do cabecalho
#define SIZE_MIN_FRAME		(SIZE_HEADER+2)														// 2 separador do payload vazio

/*
 *
 *Retorno das callbacks
 */
typedef enum {

	CMD_RESULT_EXEC_UNSUCCESS	= -3,
	CMD_RESULT_INVALID_CMD		= -2,
	CMD_RESULT_INVALID_PARAM	= -1,
	CMD_RESULT_EXEC_SUCCESS		= 0,

}ResultExec;

/*
 *
 * Id dos recursos
 */
typedef enum {

	CMD_NONE,
	CMD_LED,
	CMD_ANALOG,
	CMD_PWM,
	CMD_TOUCH,
	CMD_ACC,
	CMD_TLM,
	CMD_LOCK,
	CMD_LCD

}ResourceID;

/**
 *
 * Maquina de estado para recebimento do frame
 */
typedef enum {

	CMD_INIT,
	CMD_INIT_OK,
	CMD_RX_START,
	CMD_RX_FRAME,
	CMD_RX_END,
	CMD_RX_CR,
	CMD_RX_LF,
	CMD_FRAME_OK,
	CMD_FRAME_NOK,
//	CMD_EXEC,
//	CMD_EXEC_ERROR,
} StatusRx;

/**
 *
 */
typedef struct{

	ResourceID	id;
	char		name[LEN_RESOURCE + 1];

} Resource;


/*
 * Estrutura de dados do frame
 *
 */
typedef struct{

	int				address;
	int				dest;
	int				countFrame;
	char			operacao[LEN_OPERATION + 1];
	Resource		resource;
	ArrayPayLoad	PayLoad;

} DataCom;

#define clearData(f) memset((void*)f,0,sizeof(DataCom));

#endif /* SOURCES_DATAFRAME_H_ */