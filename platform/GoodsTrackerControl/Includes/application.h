/*
 * application.h
 *
 *  Created on: 04/06/2017
 *      Author: Jefferson
 */

#ifndef SOURCES_APPLICATION_H_
#define SOURCES_APPLICATION_H_

#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "CommunicationFrame.h"
#include "Frame.h"
#include "Telemetria.h"
#include "protocol.h"

//bits de sinalização das notificações

#define		BIT_UPDATE_GPS	0x01
#define		BIT_UPDATE_AD	0x02
#define		BIT_UPDATE_ACCE	0x04
#define		BIT_RX_FRAME	0x08

typedef enum
	{LED_GREEN	=0,
	LED_RED		=1,
	LED_BLUE	=2}LEDS;

/**
 * Ponteiro para as call backs
 */
//typedef ResultExec(*pCallBack)(CommunicationPackage*);
typedef	TaskFunction_t	pCallBack;

void app_init(void);
void decoderLockPayLoad(PayLoad* payload);

pCallBack getCallBack(Resource r);

extern TaskHandle_t	xHandleAppTask;
extern Telemetria	telemetria;

#endif /* SOURCES_APPLICATION_H_ */
