/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : DB71.c
**     Project     : GoodsTrackerControl
**     Processor   : MKL25Z128VLK4
**     Component   : BitIO
**     Version     : Component 02.086, Driver 01.00, CPU db: 3.00.000
**     Repository  : Kinetis
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-06-24, 12:17, # CodeGen: 0
**     Abstract    :
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
**     Settings    :
**          Component name                                 : DB71
**          Pin for I/O                                    : CMP0_IN3/PTC9/I2C0_SDA/TPM0_CH5
**          Pin signal                                     : LCDDB7
**          BitIO_LDD                                      : BitIO_LDD
**          Direction                                      : Input/Output
**          Initialization                                 : 
**            Init. direction                              : Output
**            Init. value                                  : 0
**          Safe mode                                      : yes
**          Optimization for                               : speed
**     Contents    :
**         SetDir    - void DB71_SetDir(bool Dir);
**         SetInput  - void DB71_SetInput(void);
**         SetOutput - void DB71_SetOutput(void);
**         GetVal    - bool DB71_GetVal(void);
**         PutVal    - void DB71_PutVal(bool Val);
**         ClrVal    - void DB71_ClrVal(void);
**         SetVal    - void DB71_SetVal(void);
**
**     Copyright : 1997 - 2015 Freescale Semiconductor, Inc. 
**     All Rights Reserved.
**     
**     Redistribution and use in source and binary forms, with or without modification,
**     are permitted provided that the following conditions are met:
**     
**     o Redistributions of source code must retain the above copyright notice, this list
**       of conditions and the following disclaimer.
**     
**     o Redistributions in binary form must reproduce the above copyright notice, this
**       list of conditions and the following disclaimer in the documentation and/or
**       other materials provided with the distribution.
**     
**     o Neither the name of Freescale Semiconductor, Inc. nor the names of its
**       contributors may be used to endorse or promote products derived from this
**       software without specific prior written permission.
**     
**     THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**     ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**     WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**     DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**     ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**     (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**     LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**     ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**     (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**     SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
**     
**     http: www.freescale.com
**     mail: support@freescale.com
** ###################################################################*/
/*!
** @file DB71.c
** @version 01.00
** @brief
**         This component "BitIO" implements an one-bit input/output.
**         It uses one bit/pin of a port.
**         Methods of this component are mostly implemented as a macros
**         (if supported by target language and compiler).
*/         
/*!
**  @addtogroup DB71_module DB71 module documentation
**  @{
*/         

/* MODULE DB71. */

#include "DB71.h"

#ifdef __cplusplus
extern "C" {
#endif 

/*
** ===================================================================
**     Method      :  DB71_SetDir (component BitIO)
**     Description :
**         This method sets direction of the component.
**     Parameters  :
**         NAME       - DESCRIPTION
**         Dir        - Direction to set (FALSE or TRUE)
**                      FALSE = Input, TRUE = Output
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_SetDir(bool Dir)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_SetInput (component BitIO)
**     Description :
**         This method sets direction of the component to input.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_SetInput(void)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_SetOutput (component BitIO)
**     Description :
**         This method sets direction of the component to output.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_SetOutput(void)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_GetVal (component BitIO)
**     Description :
**         This method returns an input value.
**           a) direction = Input  : reads the input value from the
**                                   pin and returns it
**           b) direction = Output : returns the last written value
**     Parameters  : None
**     Returns     :
**         ---             - Input value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)

** ===================================================================
*/
/*
bool DB71_GetVal(void)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_PutVal (component BitIO)
**     Description :
**         This method writes the new output value.
**           a) direction = Input  : sets the new output value;
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes the value to the
**                                   appropriate pin
**     Parameters  :
**         NAME       - DESCRIPTION
**         Val             - Output value. Possible values:
**                           FALSE - logical "0" (Low level)
**                           TRUE - logical "1" (High level)
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_PutVal(bool Val)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_ClrVal (component BitIO)
**     Description :
**         This method clears (sets to zero) the output value.
**           a) direction = Input  : sets the output value to "0";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "0" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_ClrVal(void)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/*
** ===================================================================
**     Method      :  DB71_SetVal (component BitIO)
**     Description :
**         This method sets (sets to one) the output value.
**           a) direction = Input  : sets the output value to "1";
**                                   this operation will be shown on
**                                   output after the direction has
**                                   been switched to output
**                                   (SetDir(TRUE);)
**           b) direction = Output : directly writes "1" to the
**                                   appropriate pin
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void DB71_SetVal(void)

**  This method is implemented as a macro. See DB71.h file.  **
*/

/* END DB71. */

#ifdef __cplusplus
}  /* extern "C" */
#endif 

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
