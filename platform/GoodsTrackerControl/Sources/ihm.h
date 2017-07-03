/*
 * ihm.h
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#ifndef SOURCES_IHM_H_
#define SOURCES_IHM_H_

#include <stdint.h>

#include "TSS_API.h"

typedef enum {
	IHM_EVENT_NONE,
	IHM_EVENT_CHOOSE_LEFT,
	IHM_EVENT_CHOOSE_RIGHT,
	IHM_EVENT_CHOOSE_OK,
} ihmEventType;

typedef struct {
	bool eventTreated;
	ihmEventType evType;
} ihmEvent;

#define IHM_MAX_EVENTS 16
typedef struct {
	uint8 option;
	struct {
		ihmEvent event[IHM_MAX_EVENTS]; //TODO - USAR BUFFER CIRCULAR
		uint8 head;
		uint8 tail;
	} ihmEventBuffer;

} ihmStruct;


/**
 * CHAMANDO ESTA ESTRUTURA DE IHM (INTERFACE HOMEM-M�QUINA) EM VEZ DE
 * MMI (MEN-MACHINE INTERFACE) POR PURO GOSTO PESSOAL... :)
 */

/**
 *
 */
void ihm_initialize();

/**
 *
 */
void ihm_terminate();

/**
 *
 */
void runIHM();

/**
 *
 */
int ihm_put_slide_event(TSS_CSASlider *event);

void ihm_process_events(ihmStruct *ihm);

#endif /* SOURCES_IHM_H_ */
