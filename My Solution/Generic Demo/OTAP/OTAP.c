/******************************************************************************
* Source file for OTAP module.
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*   
*******************************************************************************/

/*Include files secction*/
#include "OTAP.h"

/******************************************************************************
*******************************************************************************
* Private constants
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)
const uint8_t au8OtapVersion[4] = 
{
  mMajorOtapV_c, 
  mMiddleOtapV_c, 
  mMinorOtapV_c, 
  mConsOtapV_c
};
#endif /*(TRUE == gOtapSupported_d)*/

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)
/**OPC Global data ************************************************************/
static otapStates_t        smOpcState = mOtapStateNoInit_c;
static bool_t              bOtapTimeoutFlag;
static bool_t              bOtapPacketReceivedFlag;
static bool_t              bOtapPacketTransmittedFlag;
static bool_t              bOtapTimerFlag;
static bool_t              bOtapWriteFinishedFlag;
static uint8_t             u8OtapTxSequenceNumber;
static uint8_t             au8OtapProgrammerAddress[2];
static uint8_t             u8OtapTimerCount;
static uint16_t            su16OtapDataMessagesToReceive;
static uint16_t            su16OtapDataMessagesCount;
static eepromDataPackage_t otapEepromDataPackage;
static otapCodeLine_t      mCodeLinesFifo[mCodeLinesFifoQueueSize_c];
static uint8_t             mu8CodeLinesFifoInputIndex;
static uint8_t             mu8CodeLinesFifoOutputIndex;
static uint8_t             sau8CodeCheckSum[4];
static rxPacket_t          * OtapRxPacket;
static txPacket_t          * OtapTxPacket;

/**OFLC Global data ***********************************************************/
#pragma DATA_SEG NOT_INIT_RAM

#if defined(gMc1321xPlatform_d) || defined(gMcs08Gt60Platform_d)
 volatile uint8_t           Illegal_OP[2];
#endif
 static uint8_t             au8OflcDataArray[32];
 static uint32_t            su32EepromCheckSum;

#pragma DATA_SEG DEFAULT

#endif /*(TRUE == gOtapSupported_d)*/


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)

#pragma CODE_SEG OFLC_ROUTINES

/*Otap Firlware Loader (OFLC) Routines they are placed in reserved Flash ******/

/******************************************************************************
* Otap_Startup
*
*******************************************************************************/
void __near Otap_Startup(void)
{
  INIT_SP_FROM_STARTUP_DESC();
/* with a C style main(); we would push the return 
   address on the stack wasting 2 RAM bytes */
#ifndef MEMORY_MODEL_BANKED
  __asm JMP Otap_OflcMain; 
#else
  __asm CALL Otap_OflcMain;
#endif
}

/******************************************************************************
* Otap_OflcMain
*
*******************************************************************************/
void __near Otap_OflcMain(void)
{

  OtapInitOflcHardware();
  
  if(OtapIsThereANewCodeFileOnEEPROM()){
    if(OtapValidateCheckSumOnEEPROM()){
      OtapMoveTheNewCodeFileToFlash();
      OtapWriteMarkOnEEPROM(mOtapEepromDoneMark_c);
      OtapCallOFLC();
    }else{
      OtapWriteMarkOnEEPROM(mOtapEepromFailMark_c);
    }
  }
  OtapJumpToApp();
}
#pragma CODE_SEG DEFAULT

#endif /*(TRUE == gOtapSupported_d)*/

#if(TRUE == gOtapSupported_d)
/*Otap Protocol Component (OPC) Routines they are placed in Application Flash */

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/******************************************************************************
* Otap_OpcMCPSDataComfirm
*
*******************************************************************************/
void Otap_OpcMCPSDataComfirm(txStatus_t *pTxResult)
{
  (void)pTxResult;
   if(mOtapStateAppMode_c != smOpcState){
     bOtapPacketTransmittedFlag = TRUE;
   }
}

