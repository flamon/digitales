/*****************************************************************************
* PhyMC1320X implementation.
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/



#include "PhyMC1320X.h"

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static uint16_t u16HREReg;
static handlingRadioEventStates_t hREState;
static phyPacket_t * mDrvRxPacket;
static phyRxParams_t *mPhyRxParams;

static const uint8_t cu8ChannelRegisterValues[16][2]=
{
  {0x95, 0x50},/*Channel 0x0B */
  {0x95, 0xA0},/*Channel 0x0C */
  {0x95, 0xF0},/*Channel 0x0D */
  {0x96, 0x40},/*Channel 0x0E */
  {0x96, 0x90},/*Channel 0x0F */
  {0x96, 0xE0},/*Channel 0x10 */
  {0x97, 0x30},/*Channel 0x11 */
  {0x97, 0x80},/*Channel 0x12 */
  {0x97, 0xD0},/*Channel 0x13 */
  {0x98, 0x20},/*Channel 0x14 */
  {0x98, 0x70},/*Channel 0x15 */
  {0x98, 0xC0},/*Channel 0x16 */
  {0x99, 0x10},/*Channel 0x17 */
  {0x99, 0x60},/*Channel 0x18 */
  {0x99, 0xB0},/*Channel 0x19 */
  {0x9A, 0x00} /*Channel 0x1A */
};

static const uint8_t u8PARegisterValues[16] =
{
  0x0C, /*-16.2 dBm*/
  0x1C, /*-15.9 dBm*/
  0x2C, /*-15.3 dBm*/
  0x3C, /*-14.8 dBm*/
  0x4C, /* -8.5 dBm*/
  0x5C, /* -7.6 dBm*/
  0x6C, /* -7.2 dBm*/
  0x7C, /* -7.0 dBm*/
  0x9C, /* -1.6 dBm*/
  0xBC, /* -0.66 dBm*/
  0xCC, /*  0.62 dBm*/
  0xEC, /*  1.23 dBm*/
  0xFC, /*  1.42 dBm*/
  0xFD, /*  2.2 dBm*/
  0xFE, /*  2.9 dBm*/
  0xFF  /*  3.4 dBm*/
};


const radioRegister_t radioConfiguration[] = 
{
  {gRadioTmrCmp1AReg_c,         0x8000},
  {gRadioTmrCmp2AReg_c,         0x8000},
  {gRadioTmrCmp3AReg_c,         0x8000},
  {gRadioTmrCmp4AReg_c,         0x8000},
  {gRadioControlBReg_c,         mControlBRegValue_c},
  {gRadioGPIODataOutReg_c,      0x0381},
  {gRadioCCAThreshReg_c,        0xA08D},
  {gRadioIRQMaskReg_c,          0x8351},
  {gRadioControlAReg_c,         0x4720},
  {gRadioGPIODirReg_c,          0xC0FE},
  {gRadioPSMModeReg_c,          0xA0C0},
  {gRadioRegister0x34_c,        0xFEC6}
};

#define mPhyRegCountIc60_c 10 
#define mPhyRegCountIc68_c 12 


static phyStates_t mPhyState = mPhyNoInitState_c;

flagsRegister_t phyFlags;

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
* Inits the Radio driver and Radio registers
*
************************************************************************************/
void PhyInit(void)
{
  uint16_t initRegValCpy;
  uint8_t u8fCount;
  uint8_t u8fCountMax;

  (void)Radio_DriverInit((radioCallback_t)PhyRadioEvent);
  DelayMs(4);
  Radio_DeassertReset();

  while(mPhyResetAttnState_c != mPhyState);
  
  (void)Radio_ReadRegisterBlocking(gRadioChipIdReg_c, &initRegValCpy);
  if(0x6800 == (initRegValCpy & 0xFF00))
  {
    u8fCountMax = mPhyRegCountIc68_c;
  }
  else
  {
    u8fCountMax = mPhyRegCountIc60_c;
  }
  
  for(u8fCount = 0; u8fCount < u8fCountMax; u8fCount++){
    initRegValCpy = radioConfiguration[u8fCount].u16RegisterValue;
    (void)Radio_WriteRegisterBlocking(radioConfiguration[u8fCount].address, initRegValCpy); 
  }
 
  (void)Radio_ReadRegisterBlocking(gRadioRSTIndReg_c, &initRegValCpy); 

  mPhyState = mPhyIdleState_c;

}

