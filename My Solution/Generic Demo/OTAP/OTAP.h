/******************************************************************************
* Private header file for the OTAP module.
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/
#ifndef _OTAP_H_
#define _OTAP_H_

/* Include files section */
#include "OTAP_Interface.h"

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
*******************************************************************************/

typedef enum otapStates_tag
{
  mOtapStateNoInit_c,
  mOtapStateAppMode_c,
  mOtapStateOtapRequestReceived_c,
  mOtapStateBroadcastOtapRequestReceived_c,  
  mOtapStateWaitComfirm_c,
  mOtapStateWaitBroadcastComfirm_c,
  mOtapStateStartIdentificationProcess_c,
  mOtapStateIdentificationProcessSendDescriptor_c,
  mOtapStateWaitIdentificationProcessAck_c,
  mOtapStateSendDescriptor_c,
  mOtapStateWaitDescriptorAck_c,
  mOtapStateWaitStartOfTransmission_c,
  mOtapStateAckStartOfTransmission_c,
  mOtapStateWaitForData_c,
  mOtapStateSendWait_c,
  mOtapStateSendError_c,
  mOtapAckData_c,
  mOtapAckLastData_c,
  mOtapStateWaitEndOfTransmission_c,
  mOtapStateAckEndOfTransmission_c,
  mOtapStateWaitSaveData_c,
  mOtapStateMaxState_c
}otapStates_t;

typedef struct otapCodeLine_tag {
  uint8_t ByteCount;
  union
  {
    uint32_t u32;
    uint8_t  u8[4];
  }Address;
  uint8_t Data[32];
  uint8_t Checksum;
}otapCodeLine_t;

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
*******************************************************************************/

/*Otap version defines ********************************************************/
#define mMajorOtapV_c                 (2)
#define mMiddleOtapV_c                (0)
#define mMinorOtapV_c                 (0)
#define mConsOtapV_c                  (0)

/*Otap Protocol Frames defines ************************************************/
#define mOtapConfirmFrameId_c	        (0x10)
#define mDescriptorFrameId_c	        (0x20)
#define mStartOfTransmissionFrameId_c	(0x30)
#define mDataFrameId_c                (0x32)
#define mEndOfTransmissionFrameId_c	  (0x34)
#define mErrorFrameId_c	              (0x40)
#define mAckFrameId_c	                (0x50)
#define mWaitFrameId_c	              (0x52)

#define mOtapDescriptorFrameLenght_c  (31)
#define mOtapAckFrameLenght_c         (6)
#define mOtapErrorFrameLenght_c       (7)
#define mOtapWaitFrameLenght_c        (8)

#define mErrTypeUserCancel_c	        (0x10)
#define mErrTypeAcknowledgeError_c	  (0x20)
#define mErrTypeCommunicationError_c	(0x30)
#define mErrTypeTransferError_c	      (0x40)

#define mTxTypeFlashRoutines_c	      (0x10)
#define mTxTypeFirmware_c	            (0x20)
#define mTxTypeCalibrationValues_c	  (0x30)  
#define mTxTypeDeviceAddress_c	      (0x40)

/*Times are in 16us steps*/
#define mOtapComfirmTimeoutTime_c     (62500) //1s
#define mOtapAckTimeoutTime_c         (313)   //5ms
#define mOtapStartOfTxTimeoutTime_c   (625000)//10s
#define mOtapDataTimeoutTime_c        (625000)//10s

#define mOtapBroadcastAddress_c       (0xFFFF)

#define mAddressOfDevIdentifiedStart_c (8)
#define mAddressOfDevIdentifiedEnd_c   (123)

#define mCodeLinesFifoQueueSize_c     (4)
#define mCodeLinesFifoQueueMask_c     (mCodeLinesFifoQueueSize_c-1)

#define mOtapHcs08DeviceType_c        (11)


#if defined(gMc1323xPlatform_d)
  #define gOtapTimer_c gTmr2_c
  #define gOtapTimerChannel_c gTmrChannel0_c
  #define gOtapTimerInterruptSource_c gTmrChannel0Event_c

  #define mOtapClockDivisor_c           (gTmrClkDivBy128_c)
  #define mOtapEepromMarkAddress_c      (0x00002854)
  #define mOtapEepromCodeStartAddress_c (0x0000202C)
  #define mOtapEepromCodeEndAddress_c   (0x00013FFF)

#elif defined(gMcs08qe128Platform_d)
  #define gOtapTimer_c gTmr3_c   
  #define gOtapTimerChannel_c gTmrChannel4_c
  #define gOtapTimerInterruptSource_c gTmrChannel4Event_c

  #define mOtapClockDivisor_c           (gTmrClkDivBy64_c)
  #define mOtapEepromMarkAddress_c      (0x00002854)
  #define mOtapEepromCodeStartAddress_c (0x00002880)
  #define mOtapEepromCodeEndAddress_c   (0x0001FFFF)

#elif defined(gMcs08Gt60Platform_d)
  #define gOtapTimer_c gTmr1_c 
  #define gOtapTimerChannel_c gTmrChannel2_c
  #define gOtapTimerInterruptSource_c gTmrChannel2Event_c

  #define mOtapClockDivisor_c           (gTmrClkDivBy64_c)
  #define mOtapEepromMarkAddress_c      (0x00002000)
  #define mOtapEepromCodeStartAddress_c (0x00002880)
  #define mOtapEepromCodeEndAddress_c   (0x0000FFFF)