/******************************************************************************
* Otap_OpcMCPSDataIndication
*
*******************************************************************************/
void Otap_OpcMCPSDataIndication(rxPacket_t *pRxPacket)
{
  if(rxSuccessStatus_c == pRxPacket->rxStatus){
    bOtapTimeoutFlag = FALSE;
    if(mOtapStateAppMode_c == smOpcState){
      if(OtapIsOtapRequestFrame(pRxPacket)){
        if(OtapIsOtapRequestDestAddress(pRxPacket, gOtapId_c)){
          smOpcState = mOtapStateOtapRequestReceived_c; 
          bOtapPacketReceivedFlag = TRUE;
        }else if(OtapIsOtapRequestDestAddress(pRxPacket, mOtapBroadcastAddress_c)){
          OtapGetProgrammerAddress(pRxPacket);
          if(!IsMyAddressInTheIdentifiedList(pRxPacket)){
             OtapGetOtapRxFrame(pRxPacket);
             smOpcState = mOtapStateBroadcastOtapRequestReceived_c; 
             bOtapPacketReceivedFlag = TRUE;
          }
        }
      }
    }else{
      OtapGetOtapRxFrame(pRxPacket);
      bOtapPacketReceivedFlag = TRUE;
    }
  }else if(rxTimeOutStatus_c == pRxPacket->rxStatus){
    if(mOtapStateAppMode_c != smOpcState){
      bOtapTimeoutFlag = TRUE;
    }
  }
  
  if(mOtapStateAppMode_c != smOpcState){
    pRxPacket->rxStatus = rxAbortedStatus_c; //Avoids the App to manage Otap frames.
  }
}

#pragma CODE_SEG DEFAULT

/******************************************************************************
* Otap_OpcMain
*
*******************************************************************************/
void Otap_OpcMain(void)
{
  while(mOtapStateAppMode_c != smOpcState)
  {
    if(bOtapTimeoutFlag){
      bOtapTimeoutFlag = FALSE;
      smOpcState = mOtapStateAppMode_c;
    }else if(bOtapPacketReceivedFlag){
      bOtapPacketReceivedFlag = FALSE;
      OtapOpcManageRxEvents();
    }else if(bOtapPacketTransmittedFlag){
      bOtapPacketTransmittedFlag = FALSE;
      OtapOpcManageTxEvents();
    }else if(bOtapTimerFlag){
      bOtapTimerFlag = FALSE;
      OtapOpcManageTimerEvents();
    }
    
    if(mOtapStateWaitSaveData_c == smOpcState){
      if(IsFifoEmpty(mu8CodeLinesFifoInputIndex, mu8CodeLinesFifoOutputIndex)){
        OtapEndOpcProccess();
      }
    }
    OtapSaveData();
  }
}

/******************************************************************************
* Otap_OpcInit
*
*******************************************************************************/
void Otap_OpcInit(void)
{
  static tmrChannelConfig_t timerConfig;   

  smOpcState                    = mOtapStateAppMode_c;
  bOtapTimeoutFlag              = FALSE;
  bOtapPacketReceivedFlag       = FALSE;
  bOtapPacketTransmittedFlag    = FALSE;
  bOtapTimerFlag                = FALSE;
  bOtapWriteFinishedFlag        = TRUE;
  u8OtapTxSequenceNumber        = 0;
  su16OtapDataMessagesToReceive = 0;
  au8OtapProgrammerAddress[0]   = 0;
  au8OtapProgrammerAddress[1]   = 0;
  mu8CodeLinesFifoInputIndex    = 0;
  mu8CodeLinesFifoOutputIndex   = 0;

  /*Otap OPC timer initialization*/
  timerConfig.tmrChannel                   = gOtapTimerChannel_c;
  timerConfig.tmrChannOptMode              = gTmrOutputCompare_c;
  timerConfig.tmrPinConfig.tmrOutCompState = gTmrPinNotUsedForOutComp_c;
  (void)Tmr_SetCallbackFunc(gOtapTimer_c, gOtapTimerInterruptSource_c, (tmrCallbackFunction_t)OtapTimerEvent);
  (void)Tmr_SetChannelConfig(gOtapTimer_c, &timerConfig);

}
#else /*(TRUE == gOtapSupported_d)*/

/*Otap Protocol Component (OPC) Routines they are placed in Application Flash */

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/******************************************************************************
* Otap_OpcMCPSDataComfirm
*
*******************************************************************************/
void Otap_OpcMCPSDataComfirm(txStatus_t *pTxResult)
{(void)pTxResult;}

/******************************************************************************
* Otap_OpcMCPSDataIndication
*
*******************************************************************************/
void Otap_OpcMCPSDataIndication(rxPacket_t *pRxPacket)
{(void)pRxPacket;}

#pragma CODE_SEG DEFAULT

/******************************************************************************
* Otap_OpcMain
*
*******************************************************************************/
void Otap_OpcMain(void)
{}