/************************************************************************************
* PhyPdDataRequest
* 
* 
*
************************************************************************************/
uint8_t PhyPdDataRequest
(
phyPacket_t *pTxPacket, 
uint8_t phyTxMode, 
phyRxParams_t *pRxParams
)
{
  uint16_t pktCtlRegCpy;

  (void)phyTxMode;
  (void)pRxParams;
  
  if(mPhyNoInitState_c == mPhyState)
  {
    return gPhyBusy_c;
  }

  /*Writes the data length Register*/
  (void)Radio_ReadRegisterBlocking(gRadioTxPktCtlReg_c, &pktCtlRegCpy);
  pktCtlRegCpy = ((0xFF80 & pktCtlRegCpy) | (pTxPacket->frameLength)); 
  (void)Radio_WriteRegisterBlocking(gRadioTxPktCtlReg_c, pktCtlRegCpy);

  /*Writes the Radio's TX data Buffer*/
  (void)Radio_WriteTxBuffer(pTxPacket->data, pTxPacket->frameLength, NULL, gRadioAllowToBlock_d);
 
  mPhyState = mPhyTxState_c; 

  /*Sets the radio in transmit mode*/
  (void)PhySetTrxStateRequest(mTxMode_c);    /* transmit it */

  return gPhySuccess_c;    
}

/************************************************************************************
* PhyPlmeDozeRequest
* 
* 
*
************************************************************************************/
uint8_t PhyPlmeDozeRequest
(
		uint32_t u32Timeout
)
{
    uint16_t u16CurrentValue;
    uint32_t u32CurrentTime;

    (void)Radio_ReadRegisterBlocking(gRadioControlBReg_c, &u16CurrentValue);														 /* 
														      * Read MC13192 Doze 
                                                     	      * register. 
                                                     	      */
    if (u32Timeout == 0)        /* ACOMA mode, with clkout */
    {
    	(void)Radio_ReadRegisterBlocking(gRadioIRQMaskReg_c, &u16CurrentValue);
        u16CurrentValue &= 0xFE0D;   /* Change the IRQ_Mask to set Acoma en,*/
        u16CurrentValue |= 0x0100;   /* doze irq disabled and tmr2 disabled*/
        (void)Radio_WriteRegisterBlocking(gRadioIRQMaskReg_c, u16CurrentValue);
        
        (void)Radio_ReadRegisterBlocking(gRadioControlBReg_c, &u16CurrentValue);/* 
                                                         * Read MC13192 Doze 
                                                         * register. 
                                                         */
        
        u16CurrentValue &= 0xFDFC;
        u16CurrentValue |= 0x0201;                      /* Doze (acoma) & CLKOUT enable */
        (void)Radio_WriteRegisterBlocking(gRadioControlBReg_c, u16CurrentValue);/* 
                                                         * Write back to MC13192 to 
                                                         * enable hibernate mode. 
                                                         */
   
    } else						 /* DOZE mode with timeout, no clkout */
    {
    	(void)Radio_ReadRegisterBlocking(gRadioIRQMaskReg_c, &u16CurrentValue);
    	u16CurrentValue &= 0xFE0D;   /* Change the IRQ_Mask to set Acoma dis,*/
        u16CurrentValue |= 0x0012;   /* doze enabled and tmr2 enabled*/
        (void)Radio_WriteRegisterBlocking(gRadioIRQMaskReg_c, u16CurrentValue);
        
        (void)PhyTimeReadClock(&u32CurrentTime);
        u32Timeout += u32CurrentTime;
        (void)Radio_WriteRegisterBlocking(gRadioTmrCmp2AReg_c, ((uint16_t)(u32Timeout>>16)&0x00FF));
        (void)Radio_WriteRegisterBlocking(gRadioTmrCmp2BReg_c, ((uint16_t)u32Timeout)&0xFFFF);  /* Enable the Timer 2 
                                                         * and save the timeout value
                                                         */
        (void)Radio_ReadRegisterBlocking(gRadioControlBReg_c, &u16CurrentValue);
        
        u16CurrentValue &= 0xFDFC;						/* Disable CLKOinDozeand */
        u16CurrentValue |= 0x0001;                      /* enter in Doze mode */
        (void)Radio_WriteRegisterBlocking(gRadioControlBReg_c, u16CurrentValue);
    }
    return gPhySuccess_c;         /* Call back to MAC layer indicating success. */
}


