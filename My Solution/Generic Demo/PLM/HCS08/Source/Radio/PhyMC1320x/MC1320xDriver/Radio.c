/*****************************************************************************
* Radio driver implementation.
* Radio's driver code
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#include "Radio.h"

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

uint16_t              gu16RadioIrqStatus;
radioDriverStates_t   mRadioDriverState;
radioCallback_t       mpfRadioProcessCallback;
radioCallback_t       mpfRadioEventCallback;
uint8_t               mau8RadioDataBuffer[gRadioMaxDataLenght_c];
uint8_t*              mpu8RadioRxPointer;
uint8_t               mau8RadioDataLength;
uint16_t              mu16WrittingRegisterValue;
uint16_t*             mpu16ReadingRegisterPointer;
radioProcessStates_t  mRadioProcessState;


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
* Initialize the Radio Driver and sets the Radio's control signals to an initial 
* state.
*
************************************************************************************/
radioError_t Radio_DriverInit
(
  radioCallback_t   radioEventCallback //IN, Pointer to the Event callback
)
{
  if(NULL == radioEventCallback){
    return gRadioErrInvalidParameter_c;
  }
  
  IRQ_Init((irqCallback_t)(RadioIRQServiceRoutine));
  gu16RadioIrqStatus      = gRadioIrqNoFlags_c;
  mRadioDriverState       = mRadioStateIdle_c;
  mpfRadioEventCallback   = radioEventCallback;
  mRadioProcessState     = mRadioProcessIdle_c;
  RadioInitControlSignals();
  
  return gRadioErrNoError_c;
}

/************************************************************************************
* Function to Write on the Radio's SPI registers
*
************************************************************************************/
radioError_t Radio_WriteRegister
(
  uint8_t           regToWrite,     //IN, Addres of the Radio register to be writted  
  uint16_t          u16RegValue,    //IN, value to write on the register
  radioCallback_t   writeCallback   /*IN, Callback function to be called at the end  
                                          of the register write*/
)
{

  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }
  
  IRQ_DisableIrq();
  mu16WrittingRegisterValue  = u16RegValue;
  mpfRadioProcessCallback    = writeCallback;
  mRadioDriverState = mRadioStateWrittingRegister_c;
  mRadioProcessState = mRadioProcessSendingAddres_c;
  (void)SPI1_WriteByte(regToWrite & 0x7F);

  return gRadioErrNoError_c;
}
 
/************************************************************************************
* Function to Write on the Radio's SPI registers
*
************************************************************************************/
radioError_t Radio_WriteRegisterBlocking
(
  uint8_t           regToWrite,    //IN, Addres of the Radio register to be writted  
  uint16_t          u16RegValue    //IN, value to write on the register
)
{
  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }

  IRQ_DisableIrq();
  mRadioDriverState = mRadioStateWrittingRegister_c;
  SPI1_WriteByteBlocking(regToWrite & 0x7F);
  (void)SPI1_WriteStringBlocking((uint8_t*)&u16RegValue, 2);
  (void)SPI1_DeassertPeripheralSsPin();
  mRadioDriverState = mRadioStateIdle_c;
  
  IRQ_EnableIrq();
}

/************************************************************************************
* Function to Read from the Radio's SPI registers
*
************************************************************************************/
radioError_t Radio_ReadRegister
(
  uint8_t           regToRead,      //IN, Addres of the Radio register to be read  
  uint16_t*         pu16RegValue,   //OUT, pointer to store the register value
  radioCallback_t   readCallback    /*IN, Callback function to be called at the end  
                                          of the register read*/
)
{
  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }

  IRQ_DisableIrq();
  mpu16ReadingRegisterPointer  = pu16RegValue;
  mpfRadioProcessCallback      = readCallback;
  mRadioDriverState = mRadioStateReadingRegister_c;
  mRadioProcessState = mRadioProcessSendingAddres_c;
  (void)SPI1_WriteByte(regToRead | 0x80);

  return gRadioErrNoError_c;
}

