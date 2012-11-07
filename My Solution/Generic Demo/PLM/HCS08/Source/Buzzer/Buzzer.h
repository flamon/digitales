/*****************************************************************************
* Buzzer driver configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _BUZZER_H_               
#define _BUZZER_H_

#include "Buzzer_Interface.h"



/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

uint16_t Buzzer_ValidateTone(buzzerTone_t tone);
uint16_t Buzzer_ValidateVol(buzzerVolume_t volume);


#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 

void Buzzer_TimerEvent(void);

#pragma CODE_SEG DEFAULT


/************************************************************************************
*************************************************************************************
* Private defines and macros
*************************************************************************************
************************************************************************************/ 

#define mFreqTone1_c (3900)
#define mFreqTone2_c (1950)
#define mFreqTone3_c (1300)
#define mFreqTone4_c (976)
#define mFreqTone5_c (480)
#define mFreqTone6_c (430)
#define mFreqTone7_c (390)
#define mFreqTone8_c (350)
#define mFreqTone9_c (325)
#define mFreqTone10_c (260)

#define mClockForBuzzer_c (gSystemClock_d/128)

#define mBuzzerTone1_c (mClockForBuzzer_c / mFreqTone1_c)            
#define mBuzzerTone2_c (mClockForBuzzer_c / mFreqTone2_c)            
#define mBuzzerTone3_c (mClockForBuzzer_c / mFreqTone3_c)                       
#define mBuzzerTone4_c (mClockForBuzzer_c / mFreqTone4_c)            
#define mBuzzerTone5_c (mClockForBuzzer_c / mFreqTone5_c)           
#define mBuzzerTone6_c (mClockForBuzzer_c / mFreqTone6_c)           
#define mBuzzerTone7_c (mClockForBuzzer_c / mFreqTone7_c)               
#define mBuzzerTone8_c (mClockForBuzzer_c / mFreqTone8_c)           
#define mBuzzerTone9_c (mClockForBuzzer_c / mFreqTone9_c)           
#define mBuzzerTone10_c (mClockForBuzzer_c / mFreqTone10_c)   

#define mTimeConstant (mClockForBuzzer_c / 2)           //Equivalent to .5 s
       
#define calculateTimerRounds(seconds)   ((mTimeConstant/mPulseFreq)*2*seconds)                              

#define buzzerPin gBuzzerPort_c, gBuzzerPin_c 

#define pulseWidth(freq, vol) ((freq * vol) / 100)
#define pulseLow(freq, pulseWidth) (freq - pulseWidth)


#define highPeriodTimer()   mPeriodState =  TRUE;                   \
                            Gpio_PinSet(buzzerPin);                 \
                            (void)Tmr_Enable(gBuzzerTimer_c, gTmrClkDivBy128_c, mPulseWidth);  
                      
#define lowPeriodTimer()    mPeriodState = FALSE;                  \
                            Gpio_PinClear(buzzerPin);              \
                            gBuzzerBusy = TRUE;                    \
                            (void)Tmr_Enable(gBuzzerTimer_c, gTmrClkDivBy128_c, pulseLow(mPulseFreq, mPulseWidth));  

#endif  //__BUZZER_H__ 