/******************************************************************************
* Otap_OpcInit
*
*******************************************************************************/
void Otap_OpcInit(void)
{}

#endif /*(TRUE == gOtapSupported_d)*/


/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)

#pragma CODE_SEG OFLC_ROUTINES
//******************************************************************************
bool_t __near OtapIsThereANewCodeFileOnEEPROM(void)
{
  /*Read the Mark and check sum from eeprom*/
  (void)Eeprom4OTAP_Read(mOtapEepromMarkAddress_c,au8OflcDataArray,mOtapEepromMarkSize_c + mOtapChekSumSize_c);

  /*Looks for the "new code" mark in the loaded data*/
  if(OtapOflcStringComp(au8OflcDataArray, mOtapEepromMark_c,4)){
    StringCopy(au8OflcDataArray, (uint8_t*)&su32EepromCheckSum, 4);
    return TRUE;
  }else{
    return FALSE;
  }
}

//******************************************************************************
bool_t __near OtapValidateCheckSumOnEEPROM(void)
{
  uint32_t eepromAddress;
  uint32_t CheckSum;
  uint8_t u8i;
  
  CheckSum = 0;
  
  /*Cycle to read all the code bytes in EEPROM and calculate the CheckSum*/
  for(eepromAddress = mOtapEepromCodeStartAddress_c; 
      eepromAddress < mOtapEepromCodeEndAddress_c+1;   
      eepromAddress+=32)
  {
    /*Read the next code segment from eeprom*/
    (void)Eeprom4OTAP_Read(eepromAddress,au8OflcDataArray,32);
    
    for(u8i = 0;u8i < 32;u8i++)
    {
      CheckSum += ((uint32_t)au8OflcDataArray[u8i]) & 0x000000FF; 
    }
  }

  if(CheckSum == su32EepromCheckSum){
    return TRUE;
  }else{
    return FALSE;  
  }
}

//******************************************************************************
void __near OtapMoveTheNewCodeFileToFlash(void)
{
  uint32_t eepromAddress = mOtapEepromCodeStartAddress_c;

  
  (void)Flash4OTAP_ErasePage(eepromAddress);
  
  /*Cycle to copy all the code bytes in EEPROM to flash*/
  for(eepromAddress = mOtapEepromCodeStartAddress_c; 
      eepromAddress < mOtapEepromCodeEndAddress_c+1;   
      eepromAddress+=32)
  {
    /*Read the next code segment from eeprom*/
    (void)Eeprom4OTAP_Read(eepromAddress,au8OflcDataArray,32);
    /*Write the next code segment on flash*/
    (void)Flash4OTAP_WriteString(eepromAddress, au8OflcDataArray, 32);
    eepromAddress+=32;
    if(!(eepromAddress%512)){
      (void)Flash4OTAP_ErasePage(eepromAddress);
    } 
  }
}

//******************************************************************************
void __near OtapWriteMarkOnEEPROM(uint8_t * pu8Mark)
{
  StringCopy(pu8Mark, au8OflcDataArray,4);
  (void)Eeprom4OTAP_Write(mOtapEepromMarkAddress_c, au8OflcDataArray, mOtapEepromMarkSize_c);
}


//******************************************************************************
void __near OtapInitOflcHardware(void)
{ 
  DisableInterrupts; 
  (void)Eeprom4OTAP_Init();
  (void)Flash4OTAP_Init(gFlashClk4MHz_c);
}

//******************************************************************************
bool_t __near OtapOflcStringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare)
{
  do
  {
  }while((*au8leftString++ == *au8RightString++) && --bytesToCompare);
  return(0 == bytesToCompare);
}

#pragma CODE_SEG DEFAULT


/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

//******************************************************************************
void OtapOpcEepromCallBack(eepromErr_t eeproWriteResult)
{
  if(gEepromNoErr_c == eeproWriteResult){
    FifoMoveIndexForward(mu8CodeLinesFifoOutputIndex);
  }
  bOtapWriteFinishedFlag = TRUE;
}

//******************************************************************************
void OtapTimerEvent(void) 
{
  if(!(--u8OtapTimerCount)){
    bOtapTimerFlag = TRUE; 
    (void)Tmr_Disable(gOtapTimer_c); 
  }
}