/************************************************************************************
* Function to Read from the Radio's SPI registers
*
************************************************************************************/
radioError_t Radio_ReadRegisterBlocking
(
  uint8_t           regToRead,     //IN, Addres of the Radio register to be read  
  uint16_t*         pu16RegValue   //OUT, pointer to store the register value
)
{
  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }

  IRQ_DisableIrq();
  mRadioDriverState = mRadioStateReadingRegister_c;
  SPI1_WriteByteBlocking(regToRead | 0x80);
  (void)SPI1_ReadStringBlocking((uint8_t *)pu16RegValue, 2);
  (void)SPI1_DeassertPeripheralSsPin();
  mRadioDriverState = mRadioStateIdle_c;
  
  IRQ_EnableIrq();
  
  return gRadioErrNoError_c;
}

/************************************************************************************
* Function to write data into one of the Radio Tx buffers
*
************************************************************************************/
radioError_t Radio_WriteTxBuffer
(
  uint8_t *         pu8WrittingString, //IN, Pointer to the string to write on the Radio
  uint8_t           u8StringLenght,    //IN, String Lenght
  radioCallback_t   writeCallback,     /*IN, Callback function to be called at the end  
                                             of the buffer write*/
  radioUtilBlock_t  allowToBlock       /*IN, Specifies if the function shall wait until 
                                             the buffer is writted*/
)
{
  uint8_t * pu8AuxPtr;
  
  if((NULL == pu8WrittingString) || (gRadioMaxDataLenght_c < u8StringLenght)){
    return gRadioErrInvalidParameter_c;
  }
  
  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }
  mRadioDriverState = mRadioStateWrittingTxBuffer_c;
  
  IRQ_DisableIrq();
  /*Copies the user string to the driver's buffer */
  pu8AuxPtr = mau8RadioDataBuffer;
  RadioStringCopyBigToLittleEndian(pu8WrittingString,pu8AuxPtr,u8StringLenght);
 
  if(gRadioNoBlock_d == allowToBlock){
    mpfRadioProcessCallback = writeCallback;
    mau8RadioDataLength = u8StringLenght;

    mRadioDriverState = mRadioStateWrittingTxBuffer_c;
    mRadioProcessState = mRadioProcessSendingAddres_c;
    (void)SPI1_WriteByte(gRadioTxPktRAMReg_c & 0x7F);
  }
  else
  {
    SPI1_WriteByteBlocking(gRadioTxPktRAMReg_c & 0x7F);
    (void)SPI1_WriteStringBlocking(mau8RadioDataBuffer, u8StringLenght);
    (void)SPI1_DeassertPeripheralSsPin();
    mRadioDriverState = mRadioStateIdle_c;
    IRQ_EnableIrq();
  }

  return gRadioErrNoError_c;
}

/************************************************************************************
* Function to read data from the Radio Rx buffer
*
************************************************************************************/
radioError_t Radio_ReadRxBuffer
(
  uint8_t *         pu8ReadingString,
  uint8_t           u8StringLenght,
  radioCallback_t   readCallback,     /*IN, Callback function to be called at the end  
                                            of the buffer read*/
  radioUtilBlock_t  allowToBlock      /*IN, Specifyes if the function shall wait until 
                                            the buffer is read*/
)
{
  if((NULL == pu8ReadingString) || (gRadioMaxDataLenght_c < u8StringLenght)){
    return gRadioErrInvalidParameter_c;
  }
  
  if(mRadioAreRadioOrSPIBusy){
    return gRadioErrRadioBussy_c;    
  }
  mRadioDriverState = mRadioStateReadingRxBuffer_c;

  IRQ_DisableIrq();
  
  
  if(gRadioNoBlock_d == allowToBlock)
  {
    mpu8RadioRxPointer = pu8ReadingString;
    mpfRadioProcessCallback     = readCallback;
    mau8RadioDataLength         = u8StringLenght;

    mRadioProcessState = mRadioProcessSendingAddres_c;
  
   (void)SPI1_WriteByte(gRadioRxPktRAMReg_c | 0x80);
  }
  else
  {
    SPI1_WriteByteBlocking(gRadioRxPktRAMReg_c | 0x80);
    (void)SPI1_ReadStringBlocking(pu8ReadingString, 2);
    (void)SPI1_ReadStringBlocking(pu8ReadingString, u8StringLenght);    
    (void)SPI1_DeassertPeripheralSsPin();
    mRadioDriverState = mRadioStateIdle_c;
    IRQ_EnableIrq();
  }

  return gRadioErrNoError_c;
}