/************************************************************************************
* PhyPlmeRxRequest
* 
* 
*
************************************************************************************/
uint8_t PhyPlmeRxRequest
(
phyPacket_t *pRxData, 
phyRxParams_t *pRxParams
)
{
  uint8_t u8Status = 0;

  mPhyRxParams = pRxParams;
  mDrvRxPacket = pRxData;    /* Assign the rx_packet to Phy global. */
  mPhyState = mPhyRxState_c; 

  /* Just enable the receiver */
  u8Status = PhySetTrxStateRequest(mRxMode_c);
  return u8Status;
}

/************************************************************************************
* PhyTimeSetEventTimeout
* 
* 
*
************************************************************************************/
void PhyTimeSetEventTimeout
(
zbClock24_t *pEndTime
)
{
  /* Load the timeout value into T1 with Timer disabled. */
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1AReg_c, (mStopTimerValueA_c));
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1BReg_c, (mStopTimerValueB_c));
  /* Turn Timer1 mask on. */
  
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1AReg_c, (uint16_t) (((*pEndTime) >> 16) & 0x000000FF));
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1BReg_c, (uint16_t) ((*pEndTime) & 0x0000FFFF));
  mPhyState = mPhyRxWtoState_c;
}

/************************************************************************************
* PhyTimeReadClock
* 
* 
*
************************************************************************************/
void PhyTimeReadClock
(
zbClock24_t *pRetClk
)
{
  uint16_t currentTimeCpy;
  
  *pRetClk = 0;
  /* Read the current timer time*/
  (void)Radio_ReadRegisterBlocking(gRadioCurrentTimeAReg_c, &currentTimeCpy);
  *pRetClk = currentTimeCpy;
  *pRetClk <<=16;
  (void)Radio_ReadRegisterBlocking(gRadioCurrentTimeBReg_c, &currentTimeCpy);
  *pRetClk += currentTimeCpy;
  *pRetClk &=mRadioTimeMask_c;
}

/************************************************************************************
* PhyPlmeCcaEdRequest
* 
*
************************************************************************************/
uint8_t PhyPlmeCcaEdRequest
(
uint8_t ccaParam
)
{
  uint16_t controlARegCpy;
  
  if(ccaParam >= gInvalidCcaType_c)
  {
    return gPhyInvalidParam_c;
  }

  (void)Radio_ReadRegisterBlocking(gRadioControlAReg_c, &controlARegCpy);
  controlARegCpy &= 0xFFCF;
  if(gCcaED_c == ccaParam)
  {
    controlARegCpy |= 0x0020;
    mPhyState = mPhyEdState_c;
  }  
  else if(gCcaCCA_MODE1_c == ccaParam)
  {
    controlARegCpy |= 0x0010;
    mPhyState = mPhyCcaState_c;
  }
  (void)Radio_WriteRegisterBlocking(gRadioControlAReg_c, controlARegCpy);
  
  (void)PhySetTrxStateRequest(mCcaMode_c);
  return gPhySuccess_c;
}

/************************************************************************************
* PhyPlmeSetCurrentChannelRequest
* 
*
************************************************************************************/
uint8_t PhyPlmeSetCurrentChannelRequest
(
uint8_t channel
)
{
  uint16_t TempValue;
  
  TempValue = 0x0F00 | cu8ChannelRegisterValues[(channel-0x0B)][0];
  (void)Radio_WriteRegisterBlocking(gRadioLO1IntDivReg_c, TempValue);
  TempValue = (0x0000 | cu8ChannelRegisterValues[(channel-0x0B)][1])<<8;
  (void)Radio_WriteRegisterBlocking(gRadioLO1NumReg_c, TempValue);
   
  return gPhySuccess_c;
}

/************************************************************************************
* PhyPlmeSetPwrLevelRequest
* 
*
*
************************************************************************************/
uint8_t PhyPlmeSetPwrLevelRequest
(
uint8_t pwrLevel
)
{
  uint16_t PALvlRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioPALvlReg_c, &PALvlRegCpy);
  PALvlRegCpy &= 0xFF00;
  PALvlRegCpy |= u8PARegisterValues[pwrLevel];
  (void)Radio_WriteRegisterBlocking(gRadioPALvlReg_c, PALvlRegCpy);
  return gPhySuccess_c;
}

