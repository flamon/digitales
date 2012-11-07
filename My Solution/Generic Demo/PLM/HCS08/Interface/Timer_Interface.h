/************************************************************************************
* Public header file for the Timer driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
#ifndef _TIMER_INTERFACE_H_
#define _TIMER_INTERFACE_H_


#include  "EmbeddedTypes.h"
#include  "derivative.h"
#include  "PLM_Config.h"


/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
 
#ifndef gTimerSupported_d
#define gTimerSupported_d FALSE
#endif

#define gTimerMode_d         gInterruption_c

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 


#define gTimerReset_Counter       TPM2CNTH=0;			TPM2CNTL=0;


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/


  
#ifdef gMcs08Gt60Platform_d
  typedef enum tmrNumber_tag{
    gTmr1_c = 0,
    gTmr2_c ,
    gTmrMax_c
  } tmrNumber_t;
  
  typedef  enum tmrEvent_tag{
    gTmrOverEvent_c = 0,
    gTmrChannel0Event_c,
    gTmrChannel1Event_c,
    gTmrChannel2Event_c,
    gTmrChannel3Event_c,
    gTmrChannel4Event_c,
    gTmrMaxEvent_c
  } tmrEvent_t;

  typedef enum tmrChannel_tag{
    gTmrChannel0_c = 0,
    gTmrChannel1_c,
    gTmrChannel2_c,
    gTmrChannel3_c,
    gTmrChannel4_c,
    gTmrMaxChannel_c
  } tmrChannel_t;
#endif


#ifdef gMcs08qe128Platform_d
  typedef enum tmrNumber_tag{
    gTmr1_c = 0,
    gTmr2_c,
    gTmr3_c,
    gTmrMax_c
  } tmrNumber_t;
  
  typedef  enum tmrEvent_tag{
    gTmrOverEvent_c = 0,
    gTmrChannel0Event_c,
    gTmrChannel1Event_c,
    gTmrChannel2Event_c,
    gTmrChannel3Event_c,
    gTmrChannel4Event_c,
    gTmrChannel5Event_c,
    gTmrMaxEvent_c
  } tmrEvent_t;
  
  typedef enum tmrChannel_tag{
    gTmrChannel0_c = 0,
    gTmrChannel1_c,
    gTmrChannel2_c,
    gTmrChannel3_c,
    gTmrChannel4_c,
    gTmrChannel5_c,
    gTmrMaxChannel_c
  } tmrChannel_t;
#endif

#ifdef gMc1323xPlatform_d 
  typedef enum tmrNumber_tag{
    gTmr1_c = 0,
    gTmr2_c,
    gTmr3_c,
    gTmr4_c,
    gTmrMax_c
  } tmrNumber_t;
  
  typedef  enum tmrEvent_tag{
    gTmrOverEvent_c = 0,
    gTmrChannel0Event_c,
    gTmrMaxEvent_c
  } tmrEvent_t;
  
  typedef enum tmrChannel_tag{
    gTmrChannel0_c = 0,
    gTmrMaxChannel_c
  } tmrChannel_t;
#endif

typedef enum tmrErr_tag{
  gTmrErrNoError_c = 0,                 
  gTmrErrNullPointer_c,
  gTmrErrInvalidParameter_c,    
  gTmrErrMax_c
} tmrErr_t;

typedef enum tmrChannelMode_tag{
    gTmrInputCapture_c = 0,
    gTmrOutputCompare_c,
    gTmrEdgeAlignedPwm_c,
    gTmrCenterAlignedPwm_c,
    gTmrMaxMode_c
} tmrChannelMode_t;

typedef enum tmrClkSrc_tag{
  gTmrNotClkSrc_c = 0,
  gTmrBusRateClk_c,
  gTmrFixedSysClk_c,
  gTmrExternClk_c,
  gTmrMaxClkSrc_c
} tmrClkSrc_t;


typedef enum tmrClkDivisor_tag{
  gTmrClkDivBy1_c = 0,
  gTmrClkDivBy2_c,
  gTmrClkDivBy4_c,
  gTmrClkDivBy8_c,
  gTmrClkDivBy16_c,
  gTmrClkDivBy32_c,
  gTmrClkDivBy64_c,
  gTmrClkDivBy128_c,
  gTmrMaxClkDiv_c
} tmrClkDivisor_t;


typedef enum tmrInCaptEdge_tag{
  gTmrPinNotUsedForInCapt = 0,
  gTmrInCaptOnRisingEdge_c,
  gTmrInCaptOnFallingEdge_c,
  gTmrInCaptOnRisingOrFallingEdge_c,
  gTmrMaxInCaptEdge_c
} tmrInCaptEdge_t;


typedef enum tmrOutCompState_tag{
  gTmrPinNotUsedForOutComp_c = 0,
  gTmrToggleOut_c,
  gTmrClearOut_c,
  gTmrSetOut_c,
  gTmrMaxOutCompState_c
} tmrOutCompState_t;


typedef enum tmrPwmOut_tag{
  gTmrPinNotUsed_c = 0,
  gTmrLowTruePulses_c,
  gTmrHighTruePulses_c,
  gTmrMaxPwmOut_c
} tmrPwmOut_t;


typedef union tmrPinConfig_tag{
  tmrInCaptEdge_t    tmrInCaptEdge;
  tmrOutCompState_t  tmrOutCompState;
  tmrPwmOut_t        tmrPwmOut;
} tmrPinConfig_t;


#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near tmrCallbackFunction_t)(void);
#else
  typedef void(* __near tmrCallbackFunction_t)(void);
#endif


typedef struct tmrChannelConfig_tag{
   tmrChannel_t          tmrChannel;
   tmrChannelMode_t      tmrChannOptMode;
   tmrPinConfig_t        tmrPinConfig;
   uint16_t              tmrCompareVal;
} tmrChannelConfig_t;



/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Initializes all timers as disabled and the callback functions as NULL.
*
* The routine limitations:
*    This routine must be call previous to any other timer function.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
************************************************************************************/

