/*****************************************************************************
* SPI driver implementation.
* Driver to transmit and receive strings via SPI
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "SPI.h"


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if TRUE == gSpiSupported_d 

/* SPI 1 Globals */
#if gSpiUsePort1_d

static uint8_t         *mpu8SPI1Ptr;        // Transmit/Receive Data Pointer 
static uint8_t         mu8SPI1Counter;      // Module data counter 
static spiStatus_t     mSPI1Status;         // SPI Driver Status 
spiCallback_t          gpfSpi1CallBack;     // SPI callback

#endif //gSpiUsePort1_d

/* SPI 2 Globals */
#if gSpiUsePort2_d

static uint8_t         *mpu8SPI2Ptr;        // Transmit/Receive Data Pointer 
static uint8_t         mu8SPI2Counter;      // Module data counter 
static spiStatus_t     mSPI2Status;         // SPI Driver Status 
spiCallback_t          gpfSpi2CallBack;     // SPI callback

#endif //gSpiUsePort2_d

#endif

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

/* SPI 1 Functions */

#if TRUE == gSpiSupported_d 

#if gSpiUsePort1_d
/***********************************************************************************/


void SPI1_Init
( 
  spiBaudRateDivisor_t SpiBaud            //IN, Desired baudrate on the SPI bus
)
{
  
 /*Init the SS pins in disable mode*/
 #if gSpi1UsePeripheral1_d    
  (void)Gpio_PinClear(gSpi1Peripheral1SsPort_c, gSpi1Peripheral1SsPinMask_c);
 #endif

 #if gSpi1UsePeripheral2_d   
  (void)Gpio_PinClear(gSpi1Peripheral2SsPort_c, gSpi1Peripheral2SsPinMask_c);
 #endif   

  (void)mSpi1StatusReg;                   // Clear any Interrupt Flag 
  mSpi1BaudReg    = (uint8_t)SpiBaud;     // Baud Rate Divisor = SpiBaud 
  mSpi1Ctr1       = mSpi1Ctrl1RegVal_c; 
  mSpi1Ctr2       = mSpi1Ctrl2RegVal_c;   // SS as GPIO,SPI use separate pins 
  gpfSpi1CallBack = NULL;                 //Sets the SPI calback*/
  mSPI1Status     = mSpiIdle_c;           //Sets the SPI state machine in IDLE
  mpu8SPI1Ptr     = NULL;                 //Initializes the data pinter

}
#endif //gSpiUsePort1_d
#else //FALSE == gSpiSupported_d 

void SPI1_Init(spiBaudRateDivisor_t SpiBaud)
{
  (void)SpiBaud;
}

#endif

/***********************************************************************************/