//******************************************************************************
bool_t OtapIsOtapRequestFrame(rxPacket_t *pRxPacket)
{
  return(((uint8_t)((gOtapRequestCode_c>>24)&0x000000FF) == pRxPacket->smacPdu.u8Data[0]) &&\
         ((uint8_t)((gOtapRequestCode_c>>16)&0x000000FF) == pRxPacket->smacPdu.u8Data[1]) &&\
         ((uint8_t)((gOtapRequestCode_c>>8)&0x000000FF)  == pRxPacket->smacPdu.u8Data[2]) &&\
         ((uint8_t)((gOtapRequestCode_c)&0x000000FF)     == pRxPacket->smacPdu.u8Data[3]));
}

//******************************************************************************
bool_t OtapIsOtapRequestDestAddress(rxPacket_t *pRxPacket, uint16_t u16OtapDestAddress)
{
  return(((uint8_t)((u16OtapDestAddress>>8)&0x00FF)  == pRxPacket->smacPdu.u8Data[6]) &&\
         ((uint8_t)((u16OtapDestAddress)&0x00FF)     == pRxPacket->smacPdu.u8Data[7]));
}

//******************************************************************************
bool_t IsMyAddressInTheIdentifiedList(rxPacket_t *pRxPacket)
{
  uint8_t u8i;
  
  for(u8i = mAddressOfDevIdentifiedStart_c;u8i< mAddressOfDevIdentifiedEnd_c; u8i+=2)
  {
    if(((uint8_t)((gOtapId_c>>8)&0x00FF)  == pRxPacket->smacPdu.u8Data[u8i]) &&\
       ((uint8_t)((gOtapId_c)&0x00FF)     == pRxPacket->smacPdu.u8Data[u8i+1])){
      return TRUE;
    }
  }
  return FALSE;
}

//******************************************************************************
void OtapGetProgrammerAddress(rxPacket_t *pRxPacket)
{
  au8OtapProgrammerAddress[0] = pRxPacket->smacPdu.u8Data[4];
  au8OtapProgrammerAddress[1] = pRxPacket->smacPdu.u8Data[5];
}



#pragma CODE_SEG DEFAULT