/************************************************************************************
* PhyPpSetPromiscuous
* 
*
*
************************************************************************************/
void PhyPpSetPromiscuous
(
bool_t mode
)
{
  (void)mode;
}

/************************************************************************************
* PhyPlmeSetClockOutRate
* 
*
*
************************************************************************************/
uint8_t PhyPlmeSetClockOutRate
(
  uint8_t u8Freq
)
{
  uint16_t clkoCtlRegCpy;
  
    (void)Radio_ReadRegisterBlocking(gRadioCLKOCtlReg_c, &clkoCtlRegCpy);
    clkoCtlRegCpy &= 0xFFF8;
    clkoCtlRegCpy |= u8Freq;
    (void)Radio_WriteRegisterBlocking(gRadioCLKOCtlReg_c, clkoCtlRegCpy);

    return gPhySuccess_c;
}

/************************************************************************************
* PhyAbort
* 
*
*
************************************************************************************/
void PhyAbort(void)
{
  /* Disables the Timeout */
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1AReg_c, (uint16_t)(0x00008000));
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1BReg_c, (uint16_t)(0x00000000));
  /*sets the idle state on radio*/
  (void)PhySetTrxStateRequest(mIdleMode_c);
  mPhyState = mPhyIdleState_c;

}

/************************************************************************************
* PhyTimeDisableEventTimeout
* 
*
*
************************************************************************************/
void PhyTimeDisableEventTimeout(void)
{
  /* Disables the Timeout */
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1AReg_c, (uint16_t)(0x00008000));
  (void)Radio_WriteRegisterBlocking(gRadioTmrCmp1BReg_c, (uint16_t)(0x00000000));
}

/************************************************************************************
* PhyPpSetCcaThreshold
* 
*
*
************************************************************************************/
void PhyPpSetCcaThreshold(uint8_t ccaThreshold)
{
  uint16_t ccaThresholdRegCpy;
  
  (void)Radio_ReadRegisterBlocking(gRadioCCAThreshReg_c, &ccaThresholdRegCpy);
  ccaThresholdRegCpy &= 0x00FF;
  ccaThresholdRegCpy |= ((uint16_t)ccaThreshold<<8);
  (void)Radio_WriteRegisterBlocking(gRadioCCAThreshReg_c, ccaThresholdRegCpy);
}

/************************************************************************************
* PhyAdjustCcaOffsetCmp
* 
*   ...
*
************************************************************************************/
void PhyAdjustCcaOffsetCmp(uint8_t ccaOffset)
{
  uint16_t ccaThresholdRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioCCAThreshReg_c, &ccaThresholdRegCpy);
  ccaThresholdRegCpy &= 0xFF00;
  ccaThresholdRegCpy |= ccaOffset;
  (void)Radio_WriteRegisterBlocking(gRadioCCAThreshReg_c, ccaThresholdRegCpy);
}

/************************************************************************************
* PhyXtalAdjust
* 
*   ...
*
************************************************************************************/
void PhyXtalAdjust(uint8_t xtalTrim)
{
  uint16_t clkoCtlRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioCLKOCtlReg_c, &clkoCtlRegCpy);
  clkoCtlRegCpy &= 0x00FF;
  clkoCtlRegCpy |= ((uint16_t)xtalTrim<<8);
  (void)Radio_WriteRegisterBlocking(gRadioCLKOCtlReg_c, clkoCtlRegCpy);
}

/************************************************************************************
* PhyEnableTxNoModulation
* 
*   ...
*
************************************************************************************/
void PhyEnableTxNoModulation(void)
{
  uint16_t psmModeRegCpy;
  
  (void)Radio_ReadRegisterBlocking(gRadioPSMModeReg_c, &psmModeRegCpy);
  psmModeRegCpy |= 0x0008;
  (void)Radio_WriteRegisterBlocking(gRadioPSMModeReg_c, psmModeRegCpy);
}

