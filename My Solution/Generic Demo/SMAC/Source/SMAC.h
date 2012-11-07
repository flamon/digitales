/*****************************************************************************
* SMAC header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef __SMAC_H__               
  #define __SMAC_H__

#include "SimplePhy.h"
#include "EmbeddedTypes.h"
#include "Utilities_Interface.h"
#include "SMAC_Interface.h"

/*smacParametersValidation_d:
TRUE :  SMAC primitives validate their incoming parameters.
FALSE:  SMAC primitives do their stuff without validating their incoming 
        parameters
*Note: Setting this as FALSE will compile a smaller foot print SMAC.
*/
#define smacParametersValidation_d      TRUE

/*smacInitializationValidation_d:
TRUE :  SMAC primitives validate wheter SMAC is initialized or not.
FALSE:  SMAC primitives don't care about SMAC's initialization.
*Note: Setting this as FALSE will compile a smaller foot print SMAC.
*/
#define smacInitializationValidation_d  TRUE

/************************************************************************************
*************************************************************************************
* Module Type definitions
*************************************************************************************
************************************************************************************/

typedef enum smacStates_tag {
  mSmacStateIdle_c,
  mSmacStateTransmitting_c,
  mSmacStateReceiving_c,
  mSmacStateScanningChannels_c,
  mSmacStatePerformingTest_c,
  mSmacStateHibernate_c, 
  mSmacStateDoze_c    
} smacStates_t;

typedef union flagsRegister_tag{
  uint8_t entryRegister;
  struct{
    uint8_t flag0       :1;                                       
    uint8_t flag1       :1;                                       
    uint8_t flag2       :1;                                       
    uint8_t flag3       :1;                                       
    uint8_t flag4       :1;                                       
    uint8_t flag5       :1;                                       
    uint8_t flag6       :1;                                       
    uint8_t flag7       :1;                                       
  }flags;
}flagsRegister_t;

typedef union prssPacketPtr_tag
{
  uint8_t*    smacScanResultsPointer;
  rxPacket_t* smacRxPacketPointer;
  phyPacket_t* smacTxPacketPointer;
}prssPacketPtr_t;

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/
extern flagsRegister_t smacFlags;

#define mPromiscuousModeEnabled  (smacFlags.flags.flag0)
#define mSmacInitialized         (smacFlags.flags.flag1)

#define mSmacPerformingPRBS9     (smacFlags.flags.flag3)
#define mSmacTimeoutAsked        (smacFlags.flags.flag4)
#define mSmacPerformingCCAScan   (smacFlags.flags.flag5) 
#define mSmacPerformingEDScan    (smacFlags.flags.flag6) 
#define mSmacPerformingED        (smacFlags.flags.flag7) 

/*
Byte
1   8 bits - Major
1   8 bits - Middle
2   8 bits - Minor
2   8 bits - Month (valid 1-12)
3   8 bits - Day of the month (valid 1-31)
4   8 bits - Year from 2000
5   8 bits - Consecutive number
*/

#define mMajorSmacV_c   (2)
#define mMiddleSmacV_c  (1)
#define mMinorSmacV_c   (3)
#define mYearSmacV_c    (11)
#define mMonthSmacV_c   (7)
#define mDaySmacV_c     (4)
#define mConsSmacV_c    (6)

#if (mMajorSmacV_c > 255) || (mMiddleSmacV_c > 255) || (mMinorSmacV_c > 255)
 #error "Cobdebase version out of range"
#endif

#if (mYearSmacV_c > 255) || (mMonthSmacV_c > 12) || (mMonthSmacV_c < 1) || (mDaySmacV_c > 31) || (mDaySmacV_c<1)
 #error "Version date out of range"
#endif

#if (mConsSmacV_c > 255)
 #error "Consecutive number of SMAC version out of range"
#endif

#define areCodeBytesValid(x) ((x->smacPdu.reserved[0] == 0x7E)   \
                             && (x->smacPdu.reserved[1] == 0xFF))

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
smacErrors_t SmacScanTheNextChannel(void);
void SmacSetRxTimeout(zbClock24_t timeoutSymbols);
phyPacket_t * SmacFormatTxPacket(txPacket_t *psTxPacket);

/***********************************************************************************/
/****************************** PHY Callback Functions *****************************/
/***********************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

void PhyPdDataConfirm(void);
void PhyPdDataIndication(void);
void PhyPlmeCcaConfirm(bool_t channelInUse);
void PhyPlmeEdConfirm(uint8_t energyLevel);
void PhyPlmeSyncLossIndication(void);
void PhyPlmeWakeComfirm(void);
void PhyTimeRxTimeoutIndication(void);
void PhyTimeStartEventIndication(void);



/************************************************************************************
*************************************************************************************
* External Prototypes
*************************************************************************************
************************************************************************************/
extern void MCPSDataComfirm(txStatus_t);
extern void MCPSDataIndication(rxPacket_t *);
extern void MLMEResetIndication(void);
extern void MLMEScanComfirm(channels_t);
extern void MLMEWakeComfirm(void);

#pragma CODE_SEG DEFAULT
/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/



#endif  //__SMAC_H__ 

  