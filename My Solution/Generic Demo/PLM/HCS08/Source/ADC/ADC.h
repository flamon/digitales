/*****************************************************************************
* ADC Configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _ADC_H_               
#define _ADC_H_

#include "ADC_Interface.h"

#define gAdcParamAndInitValidation_d TRUE

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

/*ADC module states*/
typedef enum adcStatus_tag {
  mAdcNotInit_c,
  mAdcIdle_c,
  mAdcSingleConversionStarted_c,
  mAdcContinuousConversionRunning_c,
  mAdcMaxState_c,
}adcStatus_t;

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/
#define mAdcRes8Mask_c        0x20
#define mConvCompFlagMask_c   0x80
#define mIntEnableMask_c      0x40
#define mContConversionMask_c 0x20
#define mPowerUpMask_c        0x80
#define mStopConversion_c     0x1F

#if defined(gMcs08Gt60Platform_d)   
 
 #define InitAdcModule(RateDivisor,Mode,PinSelect)       \
  ATD1C = (0x40|((uint8_t)RateDivisor & 0x0F));          \
  if(gAdcMode8bit_c == Mode) ATD1C |= mAdcRes8Mask_c;    \
  ATD1PE = (uint8_t)(PinSelect & 0x000000FF); 

 #define mAdc8BitsModeSelected_c  (ATD1C & mAdcRes8Mask_c)
#elif defined(gMcs08qe128Platform_d)

 #define InitAdcModule(RateDivisor, Mode, PinSelect)     \
  ADCSC1 = 0x1F;                                         \
  ADCSC2 = 0x00;                                         \
  ADCCFG = 0x80;                                         \
  ADCCFG |=(uint8_t)((RateDivisor & 0x03)<<5);           \
  ADCCFG |=(uint8_t)((Mode & 0x03)<<5);                  \
  APCTL1 = (uint8_t)(PinSelect & 0x000000FF);            \
  APCTL2 = (uint8_t)((PinSelect>>8) & 0x000000FF);       \
  APCTL2 = (uint8_t)((PinSelect>>16) & 0x000000FF); 

#endif

#if defined(gMc1323xPlatform_d)
    #define InitAdcModule(RateDivisor,Mode,PinSelect)            /* This is a dummy define. There is no ADC in MC1323x platform */
    #define mAdc8BitsModeSelected_c                              /* This is a dummy define. There is no ADC in MC1323x platform */
#endif


#endif  //__ADC_H__ 

  