/************************************************************************************
* PhyDisableTxNoModulation
* 
*   ...
*
************************************************************************************/
void PhyDisableTxNoModulation(void)
{
  uint16_t psmModeRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioPSMModeReg_c, &psmModeRegCpy);
  psmModeRegCpy &= 0xFFC7;
  (void)Radio_WriteRegisterBlocking(gRadioPSMModeReg_c, psmModeRegCpy);
}

/************************************************************************************
* PhyEnableContinuousTxRxMode
* 
*   ...
*
************************************************************************************/
void PhyEnableContinuousTxRxMode(void)
{
  uint16_t berEnableRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioBEREnableReg_c, &berEnableRegCpy);
  berEnableRegCpy |= 0x8000;
  (void)Radio_WriteRegisterBlocking(gRadioBEREnableReg_c, berEnableRegCpy);
}

/************************************************************************************
* PhyDisableContinuousTxRxMode
* 
*   ...
*
************************************************************************************/
void PhyDisableContinuousTxRxMode(void)
{
  uint16_t berEnableRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioBEREnableReg_c, &berEnableRegCpy);
  berEnableRegCpy &= 0x7FFF;
  (void)Radio_WriteRegisterBlocking(gRadioBEREnableReg_c, berEnableRegCpy);
}

/************************************************************************************
* PhySetRadioTimerPrescaler
* 
*   ...
*
************************************************************************************/
void PhySetRadioTimerPrescaler(uint8_t Value)
{
  uint16_t ControlCRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioControlCReg_c, &ControlCRegCpy);
  ControlCRegCpy &= 0xFFF8;
  ControlCRegCpy |= Value;
  (void)Radio_WriteRegisterBlocking(gRadioControlCReg_c, ControlCRegCpy);
}

/************************************************************************************
* PhyGetRfIcVersion
* 
*   ...
*
************************************************************************************/
void PhyGetRfIcVersion(uint8_t * Buffer)
{
  uint16_t RadioChipIdRegCpy;

  (void)Radio_ReadRegisterBlocking(gRadioChipIdReg_c, &RadioChipIdRegCpy);
  Buffer[1] = (uint8_t)(RadioChipIdRegCpy & 0x00FF);
  Buffer[0] = (uint8_t)((RadioChipIdRegCpy & 0xFF00)>>8);
}

/************************************************************************************
* PhyHibernateRequest
* 
*   ...
*
************************************************************************************/
void PhyHibernateRequest(void)
{ 
  uint16_t controlBRegCpy;
  
  (void)Radio_ReadRegisterBlocking(gRadioControlBReg_c, &controlBRegCpy);
  controlBRegCpy |= 0x0002;
  (void)Radio_WriteRegisterBlocking(gRadioControlBReg_c, controlBRegCpy);
  mPhyState = mPhyHibernateState_c; 
}

/************************************************************************************
* PhyWakeRequest
* 
*   ...
*
************************************************************************************/
void PhyWakeRequest(void)
{ 
  Radio_AssertAttentionPin();
  Radio_DeassertAttentionPin();
}

/************************************************************************************
* PhyEnableRadioInterrupts
* 
*   ...
*
************************************************************************************/
void PhyEnableRadioInterrupts(void)
{
  Radio_EnableRadioInterrupts();
}

/************************************************************************************
* PhyDisableRadioInterrupts
* 
*   ...
*
************************************************************************************/
void PhyDisableRadioInterrupts(void)
{
  Radio_DisableRadioInterrupts();
}

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
/***********************************************************************************/
static uint8_t PhySetTrxStateRequest(uint8_t u8ReqMode)
{
  uint16_t controlARegCpy;
  if(u8ReqMode > mTxMode_c)
  {
    return gPhyInvalidParam_c;
  }
  Radio_DeassertTxRxEnable();
  (void)Radio_ReadRegisterBlocking(gRadioControlAReg_c, &controlARegCpy);
  controlARegCpy &= 0xFFF8;                           /* Clear mode. */
  controlARegCpy |= u8ReqMode;
  (void)Radio_WriteRegisterBlocking(gRadioControlAReg_c, controlARegCpy);
  if(mIdleMode_c != u8ReqMode)
  {
    Radio_AssertTxRxEnable();
  }

  return gPhySuccess_c;
}

/***********************************************************************************/
static void  PhyHREventsReStartSequense(void)
{
  /* Read the MC13192 trx register. Timer trigger off. */
  (void)Radio_ReadRegister(gRadioControlAReg_c, &u16HREReg, PhyHandlingRadioEvents);
  hREState = mHRERxRcvdReadingControlARegState_c;
}

