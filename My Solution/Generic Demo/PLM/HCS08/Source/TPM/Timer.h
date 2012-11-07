/************************************************************************************
* Private header file for the Timer driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*************************************************************************************/
#ifndef _TIMER_H_
#define _TIMER_H_

#include "Timer_Interface.h"

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef struct tmrModuleRegs_tag{
  uint8_t TPMxSC;
  uint8_t TPMxCNTH;
  uint8_t TPMxCNTL;
  uint8_t TPMxMODH;
  uint8_t TPMxMODL;
  uint8_t TPMxC0SC;
  uint8_t TPMxC0VH;
  uint8_t TPMxC0VL;
}tmrModuleRegs_t;

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/


#define mClkShiftBits_c               3
#define mChannOptModeShiftBits_c      4
#define mPinStateShiftBits_c          2
#define mTmrOverInterruptEnBit_c      0x40
#define mChannelInterruptEnBit_c      0x40
#define mTmrOverInterruptFlag_c       0x80
#define mTmrChannInterruptFlag_c      0x80
#define mTrmCleanInterruptFlag_c      0x7F 
#define mTmrCenterAlignedPwmBit_c     0x20
#define mTmrClearInterruptFlagMask_c  0x7F
#define mTmrMSBShiftBits_c            8
#define mTmrDisabled_c                0x00

#ifdef gMcs08Gt60Platform_d
  #define mTmrMaxEvent_c           6
  #define mTmr1MaxChannel_c        3
  #define mTmr2MaxChannel_c        5
  #define mTmr1StatCntlReg         TPM1SC
  #define mTmr2StatCntlReg         TPM2SC
  #define mTmr1Chan0StatCntlReg    TPM1C0SC
  #define mTmr1Chan1StatCntlReg    TPM1C1SC  
  #define mTmr1Chan2StatCntlReg    TPM1C2SC  
  #define mTmr2Chan0StatCntlReg    TPM2C0SC  
  #define mTmr2Chan1StatCntlReg    TPM2C1SC  
  #define mTmr2Chan2StatCntlReg    TPM2C2SC 
  #define mTmr2Chan3StatCntlReg    TPM2C3SC      
  #define mTmr2Chan4StatCntlReg    TPM2C4SC
#endif

#ifdef gMcs08qe128Platform_d
  #define mTmrMaxEvent_c     7
  #define mTmr1MaxChannel_c  3
  #define mTmr2MaxChannel_c  3
  #define mTmr3MaxChannel_c  6
  #define mTmr1StatCntlReg         TPM1SC
  #define mTmr2StatCntlReg         TPM2SC
  #define mTmr3StatCntlReg         TPM3SC
  #define mTmr1Chan0StatCntlReg    TPM1C0SC
  #define mTmr1Chan1StatCntlReg    TPM1C1SC  
  #define mTmr1Chan2StatCntlReg    TPM1C2SC  
  #define mTmr2Chan0StatCntlReg    TPM2C0SC  
  #define mTmr2Chan1StatCntlReg    TPM2C1SC  
  #define mTmr2Chan2StatCntlReg    TPM2C2SC 
  #define mTmr3Chan0StatCntlReg    TPM3C0SC  
  #define mTmr3Chan1StatCntlReg    TPM3C1SC  
  #define mTmr3Chan2StatCntlReg    TPM3C2SC      
  #define mTmr3Chan3StatCntlReg    TPM3C3SC      
  #define mTmr3Chan4StatCntlReg    TPM3C4SC
  #define mTmr3Chan5StatCntlReg    TPM3C5SC              
#endif

#ifdef gMc1323xPlatform_d
  #define mTmrMaxEvent_c    2
  #define mTmr1StatCntlReg         TPM1SC
  #define mTmr2StatCntlReg         TPM2SC
  #define mTmr3StatCntlReg         TPM3SC
  #define mTmr4StatCntlReg         TPM4SC
  #define mTmr1Chan0StatCntlReg    TPM1C0SC
  #define mTmr2Chan0StatCntlReg    TPM2C0SC  
  #define mTmr3Chan0StatCntlReg    TPM3C0SC  
  #define mTmr4Chan0StatCntlReg    TPM4C0SC 
#endif


/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/



/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


#endif /* _TIMER_H_ */