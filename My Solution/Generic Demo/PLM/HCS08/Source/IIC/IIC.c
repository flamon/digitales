/*****************************************************************************
* IIC driver implementation.
* 
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "IIC.h"

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if TRUE == gIicSupported_d   
static uint8_t *     mpu8IicTxRxPointer;
static uint8_t       mu8IicDataCounter;
static mIicStates_t  mIicState = mIicNoInit_c;
static iicCallback_t mpfIicCallback;
static uint8_t       gu8IicStarterAddress; 
static bool_t        bIicDeviceAddressFlag;
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* IIC_Init
*
************************************************************************************/
#if TRUE == gIicSupported_d 
iicErr_t IIC_Init
( 
  iicMultiplierFactors_t IICMultiplier, //IN, Multiplier factor needed to set the 
                                        //    desired baud rate
  iicClockRates_t        IICClockrate   //IN, Clock rates needed to set the baud rate
) 
{
#if (TRUE == gIicParamAndInitValidation_d)
  if((gIicMulMax_c <= IICMultiplier) || (gIicICR_Max_c <= IICClockrate))
  {
    return gIicErrInvalidParameter_c; 
  }
#endif  

  mIicState = mIicIdle_c;
  bIicDeviceAddressFlag = FALSE;

  IICC |= mIicEnableMask_c;
  IICF = ((IICMultiplier<<6)|IICClockrate);
  return gIicErrNoError_c;  
}
#else  //FALSE == gIicSupported_d

iicErr_t IIC_Init(iicMultiplierFactors_t IICMultiplier,iicClockRates_t IICClockrate)
{
  (void)IICMultiplier;
  (void)IICClockrate;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC_Write
*
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t IIC_Write
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8TxString,     //IN, Pointer to the string to be transmitted.
  uint8_t         u8StringLenght,  //IN, String Lenght 
  iicCallback_t   pfUserCallBack   //IN, to the callback function called at the end 
                                   //    of transmitting the entrie string.
) 
{
  bool_t bStartingTx;
#if (TRUE == gIicParamAndInitValidation_d)
  if(mIicNoInit_c == mIicState)
  {
    return  gIicErrInvalidCondition_c;
  }
  if((mIicMaxSlaveAddres_c <= u8SlaveAddress) || (NULL == pu8TxString) || 
     (0 == u8StringLenght) || (NULL == pfUserCallBack))
  {
    return gIicErrInvalidParameter_c; 
  }
#endif 
  if((mIicTransmittingData_c == mIicState) || (mIicReceivingData_c == mIicState))
  {
    return gIicErrModuleBusy_c; 
  }
  else if(mIicTransactionInProcess_c == mIicState)
  {
    if(gu8IicStarterAddress == u8SlaveAddress)
    {
        bStartingTx = FALSE;
    }
    else
    {
      return gIicErrModuleBusy_c; 
    }
  }
  else /*mIicIdle_c*/
  {
    gu8IicStarterAddress = u8SlaveAddress;
    IICC |= (mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);
    bStartingTx = TRUE;
  }
  mDelay20us();
  mu8IicDataCounter  = u8StringLenght;
  mpu8IicTxRxPointer = pu8TxString; 
  mpfIicCallback     = pfUserCallBack;
  
  mIicState = mIicTransmittingData_c;
  
  mIicEnableInterrupt();
  if(bStartingTx)
  {
    IICD = (gu8IicStarterAddress<<1) & 0xFE; 
  }
  else
  {
    IICD = *mpu8IicTxRxPointer++;
    mu8IicDataCounter--;
  }
  
  return gIicErrNoError_c;  
}

#else  //FALSE == gIicSupported_d

iicErr_t IIC_Write(uint8_t u8SlaveAddress,uint8_t* pu8TxString,uint8_t u8StringLenght,iicCallback_t   pfUserCallBack) 
{
  (void)u8SlaveAddress;
  (void)pu8TxString;
  (void)u8StringLenght;
  (void)pfUserCallBack;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC_Read
*
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t IIC_Read
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8RxString,     //OUT, Pointer to the string to be received.
  uint8_t         u8StringLenght,  //IN, String Lenght 
  iicCallback_t   pfUserCallBack   //IN, to the callback function called at the end 
                                   //    of receiving the entrie string.
)
{
#if (TRUE == gIicParamAndInitValidation_d)
  if(mIicNoInit_c == mIicState)
  {
    return  gIicErrInvalidCondition_c;
  }
  if((mIicMaxSlaveAddres_c <= u8SlaveAddress) || (NULL == pu8RxString) || 
     (0 == u8StringLenght) || (NULL == pfUserCallBack))
  {
    return gIicErrInvalidParameter_c; 
  }
#endif 
  if((mIicTransmittingData_c == mIicState) || (mIicReceivingData_c == mIicState))
  {
    return gIicErrModuleBusy_c; 
  }
  else if(mIicTransactionInProcess_c == mIicState)
  {
    if(gu8IicStarterAddress == u8SlaveAddress)
    {
        IICC |= mIicRepeatStartMask_c;
    }
    else
    {
      return gIicErrModuleBusy_c; 
    }
  }
  else /*mIicIdle_c*/
  {
    gu8IicStarterAddress = u8SlaveAddress;
    IICC |= (mIicInterruptEnableMask_c|mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);
  }
  
  mu8IicDataCounter  = u8StringLenght;
  mpu8IicTxRxPointer = pu8RxString; 
  mpfIicCallback     = pfUserCallBack;
  bIicDeviceAddressFlag = TRUE;
  
  mIicState = mIicReceivingData_c;
  mIicEnableInterrupt();
  IICD = (gu8IicStarterAddress<<1) | 0x01;
  
  return gIicErrNoError_c;  
}