//******************************************************************************
void OtapOpcManageRxEvents(void)
{
  switch(smOpcState) 
  {
    case mOtapStateOtapRequestReceived_c:
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapComfirmTimeoutTime_c);
         smOpcState = mOtapStateWaitComfirm_c;
         break;
    case mOtapStateBroadcastOtapRequestReceived_c:
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapComfirmTimeoutTime_c);
         smOpcState = mOtapStateWaitBroadcastComfirm_c;
         break;
    case mOtapStateWaitComfirm_c:
         if(OtapIsFrame(OtapRxPacket, mOtapConfirmFrameId_c) 
            && OtapIsFrameDestAddress(OtapRxPacket, gOtapId_c)){
           OtapTxPacket = (txPacket_t *)(&OtapRxPacket->u8DataLength);
           OtapCreateDescriptorFrame(OtapTxPacket);
           (void)MCPSDataRequest(OtapTxPacket);
           smOpcState = mOtapStateSendDescriptor_c; 
         }
         break;
    case mOtapStateWaitBroadcastComfirm_c:
         if(OtapIsFrame(OtapRxPacket, mOtapConfirmFrameId_c) 
            && OtapIsFrameDestAddress(OtapRxPacket, mOtapBroadcastAddress_c)){
           smOpcState = mOtapStateStartIdentificationProcess_c; 
           OtapStarBackOffTime();
         }
         break;
    case mOtapStateWaitIdentificationProcessAck_c:
         if(OtapIsMyAckFrame(OtapRxPacket)){
           smOpcState = mOtapStateAppMode_c;   
         }
         break;
    case mOtapStateWaitDescriptorAck_c:
         if(OtapIsMyAckFrame(OtapRxPacket)){
           (void)MLMERXEnableRequest(OtapRxPacket, mOtapStartOfTxTimeoutTime_c);
           smOpcState = mOtapStateWaitStartOfTransmission_c;   
         }
         break;
    case mOtapStateWaitStartOfTransmission_c:
         if(OtapIsFrame(OtapRxPacket, mStartOfTransmissionFrameId_c) 
            && OtapIsFrameDestAddress(OtapRxPacket, gOtapId_c)){
           OtapGetNumberOfMessagesToReceive(OtapRxPacket);
           OtapCreateAckFrame(OtapTxPacket);
           (void)MCPSDataRequest(OtapTxPacket);
           (void)Eeprom_Erase();
           smOpcState = mOtapStateAckStartOfTransmission_c; 
         }
         break;     
    case mOtapStateWaitForData_c:
         if(OtapIsFrame(OtapRxPacket, mDataFrameId_c) 
            && OtapIsFrameDestAddress(OtapRxPacket, gOtapId_c)){
           uint8_t u8Res;
           u8Res = OtapGetNextDataMessage(OtapRxPacket);
           if(mDataOk_c == u8Res)
           {
             OtapCreateAckFrame(OtapTxPacket);                
             smOpcState = mOtapAckData_c; 
           }else if(mLastDataOk_c == u8Res){
             OtapCreateErrorFrame(OtapTxPacket);                                  
             smOpcState = mOtapAckLastData_c; 
           }else if(mDataSequenceError_c == u8Res){
             OtapCreateErrorFrame(OtapTxPacket);                                  
             smOpcState = mOtapStateSendError_c; 
           }else if(mDataQueueFullWait_c == u8Res){
             OtapCreateWaitFrame(OtapTxPacket);     
             smOpcState = mOtapStateSendWait_c; 
           }
           (void)MCPSDataRequest(OtapTxPacket);
         }
         break;
    case mOtapStateWaitEndOfTransmission_c:
         if(OtapIsFrame(OtapRxPacket, mEndOfTransmissionFrameId_c)
            && OtapIsFrameDestAddress(OtapRxPacket, gOtapId_c)){
           OtapCreateAckFrame(OtapTxPacket);
           (void)MCPSDataRequest(OtapTxPacket);
           smOpcState = mOtapStateAckEndOfTransmission_c; 
         }
         break;
    default:
      smOpcState = mOtapStateAppMode_c;   
      break;
  }
}
//******************************************************************************
void OtapOpcManageTxEvents(void)
{
  switch(smOpcState) 
  {
    case mOtapStateSendDescriptor_c:
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapAckTimeoutTime_c);
         smOpcState = mOtapStateWaitDescriptorAck_c;
         break;
    case mOtapStateIdentificationProcessSendDescriptor_c:
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapAckTimeoutTime_c);
         smOpcState = mOtapStateWaitIdentificationProcessAck_c;
         break;
    case mOtapStateAckStartOfTransmission_c:
    case mOtapAckData_c:
    case mOtapStateSendWait_c:
    case mOtapStateSendError_c:        
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapDataTimeoutTime_c);
         smOpcState = mOtapStateWaitForData_c;
         break;
    case mOtapAckLastData_c:
         (void)MLMERXEnableRequest(OtapRxPacket, mOtapDataTimeoutTime_c);
         smOpcState = mOtapStateWaitEndOfTransmission_c;
         break;
    case mOtapStateAckEndOfTransmission_c:
         smOpcState = mOtapStateWaitSaveData_c;
         break;
  }
}
//******************************************************************************
void OtapOpcManageTimerEvents(void)
{
  if(mOtapStateStartIdentificationProcess_c == smOpcState){
    OtapTxPacket = (txPacket_t *)(&OtapRxPacket->u8DataLength);
    OtapCreateDescriptorFrame(OtapTxPacket);
    (void)MCPSDataRequest(OtapTxPacket);
    smOpcState = mOtapStateIdentificationProcessSendDescriptor_c; 
  }
}
//******************************************************************************
void OtapStarBackOffTime(void)
{
  u8OtapTimerCount = (uint8_t)((gOtapId_c)&0x00FF); 
  (void)Tmr_Enable(gOtapTimer_c, mOtapClockDivisor_c, m10msTimerCounts_c);  
}
//******************************************************************************

void OtapCreateAckFrame(txPacket_t *pTxPacket)
{
  /*Frame Header*/
  pTxPacket->u8DataLength      = mOtapAckFrameLenght_c;
  pTxPacket->smacPdu.u8Data[0] = mAckFrameId_c;
  pTxPacket->smacPdu.u8Data[1] = au8OtapProgrammerAddress[0];
  pTxPacket->smacPdu.u8Data[2] = au8OtapProgrammerAddress[1];
  pTxPacket->smacPdu.u8Data[3] = (uint8_t)((gOtapId_c>>8)&0x00FF);
  pTxPacket->smacPdu.u8Data[4] = (uint8_t)((gOtapId_c)&0x00FF);
}

//******************************************************************************

