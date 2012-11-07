/*****************************************************************************
* IIC Configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _IIC_H_               
#define _IIC_H_

#include "IIC_Interface.h"

/************************************************************************************
*************************************************************************************
* Constants
*************************************************************************************
************************************************************************************/

#ifndef gIicParamAndInitValidation_d
 #define gIicParamAndInitValidation_d TRUE
#endif

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

/*IIC module states*/
typedef enum mIicStates_tag{
  mIicNoInit_c,
  mIicIdle_c,
  mIicTransmittingData_c,
  mIicReceivingData_c,
  mIicTransactionInProcess_c
}mIicStates_t;

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#if defined(gMcs08Gt60Platform_d) || defined(gMcs08qe128Platform_d) 
 #define IICS IIC1S
 #define IICC IIC1C
 #define IICD IIC1D
 #define IICF IIC1F 
#endif


#define mIicEnableMask_c             (0x80)
#define mIicInterruptEnableMask_c    (0x40)
#define mIicMasterModeSelectMask_c   (0x20)
#define mIicTransmitModeSelectMask_c (0x10)
#define mIicRepeatStartMask_c        (0x04)
#define mIicInterruptFlagMask_c      (0x02) 
#define mIicReceiveAckMask_c         (0x01) 
#define mIicNoAckMask_c              (0x08)

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t iicSendNextByte(uint8_t NextByte);

#if gIicMode_d == gIicInterrupt_c   
#pragma CODE_SEG ISR_SEGMENT
#endif
void iicFinishReadWrite(iicResult_t rwResult);
#pragma CODE_SEG DEFAULT

#endif
/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/
#define mDelay20us() { {uint8_t i=40; do {} while(--i);}  }

#if gIicMode_d == gIicInterrupt_c   
  #define mIicEnableInterrupt()            \
    {IICS |= mIicInterruptFlagMask_c;      \
     IICC |= mIicInterruptEnableMask_c;}
  #define mIicDisableInterrupt()           \
    {IICC &= ~(mIicInterruptEnableMask_c);}
#else
  #define mIicEnableInterrupt()            
  #define mIicDisableInterrupt()           
#endif

#endif  //__IIC_H__ 

  