#else  //FALSE == gIicSupported_d

iicErr_t IIC_Read(uint8_t u8SlaveAddress,uint8_t* pu8RxString,uint8_t u8StringLenght,iicCallback_t   pfUserCallBack) 
{
  (void)u8SlaveAddress;
  (void)pu8RxString;
  (void)u8StringLenght;
  (void)pfUserCallBack;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC_WriteBlocking
*
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t IIC_WriteBlocking
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8TxString,     //IN, Pointer to the string to be transmitted.
  uint8_t         u8StringLenght   //IN, String Lenght 
)
{
  iicErr_t iicErr;  
  bool_t bStartingTx;
#if (TRUE == gIicParamAndInitValidation_d)
  if(mIicNoInit_c == mIicState)
  {
    return  gIicErrInvalidCondition_c;
  }
  if((mIicMaxSlaveAddres_c <= u8SlaveAddress) || (NULL == pu8TxString) || 
     (0 == u8StringLenght))
  {
    return gIicErrInvalidParameter_c; 
  }
#endif 
  if((mIicTransmittingData_c == mIicState) || (mIicReceivingData_c == mIicState))
  {
    return gIicErrModuleBusy_c; 
  }
  else if(mIicTransactionInProcess_c == mIicState)
  {
    if(gu8IicStarterAddress == u8SlaveAddress)
    {
        bStartingTx = FALSE;
    }
    else
    {
      return gIicErrModuleBusy_c; 
    }
  }
  else /*mIicIdle_c*/
  {
    gu8IicStarterAddress = u8SlaveAddress;
    IICC |= (mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);
    bStartingTx = TRUE;
  }
  
  if(bStartingTx)
  {
    iicErr = iicSendNextByte((gu8IicStarterAddress<<1) & 0xFE);
    if(iicErr) return iicErr;
  }
  
  while(u8StringLenght--)
  {
    iicErr = iicSendNextByte(*pu8TxString++);    
    if(iicErr) return iicErr;
  }

  mIicState = mIicTransactionInProcess_c;
  return gIicErrNoError_c;  
}

#else  //FALSE == gIicSupported_d

iicErr_t IIC_WriteBlocking(uint8_t u8SlaveAddress,uint8_t* pu8TxString,uint8_t u8StringLenght) 
{
  (void)u8SlaveAddress;
  (void)pu8TxString;
  (void)u8StringLenght;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC_ReadBlocking
*
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t IIC_ReadBlocking
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8RxString,     //OUT, Pointer to the string to be received.
  uint8_t         u8StringLenght   //IN, String Lenght 
)
{
  iicErr_t iicErr;  
#if (TRUE == gIicParamAndInitValidation_d)
  if(mIicNoInit_c == mIicState)
  {
    return  gIicErrInvalidCondition_c;
  }
  if((mIicMaxSlaveAddres_c <= u8SlaveAddress) || (NULL == pu8RxString) || 
     (0 == u8StringLenght))
  {
    return gIicErrInvalidParameter_c; 
  }
#endif 
  if((mIicTransmittingData_c == mIicState) || (mIicReceivingData_c == mIicState))
  {
    return gIicErrModuleBusy_c; 
  }
  else if(mIicTransactionInProcess_c == mIicState)
  {
    if(gu8IicStarterAddress == u8SlaveAddress)
    {
        IICC |= mIicRepeatStartMask_c;
    }
    else
    {
      return gIicErrModuleBusy_c; 
    }
  }
  else /*mIicIdle_c*/
  {
    gu8IicStarterAddress = u8SlaveAddress;
    IICC |= (mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);
  }
 
  iicErr = iicSendNextByte((gu8IicStarterAddress<<1) | 0x01);
  if(iicErr) return iicErr;
  
  if(1 == u8StringLenght) IICC |= mIicNoAckMask_c;
  else IICC &= ~mIicNoAckMask_c;
  IICC &= ~mIicTransmitModeSelectMask_c;
  (void)IICD;   /*Starts the first reception*/
  while(u8StringLenght){
    while(!(mIicInterruptFlagMask_c & IICS));              
    IICS |= mIicInterruptFlagMask_c; 
    u8StringLenght--;                      
    if(1 == u8StringLenght) IICC |= mIicNoAckMask_c;
    if(0 == u8StringLenght) IICC |= mIicTransmitModeSelectMask_c;
    *pu8RxString++ = IICD;
  }
 
  mIicState = mIicTransactionInProcess_c;
  return gIicErrNoError_c;  
}

