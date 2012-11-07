/*****************************************************************************
* SMAC implementation.
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "SMAC.h"


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

const uint8_t au8SmacVersion[7] = 
{
  mMajorSmacV_c, 
  mMiddleSmacV_c, 
  mMinorSmacV_c, 
  mMonthSmacV_c,
  mDaySmacV_c, 
  mYearSmacV_c, 
  mConsSmacV_c
};
                                
static uint32_t        smacTimeout;
static uint16_t        smacChannelsBeingScanned;
static uint8_t         smacEdValue;
static smacStates_t    smacState;
static phyRxParams_t   smacLastDataRxParams;
static channels_t      smacCurrentChannel;
static channels_t      smacScanCurrentChannel;
static channels_t      smacClearestChann;
static uint8_t         u8TestBuffer[65] = 
{ 
  0x42,
  0xff,0xc1,0xfb,0xe8,0x4c,0x90,0x72,0x8b,0xe7,0xb3,0x51,0x89,0x63,0xab,0x23,0x23,  
  0x02,0x84,0x18,0x72,0xaa,0x61,0x2f,0x3b,0x51,0xa8,0xe5,0x37,0x49,0xfb,0xc9,0xca,
  0x0c,0x18,0x53,0x2c,0xfd,0x45,0xe3,0x9a,0xe6,0xf1,0x5d,0xb0,0xb6,0x1b,0xb4,0xbe,
  0x2a,0x50,0xea,0xe9,0x0e,0x9c,0x4b,0x5e,0x57,0x24,0xcc,0xa1,0xb7,0x59,0xb8,0x87
};
static phyPacket_t     *pTestPacket;  

volatile static prssPacketPtr_t smacProccesPacketPtr;

flagsRegister_t smacFlags;

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/


/***********************************************************************************/
/******************************** SMAC Data primitives *****************************/
/***********************************************************************************/

/************************************************************************************
* MCPSDataRequest
* 
* This data primitive is used to send an over the air packet. This is an asyncronous 
* function,  it means it ask SMAC to transmit one OTA packet,  but when the runction 
* returns it is not sent already.
*
************************************************************************************/
smacErrors_t MCPSDataRequest
(
txPacket_t *psTxPacket //IN:Pointer to the packet to be transmitted
)
{
  
  uint8_t u8PhyRes = 0; 
  
#if(TRUE == smacParametersValidation_d)
  uint8_t u8MaxLen=0;
  
  if (mPromiscuousModeEnabled)
  {
    u8MaxLen = gMaxPromiscuousSmacSDULenght_c;
  }
  else
  {
    u8MaxLen = gMaxSmacSDULenght_c;
  }
  
  if((NULL == psTxPacket) || (u8MaxLen < psTxPacket->u8DataLength))
  {
    return gErrorOutOfRange_c;
  }
  
#endif

#if(TRUE == smacInitializationValidation_d)
 if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif
  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }
  
  smacProccesPacketPtr.smacTxPacketPointer  = SmacFormatTxPacket(psTxPacket);

  u8PhyRes = PhyPdDataRequest(smacProccesPacketPtr.smacTxPacketPointer, gDataReq_NoAck_NoCca_Unslotted_c, NULL);

  if(!u8PhyRes)
  {
    smacState= mSmacStateTransmitting_c; 
    return gErrorNoError_c;
  }
  else
  {
    return gErrorNoResourcesAvailable_c;
  }
}


