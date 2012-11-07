/*****************************************************************************
* Touchpad Driver Configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _TOUCHPAD_H_               
#define _TOUCHPAD_H_
               
#include "TouchPad_Interface.h"

#define gTouchpadParametersValidation_d TRUE


/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#define mTouchpadIicSlaveAddress_c (0x20)

#define mTpIntStatusRegAdd_c       (0x14)
#define mTpGpioDataAddress_c       (0x25)
#define mTpAbs0DataAddress_c       (0x15)
#define mTpDevStatusDataAddress_c  (0x13)
#define mTpFlashDataAddress_c      (0x00)


#define mTpGpioDataSize_c          (1)
#define mTpAbs0DataSize_c          (16)
#define mTpDevStatusDataSize_c     (2) 
#define mTpFlashDataSize_c         (19)


#define mIntStatusGpioMask_c       (0x08)
#define mIntStatusAbs0Mask_c       (0x04)
#define mIntStatusDevStatusMask_c  (0x02)
#define mIntStatusFlashMask_c      (0x01)

#define mPinchMask_c               (0x40)
#define mFlickMask_c               (0x10)
#define mEarlyTapMask_c            (0x08)
#define mDoubleTapMask_c           (0x04)
#define mTapAndHoldMask_c          (0x02)
#define mSingleTapMask_c           (0x01)

typedef enum touchpadEventStates_tag
{
  mTpEvStateNoInit_c,
  mTpEvStateIdle_c,
  mTpEvStateWrittingInterruptStatusRegisterAddress_c,
  mTpEvStateReadingInterruptStatusRegisterData_c,
  mTpEvStateWrittingEventDataAddress_c,
  mTpEvStateReadingEventData_c,
  mTpEvStateMaxState_c 
}touchpadEventStates_t;

#define mTouchpadFlashSupportEnable_d FALSE

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
void tpEventPollCallBack(iicResult_t readResult);
void tpHandlingGpioEvent(iicResult_t IicResult);
void tpHandlingAbs0Event(iicResult_t IicResult);    
void tpHandlingDevStatusEvent(iicResult_t IicResult);
void tpHandlingFlashEvent(iicResult_t IicResult);
#pragma CODE_SEG DEFAULT


#endif //_TOUCHPAD_H_