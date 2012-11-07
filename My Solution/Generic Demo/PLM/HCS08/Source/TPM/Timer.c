/************************************************************************************
* Source file for Timer driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
************************************************************************************/

#include "Timer.h"


  
/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private defines and macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private constants
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

 /************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if TRUE == gTimerSupported_d

#ifdef gMcs08Gt60Platform_d
  const uint8_t mu8TmrMaxChannel[] = {mTmr1MaxChannel_c,mTmr2MaxChannel_c};

  static uint8_t *mu8TmrStartRegs[]={&TPM1SC,&TPM2SC};
                  

  static uint8_t *mu8TmrChannStatCntlReg[gTmrMax_c][mTmr2MaxChannel_c] = 
                                  {&TPM1C0SC,&TPM1C1SC,&TPM1C2SC,NULL,NULL,
                                   &TPM2C0SC,&TPM2C1SC,&TPM2C2SC,&TPM2C3SC,&TPM2C4SC};
                                   
  static uint8_t *mu8TmrChannValueHighReg[gTmrMax_c][mTmr2MaxChannel_c] = 
                                  {&TPM1C0VH,&TPM1C1VH,&TPM1C2VH,NULL,NULL,
                                   &TPM2C0VH,&TPM2C1VH,&TPM2C2VH,&TPM2C3VH,&TPM2C4VH};

  static uint8_t *mu8TmrChannValueLowReg[gTmrMax_c][mTmr2MaxChannel_c] = 
                                  {&TPM1C0VL,&TPM1C1VL,&TPM1C2VL,NULL,NULL,
                                   &TPM2C0VL,&TPM2C1VL,&TPM2C2VL,&TPM2C3VL,&TPM2C4VL};
#endif

#ifdef gMcs08qe128Platform_d
  const uint8_t mu8TmrMaxChannel[] = {mTmr1MaxChannel_c,mTmr2MaxChannel_c,mTmr3MaxChannel_c};
  
  static uint8_t *mu8TmrStartRegs[]={&TPM1SC,&TPM2SC,&TPM3SC};
  
  static uint8_t *mu8TmrChannStatCntlReg[gTmrMax_c][mTmr3MaxChannel_c] = 
                                  {&TPM1C0SC,&TPM1C1SC,&TPM1C2SC,NULL,NULL,NULL,
                                   &TPM2C0SC,&TPM2C1SC,&TPM2C2SC,NULL,NULL,NULL,
                                   &TPM3C0SC,&TPM3C1SC,&TPM3C2SC,&TPM3C4SC,&TPM3C4SC,&TPM3C5SC};
                                   
  static uint8_t *mu8TmrChannValueHighReg[gTmrMax_c][mTmr3MaxChannel_c] = 
                                  {&TPM1C0VH,&TPM1C1VH,&TPM1C2VH,NULL,NULL,NULL,
                                   &TPM2C0VH,&TPM2C1VH,&TPM2C2VH,NULL,NULL,NULL,
                                   &TPM3C0VH,&TPM3C1VH,&TPM3C2VH,&TPM3C4VH,&TPM3C4VH,&TPM3C5VH};

  static uint8_t *mu8TmrChannValueLowReg[gTmrMax_c][mTmr3MaxChannel_c] = 
                                  {&TPM1C0VL,&TPM1C1VL,&TPM1C2VL,NULL,NULL,NULL,
                                   &TPM2C0VL,&TPM2C1VL,&TPM2C2VL,NULL,NULL,NULL,
                                   &TPM3C0VL,&TPM3C1VL,&TPM3C2VL,&TPM3C4VL,&TPM3C4VL,&TPM3C5VL};
#endif


#ifdef gMc1323xPlatform_d
  static uint8_t *mu8TmrStartRegs[] = {&TPM1SC,&TPM2SC,&TPM3SC,&TPM4SC};
#endif


static tmrCallbackFunction_t mTmrCallbackFuncEvent[gTmrMax_c][gTmrMaxEvent_c];

#endif //gTimerSupported_d

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Tmr_Init
*
* This function initializes all timers as disabled and the callback functions as NULL.
*
************************************************************************************/
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_Init(void){
  uint8_t u8TmrCount;
  uint8_t u8EventCount;
  tmrModuleRegs_t *tempTmrModule;
  
 
  for(u8TmrCount = 0; u8TmrCount < gTmrMax_c; u8TmrCount++){
    tempTmrModule = (tmrModuleRegs_t *)(mu8TmrStartRegs[u8TmrCount]);
    tempTmrModule->TPMxSC = mTmrDisabled_c;
    for(u8EventCount = 0; u8EventCount < gTmrMaxEvent_c; u8EventCount++){
      mTmrCallbackFuncEvent[u8TmrCount][u8EventCount] = NULL;
    }
  }
  
  return gTmrErrNoError_c;
}
#else