#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_AssertPeripheralSsPin
(
 uint8_t       u8PeripheralNum,           //IN, which of the peripherals on the list will be used
 spiCallback_t pfUserCallBack             //IN, Pointer to the callback function called when a frame 
                                          //is completely received or transmitted 
) 
{

  if(mSpiIdle_c == mSPI1Status){

    if(gSpi1PeripheralsOnTheBus_c >= u8PeripheralNum){

      /*Asserts the peripheral pin*/ 
      if(1 == u8PeripheralNum){

       #if gSpi1UsePeripheral1_d   
        (void)Gpio_PinSet(gSpi1Peripheral1SsPort_c, gSpi1Peripheral1SsPinMask_c);
       #endif

      }
      else{

       #if gSpi1UsePeripheral2_d 
        (void)Gpio_PinSet(gSpi1Peripheral2SsPort_c, gSpi1Peripheral2SsPinMask_c);
       #endif

      }
       gpfSpi1CallBack = pfUserCallBack;  //Sets the SPI calback
       mSPI1Status     = mSpiPeripheralAsserted_c;
       return gSpiErrNoError_c;

    }
    else {
      return gSpiErrInvalidParameter_c;
    }
  }
  else {
    return gSpiErrModuleBusy_c;
  }

}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_AssertPeripheralSsPin(uint8_t u8PeripheralNum, spiCallback_t pfUserCallBack)
{
  (void)u8PeripheralNum;
  (void)pfUserCallBack;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/

#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_DeassertPeripheralSsPin
(
void
)
{

  if(mSpiPeripheralAsserted_c == mSPI1Status){

    /*sets all te SS pins to disable*/
   #if gSpi1UsePeripheral1_d   
    (void)Gpio_PinClear(gSpi1Peripheral1SsPort_c, gSpi1Peripheral1SsPinMask_c);
   #endif
    
   #if gSpi1UsePeripheral2_d   
    (void)Gpio_PinClear(gSpi1Peripheral2SsPort_c, gSpi1Peripheral2SsPinMask_c);
   #endif   
 
    mSPI1Status = mSpiIdle_c;
    return gSpiErrNoError_c;

  }
  else{

    if(mSpiIdle_c == mSPI1Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }
  
}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_DeassertPeripheralSsPin(void)
{
  return gSpiErrNoError_c;
}

#endif


/***********************************************************************************/
#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_WriteString
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
)
{
  if(mSpiPeripheralAsserted_c == mSPI1Status){

    (void)mSpi1StatusReg;                  //Clear any unattended Interrupt Flag 
    mu8SPI1Counter = u8StringLenght - 1;   //Sets the amount of bytes to send
    mpu8SPI1Ptr    = pu8TxString;          //Sets the buffer to transmit
    mSPI1Status    = mSpiTransmittingData_c;
    mSpi1DataReg   = *mpu8SPI1Ptr++;       //Put the first byte in the SPI data register

   #if gSpi1Mode_d == gSpiInterrupt_d   
    mSpi1EnableTxInterrupt();              //Enables de SPI interrupt
   #endif

    return gSpiErrNoError_c;

  }
  else{
  
    if(mSpiIdle_c == mSPI1Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }

}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_WriteString (uint8_t *pu8TxString, uint8_t  u8StringLenght)
{
  (void)pu8TxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_WriteStringBlocking
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
)
{
  if(mSpiPeripheralAsserted_c == mSPI1Status){
    mSPI1Status    = mSpiTransmittingData_c;
    for(u8StringLenght; u8StringLenght > 0; u8StringLenght--){
       SPI1_WriteByteBlocking(*pu8TxString++);
    }
    mSPI1Status    = mSpiPeripheralAsserted_c;
    return gSpiErrNoError_c;
  }
  else{
    if(mSpiIdle_c == mSPI1Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }
  }
}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_WriteStringBlocking (uint8_t *pu8TxString, uint8_t  u8StringLenght)
{
  (void)pu8TxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_ReadString
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
) 
{

  if(mSpiPeripheralAsserted_c == mSPI1Status){

    (void)mSpi1StatusReg;                  // Clear any unattended Interrupt Flag 
    mu8SPI1Counter = u8StringLenght - 1;   //Sets the amount of bytes to send
    mpu8SPI1Ptr    = pu8RxString;          //Sets the buffer to transmit
    mSPI1Status    = mSpiReceivingData_c;
    mSpi1DataReg   = mDummyByte_c;         //Puts a dummy byte in the SPI data register

   #if gSpi1Mode_d == gSpiInterrupt_d   
    mSpi1EnableRxInterrupt();              //Enables de SPI interrupt
   #endif

    return gSpiErrNoError_c;

  }
  else{

    if(mSpiIdle_c == mSPI1Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }  

}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_ReadString (uint8_t *pu8RxString, uint8_t  u8StringLenght)
{
  (void)pu8RxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_ReadStringBlocking
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
) 
{

  if(mSpiPeripheralAsserted_c == mSPI1Status){

    mSPI1Status    = mSpiReceivingData_c;
    for(u8StringLenght; u8StringLenght > 0; u8StringLenght--){
       *pu8RxString++ = SPI1_ReadByteBlocking();
    }
    mSPI1Status    = mSpiPeripheralAsserted_c;
    return gSpiErrNoError_c;
  }
  else{

    if(mSpiIdle_c == mSPI1Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }  

}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_ReadStringBlocking (uint8_t *pu8RxString, uint8_t  u8StringLenght)
{
  (void)pu8RxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d 
#if gSpiUsePort1_d
spiErr_t SPI1_WriteByte
(
uint8_t u8TxData                           //IN, Byte to be transmitted
)
{
  return SPI1_WriteString(&u8TxData, 1);
}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_WriteByte (uint8_t  u8TxData)
{
  (void)u8TxData;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort1_d
void SPI1_WriteByteBlocking
(
uint8_t u8TxData                           //IN, Byte to be transmitted
)
{
  (void)mSpi1StatusReg;                  // Clear any unattended Interrupt Flag 
  mSpi1DataReg = u8TxData;
  while (!(mSpi1TxFlag));
}
#endif
#else //FALSE == gSpiSupported_d 

void SPI1_WriteByteBlocking (uint8_t  u8TxData)
{
  (void)u8TxData;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort1_d
spiErr_t SPI1_ReadByte
(
uint8_t * pu8RxData                        //IN, Address to put the readed byte
)
{
  return SPI1_ReadString(pu8RxData, 1);
}
#endif
#else //FALSE == gSpiSupported_d 

spiErr_t SPI1_ReadByte (uint8_t  * pu8RxData )
{
  (void)pu8RxData;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort1_d
uint8_t SPI1_ReadByteBlocking
(
void                        //IN, Address to put the readed byte
)
{
 (void)mSpi1StatusReg;                  // Clear any unattended Interrupt Flag 
  mSpi1DataReg = mDummyByte_c;
  while (!(mSpi1RxFlag));
  return mSpi1DataReg;
}
#endif
#else //FALSE == gSpiSupported_d 

uint8_t SPI1_ReadByteBlocking (void)
{
  return 0;
}

#endif

#pragma CODE_SEG DEFAULT

/************************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort1_d
#if gSpi1Mode_d == gSpiInterrupt_d

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

INTERRUPT_KEYWORD void SPI1_Isr(void)
{
#else
void SPI1_Task(void)
{

  if(mSpi1RxFlag || mSpi1TxFlag)
#endif 
  {

    uint8_t u8ReceivedByte;
    (void)mSpi1StatusReg;                   // First step to erase the Interrupt flags 
    u8ReceivedByte = mSpi1DataReg;          // Second step to erase the Interrupt flags
    if (mSpiTransmittingData_c == mSPI1Status){

      if (0 == mu8SPI1Counter){

        mSPI1Status = mSpiPeripheralAsserted_c; 

       #if gSpi1Mode_d == gSpiInterrupt_d   
        mSpi1DisableTxInterrupt();          //Disables SPI interrupt
       #endif

        if(NULL != gpfSpi1CallBack){
          gpfSpi1CallBack();                          
        }

      }
      else{
        mu8SPI1Counter--;                  //Decreases the counter
        mSpi1DataReg = *mpu8SPI1Ptr++;     // Transmit next byte 
      }

    }
    else if (mSpiReceivingData_c == mSPI1Status){
    
      *mpu8SPI1Ptr = u8ReceivedByte;       //Read SPI Data 
      if (0 == mu8SPI1Counter){
      
        mSPI1Status = mSpiPeripheralAsserted_c; 

       #if gSpi1Mode_d == gSpiInterrupt_d   
        mSpi1DisableRxInterrupt();         //Disables SPI interrupt
       #endif

        if(NULL != gpfSpi1CallBack){
          gpfSpi1CallBack();                       
        }
      
      }
      else{
        mu8SPI1Counter--;                  //Decreases the counter
        mpu8SPI1Ptr++;  
        mSpi1DataReg = mDummyByte_c;       //Puts a dummy byte in the SPI data register              
      }
    
    }
    else{                                  //It isn't transmitting or receiving 
      
    }
  
  }

}   
#pragma CODE_SEG DEFAULT

#endif //gSpiUsePort1_d

#endif //gSpiSupported_d

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/* SPI 2 Functions */

#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
/***********************************************************************************/
void SPI2_Init
( 
  spiBaudRateDivisor_t SpiBaud             //IN, Desired baudrate on the SPI bus
)
{
  
 /*sets all te SS pins to disable*/
 #if gSpi2UsePeripheral1_d   
  (void)Gpio_PinClear(gSpi2Peripheral1SsPort_c, gSpi2Peripheral1SsPinMask_c);
 #endif  
 #if gSpi2UsePeripheral2_d   
  (void)Gpio_PinClear(gSpi2Peripheral2SsPort_c, gSpi2Peripheral2SsPinMask_c);
 #endif   

  (void)mSpi2StatusReg;                    // Clear any Interrupt Flag 
  mSpi2BaudReg    = (uint8_t)SpiBaud;      // Baud Rate Divisor = SpiBaud 
  mSpi2Ctr1       = mSpi2Ctrl1RegVal_c; 
  mSpi2Ctr2       = mSpi2Ctrl2RegVal_c;    // SS as GPIO,SPI use separate pins 
  gpfSpi2CallBack = NULL;                  //Sets the SPI calback
  mSPI2Status     = mSpiIdle_c;            //Sets the SPI state machine in IDLE
  mpu8SPI2Ptr     = NULL;                  //Initializes the data pinter 

}
#endif
#else //FALSE == gSpiSupported_d
void SPI2_Init(spiBaudRateDivisor_t SpiBaud)
{
  (void)SpiBaud;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_AssertPeripheralSsPin
(
 uint8_t       u8PeripheralNum,      //IN, Which of the peripherals on the list will be used
 spiCallback_t pfUserCallBack        //IN, Pointer to the callback function called when a frame 
                                     //is completely received or transmitted 
) 
{

  if(mSpiIdle_c == mSPI2Status){

    if(gSpi2PeripheralsOnTheBus_c >= u8PeripheralNum){

      /*Asserts the peripheral pin*/ 
      if(1 == u8PeripheralNum){
       #if gSpi2UsePeripheral1_d   
        (void)Gpio_PinSet(gSpi2Peripheral1SsPort_c, gSpi2Peripheral1SsPinMask_c);
       #endif
      }
      else{
       #if gSpi2UsePeripheral2_d   
        (void)Gpio_PinSet(gSpi2Peripheral2SsPort_c, gSpi2Peripheral2SsPinMask_c);
       #endif
      }
       gpfSpi2CallBack = pfUserCallBack;   //Sets the SPI calback
       mSPI2Status     = mSpiPeripheralAsserted_c;
       return gSpiErrNoError_c;

    }
    else {
      return gSpiErrInvalidParameter_c;
    }

  }
  else {
    return gSpiErrModuleBusy_c;
  }

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_AssertPeripheralSsPin(uint8_t u8PeripheralNum, spiCallback_t pfUserCallBack)
{
  (void)u8PeripheralNum;
  (void)pfUserCallBack;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_DeassertPeripheralSsPin
(
void
)
{

  if(mSpiPeripheralAsserted_c == mSPI2Status){

     /*sets all te SS pins to disable*/
   #if gSpi2UsePeripheral1_d   
    (void)Gpio_PinClear(gSpi2Peripheral1SsPort_c, gSpi2Peripheral1SsPinMask_c);
   #endif  
   #if gSpi2UsePeripheral2_d   
    (void)Gpio_PinClear(gSpi2Peripheral2SsPort_c, gSpi2Peripheral2SsPinMask_c);
   #endif   
    mSPI2Status = mSpiIdle_c;
    return gSpiErrNoError_c;

  }
  else{

    if(mSpiIdle_c == mSPI2Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_DeassertPeripheralSsPin(void)
{
  return gSpiErrNoError_c;
}

#endif


/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_WriteString
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
)
{

  if(mSpiPeripheralAsserted_c == mSPI2Status){

    (void)mSpi2StatusReg;                  // Clear any unattended Interrupt Flag 
    mu8SPI2Counter = u8StringLenght - 1;   //Sets the amount of bytes to send
    mpu8SPI2Ptr    = pu8TxString;          //Sets the buffer to transmit
    mSPI2Status    = mSpiTransmittingData_c;
    mSpi2DataReg   = *mpu8SPI2Ptr++;       //Put the first byte in the SPI data register

   #if(gSpi2Mode_d == gSpiInterrupt_d)   
    mSpi2EnableTxInterrupt();              //Enables de SPI interrupt
   #endif

    return gSpiErrNoError_c;

  }
  else{

    if(mSpiIdle_c == mSPI2Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_WriteString(uint8_t *pu8TxString, uint8_t  u8StringLenght)
{
  (void)pu8TxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_WriteStringBlocking
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
)
{
  if(mSpiPeripheralAsserted_c == mSPI2Status){
    mSPI2Status    = mSpiTransmittingData_c;
    for(u8StringLenght; u8StringLenght > 0; u8StringLenght--){
       SPI2_WriteByteBlocking(*pu8TxString++);
    }
    mSPI2Status    = mSpiPeripheralAsserted_c;
    return gSpiErrNoError_c;
  }
  else{
  
    if(mSpiIdle_c == mSPI2Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_WriteStringBlocking(uint8_t *pu8TxString, uint8_t  u8StringLenght)
{
  (void)pu8TxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_ReadString
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
) 
{
  
  if(mSpiPeripheralAsserted_c == mSPI2Status){

    (void)mSpi2StatusReg;                  // Clear any unattended Interrupt Flag 
    mu8SPI2Counter = u8StringLenght - 1;   //Sets the amount of bytes to send
    mpu8SPI2Ptr    = pu8RxString;             //Sets the buffer to transmit
    mSPI2Status    = mSpiReceivingData_c;
    mSpi2DataReg   = mDummyByte_c;           //Puts a dummy byte in the SPI data register

   #if(gSpi2Mode_d == gSpiInterrupt_d)   
    mSpi2EnableRxInterrupt();              //Enables de SPI interrupt
   #endif

    return gSpiErrNoError_c;

  }
  else{
  
    if(mSpiIdle_c == mSPI2Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }
  
  }  

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_ReadString(uint8_t *pu8RxString, uint8_t  u8StringLenght)
{
  (void)pu8RxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_ReadStringBlocking
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
) 
{

  if(mSpiPeripheralAsserted_c == mSPI2Status){

    mSPI2Status    = mSpiReceivingData_c;
    for(u8StringLenght; u8StringLenght > 0; u8StringLenght--){
       *pu8RxString++ = SPI2_ReadByteBlocking();
    }
    mSPI2Status    = mSpiPeripheralAsserted_c;
    return gSpiErrNoError_c;
  }
  else{

    if(mSpiIdle_c == mSPI2Status){
      return gSpiErrNoPinAsserted_c;
    }
    else{
      return gSpiErrTransferInProgress_c;
    }

  }  

}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_ReadStringBlocking(uint8_t *pu8RxString, uint8_t  u8StringLenght)
{
  (void)pu8RxString;
  (void)u8StringLenght;
  return gSpiErrNoError_c;
}

#endif


/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_WriteByte
(
uint8_t u8TxData                           //IN, Byte to be transmitted
)
{
  return SPI2_WriteString(&u8TxData, 1);
}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_WriteByte(uint8_t u8TxData)
{
  (void)u8TxData;
  return gSpiErrNoError_c;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
void SPI2_WriteByteBlocking
(
uint8_t u8TxData                           //IN, Byte to be transmitted
)
{
 (void)mSpi2StatusReg;                  // Clear any unattended Interrupt Flag 
  mSpi2DataReg = u8TxData;
  while (!(mSpi2TxFlag));
}
#endif
#else //FALSE == gSpiSupported_d

void SPI2_WriteByteBlocking(uint8_t u8TxData)
{
  (void)u8TxData;
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
spiErr_t SPI2_ReadByte
(
uint8_t * pu8RxData                        //IN, Address to put the readed byte
)
{
  return SPI2_ReadString(pu8RxData, 1);
}
#endif
#else //FALSE == gSpiSupported_d

spiErr_t SPI2_ReadByte(uint8_t * pu8RxData)
{
  (void)pu8RxData;
  return gSpiErrNoError_c; 
}

#endif

/***********************************************************************************/
#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
uint8_t SPI2_ReadByteBlocking
(
void                        //IN, Address to put the readed byte
)
{
 (void)mSpi2StatusReg;                  // Clear any unattended Interrupt Flag 
  mSpi2DataReg = mDummyByte_c;
  while (!(mSpi2RxFlag));
  return mSpi2DataReg;
}
#endif
#else //FALSE == gSpiSupported_d

uint8_t SPI2_ReadByteBlocking(void)
{
  return 0; 
}

#endif

#pragma CODE_SEG DEFAULT

/***********************************************************************************/

#if TRUE == gSpiSupported_d
#if gSpiUsePort2_d
#if (gSpi2Mode_d == gSpiInterrupt_d)

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

INTERRUPT_KEYWORD void SPI2_Isr(void)
{
#else
void SPI2_Task(void)
{
  if(mSpi2RxFlag || mSpi2TxFlag)
#endif 

  {

    uint8_t u8ReceivedByte;
    (void)mSpi2StatusReg;                  // First step to erase the Interrupt flags 
    u8ReceivedByte = mSpi2DataReg;         // Second step to erase the Interrupt flags 
    if (mSpiTransmittingData_c == mSPI2Status){

      if (0 == mu8SPI2Counter){

        mSPI2Status = mSpiPeripheralAsserted_c; 

       #if gSpi2Mode_d == gSpiInterrupt_d   
        mSpi2DisableTxInterrupt();          //Disables SPI interrupt
       #endif

        if(NULL != gpfSpi2CallBack){
          gpfSpi2CallBack();       
        }

      }
      else{
        mu8SPI2Counter--;                  //Decreases the counter
        mSpi2DataReg = *mpu8SPI2Ptr++;     // Transmit next byte 
      }

    }
    else if (mSpiReceivingData_c == mSPI2Status){

      *mpu8SPI2Ptr = u8ReceivedByte;       //Read SPI Data 
      if (0 == mu8SPI2Counter){

        mSPI2Status = mSpiPeripheralAsserted_c; 

       #if gSpi2Mode_d == gSpiInterrupt_d   
        mSpi2DisableRxInterrupt();          //Disables SPI interrupt
       #endif

        if(NULL != gpfSpi2CallBack){
          gpfSpi2CallBack();       
        }

      }
      else{
        mu8SPI2Counter--;                  //Decreases the counter
        mpu8SPI2Ptr++;                     
        mSpi2DataReg = mDummyByte_c;       //Puts a dummy byte in the SPI data register
      }

    }
    else{                                  //It isn't transmitting or receiving 
      
    }

  }

}

#pragma CODE_SEG DEFAULT

#endif //gSpiUsePort1_d
#endif //gSpiSupported_d


/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/
   