/************************************************************************************
* MLMERXEnableRequest
* 
* Function used to place the radio into receive mode 
*
************************************************************************************/
smacErrors_t MLMERXEnableRequest
(
rxPacket_t *gsRxPacket, //OUT: Pointer to the structure where the reception results 
                        //     will be stored.
uint32_t u32Timeout     //IN:  32-bit timeout value, this is directly the value that 
                        //     is stored on the Radio's timer register.
)
{

  uint8_t u8PhyRes = 0; 
  phyPacket_t * pAuxPacket;

#if(TRUE == smacParametersValidation_d)
  uint8_t u8MaxLen=0;
#endif
  
  if (mPromiscuousModeEnabled)
  {
#if(TRUE == smacParametersValidation_d)
    u8MaxLen = gMaxPromiscuousSmacSDULenght_c;
#endif
    pAuxPacket = (phyPacket_t *)&gsRxPacket->smacPdu.reserved[1]; 
  }
  else
  {
#if(TRUE == smacParametersValidation_d)
    u8MaxLen = gMaxSmacSDULenght_c;
#endif
    pAuxPacket = (phyPacket_t *)&gsRxPacket->u8DataLength; 
  }
  
#if(TRUE == smacParametersValidation_d)
  if((NULL == gsRxPacket) || (u8MaxLen < gsRxPacket->u8MaxDataLength))
  {
    return gErrorOutOfRange_c;
  }
#endif
  


#if(TRUE == smacInitializationValidation_d)
 if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }

  
  u8PhyRes = PhyPlmeRxRequest(pAuxPacket, &smacLastDataRxParams);

  if(!u8PhyRes)
  {
  
    if(u32Timeout)
    {
      smacTimeout = u32Timeout; 
      SmacSetRxTimeout((zbClock24_t)smacTimeout);
      mSmacTimeoutAsked = TRUE;
    }
    else
    {
      mSmacTimeoutAsked = FALSE;
    }

    gsRxPacket->rxStatus = rxProcessingReceptionStatus_c;
    smacProccesPacketPtr.smacRxPacketPointer  = gsRxPacket;

    smacState= mSmacStateReceiving_c; 
    return gErrorNoError_c;
  }
  else
  {
    return gErrorNoResourcesAvailable_c;
  }
}

/************************************************************************************
* MLMERXDisableRequest
* 
* Returns the radio to idle mode from receive mode.
*
************************************************************************************/
smacErrors_t MLMERXDisableRequest(void)
{
#if(TRUE == smacInitializationValidation_d)
 if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif
  if((mSmacStateReceiving_c != smacState) && (mSmacStateIdle_c != smacState))
  {
    return gErrorNoValidCondition_c;
  }
  
  PhyPlmeForceTrxOffRequest();
  mSmacTimeoutAsked = FALSE;
  
  smacState= mSmacStateIdle_c;
  return gErrorNoError_c;

}
 

/************************************************************************************
* MLMELinkQuality
* 
* This  function  returns  an  integer  value  that is the link quality from the last 
* received packet of the form:  dBm = (-Link Quality/2).
*
************************************************************************************/
uint8_t MLMELinkQuality(void)
{
#if(TRUE == smacInitializationValidation_d)
 if(FALSE == mSmacInitialized)
  {
    return 0;
  }
#endif
  return smacLastDataRxParams.linkQuality;
}



/***********************************************************************************/
/******************************** SMAC Radio primitives ****************************/
/***********************************************************************************/

/************************************************************************************
* MLMERadioInit
* 
* This function initializes the Radio parameters.
*
************************************************************************************/
smacErrors_t MLMERadioInit(void)
{
  uint8_t u8PhyRes = 0; 
  
  PhyInit();
  
  u8PhyRes =  PhyPlmeSetCurrentChannelRequest((uint8_t)gChannel11_c);
  u8PhyRes += PhyPlmeSetPwrLevelRequest(15);
  
  PhyPpSetCcaThreshold(129); //
  PhyPpSetPromiscuous(TRUE);
  
  smacCurrentChannel = gChannel11_c;
  smacState= mSmacStateIdle_c;
  smacLastDataRxParams.linkQuality = 0;
  smacLastDataRxParams.timeStamp = 0;
  smacEdValue = 0;
  smacClearestChann = gChannelOutOfRange_c; 
  smacFlags.flags.flag0 = gPromiscuousMode_d;
  smacFlags.flags.flag1 = 0;
  smacFlags.flags.flag2 = 0;
  smacFlags.flags.flag3 = 0;
  smacFlags.flags.flag4 = 0;
  smacFlags.flags.flag5 = 0;
  smacFlags.flags.flag6 = 0;
  smacFlags.flags.flag7 = 0;
  /* it means:
              mPromiscuousModeEnabled = FALSE;
              mSmacInitialized        = FALSE;
              mSmacTimeoutAsked       = FALSE;
              mSmacPerformingCCAScan  = FALSE;
              mSmacPerformingEDScan   = FALSE;
              mSmacPerformingED       = FALSE;
  */
        
  if(!u8PhyRes)
  {
#if(TRUE == smacInitializationValidation_d)
    mSmacInitialized = TRUE;  
#endif
    return gErrorNoError_c;
  }
  else
  {
    return gErrorNoResourcesAvailable_c;
  }
}