/************************************************************************************
* Function to know if the Radio driver is on Idle state
*
************************************************************************************/
bool_t Radio_IsRadioIdle(void)
{
  if(mRadioStateIdle_c != mRadioDriverState){
    return FALSE;
  }
  else{
    return TRUE;
  }
}


/************************************************************************************
* ...
*
*
************************************************************************************/
void Radio_EnableRadioInterrupts(void)
{
  IRQ_EnableIrq();
  mSpi1EnableRxInterrupt();
  mSpi1EnableTxInterrupt();
}

/************************************************************************************
* ...
*
*
************************************************************************************/
void Radio_DisableRadioInterrupts(void)
{
  IRQ_DisableIrq();
  mSpi1DisableRxInterrupt();
  mSpi1DisableTxInterrupt();
}

/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/
/***********************************************************************************/
/*Callback function called by the IRQ driver*/
void RadioIRQServiceRoutine(void)
{
  volatile radioError_t RadError;
  
  IRQ_DisableIrq();
  /*Some functionality from the LPW library shall be included
  here to allow the radio driver to work on LPW mode*/
  RadError = Radio_ReadRegister\
  (gRadioIRQStatusReg_c, &gu16RadioIrqStatus, mpfRadioEventCallback);
}


/***********************************************************************************/
void RadioInitControlSignals(void)
{
  (void)Gpio_PinClear(gRadioAttnPort_c, gRadioAttnPinMask_c);
  (void)Gpio_PinClear(gRadioRxTxEnablePort_c, gRadioRxTxEnablePinMask_c);
  (void)Gpio_PinSet(gRadioResetRadioPort_c, gRadioResetRadioPinMask_c);
}

/***********************************************************************************/
void RadioProcess(void)
{
  volatile spiErr_t SpiResp;

  if(mRadioProcessSendingAddres_c == mRadioProcessState){
    mRadioProcessState = mRadioProcessRdWrData_c;
    switch(mRadioDriverState){
      case mRadioStateWrittingRegister_c:
       SpiResp = SPI1_WriteString((uint8_t *)&mu16WrittingRegisterValue,2);   
      break;
      case mRadioStateReadingRegister_c:
       SpiResp = SPI1_ReadString((uint8_t *)mpu16ReadingRegisterPointer,2);       
      break;
      case mRadioStateWrittingTxBuffer_c:
       SpiResp = SPI1_WriteString(mau8RadioDataBuffer,mau8RadioDataLength);   
      break;
      case mRadioStateReadingRxBuffer_c:
       SpiResp = SPI1_ReadString(mpu8RadioRxPointer,2);
       mRadioProcessState = mRadioProcessDiscartingBytes_c;       
      break;
      default:
      break;
    }
  }
  else if(mRadioProcessDiscartingBytes_c == mRadioProcessState){
     SpiResp = SPI1_ReadString(mpu8RadioRxPointer,mau8RadioDataLength);
     mRadioProcessState = mRadioProcessRdWrData_c;       
  }
  else{
    mRadioProcessState = mRadioProcessIdle_c;
    (void)SPI1_DeassertPeripheralSsPin();
    mRadioDriverState = mRadioStateIdle_c;    
    if(NULL != mpfRadioProcessCallback){
     mpfRadioProcessCallback();
    }
    IRQ_EnableIrq();
  }
}

#pragma CODE_SEG DEFAULT

/***********************************************************************************/




























