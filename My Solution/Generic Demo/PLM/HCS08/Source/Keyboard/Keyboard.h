/*****************************************************************************
* Keyboard driver configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _KEYBOARD_H_               
#define _KEYBOARD_H_

#include "Keyboard_Interface.h"


/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
 
typedef enum mSweepingState_tag{
    KMD0_c,
    KMD1_c,
#if gTargetBoard_c == gMc1323xRcm_c   
    KMD2_c,
    KMD3_c,
    KMD4_c,
    KMD5_c,
#endif    
    MaxKMD_c   
 }mSweepingState_t;
 

/************************************************************************************
*************************************************************************************
* Private defines and macros
*************************************************************************************
************************************************************************************/ 
 
   
#if gTargetBoard_c == gMc1323xRcm_c 
  #define mKeyboardPortBMask_c (0x3F)
  #define mKbiRows1Mask_c (0x0F)
  #define mKbiRows2Mask_c (0x88)
  #define mOutputPort_c (0x0F)
  #define mRows1Port_c PTCD
  #define mRows2Port_c PTDD
#endif

#if gTargetBoard_c == gMc1323xRem_c
  #define mKbiRows1Mask_c (0x03)
  #define mOutputPort_c (0x03)
#endif

#define EnableDebouncingTime()  mSweeping = FALSE;                   \
                                TPM1CNT = 0;                         \
                                (void)Tmr_Enable(gTmr1_c, gTmrClkDivBy128_c, mDebouncingTimeMs);  
                              
#define EnableSweepingTime()   mSweeping = TRUE;                     \
                               TPM1CNT = 0;                          \
                               (void)Tmr_Enable(gTmr1_c, gTmrClkDivBy128_c, mKeyboardSweepTimeMs); 
                            

#define mDebouncingTime_c (0x1000)      //0x1152 Equivalent to 35.4ms

#define mSweepTime_c (0x0230)           //0x036A Equivalent to 7ms


#endif  //__Keyboard_H__ 
