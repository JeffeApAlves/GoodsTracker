/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.5 [05.21]
 **     for the Freescale Kinetis series of microcontrollers.
 **
 ** ###################################################################
 */

/* Begin of <includes> initialization, DO NOT MODIFY LINES BELOW */

#include "TSK1.h"
#include "FRTOS1.h"
#include "frtos_tasks.h"

/* End <includes> initialization, DO NOT MODIFY LINES ABOVE */

#include "application.h"
#include "AppQueues.h"
#include "ihm.h"
#include "protocol.h"
#include "Level.h"
#include "accelerometer.h"
#include "gps.h"

/*############################################################################################
  #  Main task
  ############################################################################################*/

static portTASK_FUNCTION(main_task, pvParameters) {

	initQueues();

	initApp();

	for (;;) {

		runApp();
	}

	vTaskDelete(main_task);
}


/*############################################################################################
 #  Comunicacao com o computador
 ############################################################################################*/

static portTASK_FUNCTION(communication_task, pvParameters) {

	initCommunication();

	for (;;) {

//		runCommunication();

		processRx();

		vTaskDelay(xCommunicationDelay);
	}

	vTaskDelete(communication_task);
}


/*############################################################################################
 #  Leitura do AD
 ############################################################################################*/

static portTASK_FUNCTION(data_task, pvParameters) {

	for (;;) {

		runAnalog();

		vTaskDelay(xDataDelay);
	}

	vTaskDelete(data_task);
}

/*############################################################################################
 #  Gerencimento do IHM LCD-Touch
 ############################################################################################*/

static portTASK_FUNCTION(ihm_task, pvParameters) {

	ihm_initialize();

	for (;;) {

		runIHM();

		vTaskDelay(xIHMDelay);
	}

	ihm_terminate();
	vTaskDelete(ihm_task);
}


/*############################################################################################
 #  Task de gerenciamento do GPS e protocolo NMEA
 ############################################################################################*/

static portTASK_FUNCTION(gps_task, pvParameters) {

	NMEA_init();

	for (;;) {

		runNMEA();

		vTaskDelay(xGPSDelay);
	}

	vTaskDelete(gps_task);
}

/*############################################################################################
 #  Task leitura do acelerometro
 ############################################################################################*/

static portTASK_FUNCTION(accel_task, pvParameters) {

	initAccel();

	for (;;) {

		runAccelerometer();

		vTaskDelay(xAccelDelay);
	}

	vTaskDelete(accel_task);
}

/*############################################################################################
 #  Task para execucao da call back dos cmds recebidos
 ############################################################################################*/

static portTASK_FUNCTION(callback_task, pvParameters) {

	for (;;) {

		runCallBack();
	}

	vTaskDelete(callback_task);
}

/*############################################################################################
 #  Task para processamento das transmissao
 ############################################################################################*/

static portTASK_FUNCTION(runTx_task, pvParameters) {

	for (;;) {

		processTx();
	}

	vTaskDelete(runTx_task);
}

/*############################################################################################
 #  Funcao para criacao das tasks
 ############################################################################################*/

void CreateTasks(void) {

	if (FRTOS1_xTaskCreate(
			main_task, /* pointer to the task */
			"mains_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleMainTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			communication_task, /* pointer to the task */
			"communication_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleCommunicationTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			data_task, /* pointer to the task */
			"data_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleDataTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
	if (FRTOS1_xTaskCreate(
			ihm_task, /* pointer to the task */
			"ihm_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleIHMTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			gps_task, /* pointer to the task */
			"gps_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleGPSTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			accel_task, /* pointer to the task */
			"accel_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleAccelTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			callback_task, /* pointer to the task */
			"callback_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleCallBackTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}

	if (FRTOS1_xTaskCreate(
			runTx_task, /* pointer to the task */
			"runTx_task", /* task name for kernel awareness debugging */
			configMINIMAL_STACK_SIZE + 0, /* task stack size */
			(void*)NULL, /* optional task startup argument */
			tskIDLE_PRIORITY + 0, /* initial priority */
			&xHandleRunTxTask /* optional task handle to create */
	) != pdPASS) {
		/*lint -e527 */
		for (;;) {
		}; /* error! probably out of memory */
		/*lint +e527 */
	}
}