/************************************************************************************
* MLMEDozeRequest
* 
* Doze request allow the user to put the radio either in Normal Doze Mode (without 
* CLKO but with automatic wake up) or Acoma Doze Mode (with CLKout, but without 
* timeout). 
* This primitive is used just on the MC1320x and MC1321x platforms, for 
* the MC1323x radio it is just an stub.
*
************************************************************************************/
smacErrors_t MLMEDozeRequest
(
uint32_t u32Timeout //IN: Is the automatic wake up time for this mode.
)
{
#if defined (gMcs08Gt60Platform_d) || defined (gMcs08qe128Platform_d)
	uint8_t u8Status = 0;	
	if(mSmacStateIdle_c != smacState)
		{
		      return gErrorBusy_c; /*the radio is busy in other activity as transmitting  
			                        or receiving data. Or performing a channel scan*/
		}
		
			
			u8Status = PhyPlmeDozeRequest(u32Timeout);
		    if (u8Status==gErrorNoError_c){
		    	smacState = mSmacStateDoze_c;
		    return gErrorNoError_c; /*The radio has been set in Doze Mode*/;
		    }
#endif
#if defined gMc1323xPlatform_d 
		    return gErrorNoError_c; 
	#else
		    return gErrorNoError_c;
#endif
}

/************************************************************************************
* MLMEEnergyDetect
* 
* This call starts an energy detect(ED) cycle, it returns the energy value (-power/2)
* via the returned argument. For example, if the Energy Detect returns 80 then the 
* interpreted value is -80/2 or -40 dBm.
*
************************************************************************************/
uint8_t MLMEEnergyDetect(void)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return 0xFF;
  }
#endif

  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }
  mSmacPerformingED = TRUE;
  smacEdValue = 0;
  
  if(PhyPlmeEdRequest())
  {
    return 0xFF;
  }
  
  while(TRUE == mSmacPerformingED)
  {
    /*Add a method to avoid blocking the system*/
  }
  
  return smacEdValue;
}

/************************************************************************************
* MLMEFEGainAdjust
* 
* This is a compensator for the energy detection and AGC. In case users need to 
* calibrate the readings due to a specific application like an external low noise 
* amplifier, this is where users can set the offset.
* This primitive is used just on the MC1320x and MC1321x platforms, for the 
* MC1323x radio it is just an stub.
*
************************************************************************************/
smacErrors_t MLMEFEGainAdjust
(
uint8_t u8GainValue //IN: 8 bit value for the gain adjust.
)
{
  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }
  PhyAdjustCcaOffsetCmp(u8GainValue);
  
  return gErrorNoError_c;
}

/************************************************************************************
* MLMESetChannelRequest
* 
* This function sets the frequency on which the radio will transmit or receive on.
*
************************************************************************************/
smacErrors_t MLMESetChannelRequest
(
channels_t newChannel //IN: Enumeration value that represents the requested channel. 
)
{
 uint8_t u8PhyRes;

#if(TRUE == smacParametersValidation_d)
  if((gChannel11_c > newChannel) || (gChannel26_c < newChannel))
  {
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  if((mSmacStateIdle_c != smacState) && (mSmacStatePerformingTest_c != smacState))
  {
    return gErrorBusy_c;
  }
  
  u8PhyRes = PhyPlmeSetCurrentChannelRequest((uint8_t)newChannel);
  
  if(!u8PhyRes)
  {
    smacCurrentChannel = newChannel;
    return gErrorNoError_c;
  }
  else 
  {
    return gErrorNoResourcesAvailable_c;
  }
}

/************************************************************************************
* MLMEGetChannelRequest
* 
* This function returns the current channel, if an error is detected it returns 
* gChannelOutOfRange_c.
*
************************************************************************************/
channels_t MLMEGetChannelRequest(void)
{
#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gChannelOutOfRange_c;
  }
#endif
  if((gChannel11_c > smacCurrentChannel) || (gChannel26_c < smacCurrentChannel))
  {
    return gChannelOutOfRange_c;
  }
  else
  {
    return smacCurrentChannel;
  }
}

