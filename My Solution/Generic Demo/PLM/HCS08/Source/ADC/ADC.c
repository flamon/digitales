/*****************************************************************************
* ADC driver implementation.
* Driver to perform Analog to Digital Conversions
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "ADC.h"

#if gAdcSupported_d == TRUE

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

adcStatus_t    adcState = mAdcNotInit_c;
adcCallback_t  mConvCallBack = NULL;


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* ADC_Init
*
************************************************************************************/
adcErrors_t ADC_Init
( 
  adcConversionRateDivisor_t adcRateDivisor,
  adcConversionModes_t       adcMode,
  uint32_t                   u32AdcPinSelect
)
{
#if (TRUE == gAdcParamAndInitValidation_d)
  if((gAdcRateMaxDivisor_c <= adcRateDivisor) || (gAdcModeMax_c<= adcMode) || (!u32AdcPinSelect))
  {
    return gAdcErrInvalidParam_c; 
  }
#endif  

  adcState = mAdcIdle_c;
  InitAdcModule(adcRateDivisor, adcMode, u32AdcPinSelect);

  return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetSingleConversion
*
************************************************************************************/
adcErrors_t ADC_SetSingleConversion
(
 adcChannel_t    adcChannel, 
 adcCallback_t   convCallBack   
)
{
#if (TRUE == gAdcParamAndInitValidation_d)
  if((gAdcMaxChannel_c <= adcChannel) || (NULL  == convCallBack))
  {
    return gAdcErrInvalidParam_c; 
  }
#endif  
  if(mAdcIdle_c != adcState)
  {
    return gAdcErrModuleBusy_c;
  }
  adcState = mAdcSingleConversionStarted_c;
  mConvCallBack = convCallBack;
  
#if defined(gMcs08Gt60Platform_d) 
  ATD1C |= mPowerUpMask_c; 
  ATD1SC = mIntEnableMask_c|((uint8_t)(adcChannel & 0x1F));
#elif defined(gMcs08qe128Platform_d)
  ADCSC1 = mIntEnableMask_c|((uint8_t)(adcChannel & 0x1F));
#endif
   
  return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetSingleConversionBlocking
*
************************************************************************************/
adcErrors_t ADC_SetSingleConversionBlocking
(
 adcChannel_t    adcChannel, 
 uint16_t *      pu16ConvResult   
)
{
#if (TRUE == gAdcParamAndInitValidation_d)
  if((gAdcMaxChannel_c <= adcChannel) || (NULL  == pu16ConvResult))
  {
    return gAdcErrInvalidParam_c; 
  }
#endif  
  
  if(mAdcIdle_c != adcState)
  {
    return gAdcErrModuleBusy_c;
  }

#if defined(gMcs08Gt60Platform_d) 
  ATD1C |= mPowerUpMask_c; 
  ATD1SC = 0x00|((uint8_t)(adcChannel & 0x1F));
  while(!(ATD1SC & mConvCompFlagMask_c));
  if(mAdc8BitsModeSelected_c)
  {
    *pu16ConvResult = (0x00FF & (uint16_t)ATD1RH);
  }
  else
  {
    *pu16ConvResult = ATD1R;
  }
#elif defined(gMcs08qe128Platform_d)
  ADCSC1 = 0x00|((uint8_t)(adcChannel & 0x1F));
  while(!(ADCSC1 & mConvCompFlagMask_c));
  *pu16ConvResult = ADCR;
#endif
   
  return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetContinuousConversions
*
************************************************************************************/
adcErrors_t ADC_SetContinuousConversions
(
 adcChannel_t    adcChannel, 
 adcCallback_t   convCallBack   
)
{
#if (TRUE == gAdcParamAndInitValidation_d)
  if((gAdcMaxChannel_c <= adcChannel) || (NULL  == convCallBack))
  {
    return gAdcErrInvalidParam_c; 
  }
#endif  
  if(mAdcIdle_c != adcState)
  {
    return gAdcErrModuleBusy_c;
  }
  adcState = mAdcContinuousConversionRunning_c;
  mConvCallBack = convCallBack;
  
#if defined(gMcs08Gt60Platform_d) 
  ATD1C |= mPowerUpMask_c; 
  ATD1SC = mContConversionMask_c|mIntEnableMask_c|((uint8_t)(adcChannel & 0x1F));
#elif defined(gMcs08qe128Platform_d)
  ADCSC1 = mContConversionMask_c|mIntEnableMask_c|((uint8_t)(adcChannel & 0x1F));
#endif
   
  return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_StopConversion
*
************************************************************************************/
void ADC_StopConversion
(
void
)
{
  adcState = mAdcIdle_c;

#if defined(gMcs08Gt60Platform_d) 
  ATD1SC = mStopConversion_c;
  ATD1C &= ~mPowerUpMask_c; 
#elif defined(gMcs08qe128Platform_d)
  ADCSC1 = mStopConversion_c;
#endif
}
/************************************************************************************
*
* ADC Interrupr service function
*
************************************************************************************/
#if gAdcMode_d == gAdcInterrupt_c   

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

INTERRUPT_KEYWORD void ADC_Isr(void)
{
#else
void ADC_Task(void)
{
 #if defined(gMcs08Gt60Platform_d) 
  if(ATD1SC & mConvCompFlagMask_c)
 #elif defined(gMcs08qe128Platform_d)
  if(ADCSC1 & mConvCompFlagMask_c)
 #endif
#endif
  {
    uint16_t u16ConvResult = 0;
  #if defined(gMcs08Gt60Platform_d) 
    if(mAdc8BitsModeSelected_c)
    {
      u16ConvResult = (0x00FF & (uint16_t)ATD1RH);
    }
    else
    {
      u16ConvResult = ATD1R;
    }
    
  #elif defined(gMcs08qe128Platform_d)
    u16ConvResult = ADCR;
  #endif
    if(mAdcSingleConversionStarted_c == adcState)
    {
      adcState = mAdcIdle_c;
    #if defined(gMcs08Gt60Platform_d) 
      ATD1SC = mStopConversion_c;
      ATD1C &= ~mPowerUpMask_c; 
    #elif defined(gMcs08qe128Platform_d)
      ADCSC1 = mStopConversion_c;
    #endif
    }
    mConvCallBack(u16ConvResult);
  }
}

#pragma CODE_SEG DEFAULT

/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/



#else                    /* gAdcSupported_d == FALSE */

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* ADC_Init
*
************************************************************************************/
adcErrors_t ADC_Init
( 
  adcConversionRateDivisor_t adcRateDivisor,
  adcConversionModes_t       adcMode,
  uint32_t                   u32AdcPinSelect
)
{
    (void)adcRateDivisor;
    (void)adcMode;
    (void)u32AdcPinSelect;
    return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetSingleConversion
*
************************************************************************************/
adcErrors_t ADC_SetSingleConversion
(
 adcChannel_t    adcChannel, 
 adcCallback_t   convCallBack   
)
{
    (void)adcChannel;
    (void)convCallBack;
    return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetSingleConversionBlocking
*
************************************************************************************/
adcErrors_t ADC_SetSingleConversionBlocking
(
 adcChannel_t    adcChannel, 
 uint16_t *      pu16ConvResult   
)
{
    (void)adcChannel;
    (void)pu16ConvResult;
    return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_SetContinuousConversions
*
************************************************************************************/
adcErrors_t ADC_SetContinuousConversions
(
 adcChannel_t    adcChannel, 
 adcCallback_t   convCallBack   
)
{
    (void)adcChannel;
    (void)convCallBack;
    return gAdcErrNoError_c;  
}

/************************************************************************************
*
* ADC_StopConversion
*
************************************************************************************/
void ADC_StopConversion
(
void
)
{
  
}
/************************************************************************************
*
* ADC Interrupr service function
*
************************************************************************************/
#if gAdcMode_d == gAdcInterrupt_c   

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

INTERRUPT_KEYWORD void ADC_Isr(void)
{
#else
void ADC_Task(void)
{
 #if defined(gMcs08Gt60Platform_d) 
  if(ATD1SC & mConvCompFlagMask_c)
 #elif defined(gMcs08qe128Platform_d)
  if(ADCSC1 & mConvCompFlagMask_c)
 #endif
#endif
  {
  
  }
}

#pragma CODE_SEG DEFAULT

#endif