/*****************************************************************************                   
* Keyboard driver implementation.
* Driver to manage the RCM and REM keyboard for MC1323x platform.
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#include "Keyboard.h"
#include "KBI_Interface.h"
#include "derivative.h"
#include "PortConfig.h" 
#include "app_config.h"
#include "Timer_Interface.h"
#include "board_config.h"
#include "GPIO_Interface.h"

#if gKeyboardSupported_d == TRUE

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 

void Keyboard_WhichKeyWasPressed(void);
void Keyboard_TimerEvent(void);  
void Keyboard_Listening(void);  
void Keyboard_KbiEvent(kbiPressed_t PressedKey); 
void Keyboard_WhichKeyWasPressed(void); 

#pragma CODE_SEG DEFAULT

keyboardErr_t Keyboard_InitKeyboard(keyboardCallbackFunc_t pCallback);


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
  
static uint16_t mDebouncingTimeMs;                         
static uint16_t mKeyboardSweepTimeMs;             
static keyboardButton_t mActiveButton;
static keyboardCallbackFunc_t mpKeyboardCallback;
static mSweepingState_t mSweepKmd;      
static kbiPressed_t mKbiPressed;
static bool_t mSweeping;
static tmrChannelConfig_t mTimerConfig;   

             

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
*  Keyboard_InitKeyboard
*
************************************************************************************/