/***********************************************************************************/
static void PhyChangePacketEndianess(phyPacket_t * packet, uint8_t packetLength)
{
  uint8_t u8TempByte, i;
  for (i=0; i<packetLength; i+=2) 
  { 
    u8TempByte = packet->data[i+1];
    packet->data[i+1] = packet->data[i];
    packet->data[i] = u8TempByte;
  }
}


/***********************************************************************************/
/**************************** Radio Callback Functions *****************************/
/***********************************************************************************/
static void PhyRadioEvent(void) 
{
  volatile uint16_t vStatusContent;   

  vStatusContent = Radio_IrqStatusRegValue();
  vStatusContent &= mUsedStatusFlagsMask_c;
  Radio_DeassertTxRxEnable();       /* Forces the MC13192 to idle. */

  if (mPhyState != mPhyRxWtoState_c) 
  { /* If timeout not being used, mask out timer. */
    vStatusContent &= ~(gRadioIrqFlagTmr1Irq_c);
  }
  
  if (mDoesEventHappenedWhilePhyIdle_c) 
  {   
    return;
  }

  if (vStatusContent & gRadioIrqFlagDozeIrq_c) 
  {
    (void)Radio_WriteRegister(gRadioTmrCmp2AReg_c, 0x8000, PhyHandlingRadioEvents);
    hREState = mHREDozeDisablingTimerState_c;
    return;
  }  
  
  if (vStatusContent & gRadioIrqFlagAttnIrq_c) 
  {
    (void)Radio_ReadRegister(gRadioResetReg_c, &u16HREReg, PhyHandlingRadioEvents);
    hREState = mHREAttnReadingRSTIndRegState_c;
    return;
  }
  
  if (vStatusContent & gRadioIrqFlagTmr1Irq_c) 
  {
    (void)Radio_WriteRegister(gRadioTmrCmp1AReg_c, 0x8000, PhyHandlingRadioEvents);
    hREState = mHRETmr1WrittingTmrCmp1ARegState_c;
    return;
  }

  if (vStatusContent & gRadioIrqFlagPllLockIrq_c) 
  {
    mPhyState = mPhyIdleState_c;
    PhyPlmeSyncLossIndication();
    return;
  }
  
  if(vStatusContent & gRadioIrqFlagTxSentIrq_c)
  {
    mPhyState = mPhyIdleState_c;
    PhyPdDataConfirm();
  }
  
  if (vStatusContent & gRadioIrqFlagCcaIrq_c)  
  {
        
    if(mPhyCcaState_c == mPhyState) 
    {
      mPhyState = mPhyIdleState_c;
      PhyPlmeCcaConfirm((vStatusContent & gRadioIrqFlagCca_c) == gRadioIrqFlagCca_c);
    }
    else if(mPhyEdState_c == mPhyState)
    {
      (void)Radio_ReadRegister(gRadioRXStatusReg_c, &u16HREReg, PhyHandlingRadioEvents);
      hREState = mHREEdReadingRxStatusRegState_c;
      return;
    }
    return;
  }
  
  if(vStatusContent & gRadioIrqFlagRxRcvdIrq_c) 
  {  
    if(!(vStatusContent & gRadioIrqFlagCrcValid_c)) 
    {
      PhyHREventsReStartSequense();
    }
    else 
    {
      /* Read received packet length register and mask off length bits */
      //Gpio_PinToggle(gGpioPortD_c, gGpioPin4Mask_c);
      (void)Radio_ReadRegister(gRadioRXStatusReg_c, &u16HREReg, PhyHandlingRadioEvents);
      hREState = mHRERxRcvdReadingRXStatusRegState_c;
    }
  }
}