void OtapCreateErrorFrame(txPacket_t *pTxPacket)
{
  /*Frame Header*/
  OtapCreateFrameHeader(pTxPacket, mErrorFrameId_c, mOtapErrorFrameLenght_c);
  /*Frame Content*/
  pTxPacket->smacPdu.u8Data[6] = mErrTypeTransferError_c;
}

//******************************************************************************

void OtapCreateWaitFrame(txPacket_t *pTxPacket)
{
  /*Frame Header*/
  OtapCreateFrameHeader(pTxPacket, mWaitFrameId_c, mOtapWaitFrameLenght_c);
  /*Frame Content*/  
  pTxPacket->smacPdu.u8Data[6] = (uint8_t)((mOtapWaitTimeMs_c>>8)&0x00FF);
  pTxPacket->smacPdu.u8Data[7] = (uint8_t)((mOtapWaitTimeMs_c)&0x00FF);
}

//******************************************************************************
void OtapCreateDescriptorFrame(txPacket_t *pTxPacket)
{
  uint8_t au8SmacVer[7];
  
  /*Frame Header*/
  OtapCreateFrameHeader(pTxPacket, mDescriptorFrameId_c, mOtapDescriptorFrameLenght_c);
  
  (void)MLMEGetRficVersion(gSwSmacVersion_c, au8SmacVer);
  /*Frame Content*/

  OtapOpcStringCopy(au8SmacVer,&(pTxPacket->smacPdu.u8Data[6]), 4);
  OtapOpcStringCopy((uint8_t*)au8OtapVersion,&(pTxPacket->smacPdu.u8Data[10]), 4);
  pTxPacket->smacPdu.u8Data[14] = mOtapHcs08DeviceType_c;
  StringCopy(gOtapDeviceDescription_c, &(pTxPacket->smacPdu.u8Data[15]), 16);
  
}

//******************************************************************************
void OtapCreateFrameHeader(txPacket_t *pTxPacket, uint8_t u8FrameId, 
                           uint8_t u8FrameLength)
{
  /*Frame Header*/
  pTxPacket->u8DataLength      = u8FrameLength;
  pTxPacket->smacPdu.u8Data[0] = u8FrameId;
  pTxPacket->smacPdu.u8Data[1] = au8OtapProgrammerAddress[0];
  pTxPacket->smacPdu.u8Data[2] = au8OtapProgrammerAddress[1];
  pTxPacket->smacPdu.u8Data[3] = (uint8_t)((gOtapId_c>>8)&0x00FF);
  pTxPacket->smacPdu.u8Data[4] = (uint8_t)((gOtapId_c)&0x00FF);
  pTxPacket->smacPdu.u8Data[5] = u8OtapTxSequenceNumber++;
}

//******************************************************************************
bool_t OtapIsFrame(rxPacket_t *pRxPacket, uint8_t u8FrameId)
{
  return((u8FrameId                   == pRxPacket->smacPdu.u8Data[0]) && \
         (au8OtapProgrammerAddress[0] == pRxPacket->smacPdu.u8Data[1]) && \
         (au8OtapProgrammerAddress[1] == pRxPacket->smacPdu.u8Data[2]));
}

//******************************************************************************
bool_t OtapIsMyAckFrame(rxPacket_t *pRxPacket)
{
  return((OtapIsFrameDestAddress(pRxPacket, gOtapId_c)          ) && \
         (OtapIsFrame(pRxPacket, mAckFrameId_c)                 ) && \
         (u8OtapTxSequenceNumber == pRxPacket->smacPdu.u8Data[5]));
}

//******************************************************************************
bool_t OtapIsFrameDestAddress(rxPacket_t *pRxPacket, uint16_t u16OtapDestAddress)
{
  return(((uint8_t)((u16OtapDestAddress>>8)&0x00FF)  == pRxPacket->smacPdu.u8Data[3]) &&\
         ((uint8_t)((u16OtapDestAddress)&0x00FF)     == pRxPacket->smacPdu.u8Data[4]));
}


//******************************************************************************
void OtapGetNumberOfMessagesToReceive(rxPacket_t *pRxPacket)
{
  su16OtapDataMessagesToReceive =  (((uint16_t)pRxPacket->smacPdu.u8Data[7]) << 8) & 0xFF00;
  su16OtapDataMessagesToReceive += ((uint16_t)pRxPacket->smacPdu.u8Data[8]) & 0x00FF;
  
  su16OtapDataMessagesCount = 0;
}