tmrErr_t Tmr_Init(void)
{
   return gTmrErrNoError_c;
}

#endif //gTimerSupported_d  

/************************************************************************************
* Tmr_SetChannelConfig
*
* Configure the Timer channels by setting the operation mode, the edge or level for
* the pin associated to the channel and the compare value. Also it sets the 
* interruption bit if the interruption mode was selected for this module.
*
************************************************************************************/
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_SetChannelConfig(
  tmrNumber_t         tmrNumber,
  tmrChannelConfig_t  *pTmrChanConfig
){

  tmrModuleRegs_t *tempTmrModule;
  tempTmrModule = (tmrModuleRegs_t *)(mu8TmrStartRegs[tmrNumber]);  
     
  if((NULL == pTmrChanConfig)){
    return gTmrErrNullPointer_c;
  } 
  
  else{
    /*Validation for valid parameters*/
    if (gTmrMax_c > tmrNumber){
#ifdef gMc1323xPlatform_d 
      if((gTmrMaxChannel_c > pTmrChanConfig->tmrChannel) && 
#else     
      if((mu8TmrMaxChannel[tmrNumber] > pTmrChanConfig->tmrChannel) && 
#endif
        (gTmrMaxMode_c > pTmrChanConfig->tmrChannOptMode)){
        
        /*Operation Mode and input edge or output level are set in TPMxCnSC*/        
        
#ifndef gMc1323xPlatform_d        
        *mu8TmrChannStatCntlReg[tmrNumber][pTmrChanConfig->tmrChannel] =\
                           ((pTmrChanConfig->tmrChannOptMode << mChannOptModeShiftBits_c) |\
                            (pTmrChanConfig->tmrPinConfig.tmrInCaptEdge << mPinStateShiftBits_c)
  #if gTimerMode_d == gInterruption_c
                            |(mChannelInterruptEnBit_c)
  #endif
                           );
#endif                           
#ifdef gMc1323xPlatform_d
        tempTmrModule->TPMxC0SC = \
                           ((pTmrChanConfig->tmrChannOptMode << mChannOptModeShiftBits_c) |\
                            (pTmrChanConfig->tmrPinConfig.tmrInCaptEdge << mPinStateShiftBits_c)
  #if gTimerMode_d == gInterruption_c
                            |(mChannelInterruptEnBit_c)
  #endif
                           );
#endif                           

        /*The center Aligned Bit is enabled in TPMxSC if this mode was selected*/                       
        if(gTmrCenterAlignedPwm_c == pTmrChanConfig->tmrChannOptMode){
          *mu8TmrStartRegs[tmrNumber] |= mTmrCenterAlignedPwmBit_c;
        }
         
        /*Output Compare Value is set in TPMxCnVH and TPMxCnVL*/
        
#ifndef gMc1323xPlatform_d
        *mu8TmrChannValueHighReg[tmrNumber][pTmrChanConfig->tmrChannel] =\
                                          (pTmrChanConfig->tmrCompareVal >> mTmrMSBShiftBits_c); 
        *mu8TmrChannValueLowReg[tmrNumber][pTmrChanConfig->tmrChannel] =\
                                          (uint8_t)pTmrChanConfig->tmrCompareVal; 
#endif                                       
                                          
#ifdef gMc1323xPlatform_d
        tempTmrModule->TPMxC0VH = (pTmrChanConfig->tmrCompareVal >> mTmrMSBShiftBits_c); 
        tempTmrModule->TPMxC0VL = (uint8_t)pTmrChanConfig->tmrCompareVal; 
#endif
                                          
        return gTmrErrNoError_c;   
      } 
      else{
        return gTmrErrInvalidParameter_c;        
      }
    }
    else{
      return gTmrErrInvalidParameter_c;
    }
  }
} 
#else

tmrErr_t Tmr_SetChannelConfig(tmrNumber_t tmrNumber, tmrChannelConfig_t  *pTmrChanConfig)
{
   (void)tmrNumber;
   (void)pTmrChanConfig;
   return gTmrErrNoError_c;
}

#endif //gTimerSupported_d 

/************************************************************************************
* Tmr_SetCallbackFunc
*
* This function sets the callback function for the timer events when a hardware
* interrupt occurs or the task function detects a timer flag enabled.
*
************************************************************************************/
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_SetCallbackFunc(
  tmrNumber_t tmrNumber,
  tmrEvent_t tmrEvent,
  tmrCallbackFunction_t ptmrCallback
){

  if ((gTmrMax_c > tmrNumber) && (gTmrMaxEvent_c > tmrEvent)){
    mTmrCallbackFuncEvent[tmrNumber][tmrEvent] = ptmrCallback;
    return gTmrErrNoError_c;
  }
  else{
    return gTmrErrInvalidParameter_c;
  }
  
} 
#else

tmrErr_t Tmr_SetCallbackFunc(tmrNumber_t tmrNumber, tmrEvent_t tmrEvent,tmrCallbackFunction_t ptmrCallback)
{
   (void)tmrNumber;
   (void)tmrEvent;
   (void)ptmrCallback;
   return gTmrErrNoError_c;
}

#endif //gTimerSupported  
     
/************************************************************************************
* Tmr_SetClkConfig
*
* This function configures the clock source and clock divisor to set a specific 
* operation frequency.  
*
************************************************************************************/
#ifndef gMc1323xPlatform_d 
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_SetClkConfig(
  tmrNumber_t      tmrNumber,
  tmrClkSrc_t      tmrClkSrc, 
  tmrClkDivisor_t  tmrClkDivisor
){

  if((gTmrMax_c > tmrNumber) && (gTmrMaxClkDiv_c > tmrClkDivisor) && (gTmrMaxClkSrc_c> tmrClkSrc)){
    
    *mu8TmrStartRegs[tmrNumber] |= ((tmrClkSrc << mClkShiftBits_c) | (tmrClkDivisor));
        
    return gTmrErrNoError_c;
  } 
  
  else{
    return gTmrErrInvalidParameter_c;
  } 
}  
#else

tmrErr_t Tmr_SetClkConfig(tmrNumber_t tmrNumber, tmrClkSrc_t tmrClkSrc,tmrClkDivisor_t  tmrClkDivisor)
{
   (void)tmrNumber;
   (void)tmrClkSrc;
   (void)tmrClkDivisor;
   return gTmrErrNoError_c;
}

#endif //gTimerSupported     
#endif

/************************************************************************************
* Tmr_Disable
*
* This function disables the timer module passed as parameter.  
*
************************************************************************************/
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_Disable(tmrNumber_t tmrNumber){

  if((gTmrMax_c > tmrNumber)){
    
    *mu8TmrStartRegs[tmrNumber] = mTmrDisabled_c;
    return gTmrErrNoError_c;
  } 
  
  else{
    return gTmrErrInvalidParameter_c;
  }
}
#else

tmrErr_t Tmr_Disable(tmrNumber_t tmrNumber)
{
   (void)tmrNumber;
   return gTmrErrNoError_c;
}

#endif //gTimerSupported   

/************************************************************************************
* Tmr_Enable
*
* This function enables the timer module passed as parameter setting the bus clock as
* source clock, the clock divisor and module value for overflow events.
*
************************************************************************************/
#if TRUE == gTimerSupported_d
tmrErr_t Tmr_Enable(
  tmrNumber_t      tmrNumber, 
  tmrClkDivisor_t  tmrClkDivisor,
  uint16_t         tmrOverflowValue
){

  tmrModuleRegs_t *tempTmrModuleReg;
  
  if((gTmrMax_c > tmrNumber) && (gTmrMaxClkDiv_c > tmrClkDivisor)){
  
    tempTmrModuleReg = (tmrModuleRegs_t *)(mu8TmrStartRegs[tmrNumber]);
    //tempTmrModuleReg->TPMxCNTH = 0;
    tempTmrModuleReg->TPMxSC |= ((gTmrBusRateClk_c << mClkShiftBits_c) | (tmrClkDivisor) 
#if gTimerMode_d == gInterruption_c    
                                       |(mTmrOverInterruptEnBit_c)    
#endif   
      );
      
    tempTmrModuleReg->TPMxMODH = tmrOverflowValue >> mTmrMSBShiftBits_c; 
    tempTmrModuleReg->TPMxMODL = (uint8_t)tmrOverflowValue;   
    
    return gTmrErrNoError_c;
  }
  
  else{
    return gTmrErrInvalidParameter_c;
  }
} 
#else

tmrErr_t Tmr_Enable(tmrNumber_t tmrNumber, tmrClkDivisor_t  tmrClkDivisor, uint16_t tmrOverflowValue)
{
   (void)tmrNumber;
   (void)tmrClkDivisor;
   (void)tmrOverflowValue;
   return gTmrErrNoError_c;
}

#endif //gTimerSupported 
 
#pragma CODE_SEG DEFAULT
  
/************************************************************************************
* Tmr_Tmr1Isr or Tmr_Tmr1Task
*
* This function is the interrupt request routine for timer 1.
*
************************************************************************************/
#if TRUE == gTimerSupported_d
#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Tmr_Tmr1Isr(){
#else
void Tmr_Tmr1Task(){
#endif
                
  if( mTmr1StatCntlReg & mTmrOverInterruptFlag_c ){
    mTmr1StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr1_c][gTmrOverEvent_c] != NULL){
      mTmrCallbackFuncEvent[gTmr1_c][gTmrOverEvent_c]();
    }
  } 
  
  else if( mTmr1Chan0StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr1Chan0StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel0Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel0Event_c]();
    }
  } 

