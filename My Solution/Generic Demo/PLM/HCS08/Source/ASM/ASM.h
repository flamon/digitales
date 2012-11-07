/************************************************************************************
* Private header file for the ASM driver.
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
#ifndef _ASM_DRIVER_H_
#define _ASM_DRIVER_H_

#include "ASM_Interface.h"

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/



/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/

typedef enum asmAesState_tag
{
  gAsmStateIdle_c,
  gAsmStateCiphering_c,
  gAsmStateFinishBlockCipher_c,
  gAsmStateFinal_c,
  gAsmStateMax_c
}asmAesState_t;


/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/

#define mAsmCntl1Reg            (AES_CONTROL1)
#define mAsmCntl2Reg            (AES_CONTROL2)
#define mAsmD0Reg               (ASM_Data_0)

/* Make code evaluations before changing next lines! */
#define gAsmMaxSelfTestCycles   (111) /* 3330 + 10% */
#define gAsmMaxAesEngineCycles  (2)   /* 11 + 10% */


/* AES Control1 register bits */
#define AES_CTRL1_CLEAR         (0x80)
#define AES_CTRL1_START         (0x40)
#define AES_CTRL1_SELFTST       (0x20)
#define AES_CTRL1_CTR           (0x10)
#define AES_CTRL1_CBC           (0x08)
#define AES_CTRL1_AES           (0x04)
#define AES_CTRL1_LOADMAC       (0x02)
#define AES_CTRL1_AESMSK        (0x01)

/* AES Control2 register bits */
#define AES_CTRL2_REGTYPE_KEY      (0 << 5)
#define AES_CTRL2_REGTYPE_DATA     (1 << 5)
#define AES_CTRL2_REGTYPE_CTR      (2 << 5)
#define AES_CTRL2_REGTYPE_CTRRES   (3 << 5)
#define AES_CTRL2_REGTYPE_CBCRES   (4 << 5)
#define AES_CTRL2_REGTYPE_MAC      (5 << 5)
#define AES_CTRL2_REGTYPE_AESRES   (6 << 5)

#define AES_CTRL2_IRQFLAG          (0x04)
#define AES_CTRL2_TSTPAS           (0x02)
#define AES_CTRL2_DONE             (0x01)

#define mAsmCntl1RegModesMask_c    ((uint8_t)0x1F)
#define mAsmBlockSize_c            (16)
#define mAesLastRound_c            (54)
#define mFirstRound_c              (1)
#define mEighthRound_c             (128)
#define mNinthRound_c              (27)


#ifdef gMc1323xPlatform_d
  #define is_cbc_mode_selected()\
    ( (gAsmModeCbcMac_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
      (gAsmCipher_c == mAsmAction)) || ((gAsmModeCcm_c == mCipherMode)     && \
      (gAsmDecipher_c == mAsmAction) && (mCcmSecondRound)) )
      
  #define is_ctr_mode_selected()\
    ( (gAsmModeCtr_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
      (gAsmCipher_c == mAsmAction)) || ((gAsmModeCcm_c == mCipherMode) && \
      (gAsmDecipher_c == mAsmAction) && (!mCcmSecondRound)))
#endif

#ifdef gMcs08Gt60Platform_d
#define is_ctr_mode_selected() \
  ( (gAsmModeCtr_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
    (gAsmCipher_c == mAsmAction) && (mCcmSecondRound))                || \
    ((gAsmModeCcm_c == mCipherMode) && (gAsmDecipher_c == mAsmAction) && \
    (!mCcmSecondRound)) )

#define is_cbc_mode_selected() \
  ( (gAsmModeCbcMac_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
    (gAsmCipher_c == mAsmAction) && (!mCcmSecondRound))                  || \
    ((gAsmModeCcm_c == mCipherMode) && (gAsmDecipher_c == mAsmAction)    && \
    (mCcmSecondRound)))  
    
#endif    
    

#ifdef gMcs08qe128Platform_d
#define is_ctr_mode_selected() \
  ( (gAsmModeCtr_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
    (gAsmCipher_c == mAsmAction) && (mCcmSecondRound))                || \
    ((gAsmModeCcm_c == mCipherMode) && (gAsmDecipher_c == mAsmAction) && \
    (!mCcmSecondRound)) )

#define is_cbc_mode_selected() \
  ( (gAsmModeCbcMac_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
    (gAsmCipher_c == mAsmAction) && (!mCcmSecondRound))                  || \
    ((gAsmModeCcm_c == mCipherMode) && (gAsmDecipher_c == mAsmAction)    && \
    (mCcmSecondRound)))  
    
#endif      
    
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#endif /* _ASM_DRIVER_H_ */
