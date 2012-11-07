/*****************************************************************************
* Keyboard Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __KEYBOARD_INTERFACE_H__               
  #define __KEYBOARD_INTERFACE_H__
 
#include "derivative.h"     
#include "EmbeddedTypes.h"  
#include "KBI_Interface.h"
#include "board_config.h"
#include "PLM_config.h"

#ifndef gKeyboardSupported_d
    #define gKeyboardSupported_d FALSE
#endif    


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef enum keyboardErr_tag{
  gKeyboardErrNoError_c = 0,                 
  gKeyboardErrNullPointer_c,    
  gKeyboardErrMax_c
}keyboardErr_t;

typedef enum keyboardButton_tag
{
  gSw1_c,
  gSw2_c,  
  gSw3_c,
  gSw4_c,
  gSw5_c,
  gSw6_c,
  gSw7_c,
  gSw8_c,
#if gTargetBoard_c == gMc1323xRcm_c  
  gSw9_c,
  gSw10_c,
  gSw11_c,
  gSw12_c,
  gSw13_c,
  gSw14_c,
  gSw15_c,
  gSw16_c,
  gSw17_c,
  gSw18_c,
  gSw19_c,
  gSw20_c,
  gSw21_c,
  gSw22_c,
  gSw23_c,
  gSw24_c,
  gSw25_c,
  gSw26_c,
  gSw27_c,
  gSw28_c,
  gSw29_c,
  gSk1_c,
  gSk2_c,
  gSk3_c,
  gSk4_c ,
#endif  
  ButtonMaxButton_c
}keyboardButton_t;


#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near keyboardCallbackFunc_t)(keyboardButton_t);
#else
  typedef void(* __near keyboardCallbackFunc_t)(keyboardButton_t);
#endif



/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
*  Keyboard_InitKeyboard
*
*  This function initializes the keyboard and configures TPM1 to be used by the 
*  Keyboard module.
*
*  Function assumptions:
*       TPM1 and KBI interrupts enabled.
*       KBI, Port C and Port D initialized outside Keyboard driver.
*       Optional 32MHz output in PTD7 disabled.
*
*  Return value:
*       gKeyboardErrNullPointer_c if the function received a null pointer.
*       gKeyboardErrNoError_c the module was initialized properly.
*
************************************************************************************/

extern keyboardErr_t Keyboard_InitKeyboard(keyboardCallbackFunc_t pCallback);


/************************************************************************************
*
*  Keyboard_KbiEvent
*
*  This function is called by the Kbi callback function in main. 
*  No actions needed by the user.
*
************************************************************************************/
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif 

extern void Keyboard_KbiEvent(kbiPressed_t PressedKey);       

#pragma CODE_SEG DEFAULT

#endif    //__KEYBOARD_INTERFACE_H__