keyboardErr_t Keyboard_InitKeyboard(keyboardCallbackFunc_t pCallback)    /* Please run Tmr_Init() function outside the driver before this function */
{
  if(NULL == pCallback)
  {
    return gKeyboardErrNullPointer_c;
  }
  else
  {
    /*Variables initialization*/
    mDebouncingTimeMs = mDebouncingTime_c;
    mKeyboardSweepTimeMs = mSweepTime_c;
    mActiveButton = ButtonMaxButton_c;
    mSweepKmd = KMD0_c; 
    mSweeping =  TRUE;
    
    mpKeyboardCallback = pCallback;                
     
    /*Timer configuration*/                            
    mTimerConfig.tmrChannel = gTmrChannel0_c;
    mTimerConfig.tmrChannOptMode= gTmrOutputCompare_c;
    mTimerConfig.tmrPinConfig.tmrOutCompState = gTmrPinNotUsedForOutComp_c;
    mTimerConfig.tmrCompareVal = mDebouncingTimeMs;
    (void)Tmr_SetCallbackFunc(gTmr1_c, gTmrChannel0Event_c, (tmrCallbackFunction_t)Keyboard_TimerEvent);
    (void)Tmr_SetChannelConfig(gTmr1_c, &mTimerConfig);
    mSweepKmd = KMD0_c;
    EnableSweepingTime();     

    return gKeyboardErrNoError_c;
  }
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

/************************************************************************************
*
*  Keyboard_Listening
*
************************************************************************************/

void Keyboard_Listening(void)
{
    #if gTargetBoard_c == gMc1323xRcm_c 
    
      switch (mSweepKmd)
      {
        case KMD0_c:
          Gpio_PinClear(gGpioPortD_c, gGpioPin3Mask_c);
          Gpio_PinClear(gGpioPortD_c, gGpioPin7Mask_c);
          Gpio_PortWrite(gGpioPortC_c, ((mRows1Port_c | mKbiRows1Mask_c) & 0xF1));
          break;
        
        case KMD1_c:
          Gpio_PinClear(gGpioPortD_c, gGpioPin3Mask_c);
          Gpio_PinClear(gGpioPortD_c, gGpioPin7Mask_c);
          Gpio_PortWrite(gGpioPortC_c, ((mRows1Port_c | mKbiRows1Mask_c) & 0xF2));          
          break;
          
        case KMD2_c:
          Gpio_PinClear(gGpioPortD_c, gGpioPin3Mask_c);
          Gpio_PinClear(gGpioPortD_c, gGpioPin7Mask_c);
          Gpio_PortWrite(gGpioPortC_c, ((mRows1Port_c | mKbiRows1Mask_c) & 0xF4));          
          break;
          
        case KMD3_c:
          Gpio_PinClear(gGpioPortD_c, gGpioPin3Mask_c);
          Gpio_PinClear(gGpioPortD_c, gGpioPin7Mask_c);
          Gpio_PortWrite(gGpioPortC_c, ((mRows1Port_c | mKbiRows1Mask_c) & 0xF8));   
          break;
          
        case KMD4_c: 
            Gpio_PortWrite(gGpioPortC_c, (mRows1Port_c & 0xF0)); 
            Gpio_PinSet(gGpioPortD_c, gGpioPin3Mask_c); 
            Gpio_PinClear(gGpioPortD_c, gGpioPin7Mask_c); 
            break; 
            
        case KMD5_c: 
            Gpio_PortWrite(gGpioPortC_c, (mRows1Port_c & 0xF0)); 
            Gpio_PinClear(gGpioPortD_c, gGpioPin3Mask_c); 
            Gpio_PinSet(gGpioPortD_c, gGpioPin7Mask_c); 
            break;   
      }
      
    #endif
     
    #if gTargetBoard_c == gMc1323xRem_c 
     
      switch (mSweepKmd)
      {
        case KMD0_c:
          Gpio_PinSet(gGpioPortC_c, gGpioPin0Mask_c);
          Gpio_PinClear(gGpioPortC_c, gGpioPin1Mask_c);
          break;
        
        case KMD1_c:
          Gpio_PinClear(gGpioPortC_c, gGpioPin0Mask_c);
          Gpio_PinSet(gGpioPortC_c, gGpioPin1Mask_c);
          break;
      }
    #endif
    
    EnableSweepingTime();
      
  
}

/************************************************************************************
*
*  Keyboard_KbiEvent
*
************************************************************************************/



void Keyboard_KbiEvent(kbiPressed_t PressedKey)       /*This is the keyboard callback*/
{  
  KBI1SC_KBIE = 0;                                   //Disable KBI interruptions 
  mKbiPressed = PressedKey;
  EnableDebouncingTime();
   
} 

/************************************************************************************
*
*  Keyboard_WhichKeyWasPressed
*
************************************************************************************/ 


#if gTargetBoard_c == gMc1323xRcm_c 
void Keyboard_WhichKeyWasPressed(void)
{
  switch(mKbiPressed)
   {
    case (gKbiPressedKey0_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw1_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw7_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw13_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw19_c;
          break;
          
        case(KMD4_c):
          mActiveButton = gSw25_c;
          break;
        
        case(KMD5_c):
          mActiveButton = gSk1_c;
          break;
      }
      
    break;
    
    case(gKbiPressedKey1_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw2_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw8_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw14_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw20_c;
          break;
          
        case(KMD4_c):
          mActiveButton = gSw26_c;
          break;
        
        case(KMD5_c):
          mActiveButton = gSk2_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey2_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw3_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw9_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw15_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw21_c;
          break;
          
        case(KMD4_c):
          mActiveButton = gSw27_c;
          break;
        
        case(KMD5_c):
          mActiveButton = gSk3_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey3_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw4_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw10_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw16_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw22_c;
          break;
          
        case(KMD4_c):
          mActiveButton = gSw28_c;
          break;
        
        case(KMD5_c):
          mActiveButton = gSk4_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey4_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw5_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw11_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw17_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw23_c;
          break;
          
        case(KMD4_c):
          mActiveButton = gSw29_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey5_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw6_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw12_c;
          break;
          
        case(KMD2_c):
          mActiveButton = gSw18_c;
          break;
          
        case(KMD3_c):
          mActiveButton = gSw24_c;
          break;
      
      }
      
    break;  
    
    default:
      asm nop; 
      break;    
      
   }
    
    mpKeyboardCallback(mActiveButton);
    KBI1SC_KBIE = 1;

}
#endif

#if gTargetBoard_c == gMc1323xRem_c 

void Keyboard_WhichKeyWasPressed(void)
{

  switch(mKbiPressed)
   {
    case (gKbiPressedKey0_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw1_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw5_c;
          break;
      }
      
    break;
    
    case(gKbiPressedKey1_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw2_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw6_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey2_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw3_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw7_c;
          break;
      }
      
    break;  
    
    case(gKbiPressedKey3_c):
      switch(mSweepKmd)
      {
        case(KMD0_c):
          mActiveButton = gSw4_c;
          break;
          
        case(KMD1_c):
          mActiveButton = gSw8_c;
          break;
      }
      
    break;  
   
    
    default:
      asm nop;     
      break;    

   }   
   
    mpKeyboardCallback(mActiveButton);
    KBI1SC_KBIE = 1;

}

#endif

/************************************************************************************
*
*  Keyboard_TimerEvent
*
************************************************************************************/     

void Keyboard_TimerEvent(void)       /*This is the timer callback*/
{
  if(mSweeping)
  {
     #if gTargetBoard_c == gMc1323xRcm_c  
     if (mSweepKmd == KMD5_c)
     {
        mSweepKmd = KMD0_c;
     }
     else
     {
        mSweepKmd = (mSweepingState_t)(mSweepKmd + 1);
     }
     #endif
     
     #if gTargetBoard_c == gMc1323xRem_c 
     if (mSweepKmd == KMD1_c)
     {
        mSweepKmd = KMD0_c;
     }
     else
     {
        mSweepKmd = KMD1_c; 
     }
     #endif
     
     Keyboard_Listening();         
  }
  else
  {
     if (mKbiPressed == (PTBD & mKbiPressed)) 
     {
        Keyboard_WhichKeyWasPressed();
        
     }
     KBI1SC_KBIE = 1;
     #if gTargetBoard_c == gMc1323xRcm_c  
     if (mSweepKmd == KMD5_c)
     {
        mSweepKmd = KMD0_c;
     }
     else
     {
        mSweepKmd = (mSweepingState_t)(mSweepKmd + 1);
     }
     #endif
     
     #if gTargetBoard_c == gMc1323xRem_c 
     if (mSweepKmd == KMD1_c)
     {
        mSweepKmd = KMD0_c;
     }
     else
     {
        mSweepKmd = KMD1_c; 
     }
     #endif
     
     EnableSweepingTime();
  }
  
  
}      

#pragma CODE_SEG DEFAULT

#else

/************************************************************************************
*
*  Keyboard_InitKeyboard
*
************************************************************************************/


keyboardErr_t Keyboard_InitKeyboard(keyboardCallbackFunc_t pCallback)
{
  (void)pCallback;
  return gKeyboardErrNoError_c;
}

#endif




   
