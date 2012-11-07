/*****************************************************************************
* Radio Configuration header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __RADIO_H__               
  #define __RADIO_H__

#include "Radio_Interface.h"


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

typedef enum radioDriverStates_tag{
  mRadioStateIdle_c,
  mRadioStateReadingPendingIrq_c,
  mRadioStateWrittingRegister_c,
  mRadioStateReadingRegister_c,
  mRadioStateWrittingTxBuffer_c,
  mRadioStateReadingRxBuffer_c,
  mRadioStateMaxState_c
}radioDriverStates_t;


typedef enum radioProcessStates_tag{
  mRadioProcessIdle_c,
  mRadioProcessSendingAddres_c,
  mRadioProcessDiscartingBytes_c,
  mRadioProcessRdWrData_c,
  mRadioProcessMax_c
}radioProcessStates_t;

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#define mRadioAreRadioOrSPIBusy                                   \
((mRadioStateIdle_c != mRadioDriverState) ||                      \
 (gSpiErrNoError_c  != SPI1_AssertPeripheralSsPin(1, RadioProcess)))


/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/
#define RadioStringCopyBigToLittleEndian(sSourcePtr, sDestPtr, stringlength)\
  {                                                                     \
    uint8_t u8f;                                                        \
    for(u8f = 0; u8f < (stringlength/2);u8f++){                         \
      *sDestPtr++ = *(sSourcePtr+1);                                    \
      *sDestPtr++ = *sSourcePtr;                                        \
      sSourcePtr += 2;                                                  \
    }                                                                   \
    if(stringlength & 0x01) {                                           \
      *sDestPtr++ = 0;                                                  \
      *sDestPtr++ = *sSourcePtr;                                        \
      stringlength++;                                                   \
    }                                                                   \
  }                                                             
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

/*Callback function called by the IRQ driver*/
void RadioIRQServiceRoutine(void);

/*Control Signals init function*/
void RadioInitControlSignals(void);

/*Radio Process funtion*/
void RadioProcess(void);
#pragma CODE_SEG DEFAULT




#endif //__RADIO_H__         