extern tmrErr_t Tmr_Init(void);

/************************************************************************************
* Tmr_SetCallbackFunc
*
* This function sets the callback function for the timer events when a hardware
* interrupt occurs or the task function detects a timer flag enabled.
*
* Return value:
*   An rtiErr_t value with the result of the operation.

************************************************************************************/

extern tmrErr_t Tmr_SetCallbackFunc(
  tmrNumber_t tmrNumber,              //IN:Number of timer to set the callback.
  tmrEvent_t tmrEvent,                //IN:Timer event associated to the callback.
  tmrCallbackFunction_t ptmrCallback  //IN:Pointer to the callback function.
);
 

/************************************************************************************
* Tmr_SetChannelConfig function configures the Timer channels by setting the 
* operation mode, the edge or level for the pin associated to the channel and the
* compare value. Also set the interruption bit if the interruption mode was 
* selected for this module.
* 
* Interface assumptions:
*    This routine considers that the operation mode (polling or interruption) was 
*    selected previous to be called.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
************************************************************************************/

extern tmrErr_t Tmr_SetChannelConfig(
  tmrNumber_t         tmrNumber,   //IN:Number of timer where is the channel to configure.
  tmrChannelConfig_t *pTmrConfig   //IN:Pointer to structure with the configuration parameters.
);

/************************************************************************************
* This function configures the clock source and clock divisor to set a specific 
* operation frequency.  
*
* The routine limitations:
*   This function is not implemented for the MC1323x platform because this platform
*   only have one possible source clock.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
************************************************************************************/

#ifndef gMc1323xPlatform_d 
extern tmrErr_t Tmr_SetClkConfig(
  tmrNumber_t      tmrNumber,       //IN:Number of timer to configure
  tmrClkSrc_t      tmrClkSrc,       //IN:Clock source to be set.
  tmrClkDivisor_t  tmrClkDivisor    //IN:Clock divisor to be set.
);
#endif

/************************************************************************************
* This function disables the timer module passed as parameter.  
*
* The routine limitations:
*   This function disables one timer per call.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
************************************************************************************/

extern tmrErr_t Tmr_Disable(
  tmrNumber_t tmrNumber             //IN:Number of timer to be disbaled.
);

/************************************************************************************
* This function enables the timer module passed as parameter setting the bus clock as
* source clock, the clock divisor and module value for overflow events. Also
* the interrupt for overflow event is enabled if this mode is selected.
*
* Interface assumptions:
*    This routine does not validate that a callback function was set previously 
*    to enable the interruption. If a callback function is required use the 
*    Tmr_SetCallbackFunc() function. 
*
*
* The routine limitations:
*   This function enables the timer with the bus clock as source, if is required to
*   use a different source, the function Tmr_SetClkConfig should be call.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
************************************************************************************/

extern tmrErr_t Tmr_Enable(
  tmrNumber_t       tmrNumber,         //IN:Number of timer to be enabled.
  tmrClkDivisor_t   tmrClkDivisor,     //IN:Clock divisor to be set.
  uint16_t          tmrOverflowValue   //IN:Value for the timer module.
);

#pragma CODE_SEG DEFAULT


/************************************************************************************
* This function is the interrupt request routine for timer 1.
*
* The routine limitations:
*   This function operates as interrupt routine only when this mode is selected, in
*   other case it is the task function that must be polling the status flag.
*
* Return value:
*   None.
*
*
************************************************************************************/


#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
extern INTERRUPT_KEYWORD void Tmr_Tmr1Isr(void);
#pragma CODE_SEG DEFAULT
#else
extern void Tmr_Tmr1Task(void);
#endif


/************************************************************************************
* This function is the interrupt request routine for timer 2.
*
* The routine limitations:
*   This function operates as interrupt routine only when this mode is selected, in
*   other case it is the task function that must be polling the status flag.
*
* Return value:
*   None.
*
*
************************************************************************************/


#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
extern INTERRUPT_KEYWORD void Tmr_Tmr2Isr(void);
#pragma CODE_SEG DEFAULT
#else
extern void Tmr_Tmr2Task(void);
#endif


/************************************************************************************
* This function is the interrupt request routine for timer 3.
*
* The routine limitations:
*   This function operates as interrupt routine only when this mode is selected, in
*   other case it is the task function that must be polling the status flag.
*
* Return value:
*   None.
*
*
************************************************************************************/

#ifndef  gMcs08Gt60Platform_d 
#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
extern INTERRUPT_KEYWORD void Tmr_Tmr3Isr(void);
#pragma CODE_SEG DEFAULT
#else
extern void Tmr_Tmr3Task(void);
#endif


/************************************************************************************
* This function is the interrupt request routine for the timer 4.
*
* The routine limitations:
*   This function operates as interrupt routine only when this mode is selected, in
*   other case it is the task function that must be polling the status flag.
*
* Return value:
*   None.
*
*
************************************************************************************/

#ifdef  gMc1323xPlatform_d 
#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
extern INTERRUPT_KEYWORD void Tmr_Tmr4Isr(void);
#pragma CODE_SEG DEFAULT
#else
extern void Tmr_Tmr4Task(void);
#endif

#endif
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


#endif  /* _TIMER_INTERFACE_H_ */