#endif

#define m10msTimerCounts_c             (1250)

#define mLastDataOk_c                  (3)
#define mDataQueueFullWait_c           (2)
#define mDataSequenceError_c           (1) 
#define mDataOk_c                      (0)

#define mOtapWaitTimeMs_c              (500)

#define mOtapEepromMark_c              ("OkGo")
#define mOtapEepromMark0_c             ('O')
#define mOtapEepromMark1_c             ('k')
#define mOtapEepromMark2_c             ('G')
#define mOtapEepromMark3_c             ('o')

#define mOtapEepromFailMark_c          ("Fail")
#define mOtapEepromFailMark0_c         ('F')
#define mOtapEepromFailMark1_c         ('a')
#define mOtapEepromFailMark2_c         ('i')
#define mOtapEepromFailMark3_c         ('l')

#define mOtapEepromDoneMark_c          ("Done")
#define mOtapEepromDoneMark0_c         ('D')
#define mOtapEepromDoneMark1_c         ('o')
#define mOtapEepromDoneMark2_c         ('n')
#define mOtapEepromDoneMark3_c         ('e')

#define mOtapEepromMarkSize_c          (4)
#define mOtapChekSumSize_c             (4)

#define OtapGetOtapRxFrame(pPacket) OtapRxPacket = pPacket

#define IsFifoFull(InIndex, OutIndex)  (InIndex == OutIndex)
#define IsFifoEmpty(InIndex, OutIndex) (((InIndex - OutIndex) & mCodeLinesFifoQueueMask_c) == mCodeLinesFifoQueueMask_c)
#define FifoMoveIndexForward(Index)    Index = ((Index + 1) & mCodeLinesFifoQueueMask_c)

#if defined(gMc1321xPlatform_d) || defined(gMcs08Gt60Platform_d)
#pragma DATA_SEG NOT_INIT_RAM
extern volatile uint8_t Illegal_OP;

#define OtapCallOFLC()         \
          Illegal_OP[0] = 0xAC;\
          Illegal_OP[1] = 0xAC;\
          __asm JMP Illegal_OP 
         
#pragma DATA_SEG DEFAULT
#elif defined(gMcs08qe128Platform_d) || defined(gMc1323xPlatform_d)

#define mStopeMask_c (0x20)

#define OtapCallOFLC()                \
          if(SOPT1 & mStopeMask_c)    \
          {                           \
            __asm JMP Otap_Startup;   \
          }                           \
          else                        \
          {                           \
            __asm STOP;               \
          }
#endif

  
#ifndef MEMORY_MODEL_BANKED
#define OtapJumpToApp()  __asm JMP _Startup 
#else
#define OtapJumpToApp()  __asm CALL _Startup;
#endif

/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)
#pragma CODE_SEG OFLC_ROUTINES
bool_t __near OtapIsThereANewCodeFileOnEEPROM(void);
bool_t __near OtapValidateCheckSumOnEEPROM(void);
void   __near OtapMoveTheNewCodeFileToFlash(void);
void   __near OtapWriteMarkOnEEPROM(uint8_t * pu8Mark);
void   __near OtapInitOflcHardware(void);
bool_t __near OtapOflcStringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare);
#pragma CODE_SEG DEFAULT

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
extern void _Startup (void);
void   OtapOpcEepromCallBack(eepromErr_t eeproWriteResult);
void   OtapTimerEvent(void); 
bool_t OtapIsOtapRequestFrame(rxPacket_t *pRxPacket);
void   OtapGetProgrammerAddress(rxPacket_t *pRxPacket);
bool_t OtapIsOtapRequestDestAddress(rxPacket_t *pRxPacket, uint16_t u16OtapDestAddress);
bool_t IsMyAddressInTheIdentifiedList(rxPacket_t *pRxPacket);
#pragma CODE_SEG DEFAULT
void OtapOpcManageRxEvents(void);
void OtapOpcManageTxEvents(void);
void OtapOpcManageTimerEvents(void);
void OtapCreateAckFrame(txPacket_t *pTxPacket);
void OtapCreateErrorFrame(txPacket_t *pTxPacket);
void OtapCreateWaitFrame(txPacket_t *pTxPacket);
void OtapStarBackOffTime(void);
void OtapCreateDescriptorFrame(txPacket_t *pTxPacket);
void OtapCreateFrameHeader(txPacket_t *pTxPacket, uint8_t u8FrameId, uint8_t u8FrameLength);
bool_t OtapIsFrame(rxPacket_t *pRxPacket, uint8_t u8FrameId);
bool_t OtapIsMyAckFrame(rxPacket_t *pRxPacket);
bool_t OtapIsFrameDestAddress(rxPacket_t *pRxPacket, uint16_t u16OtapDestAddress);
void OtapGetNumberOfMessagesToReceive(rxPacket_t *pRxPacket);
void OtapSaveData(void);
void OtapEndOpcProccess(void);
uint8_t OtapGetNextDataMessage(rxPacket_t *pRxPacket);
void OtapOpcStringCopy(uint8_t * pu8Srce, uint8_t * pu8Dest, uint8_t u8StrSize);
#endif /*(TRUE == gOtapSupported_d)*/

#endif /* _OTAP_H_ */