#else  //FALSE == gIicSupported_d

iicErr_t IIC_ReadBlocking(uint8_t u8SlaveAddress,uint8_t* pu8RxString,uint8_t u8StringLenght) 
{
  (void)u8SlaveAddress;
  (void)pu8RxString;
  (void)u8StringLenght;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC_StopTransaction
*
************************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t IIC_StopTransaction
(
  uint8_t         u8SlaveAddress  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
)
{
#if (TRUE == gIicParamAndInitValidation_d)
  if(mIicNoInit_c == mIicState)
  {
    return  gIicErrInvalidCondition_c;
  }
  if(mIicMaxSlaveAddres_c <= u8SlaveAddress)
  {
    return gIicErrInvalidParameter_c; 
  }
#endif 
  if((mIicTransmittingData_c == mIicState) || (mIicReceivingData_c == mIicState))
  {
    return gIicErrModuleBusy_c; 
  }
  else if(mIicTransactionInProcess_c == mIicState)
  {
    if(gu8IicStarterAddress != u8SlaveAddress)
    {
      return gIicErrModuleBusy_c; 
    }
  }

  IICC &= ~(mIicInterruptEnableMask_c|mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);

  mIicState = mIicIdle_c;
  return gIicErrNoError_c;  

}
#else  //FALSE == gIicSupported_d

iicErr_t IIC_StopTransaction(uint8_t u8SlaveAddress) 
{
  (void)u8SlaveAddress;
  return gIicErrNoError_c;
}
#endif

/************************************************************************************
*
* IIC Interrupt service function
*
************************************************************************************/
#if TRUE == gIicSupported_d

#if gIicMode_d == gIicInterrupt_c  

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

INTERRUPT_KEYWORD void IIC_Isr(void)
{
#else
void IIC_Task(void)
{
#endif
  IICS |= mIicInterruptFlagMask_c; 
  if(mIicTransmitModeSelectMask_c & IICC)
  {
    if(mIicReceiveAckMask_c & IICS)
    {
      iicFinishReadWrite(gIicTxNoAck_c);
      return;
    }
  }
  
  if(mIicTransmittingData_c == mIicState)
  {
    if(mu8IicDataCounter--)
    {
      IICD = *mpu8IicTxRxPointer++;
    }
    else
    {
      iicFinishReadWrite(gIicTxRxSuccess_c);
    }
  }
  else if(mIicReceivingData_c == mIicState)
  {
    if(mu8IicDataCounter)
    {
      if(1 == mu8IicDataCounter) IICC |= mIicNoAckMask_c;
      else IICC &= ~mIicNoAckMask_c;
      mu8IicDataCounter--;
      if(bIicDeviceAddressFlag)
      {
        bIicDeviceAddressFlag = FALSE;
        IICC &= ~mIicTransmitModeSelectMask_c;
        (void)IICD;   /*Starts the first reception*/
      }
      else
      {
        *mpu8IicTxRxPointer++ = IICD;         
      }
    }
    else
    {
      IICC |= mIicTransmitModeSelectMask_c;
      *mpu8IicTxRxPointer++ = IICD;   
      iicFinishReadWrite(gIicTxRxSuccess_c);
    }
  }
  else
  {
    IICC &= ~(mIicInterruptEnableMask_c|mIicMasterModeSelectMask_c|mIicTransmitModeSelectMask_c);
    mIicState = mIicIdle_c;
  }
}

#pragma CODE_SEG DEFAULT

#endif 

/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/
/***********************************************************************************/
#if TRUE == gIicSupported_d
iicErr_t iicSendNextByte(uint8_t NextByte)
{
  IICD = NextByte;                                       
  while(!(mIicInterruptFlagMask_c & IICS));             
  IICS |= mIicInterruptFlagMask_c;                       
  if(mIicReceiveAckMask_c & IICS) return gIicErrNoAck_c;
  else return gIicErrNoError_c;
}

#if gIicMode_d == gIicInterrupt_c   
#pragma CODE_SEG ISR_SEGMENT
#endif
void iicFinishReadWrite(iicResult_t rwResult)
{
  mIicState = mIicTransactionInProcess_c;
  mIicDisableInterrupt();
  mpfIicCallback(rwResult);
}
#pragma CODE_SEG DEFAULT

#endif
