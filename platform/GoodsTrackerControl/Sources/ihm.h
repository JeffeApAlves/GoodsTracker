/*
 * ihm.h
 *
 *  Created on: 20/06/2017
 *      Author: Fl�vio Soares
 */
#include "Events.h"

#ifndef SOURCES_IHM_H_
#define SOURCES_IHM_H_

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
void IHM_Run();

/**
 *
 */
int ihm_put_slide_event(TSS_CSASlider *event);

#endif /* SOURCES_IHM_H_ */
