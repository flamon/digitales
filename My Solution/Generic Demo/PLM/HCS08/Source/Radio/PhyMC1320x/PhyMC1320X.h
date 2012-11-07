/*****************************************************************************
* PhyMC1320X header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __PHYMC1320X_H__               
  #define __PHYMC1320X_H__

#include "EmbeddedTypes.h"
#include "Radio_Interface.h"
#include "Utilities_Interface.h"
#include "derivative.h"
#include "PhyMC1320X_Interface.h"
#include "PortConfig.h"


/*ParametersValidation_d:
TRUE :  Primitives validate their incoming parameters.
FALSE:  Primitives do their stuff without validating their incoming 
        parameters
*Note: Setting this as FALSE will compile a smaller foot print.
*/
#define ParametersValidation_d      TRUE

/*InitializationValidation_d:
TRUE :  Primitives validate wheter initialization has been performed or not.
FALSE:  Primitives don't care about initialization.
*Note: Setting this as FALSE will compile a smaller foot print.
*/
#define InitializationValidation_d  TRUE

/************************************************************************************
*************************************************************************************
* Module Type definitions
*************************************************************************************
************************************************************************************/

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

typedef enum handlingRadioEventStates_tag{
 mHREIdleState_c = 0, 
 mHREDozeDisablingTimerState_c,
 mHREDozeReadingControlBRegState_c,
 mHREDozeDisablingDozeState_c,
 mHREAttnReadingRSTIndRegState_c,
 mHRETmr1WrittingTmrCmp1ARegState_c,
 mHRETmr1WrittingTmrCmp1BRegState_c,
 mHRERxRcvdReadingControlARegState_c,
 mHRERxRcvdWrittingControlARegState_c,
 mHRERxRcvdReadingRXStatusRegState_c,
 mHRERxRcvdWrittingTmrCmp1ARegState_c,
 mHRERxRcvdWrittingTmrCmp1BRegState_c,
 mHRERxRcvdReadingRxPacketState_c, 
 mHREEdReadingRxStatusRegState_c,
 mHREMaxStateState_c
}handlingRadioEventStates_t;

typedef enum phyStates_tag{
 mPhyNoInitState_c = 0,
 mPhyIdleState_c, 
 mPhyRxState_c,
 mPhyTxState_c,
 mPhyCcaState_c,
 mPhyEdState_c,
 mPhyRxWtoState_c,
 mPhyHibernateState_c,
 mPhyDozeState_c,
 mPhyResetAttnState_c,
 mPhyIdleAttnState_c,
 mPhyMaxState_c
}phyStates_t;



/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/
extern flagsRegister_t phyFlags;



#define mResetBitMask_c      (0x0080)  /*!< Reset bit Mask */

/* Modes defined for the transciever */
#define mIdleMode_c               (0x00)		/*!< Idle mode */
#define mRxMode_c                 (0x02)		/*!< Receive mode */
#define mTxMode_c                 (0x03)		/*!< Trasmit mode */
#define mCcaMode_c                (0x01)		/*!< CCA mode */

#define mMinPhyLength_c 3

#define mUsedStatusFlagsMask_c \
(gRadioIrqFlagTxSentIrq_c  | gRadioIrqFlagRxRcvdIrq_c | gRadioIrqFlagAttnIrq_c | \
gRadioIrqFlagCrcValid_c   | gRadioIrqFlagTmr1Irq_c   | gRadioIrqFlagCcaIrq_c  | \
gRadioIrqFlagPllLockIrq_c | gRadioIrqFlagDozeIrq_c   | gRadioIrqFlagCca_c)

#define mDoesEventHappenedWhilePhyIdle_c (mPhyIdleState_c == mPhyState)
#define mIsPhyOnARadioActiveState_c \
((mPhyRxState_c == mPhyState) || (mPhyRxWtoState_c == mPhyState) || \
(mPhyCcaState_c == mPhyState) || (mPhyPulseTxState_c == mPhyState))

#define  IsRadioRecoveringFromReset(RSTIndRegCopy) (!(RSTIndRegCopy &= mResetBitMask_c))

#define mStopTimerValueA_c (0x8000)
#define mStopTimerValueB_c (0x0000)

#define mRadioTimeMask_c (0x00FFFFFF)

#if (TRUE == gUseExternalAntennaSwitch_c)
 #define mControlBRegValue_c 0x5C00
#else
 #define mControlBRegValue_c 0x4C00
#endif

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

static uint8_t PhySetTrxStateRequest(uint8_t u8ReqMode);
static void PhyHREventsReStartSequense(void);
static void PhyChangePacketEndianess(phyPacket_t * packet, uint8_t packetLength);

/***********************************************************************************/
/**************************** Radio Callback Functions *****************************/
/***********************************************************************************/
static void PhyRadioEvent(void); 
static void PhyHandlingRadioEvents(void);


/************************************************************************************
*************************************************************************************
* External Prototypes
*************************************************************************************
************************************************************************************/

/*Phy Callbacks This shall be user-implemented*/
extern void PhyPdDataConfirm(void);
extern void PhyPdDataIndication(void);
extern void PhyPlmeCcaConfirm(bool_t);
extern void PhyPlmeEdConfirm(uint8_t);
extern void PhyPlmeSyncLossIndication(void);
extern void PhyPlmeResetIndication(void);
extern void PhyTimeRxTimeoutIndication(void);
extern void PhyTimeStartEventIndication(void);
extern void PhyPlmeWakeComfirm(void);

#pragma CODE_SEG DEFAULT


/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/


#endif  //__PHYMC1320X_H__ 



  