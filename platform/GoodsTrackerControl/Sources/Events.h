/* ###################################################################
**     Filename    : Events.h
**     Project     : GoodsTrackerControl
**     Processor   : MKL25Z128VLK4
**     Component   : Events
**     Version     : Driver 01.00
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-20, 13:54, # CodeGen: 0
**     Abstract    :
**         This is user's event module.
**         Put your event handler code here.
**     Contents    :
**         Cpu_OnNMIINT - void Cpu_OnNMIINT(void);
**
** ###################################################################*/
/*!
** @file Events.h
** @version 01.00
** @brief
**         This is user's event module.
**         Put your event handler code here.
*/         
/*!
**  @addtogroup Events_module Events module documentation
**  @{
*/         

#ifndef __Events_H
#define __Events_H
/* MODULE Events */

#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"
#include "LCDout.h"
#include "EN1.h"
#include "BitIoLdd1.h"
#include "RS1.h"
#include "BitIoLdd2.h"
#include "DB41.h"
#include "BitIoLdd7.h"
#include "DB51.h"
#include "BitIoLdd8.h"
#include "DB61.h"
#include "BitIoLdd9.h"
#include "DB71.h"
#include "BitIoLdd10.h"
#include "TSSin.h"
#include "LED_R.h"
#include "LEDpin1.h"
#include "BitIoLdd11.h"
#include "LED_G.h"
#include "LEDpin2.h"
#include "BitIoLdd12.h"
#include "LED_B.h"
#include "LEDpin3.h"
#include "BitIoLdd13.h"
#include "WAIT1.h"
#include "MCUC1.h"
#include "GT_FRTOS.h"
#include "UTIL1.h"
#include "TU1.h"
#include "GI2C1.h"
#include "I2C1.h"
#include "SDA1.h"
#include "BitIoLdd14.h"
#include "SCL1.h"
#include "BitIoLdd15.h"
#include "CS1.h"
#include "GT_TSK.h"
#include "GT_AsyncSerial.h"
#include "ASerialLdd1.h"
#include "GT_AD1.h"
#include "AdcLdd1.h"
#include "GT_TI1.h"
#include "TimerIntLdd1.h"
#include "MMA1.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Event       :  Cpu_OnNMIINT (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Non maskable interrupt had
**         occurred. This event is automatically enabled when the [NMI
**         interrupt] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnNMIINT(void);


void TSSin_fOnFault(byte u8FaultElecNum);
/*
** ===================================================================
**     Event       :  TSSin_fOnFault (module Events)
**
**     Component   :  TSSin [TSS_Library]
**     Description :
**         This callback function is called by TSS after Fault
**         occurence. This event is enabled always and depends on
**         selection 'generate code' if the callback is used.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**         Option is available from TSS3.0 version.
**     Parameters  :
**         NAME            - DESCRIPTION
**         u8FaultElecNum  - The value defines
**                           electrode number on which measurement fault
**                           occured.
**     Returns     : Nothing
** ===================================================================
*/

