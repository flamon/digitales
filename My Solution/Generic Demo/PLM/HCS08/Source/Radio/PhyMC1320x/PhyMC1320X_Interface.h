/*****************************************************************************
* PhyMC1320X Interface header file
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __PHYMC1320X_INTERFACE_H__               
  #define __PHYMC1320X_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "Radio_Interface.h"

/************************************************************************************
*************************************************************************************
* Interface macro definitions
*************************************************************************************
************************************************************************************/

enum {
  gCcaED_c,            
  gCcaCCA_MODE1_c,     
  gInvalidCcaType_c    
};

#define gCcaTypePos_c      (0)
#define gDataReq_NoAck_NoCca_Unslotted_c	((gCcaCCA_MODE1_c << gCcaTypePos_c))
#define gPhyMaxDataLength_c   (127)

#define PhyPlmeForceTrxOffRequest()  PhyAbort()
#define PhyPlmeEdRequest()           PhyPlmeCcaEdRequest(gCcaED_c)
#define PhyPlmeCcaRequest(arg)       PhyPlmeCcaEdRequest(gCcaCCA_MODE1_c)
#define PhyXCVRContReset()           Radio_AssertReset()
#define PhyXCVRRestart()             Radio_DeassertReset()

/*PHY requests exit states*/
enum {
  gPhySuccess_c,
  gPhyBusy_c,
  gPhyInvalidParam_c
};


/************************************************************************************
*************************************************************************************
* Interface Type definitions
*************************************************************************************
************************************************************************************/

typedef struct phyRxParams_tag {
  zbClock24_t timeStamp;
  uint8_t     linkQuality;
} phyRxParams_t;

typedef struct phyPacket_tag {
  uint8_t frameLength;
  uint8_t data[1];
} phyPacket_t;

typedef uint32_t zbClock24_t;


/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/************************************************************************************
* PhyInit
* 
************************************************************************************/
extern void PhyInit(void);

/************************************************************************************
* PhyPdDataRequest
* 
************************************************************************************/
extern uint8_t PhyPdDataRequest
(
phyPacket_t *pTxPacket, 
uint8_t phyTxMode, 
phyRxParams_t *pRxParams
);

/************************************************************************************
* PhyPlmeDozeRequest
* 
************************************************************************************/
extern uint8_t PhyPlmeDozeRequest
(
		uint32_t u32Timeout
);

/************************************************************************************
* PhyPlmeRxRequest
* 
************************************************************************************/
extern uint8_t PhyPlmeRxRequest
(
phyPacket_t *pRxData, 
phyRxParams_t *pRxParams
);

/************************************************************************************
* PhyTimeSetEventTimeout
* 
************************************************************************************/
extern void PhyTimeSetEventTimeout
(
zbClock24_t *pEndTime
);

/************************************************************************************
* PhyTimeReadClock
* 
************************************************************************************/
extern void PhyTimeReadClock
(
zbClock24_t *pRetClk
);

/************************************************************************************
* PhyPlmeCcaEdRequest
* 
************************************************************************************/
extern uint8_t PhyPlmeCcaEdRequest
(
uint8_t ccaParam
);

/************************************************************************************
* PhyPlmeSetCurrentChannelRequest
* 
************************************************************************************/
extern uint8_t PhyPlmeSetCurrentChannelRequest
(
uint8_t channel
);

/************************************************************************************
* PhyPlmeSetPwrLevelRequest
* 
************************************************************************************/
extern uint8_t PhyPlmeSetPwrLevelRequest
(
uint8_t pwrLevel
);

/************************************************************************************
* PhyPpSetPromiscuous
* 
************************************************************************************/
extern void PhyPpSetPromiscuous
(
bool_t mode
);

/************************************************************************************
* PhyPlmeSetClockOutRate
* 
************************************************************************************/
extern uint8_t PhyPlmeSetClockOutRate
(
uint8_t u8Freq
);

/************************************************************************************
* PhyPlmeSetClockOutRate
* 
************************************************************************************/
extern void PhyTimeDisableEventTimeout(void);


/************************************************************************************
* PhyAbort
* 
************************************************************************************/
extern void PhyAbort(void);


/************************************************************************************
* PhyPpSetCcaThreshold
* 
************************************************************************************/
extern void PhyPpSetCcaThreshold
(
uint8_t ccaThreshold
);

/************************************************************************************
* PhyAdjustCcaOffsetCmp
* 
************************************************************************************/
extern void PhyAdjustCcaOffsetCmp
(
uint8_t ccaOffset
);

/************************************************************************************
* PhyXtalAdjust
* 
************************************************************************************/
extern void PhyXtalAdjust
(
uint8_t xtalTrim
);

/************************************************************************************
* PhyEnableTxNoModulation
* 
************************************************************************************/
extern void PhyEnableTxNoModulation(void);

/************************************************************************************
* PhyDisableTxNoModulation
* 
************************************************************************************/
extern void PhyDisableTxNoModulation(void);

/************************************************************************************
* PhyEnableContinuousTxRxMode
* 
************************************************************************************/
extern void PhyEnableContinuousTxRxMode(void);

/************************************************************************************
* PhyDisableContinuousTxRxMode
* 
************************************************************************************/
extern void PhyDisableContinuousTxRxMode(void);


/************************************************************************************
* PhySetRadioTimerPrescaler
* 
************************************************************************************/
extern void PhySetRadioTimerPrescaler
(
uint8_t Value
);

/************************************************************************************
* PhyGetRfIcVersion
* 
************************************************************************************/
extern void PhyGetRfIcVersion
(
uint8_t * Buffer
);

/************************************************************************************
* PhyHibernateRequest
* 
************************************************************************************/
extern void PhyHibernateRequest(void);

/************************************************************************************
* PhyWakeRequest
* 
************************************************************************************/
extern void PhyWakeRequest(void);

/************************************************************************************
* PhyEnableRadioInterrupts
* 
************************************************************************************/
extern void PhyEnableRadioInterrupts(void);

/************************************************************************************
* PhyDisableRadioInterrupts
* 
************************************************************************************/
extern void PhyDisableRadioInterrupts(void);

#pragma CODE_SEG DEFAULT


#endif //__PHYMC1320X_INTERFACE_H__

 