#ifndef gMc1323xPlatform_d
  else if( mTmr1Chan1StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr1Chan1StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel1Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel1Event_c]();
    }
  } 
  
  else if( mTmr1Chan2StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr1Chan2StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel2Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr1_c][gTmrChannel2Event_c]();
    }
  } 
#endif
}          
#pragma CODE_SEG DEFAULT
 
/************************************************************************************
* Tmr_Tmr2Isr or Tmr_Tmr2Task
*
* This function is the interrupt request routine for timer 2.
*
************************************************************************************/

#if gTimerMode_d == gInterruption_c
#pragma CODE_SEG ISR_SEGMENT 
INTERRUPT_KEYWORD void Tmr_Tmr2Isr(){
#else
void Tmr_Tmr2Task(){
#endif

  if( mTmr2StatCntlReg & mTmrOverInterruptFlag_c ){
    mTmr2StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrOverEvent_c] != NULL){    
      mTmrCallbackFuncEvent[gTmr2_c][gTmrOverEvent_c]();
    }
  } 
  
  else if( mTmr2Chan0StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr2Chan0StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel0Event_c] != NULL){    
      mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel0Event_c]();
    }
  } 

#ifndef gMc1323xPlatform_d
  else if( mTmr2Chan1StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr2Chan1StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel1Event_c] != NULL){    
      mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel1Event_c]();
    }
  } 
  
  else if( mTmr2Chan2StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr2Chan2StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel2Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel2Event_c]();
    }
  } 