void TSSin_fOnInit(void);
/*
** ===================================================================
**     Event       :  TSSin_fOnInit (module Events)
**
**     Component   :  TSSin [TSS_Library]
**     Description :
**         This callback function is automatically called during the
**         TSS Init function execution. The function is intended for
**         implementation of peripherals initialization. TSS Component
**         automatically enables clock for all used TSS peripherals in
**         the internal function TSS_InitDevices which is called by
**         this callback.
**         This event is enabled always and depends on selection
**         'generate code' if the callback is used.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void TSSin_fCallBack0(TSS_CONTROL_ID u8ControlId);
/*
** ===================================================================
**     Event       :  TSSin_fCallBack0 (module Events)
**
**     Component   :  TSSin [TSS_Library]
**     Description :
**         Callback definition for Control 0. This event is enabled
**         only if Control 0 is enabled.
**         The default CallBack Name is automatically generated with
**         automatic prefix update by current Component Name. User can
**         define own name, but then the automatic name update is not
**         functional.
**     Parameters  :
**         NAME            - DESCRIPTION
**         u8ControlId     - Valid unique Identifier of
**                           the Control which generated the CallBack
**                           function. This Id can be used for finding
**                           of Callback's source Control.
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  GT_TI1_OnInterrupt (module Events)
**
**     Component   :  GT_TI1 [TimerInt]
**     Description :
**         When a timer interrupt occurs this event is called (only
**         when the component is enabled - <Enable> and the events are
**         enabled - <EnableEvent>). This event is enabled only if a
**         <interrupt service/event> is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GT_TI1_OnInterrupt(void);

void GT_AD1_OnEnd(void);
/*
** ===================================================================
**     Event       :  GT_AD1_OnEnd (module Events)
**
**     Component   :  GT_AD1 [ADC]
**     Description :
**         This event is called after the measurement (which consists
**         of <1 or more conversions>) is/are finished.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void GT_AD1_OnCalibrationEnd(void);
/*
** ===================================================================
**     Event       :  GT_AD1_OnCalibrationEnd (module Events)
**
**     Component   :  GT_AD1 [ADC]
**     Description :
**         This event is called when the calibration has been finished.
**         User should check if the calibration pass or fail by
**         Calibration status method./nThis event is enabled only if
**         the <Interrupt service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  GT_AsyncSerial_OnError (module Events)
**
**     Component   :  GT_AsyncSerial [AsynchroSerial]
**     Description :
**         This event is called when a channel error (not the error
**         returned by a given method) occurs. The errors can be read
**         using <GetError> method.
**         The event is available only when the <Interrupt
**         service/event> property is enabled.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GT_AsyncSerial_OnError(void);

/*
** ===================================================================
**     Event       :  GT_AsyncSerial_OnRxChar (module Events)
**
**     Component   :  GT_AsyncSerial [AsynchroSerial]
**     Description :
**         This event is called after a correct character is received.
**         The event is available only when the <Interrupt
**         service/event> property is enabled and either the <Receiver>
**         property is enabled or the <SCI output mode> property (if
**         supported) is set to Single-wire mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GT_AsyncSerial_OnRxChar(void);

/*
** ===================================================================
**     Event       :  GT_AsyncSerial_OnTxChar (module Events)
**
**     Component   :  GT_AsyncSerial [AsynchroSerial]
**     Description :
**         This event is called after a character is transmitted.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void GT_AsyncSerial_OnTxChar(void);

void GT_FRTOS_vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName);
/*
** ===================================================================
**     Event       :  GT_FRTOS_vApplicationStackOverflowHook (module Events)
**
**     Component   :  GT_FRTOS [FreeRTOS]
**     Description :
**         if enabled, this hook will be called in case of a stack
**         overflow.
**     Parameters  :
**         NAME            - DESCRIPTION
**         pxTask          - Task handle
**       * pcTaskName      - Pointer to task name
**     Returns     : Nothing
** ===================================================================
*/

void GT_FRTOS_vApplicationMallocFailedHook(void);
/*
** ===================================================================
**     Event       :  GT_FRTOS_vApplicationMallocFailedHook (module Events)
**
**     Component   :  GT_FRTOS [FreeRTOS]
**     Description :
**         If enabled, the RTOS will call this hook in case memory
**         allocation failed.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void GT_FRTOS_vApplicationTickHook(void);
/*
** ===================================================================
**     Event       :  GT_FRTOS_vApplicationTickHook (module Events)
**
**     Component   :  GT_FRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called by the RTOS for every
**         tick increment.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

void GT_FRTOS_vApplicationIdleHook(void);
/*
** ===================================================================
**     Event       :  GT_FRTOS_vApplicationIdleHook (module Events)
**
**     Component   :  GT_FRTOS [FreeRTOS]
**     Description :
**         If enabled, this hook will be called when the RTOS is idle.
**         This might be a good place to go into low power mode.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/

/*
** ===================================================================
**     Event       :  Cpu_OnSupervisorCall (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Supervisor Call exception had
**         occurred. This event is automatically enabled when the
**         [Supervisor Call] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnSupervisorCall(void);

/*
** ===================================================================
**     Event       :  Cpu_OnPendableService (module Events)
**
**     Component   :  Cpu [MKL25Z128LK4]
*/
/*!
**     @brief
**         This event is called when the Pendable Service exception had
**         occurred. This event is automatically enabled when the
**         [Pendable Service] property is set to 'Enabled'.
*/
/* ===================================================================*/
void Cpu_OnPendableService(void);

/* END Events */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

#endif 
/* ifndef __Events_H*/
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