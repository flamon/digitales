/************************************************************************************
* Source file for Real Time Interrupt driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
************************************************************************************/

#include "RTI.h"

#pragma MESSAGE DISABLE C2705


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
#if TRUE == gRtiSupported_d
static  rtiCallbackFunc_t mpRtiCallback;
#endif //gRtiSupported_d
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


/************************************************************************************/
#if TRUE == gRtiSupported_d
rtiErr_t Rti_Init(rtiCallbackFunc_t pCallback){
  
  if( NULL == pCallback ){
    return gRtiErrNullPointer_c;
  } 
  else{
    mpRtiCallback = pCallback;
#if  gRtiMode_d == gInterruption_c
    mRtiStatCntlReg |= mRtiInterrupEnBit_c;
#else
    mRtiStatCntlReg = mNoneConfig_c;
#endif
  }
  
  return gRtiErrNoError_c;
} 
#else

rtiErr_t Rti_Init(rtiCallbackFunc_t pCallback)
{
  (void)pCallback;
  return gRtiErrNoError_c;
}

#endif //gRtiSupported_d

/************************************************************************************/
#if TRUE == gRtiSupported_d
rtiErr_t Rti_SetConfig(rtiConfig_t * pRtiConfig){

  uint8_t u8TempReg;
    
  if(NULL != pRtiConfig){
    if((pRtiConfig->rtiClockSrc < gRtiMaxClkSrc_c) &&
       (pRtiConfig->rtiTimePeriod.rti1kHzIntClkPeriod < gRti1kHzIntClkMaxPeriod_c)){
      
      u8TempReg = mRtiStatCntlReg;
      u8TempReg &= mRtiConfigMask_c;
      u8TempReg |= (uint8_t)(pRtiConfig->rtiClockSrc << mRtiClkSrcShiftBits_c);
      u8TempReg |= (uint8_t)(pRtiConfig->rtiTimePeriod.rti1kHzIntClkPeriod);
      mRtiStatCntlReg = u8TempReg;

#ifndef gMcs08Gt60Platform_d
      mRtiModuleReg = pRtiConfig->rtiCntMod;
#endif
      return gRtiErrNoError_c;
    } 
    
    else{
      return gRtiErrInvalidParameter_c;
    }
  } 
  
  else{
    return gRtiErrNullPointer_c;
  }
}
#else

rtiErr_t Rti_SetConfig(rtiConfig_t * pRtiConfig)
{
  (void)pRtiConfig;
  return gRtiErrNoError_c;
}

#endif //gRtiSupported_d  


/************************************************************************************/
#if TRUE == gRtiSupported_d
rtiErr_t Rti_GetConfig(rtiConfig_t * pRtiConfig){

  uint8_t u8TempReg;
    
  if(NULL != pRtiConfig ){
    
    u8TempReg = mRtiStatCntlReg;
    pRtiConfig->rtiTimePeriod.rti1kHzIntClkPeriod = (u8TempReg & mRtiPeriodMask_c);
    
    if(mNoneConfig_c == pRtiConfig->rtiTimePeriod.rti1kHzIntClkPeriod){
      return  gRtiErrRtiIsNotConfig_c;
    } 
    else{
      pRtiConfig->rtiClockSrc = ((u8TempReg & mRtiClockSrcMask_c) >> mRtiClkSrcShiftBits_c);

#ifndef gMcs08Gt60Platform_d
      pRtiConfig->rtiCntMod = mRtiModuleReg;
#endif      
      
      return gRtiErrNoError_c;
    }
  } 
  
  else{
    return gRtiErrNullPointer_c;
  }
}
#else

rtiErr_t Rti_GetConfig(rtiConfig_t * pRtiConfig)
{
  (void)pRtiConfig;
  return gRtiErrNoError_c;
}

#endif //gRtiSupported_d    


/************************************************************************************/
#if TRUE == gRtiSupported_d

#if  gRtiMode_d == gInterruption_c

#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Rti_Isr(void){

#else if gRtiMode_d == gRtiPoll_c

void Rti_Task(void){
  
 if(mRtiStatCntlReg & mRtiInterruptFlag_c){
#endif
  
    mRtiStatCntlReg |=  mRtiInterruptAck_c;
#if gRtiMode_d == gPolling_c
  }
#endif

   mpRtiCallback();
  
} 

#endif //gRtiSupported_d  

#pragma CODE_SEG DEFAULT
/************************************************************************************/

