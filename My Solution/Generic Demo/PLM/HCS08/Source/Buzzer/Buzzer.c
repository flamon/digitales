/*****************************************************************************                   
* Buzzer driver implementation.
* Driver to manage the buzzer of the following boards:
*   1320x-QE128-EVB
*   1321x-SRB and NCB
*   1323x RCM
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#include "Buzzer.h"

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

bool_t gBuzzerBusy;


#if gBuzzerSupported_d == TRUE

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
  
static uint16_t mPulseWidth;                         //Beep Volume
static uint16_t mPulseFreq;                          //Beep Tone
static uint16_t mTempPulseWidth;
static uint16_t mTempPulseFreq;                      //Beep Tone
static tmrChannelConfig_t mBuzzerTimerConfig;   
static uint16_t mDefaultFreq; 
static uint16_t mDefaultWidth;
static bool_t mPeriodState;
static uint16_t mTimerRounds;
static uint16_t mCountPeriod;
static bool_t mBuzzerBeepFlag;



/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/******************************************************************************
* Buzzer_Init 
*
*******************************************************************************/

void Buzzer_Init(void)
{    
    /*Timer configuration*/
    
    mBuzzerTimerConfig.tmrChannel = gBuzzerTimerChannel_c;
    mBuzzerTimerConfig.tmrChannOptMode= gTmrOutputCompare_c;
    mBuzzerTimerConfig.tmrPinConfig.tmrOutCompState = gTmrPinNotUsedForOutComp_c;
    (void)Tmr_SetCallbackFunc(gBuzzerTimer_c, gBuzzerTimerInterruptSource_c, (tmrCallbackFunction_t)Buzzer_TimerEvent);
    (void)Tmr_SetChannelConfig(gBuzzerTimer_c, &mBuzzerTimerConfig);
    mDefaultFreq = mBuzzerTone6_c;
    mPulseFreq = mDefaultFreq;
    mDefaultWidth = pulseWidth(mDefaultFreq, Buzzer_ValidateVol(gBuzzerVolume1_c));
    mPulseWidth = mDefaultWidth;
    mPeriodState = FALSE;
    gBuzzerBusy = FALSE;
    mCountPeriod = 0;  
}

/******************************************************************************
* Buzzer_ShortBeep 
*
*******************************************************************************/

void Buzzer_ShortBeep(void)
{
  mCountPeriod = 0;
  mTimerRounds = (uint16_t)calculateTimerRounds(gShortBeepLength_c);
  lowPeriodTimer();
}

/******************************************************************************
* Buzzer_LongBeep 
*
*******************************************************************************/

void Buzzer_LongBeep(void)
{
  mCountPeriod = 0;
  mTimerRounds = (uint16_t)calculateTimerRounds(gLongBeepLength_c);
  lowPeriodTimer();
  
}

/******************************************************************************
* Buzzer_SetBeepTone 
*
*******************************************************************************/

void Buzzer_SetBeepTone(buzzerTone_t beepTone)
{
  mPulseFreq = Buzzer_ValidateTone(beepTone);  
}

/******************************************************************************
* Buzzer_SetBeepVolume 
*
*******************************************************************************/

void Buzzer_SetBeepVolume(buzzerVolume_t beepVol)
{
  mPulseWidth = Buzzer_ValidateVol(beepVol);

}

/******************************************************************************
* Buzzer_Beep
*
*******************************************************************************/

void Buzzer_Beep(uint8_t beepSecs, buzzerTone_t beepTone, buzzerVolume_t beepVol)
{
  mBuzzerBeepFlag = TRUE;
  mTempPulseFreq = mPulseFreq;
  mTempPulseWidth = mPulseWidth;
  mPulseFreq = Buzzer_ValidateTone(beepTone);
  mPulseWidth = pulseWidth(mPulseFreq, Buzzer_ValidateVol(beepVol));
  mCountPeriod = 0;
  mTimerRounds = (uint16_t)calculateTimerRounds(beepSecs);
  lowPeriodTimer(); 

}

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 


/************************************************************************************
*************************************************************************************
* Module functions
*************************************************************************************
************************************************************************************/

void Buzzer_TimerEvent(void)
{  
  
  if(mCountPeriod == mTimerRounds)
  { 
    (void)Tmr_Disable(gBuzzerTimer_c);
        
    if(mBuzzerBeepFlag)
    {
      mPulseFreq = mTempPulseFreq;
      mPulseWidth = mTempPulseWidth;    
    }
    gBuzzerBusy = FALSE; 
    
  }
  else
  {  
    if(mPeriodState)
    {
      mCountPeriod += 1;
      lowPeriodTimer();       
    }
    else
    {
      highPeriodTimer();
            
    }
   
  }
  
}

/***********************************************************************************/

#pragma CODE_SEG DEFAULT

uint16_t Buzzer_ValidateTone(buzzerTone_t tone)
{
  switch (tone)
  {
    case gBuzzerTone1_c:
      return mBuzzerTone1_c;
    break;
    
    case gBuzzerTone2_c:
      return mBuzzerTone2_c;
    break;
    
    case gBuzzerTone3_c:
      return mBuzzerTone3_c;
    break;
    
    case gBuzzerTone4_c:
      return mBuzzerTone4_c;
    break;
    
    case gBuzzerTone5_c:
      return mBuzzerTone5_c;
    break;
    
    case gBuzzerTone6_c:
      return mBuzzerTone6_c;
    break;
    
    case gBuzzerTone7_c:
      return mBuzzerTone7_c;
    break;
    
    case gBuzzerTone8_c:
      return mBuzzerTone8_c;
    break;
    
    case gBuzzerTone9_c:
      return mBuzzerTone9_c;
    break;
    
    case gBuzzerTone10_c:
      return mBuzzerTone10_c;
    break;
    
    default:
      return mBuzzerTone1_c;
    break;
    
  }
  
}

/***********************************************************************************/

uint16_t Buzzer_ValidateVol(buzzerVolume_t volume)
{
  return ((volume + 1) * 15);
}

#else


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/******************************************************************************
* Buzzer_Init 
*
*******************************************************************************/

void Buzzer_Init(void)
{    
    
}

/******************************************************************************
* Buzzer_ShortBeep 
*
*******************************************************************************/

void Buzzer_ShortBeep(void)
{
  
}

/******************************************************************************
* Buzzer_LongBeep 
*
*******************************************************************************/

void Buzzer_LongBeep(void)
{
  
}

/******************************************************************************
* Buzzer_SetBeepTone 
*
*******************************************************************************/

void Buzzer_SetBeepTone(buzzerTone_t beepTone)
{
    (void)beepTone;
}

/******************************************************************************
* Buzzer_SetBeepVolume 
*
*******************************************************************************/

void Buzzer_SetBeepVolume(buzzerVolume_t beepVol)
{                    
    (void)beepVol;
}

/******************************************************************************
* Buzzer_Beep
*
*******************************************************************************/

void Buzzer_Beep(uint8_t beepSecs, buzzerTone_t beepTone, buzzerVolume_t beepVol)
{
    (void)beepSecs;
    (void)beepTone;
    (void)beepVol;                  
}

#endif

 