#endif

#ifdef gMcs08Gt60Platform_c
  else if( mTmr2Chan3StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr2Chan3StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel3Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel3Event_c]();
    }
  } 
  
  else if( mTmr2Chan4StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr2Chan4StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel4Event_c] != NULL){    
      mTmrCallbackFuncEvent[gTmr2_c][gTmrChannel4Event_c]();
    }
  }
#endif    
}
#pragma CODE_SEG DEFAULT                  

/************************************************************************************
* Tmr_Tmr3Isr or Tmr_Tmr3Task
*
* This function is the interrupt request routine for timer 3.
*
************************************************************************************/

#ifndef  gMcs08Gt60Platform_d 
 
#if gTimerMode_d == gInterruption_c
#pragma CODE_SEG ISR_SEGMENT 
INTERRUPT_KEYWORD void Tmr_Tmr3Isr(){
#else
void Tmr_Tmr3Task(){
#endif

  if( mTmr3StatCntlReg & mTmrOverInterruptFlag_c ){
    mTmr3StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrOverEvent_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrOverEvent_c]();
    }
  } 
  
  else if( mTmr3Chan0StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan0StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel0Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel0Event_c]();
    }
  } 

#ifndef  gMc1323xPlatform_d
  else if( mTmr3Chan1StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan1StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel1Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel1Event_c]();
    }
  } 
  
  else if( mTmr3Chan2StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan2StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel2Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel2Event_c]();
    }
  } 
  
  else if( mTmr3Chan3StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan3StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel3Event_c] != NULL){    
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel3Event_c]();
    }
    
  } 
  
  else if( mTmr3Chan4StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan4StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel4Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel4Event_c]();
    }
  } 
  
  else if( mTmr3Chan5StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr3Chan5StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel4Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr3_c][gTmrChannel4Event_c]();
    }
  } 
#endif

}
#pragma CODE_SEG DEFAULT   
#endif  

/************************************************************************************
* Tmr_Tmr4Isr or Tmr_Tmr4Task
*
* This function is the interrupt request routine for timer 4.
*
************************************************************************************/

#ifdef gMc1323xPlatform_d

#if gTimerMode_d == gInterruption_c 
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Tmr_Tmr4Isr(){
#else
void Tmr_Tmr4Task(){
#endif

  if( mTmr4StatCntlReg & mTmrOverInterruptFlag_c ){
    mTmr4StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr4_c][gTmrOverEvent_c] != NULL){
      mTmrCallbackFuncEvent[gTmr4_c][gTmrOverEvent_c]();
    }
  } 
  else if( mTmr4Chan0StatCntlReg & mTmrChannInterruptFlag_c ){
    mTmr4Chan0StatCntlReg &= mTmrClearInterruptFlagMask_c;
    if(mTmrCallbackFuncEvent[gTmr4_c][gTmrChannel0Event_c] != NULL){
      mTmrCallbackFuncEvent[gTmr4_c][gTmrChannel0Event_c]();
    }
  } 
   
} 
#pragma CODE_SEG DEFAULT
#endif 
#endif //gTimerSupported
/************************************************************************************/