//******************************************************************************
uint8_t OtapGetNextDataMessage(rxPacket_t *pRxPacket)
{
  uint16_t u16FrameMessagesCount;
 
  u16FrameMessagesCount =  (((uint16_t)pRxPacket->smacPdu.u8Data[6]) << 8) & 0xFF00;
  u16FrameMessagesCount += ((uint16_t)pRxPacket->smacPdu.u8Data[7]) & 0x00FF;
  su16OtapDataMessagesCount++;
 
  if(u16FrameMessagesCount != su16OtapDataMessagesCount){
    return mDataSequenceError_c;        
  }else if(IsFifoFull(mu8CodeLinesFifoInputIndex, mu8CodeLinesFifoOutputIndex)){
    return mDataQueueFullWait_c;    
  }else{
    if(su16OtapDataMessagesToReceive == su16OtapDataMessagesCount){
      OtapOpcStringCopy(&(pRxPacket->smacPdu.u8Data[8]), sau8CodeCheckSum, 4);
      return mLastDataOk_c;        
    }else{
      mCodeLinesFifo[mu8CodeLinesFifoInputIndex].ByteCount     = pRxPacket->smacPdu.u8Data[8];
      OtapOpcStringCopy(&(pRxPacket->smacPdu.u8Data[9]), 
                        (mCodeLinesFifo[mu8CodeLinesFifoInputIndex].Address.u8), 4);
      OtapOpcStringCopy(&(pRxPacket->smacPdu.u8Data[13]), 
                        (mCodeLinesFifo[mu8CodeLinesFifoInputIndex].Data),  
                        mCodeLinesFifo[mu8CodeLinesFifoInputIndex].ByteCount);
     
      FifoMoveIndexForward(mu8CodeLinesFifoInputIndex);
      return mDataOk_c;        
    }
  }
}

//******************************************************************************
void OtapSaveData(void)
{
  eepromErr_t res;
  if(!IsFifoEmpty(mu8CodeLinesFifoInputIndex, mu8CodeLinesFifoOutputIndex) && bOtapWriteFinishedFlag)
  {
    otapEepromDataPackage.EepromAddress.u32Address = mCodeLinesFifo[mu8CodeLinesFifoOutputIndex].Address.u32;
    otapEepromDataPackage.u8DataSize = mCodeLinesFifo[mu8CodeLinesFifoOutputIndex].ByteCount;
    otapEepromDataPackage.pu8Data = mCodeLinesFifo[mu8CodeLinesFifoOutputIndex].Data;
    otapEepromDataPackage.pfEepromCallback = OtapOpcEepromCallBack;
    bOtapWriteFinishedFlag = FALSE;
    res = Eeprom_Write(&otapEepromDataPackage);
    if(res){
      bOtapWriteFinishedFlag = TRUE;/*It forces it to try it again*/
    }
  }
}

//******************************************************************************
void OtapEndOpcProccess(void)
{
  eepromErr_t res;
  uint8_t au8MarkAndCheckSum[mOtapEepromMarkSize_c + mOtapChekSumSize_c];

  OtapOpcStringCopy(mOtapEepromMark_c, &au8MarkAndCheckSum[0], 4);
  OtapOpcStringCopy(sau8CodeCheckSum, &au8MarkAndCheckSum[4], 4);

  otapEepromDataPackage.EepromAddress.u32Address = mOtapEepromMarkAddress_c;
  otapEepromDataPackage.u8DataSize = mOtapEepromMarkSize_c + mOtapChekSumSize_c;
  otapEepromDataPackage.pu8Data = au8MarkAndCheckSum;
  otapEepromDataPackage.pfEepromCallback = OtapOpcEepromCallBack;
  bOtapWriteFinishedFlag = FALSE;
  res = Eeprom_Write(&otapEepromDataPackage);
  do
  {
    bOtapWriteFinishedFlag = TRUE;/*It forces it to try it again*/
  }while(res);
  while(bOtapWriteFinishedFlag);
  OtapCallOFLC();
}

//******************************************************************************
void OtapOpcStringCopy(uint8_t * pu8Srce, uint8_t * pu8Dest, uint8_t u8StrSize)
{
  uint8_t u8f;
  for(u8f = 0; u8f < u8StrSize;u8f++) *(pu8Dest+u8f) = *(pu8Srce+u8f);
}

#endif /*(TRUE == gOtapSupported_d)*/