/************************************************************************************/
static void PhyHandlingRadioEvents(void) 
{
  static uint8_t u8DataLength;
  
  switch(hREState){
    case mHREDozeDisablingTimerState_c:
         (void)Radio_ReadRegister(gRadioControlBReg_c, &u16HREReg, PhyHandlingRadioEvents);
         hREState = mHREDozeReadingControlBRegState_c;
         break;
    case mHREDozeReadingControlBRegState_c:
         u16HREReg &= 0xFFFC;
         (void)Radio_WriteRegister(gRadioControlBReg_c, u16HREReg, PhyHandlingRadioEvents);
         hREState = mHREDozeDisablingDozeState_c;
         break;
    case mHREDozeDisablingDozeState_c:
         mPhyState = mPhyIdleState_c;        
         hREState = mHREIdleState_c;
         PhyPlmeWakeComfirm();         /*Informs to the NHL that the radio is awake*/
         break;
    case mHREAttnReadingRSTIndRegState_c:
         if(IsRadioRecoveringFromReset(u16HREReg)) { 
           mPhyState = mPhyResetAttnState_c;
           PhyPlmeResetIndication();
           hREState = mHREIdleState_c;
         }
         else 
         {
           (void)Radio_WriteRegister(gRadioTmrCmp2AReg_c, 0x8000, PhyHandlingRadioEvents);
           hREState = mHREDozeDisablingTimerState_c;
         }
         break;
    case mHRETmr1WrittingTmrCmp1ARegState_c:
         (void)Radio_WriteRegister(gRadioTmrCmp1BReg_c, 0x0000, PhyHandlingRadioEvents);
         hREState = mHRETmr1WrittingTmrCmp1BRegState_c;
         break;
    case mHRETmr1WrittingTmrCmp1BRegState_c:
         if(mPhyRxWtoState_c == mPhyState)
         {
           mPhyState = mPhyIdleState_c;
           PhyTimeRxTimeoutIndication();  /* TIMEOUT notification */
         }
         hREState = mHREIdleState_c;
         break;
    case mHREEdReadingRxStatusRegState_c:
         mPhyState = mPhyIdleState_c;
         PhyPlmeEdConfirm((uint8_t)((u16HREReg & 0xFF00)>>8));
         break;
    case mHRERxRcvdReadingControlARegState_c:
         u16HREReg &= 0xFF7F;
         (void)Radio_WriteRegister(gRadioControlAReg_c, u16HREReg, PhyHandlingRadioEvents);
         hREState = mHRERxRcvdWrittingControlARegState_c;
         break;
    case mHRERxRcvdWrittingControlARegState_c: 
         Radio_AssertTxRxEnable();          /* Re-start the sequence. */
         hREState = mHREIdleState_c;
         break;
    case mHRERxRcvdReadingRXStatusRegState_c:
         u8DataLength = (uint8_t)(u16HREReg & 0x007F);
         mPhyRxParams->linkQuality = (uint8_t)((u16HREReg & 0xFF00)>>8);                
         mPhyRxParams->linkQuality = (mPhyRxParams->linkQuality / 2);		 
         if (u8DataLength < mMinPhyLength_c) /* Rx_pkt_length is bad when <3 because of CRC and byte codes. */
         {
            PhyHREventsReStartSequense();
         }
         else
         {/* A valid packet has been received. */
           mPhyState = mPhyIdleState_c;             /* Set the rtx_state to idle */
           (void)Radio_WriteRegister(gRadioTmrCmp1AReg_c, 0x8000, PhyHandlingRadioEvents);
           hREState = mHRERxRcvdWrittingTmrCmp1ARegState_c;
         }
         break;
    case mHRERxRcvdWrittingTmrCmp1ARegState_c:
         (void)Radio_WriteRegister(gRadioTmrCmp1BReg_c, 0x0000, PhyHandlingRadioEvents);
         hREState = mHRERxRcvdWrittingTmrCmp1BRegState_c;
         break;
    case mHRERxRcvdWrittingTmrCmp1BRegState_c:
         (void)Radio_ReadRxBuffer(mDrvRxPacket->data, u8DataLength,PhyHandlingRadioEvents, gRadioNoBlock_d);
         hREState = mHRERxRcvdReadingRxPacketState_c;
         break;
    case mHRERxRcvdReadingRxPacketState_c:
         mDrvRxPacket->frameLength = u8DataLength;
         PhyChangePacketEndianess(mDrvRxPacket, u8DataLength);
         PhyPdDataIndication(); /* Notify PHY that there is data available. */
         hREState = mHREIdleState_c;         
         break;
    default:
         mPhyState = mPhyIdleState_c;
         hREState = mHREIdleState_c;
         break;
  }
}

#pragma CODE_SEG DEFAULT