/************************************************************************************
* MLMEHibernateRequest
* 
* This call places the radio into Hibernate mode. 
* On the MC1320x and MC1321x platforms it sets the radio to hibernate 
* operation mode, for the MC1323x radio it disables the clock source for the 
* tranceiver module.
*
************************************************************************************/
smacErrors_t MLMEHibernateRequest(void)
{
 #if defined(gMc1323xPlatform_d) 
  return gErrorNoError_c;
 #else
 
 #if defined(gMcs08Gt60Platform_d) || defined(gMcs08qe128Platform_d) 

 #if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
 #endif

  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }

  PhyHibernateRequest();
  
  smacState = mSmacStateHibernate_c;
  
  return gErrorNoError_c;
  
 #endif
    
 #endif
}


/************************************************************************************
* MLMEPAOutputAdjust
* 
* This function adjusts the output power of the transmitter. 
*
************************************************************************************/
smacErrors_t MLMEPAOutputAdjust
(
uint8_t u8PaValue //IN: Value for the output power desired. Values 0-15 are required.
)
{    
  uint8_t u8PhyRes;

#if(TRUE == smacParametersValidation_d)
  if(gMaxOutputPower_c < u8PaValue)
  {
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  if((mSmacStateIdle_c != smacState) && (mSmacStatePerformingTest_c != smacState))
  {
    return gErrorBusy_c;
  }
  
  u8PhyRes = PhyPlmeSetPwrLevelRequest(u8PaValue);
  
  if(!u8PhyRes)
  {
    return gErrorNoError_c;
  }
  else 
  {
    return gErrorNoResourcesAvailable_c;
  }
}

/************************************************************************************
* MLMEPHYXtalAdjust
* 
* This function is used to adjust the radio reference clock by a trim value.
*
************************************************************************************/
smacErrors_t MLMEXtalAdjust
(
uint8_t u8ReqValue //IN: value representing the trim value to the oscillator.
)
{
  if((mSmacStateIdle_c != smacState) && (mSmacStatePerformingTest_c != smacState))
  {
    return gErrorBusy_c;
  }
  
  PhyXtalAdjust(u8ReqValue);
  
  return gErrorNoError_c;
}

/************************************************************************************
* MLMESoftReset
* 
* This function performs a software reset on the radio,  PHY and SMAC state machines.
*
************************************************************************************/
smacErrors_t MLMESoftReset(void)
{
  PhyAbort();
  return MLMERadioInit();
}

/************************************************************************************
* MLMESetClockRate
* 
* This function is called to set the desired clock out from radio.
* This primitive is used just on the MC1320x and MC1321x platforms, for the 
* MC1323x radio it is just an stub.
*
************************************************************************************/
smacErrors_t MLMESetClockRate
(
clkoFrequency_t Freq  //IN: Enumeration value that represents radio clock out 
                      //    frequency (CLKO). 
)
{
  return PhyPlmeSetClockOutRate(Freq);
}



/************************************************************************************
* MLMESetTmrPrescale
* 
* This function changes the rate at which the radio timers operate.
*
************************************************************************************/
smacErrors_t MLMESetTmrPrescale
(
timerTimeBase_t TimeBase //IN: enumeration value that represents that represents  
                         //    timer prescale or time base.
)
{
#if(TRUE == smacParametersValidation_d)
  if((gTimeBase500kHz_c > TimeBase) || (gTimeBase16p625kHz_c < TimeBase))
  {
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  PhySetRadioTimerPrescaler((uint8_t)TimeBase);
  return gErrorNoError_c;
}

/************************************************************************************
* MLMEWakeRequest
* 
* This primitive brings the radio out of low power mode.
*
************************************************************************************/
smacErrors_t MLMEWakeRequest(void)
{
 #if defined(gMc1323xPlatform_d) 
  return gErrorNoError_c; 
#else
   #if defined(gMcs08Gt60Platform_d)  || defined (gMcs08qe128Platform_d)

 #if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
 #endif

  if(mSmacStateIdle_c == smacState)
  {
    return gErrorNoValidCondition_c;
  }

  if (smacState == mSmacStateDoze_c || smacState == mSmacStateHibernate_c)
  {
      PhyWakeRequest();
      return gErrorNoError_c;
  }

  return gErrorNoValidCondition_c;
   #endif 
 #endif

}

/************************************************************************************
* XCVRContReset
* 
* This function asserts the reset line of the transceiver, shutting it down to its 
* lowest power mode.
* This primitive is used just on the MC1320x and MC1321x platforms, for the
* MC1323x radio it is just an stub.
*
************************************************************************************/
void XCVRContReset(void)
{
 #if defined(gMcs08Gt60Platform_d) 
  PhyXCVRContReset(); 
 #endif 
}

/************************************************************************************
* XCVRRestart
* 
* This function deasserts the reset line, thus it power the transceiver up.
* This primitive is used just on the MC1320x and MC1321x platforms, for the
* MC1323x radio it is just an stub.
*
************************************************************************************/
void XCVRRestart(void)
{
 #if defined(gMcs08Gt60Platform_d) 
  PhyXCVRRestart(); 
 #endif 
}


/***********************************************************************************/
/***************************** SMAC management primitives **************************/
/***********************************************************************************/

/************************************************************************************
* MLMEGetRficVersion
* 
* This function is used to read the version number of different hardware and software 
* modules inside the SMAC platform.
*
************************************************************************************/
smacErrors_t MLMEGetRficVersion
(
versionedEntity_t Entity, //IN: The module for which the version is required.  
uint8_t *Buffer           //OUT: A pointer to the buffer where the version will be 
                          //     written.
)
{
  uint8_t u8i;

#if(TRUE == smacParametersValidation_d)
  if(gMaxVersionedEntity_c <= Entity){
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }

  if(gSwSmacVersion_c == Entity)
  {
    for(u8i=0; u8i < gSwSmacVersionLength_c; u8i++)
    {
      *Buffer++ = au8SmacVersion[u8i];
    }
  }
  else
  {
    PhyGetRfIcVersion(Buffer);    
  }

  return gErrorNoError_c;
}

/************************************************************************************
* MLMESetPromiscuousMode
* 
* SMAC appends 0xFF7E as the first two bytes of the packet to identify its own 
* packets from other packets in the same PHY layer. When promiscuous mode is off 
* SMAC filters the received packets and just allows those beginning with 0xFF7E; 
* when promiscuous mode is on SMAC let pass all the messages beginning or not with
* 0xFF7E. MLMESetPromiscuousMode allows setting or clearing the promiscuous mode. 
*
************************************************************************************/
void MLMESetPromiscuousMode
(
bool_t isPromiscousMode //IN: This is a boolean value that indicates if the promiscuous 
                        //    mode is on (TRUE) or off (FALSE).
)
{
   mPromiscuousModeEnabled = isPromiscousMode;
}

/************************************************************************************
* MLMEGetPromiscuousMode
* 
* This function returns the current state of promiscuous mode.
*
************************************************************************************/
bool_t MLMEGetPromiscuousMode(void)
{
  return mPromiscuousModeEnabled;
}

/************************************************************************************
* MLMEScanRequest
* 
* This function scans the different channels using one out of two techniques and 
* returns the amount of energy in all of the channels.
*
************************************************************************************/
smacErrors_t MLMEScanRequest
(
uint16_t u16ChannelsToScan, //IN: Bitmap of the Channels to be scanned.
scanModes_t ScanMode,       //IN: Technique to be used CCA or ED.
uint8_t *pu8ChannelScan     //OUT: The buffer where the values of the scan will be 
                            //     returned. 
)
{

  smacErrors_t scanRes;

#if(TRUE == smacParametersValidation_d)
  if((0 == u16ChannelsToScan) || (gMaxScanMode_c <= ScanMode) || (NULL == pu8ChannelScan))
  {
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif

  if(mSmacStateIdle_c != smacState)
  {
    return gErrorBusy_c;
  }
  smacChannelsBeingScanned = u16ChannelsToScan;
  smacScanCurrentChannel   = gChannel11_c;
  smacClearestChann        = gChannel11_c;
  smacProccesPacketPtr.smacScanResultsPointer = pu8ChannelScan;
  smacEdValue              = 0xFF;
  
  if(gScanModeED_c == ScanMode)
  {
    mSmacPerformingEDScan = TRUE;
  }
  else
  {
    mSmacPerformingCCAScan = TRUE;
  }

  smacState = mSmacStateScanningChannels_c; 
  scanRes = SmacScanTheNextChannel();
  
  if(scanRes)
  {
    smacState = mSmacStateIdle_c; 
    mSmacPerformingEDScan  = FALSE;
    mSmacPerformingCCAScan = FALSE;
  }
  
  return scanRes;
}

/************************************************************************************
* MLMETestMode
* 
* By employing this function, users can execute a test of the radio. Test mode 
* implements the following:
*   -PRBS9 Mode, 
*   -Force_idle, 
*   -Continuos RX, 
*   -Continuos TX without modulation, 
*   -Continuos TX with modulation.
*
************************************************************************************/
smacErrors_t MLMETestMode
(
smacTestMode_t  mode  //IN: The test mode to start.
)
{
  uint8_t u8PhyRes = 0; 
  

#if(TRUE == smacParametersValidation_d)
  if(gMaxTestMode_c <= mode)
  {
    return gErrorOutOfRange_c;
  }
#endif

#if(TRUE == smacInitializationValidation_d)
  if(FALSE == mSmacInitialized)
  {
    return gErrorNoValidCondition_c;
  }
#endif
  
  if((mSmacStateIdle_c != smacState) && (mSmacStatePerformingTest_c != smacState))
  {
    return gErrorBusy_c;
  }

  pTestPacket = (phyPacket_t*)&u8TestBuffer;
  
  smacState = mSmacStatePerformingTest_c;
  PhyDisableContinuousTxRxMode();
  PhyDisableTxNoModulation();
  PhyAbort();
  
   
  if(gTestModeForceIdle_c == mode){
    mSmacPerformingPRBS9 = FALSE;
    smacState = mSmacStateIdle_c;
  }
   
  else if(gTestModeContinuousTxModulated_c == mode){
    PhyDisableTxNoModulation();
    PhyEnableContinuousTxRxMode();
    /*Just begin a dummy Tx to start the radio Test mode*/
    (void)PhyPdDataRequest(pTestPacket, gDataReq_NoAck_NoCca_Unslotted_c, NULL);
  } 
  
  else if(gTestModeContinuousTxUnmodulated_c == mode){
    PhyEnableContinuousTxRxMode();
    PhyEnableTxNoModulation();
    /*Just begin a dummy Tx to start the radio Test mode*/
    (void)PhyPdDataRequest(pTestPacket, gDataReq_NoAck_NoCca_Unslotted_c, NULL);
  } 
  
  else if(gTestModeContinuousRx_c == mode){
    PhyDisableTxNoModulation();
    PhyEnableContinuousTxRxMode();
    (void)PhyPlmeRxRequest(pTestPacket,&smacLastDataRxParams);
  }

  else if(gTestModePRBS9_c == mode){
    mSmacPerformingPRBS9 = TRUE;
    (void)PhyPdDataRequest(pTestPacket, gDataReq_NoAck_NoCca_Unslotted_c, NULL);
  }
  return gErrorNoError_c;
}




/***********************************************************************************/
/******************************** SMAC MCU primitives ******************************/
/***********************************************************************************/


/************************************************************************************
* SMACEnableInterrupts
* 
* This function enables the interrupts that are used by SMAC.
*
************************************************************************************/
void SMACEnableInterrupts(void)
{
  PhyEnableRadioInterrupts();
}

/************************************************************************************
* SMACDisableInterrupts
* 
* This function disables the interrupts that are used by SMAC.
*
************************************************************************************/
void SMACDisableInterrupts(void)
{
  PhyDisableRadioInterrupts();
}


/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/

/***********************************************************************************/
smacErrors_t SmacScanTheNextChannel(void)
{
  uint8_t  u8PhyRes;
  uint16_t u16ChannelMask;

  /*Look for the next channel to scan*/
  u16ChannelMask = 0x0001 << (uint8_t)(smacScanCurrentChannel - gChannel11_c);
  while(!(u16ChannelMask & smacChannelsBeingScanned) && (u16ChannelMask))
  {
    smacScanCurrentChannel++;
    u16ChannelMask<<=1;
  }

  if(gTotalChannels_c == smacScanCurrentChannel)
  {
    return gErrorOutOfRange_c;
  }
  else
  {
    u8PhyRes = PhyPlmeSetCurrentChannelRequest(smacScanCurrentChannel);
    if(!u8PhyRes)
    {
      if(mSmacPerformingEDScan)
      {
        return (smacErrors_t)PhyPlmeEdRequest();
      }
      else
      {
        return (smacErrors_t)PhyPlmeCcaRequest(gCcaCCA_MODE1_c);
      }
    }
    else 
    {
      return (smacErrors_t)u8PhyRes;
    }
  }
}
 
/*********************************************************/
void SmacSetRxTimeout(zbClock24_t timeoutSymbols)
{
  zbClock24_t time;
  PhyTimeReadClock(&time);
  time += timeoutSymbols + 4;
  PhyTimeSetEventTimeout(&time);
}

/*********************************************************/
phyPacket_t * SmacFormatTxPacket(txPacket_t *psTxPacket)
{
  phyPacket_t * packetToPhy;
  
  packetToPhy = (phyPacket_t *)psTxPacket;

  if(FALSE == mPromiscuousModeEnabled)
  {
    packetToPhy = (phyPacket_t *)psTxPacket;
    packetToPhy->frameLength += 2; //Add 2 for Code bytes
    packetToPhy->data[0] = 0x7E;
    packetToPhy->data[1] = 0xFF;
  }
  else
  {
    packetToPhy = (phyPacket_t *)&psTxPacket->smacPdu.reserved[1];
    packetToPhy->frameLength = psTxPacket->u8DataLength;
  }
  
  packetToPhy->frameLength += 2; //Add 2 for FCS
  
  return packetToPhy;
}
 
  
   
/***********************************************************************************/
/****************************** PHY Callback Functions *****************************/
/***********************************************************************************/

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/***********************************************************************************/
void PhyPdDataConfirm(void)
{
  if(mSmacStateTransmitting_c == smacState)
  {
    if(!mPromiscuousModeEnabled)
    {
      smacProccesPacketPtr.smacTxPacketPointer->frameLength-=4;
    }
    else
    {
      smacProccesPacketPtr.smacTxPacketPointer->frameLength-= 2;
    }
    MCPSDataComfirm(txSuccessStatus_c);
    smacState= mSmacStateIdle_c;
  }
  else if(mSmacStatePerformingTest_c == smacState)
  {
    if(TRUE == mSmacPerformingPRBS9)
    {
      DelayMs(5);  
      (void)PhyPdDataRequest(pTestPacket, gDataReq_NoAck_NoCca_Unslotted_c, NULL);
    }
  }
}


/***********************************************************************************/
void PhyPdDataIndication(void)
{
  uint8_t  u8PhyRes;

  if(!mPromiscuousModeEnabled)
  {
    smacProccesPacketPtr.smacRxPacketPointer->u8DataLength-=4;
  }
  else
  {
    smacProccesPacketPtr.smacRxPacketPointer->u8DataLength=smacProccesPacketPtr.smacRxPacketPointer->smacPdu.reserved[1] - 2;
  }
  
  if( ((!mPromiscuousModeEnabled) && (FALSE == areCodeBytesValid(smacProccesPacketPtr.smacRxPacketPointer)) )||
    (smacProccesPacketPtr.smacRxPacketPointer->u8DataLength > smacProccesPacketPtr.smacRxPacketPointer->u8MaxDataLength))
  {
    u8PhyRes = PhyPlmeRxRequest((phyPacket_t *)&(smacProccesPacketPtr.smacRxPacketPointer->u8DataLength), &smacLastDataRxParams);
    if(!u8PhyRes){
      if(mSmacTimeoutAsked)
      {
        SmacSetRxTimeout((zbClock24_t)smacTimeout);
      }
    }else{
      smacProccesPacketPtr.smacRxPacketPointer->rxStatus = rxAbortedStatus_c;
      PhyTimeDisableEventTimeout();
      MCPSDataIndication(smacProccesPacketPtr.smacRxPacketPointer);
      smacState = mSmacStateIdle_c;
    }
  }
  else
  {
      smacProccesPacketPtr.smacRxPacketPointer->rxStatus = rxSuccessStatus_c;
      PhyTimeDisableEventTimeout();
      MCPSDataIndication(smacProccesPacketPtr.smacRxPacketPointer);
      smacState= mSmacStateIdle_c;
  }
}

/***********************************************************************************/
void PhyPlmeCcaConfirm(bool_t channelInUse)
{
  smacErrors_t scanRes;
  smacProccesPacketPtr.smacScanResultsPointer[smacScanCurrentChannel - gChannel11_c] = channelInUse;
  smacScanCurrentChannel++;
  scanRes = SmacScanTheNextChannel();
  
  if(scanRes)
  {
    smacClearestChann = gChannelOutOfRange_c; 
    (void)PhyPlmeSetCurrentChannelRequest(smacCurrentChannel);
    MLMEScanComfirm(smacClearestChann);
    mSmacPerformingCCAScan = FALSE;
    smacState= mSmacStateIdle_c;
  }
}

/***********************************************************************************/
void PhyPlmeEdConfirm(uint8_t energyLevel)
{
  smacErrors_t scanRes;
  
  if(mSmacPerformingED)
  {
    smacEdValue = energyLevel; 
    mSmacPerformingED = FALSE;
  }
  else if(mSmacPerformingEDScan)
  {
    if(energyLevel < smacEdValue)
    {
      smacEdValue = energyLevel;
      smacClearestChann = smacScanCurrentChannel;
    }
    smacProccesPacketPtr.smacScanResultsPointer[smacScanCurrentChannel - gChannel11_c] = energyLevel; 
    smacScanCurrentChannel++;
    scanRes = SmacScanTheNextChannel();
    
    if(scanRes)
    {
      MLMEScanComfirm(smacClearestChann);
      (void)PhyPlmeSetCurrentChannelRequest(smacCurrentChannel);
      mSmacPerformingEDScan  = FALSE;
      smacState= mSmacStateIdle_c;
    }
  }
}

/***********************************************************************************/
void PhyPlmeSyncLossIndication(void)
{
  uint8_t  u8PhyRes;
  if(mSmacStateReceiving_c == smacState)
  {
    u8PhyRes = PhyPlmeRxRequest((phyPacket_t *)&(smacProccesPacketPtr.smacRxPacketPointer->u8DataLength), &smacLastDataRxParams);
    if(u8PhyRes)
    {
      smacProccesPacketPtr.smacRxPacketPointer->rxStatus = rxAbortedStatus_c;
      PhyTimeDisableEventTimeout();
      MCPSDataIndication(smacProccesPacketPtr.smacRxPacketPointer);
      smacState= mSmacStateIdle_c;
    }
  }
  else if(mSmacStateTransmitting_c == smacState)
  {
    (void)PhyPdDataRequest(smacProccesPacketPtr.smacTxPacketPointer, gDataReq_NoAck_NoCca_Unslotted_c, NULL);
  }
  else if(mSmacStateScanningChannels_c == smacState)
  {
    (void)SmacScanTheNextChannel();
  }
  else if(TRUE == mSmacPerformingED)
  {
    if(PhyPlmeEdRequest())
    {
      smacEdValue = 0xFF;
      mSmacPerformingED = FALSE;
    }
  }
}

/***********************************************************************************/
void PhyPlmeResetIndication(void)
{
  MLMEResetIndication();
}

/***********************************************************************************/
void    PhyPlmeWakeComfirm(void)
{
  MLMEWakeComfirm();
}

/***********************************************************************************/
void PhyTimeRxTimeoutIndication(void)
{
  smacProccesPacketPtr.smacRxPacketPointer->rxStatus = rxTimeOutStatus_c;
  MCPSDataIndication(smacProccesPacketPtr.smacRxPacketPointer);
  smacState= mSmacStateIdle_c;
}

/***********************************************************************************/
void PhyTimeStartEventIndication(void)
{
  
}

#pragma CODE_SEG DEFAULT



