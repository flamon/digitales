/************************************************************************************
* EEPROM driver implementation. 
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* Freescale Semiconductor Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/


#include "Eeprom.h"


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if TRUE == gEepromSupported_d                                          

static EepromStateMachine_t mEepromActualState = mEepromStateIdle_c;
static eepromCallback_t mpfEepromCallback = NULL;
static uint8_t*  mpu8Data;
static uint8_t	 mu8DataSize;
static uint8_t   mu8EepromAddress[8] = {0};

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
static uint8_t   mu8DeviceAddress;
#endif

#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
static uint16_t	 mu16EepromPageNumber = 0;
#endif

#endif //gEepromSupported_d

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
*  Eeprom_Init
*
************************************************************************************/

#if TRUE == gEepromSupported_d  

// Eeprom_Init function for Target Board 1320xQE128EVB, which has AT24C1024B eeprom  
#if gTargetBoard_c == gMc1320xS08qe128Evb_c 
 eepromErr_t  Eeprom_Init (void) 
 {
    uint8_t au8TestMemAddress[2] = {0x00, 0x00};
    uint8_t au8TestMemBuffer[5] = {0};
    iicErr_t SerialComError = gIicErrNoError_c;    
 
    // Check if I2C is previously initialized     
    /*
    if(FALSE == mEepromSerialComEnableRegister) 
    {  
      return gEepromErrSerialComNoInit_c; 
    }
    // Not implemented for this version. User must initilize before using this driver. 
    */
    
    mEepromActualState = mEepromStateIdle_c;
    SerialComError = IIC_WriteBlocking(mEepromAt24C1024bwI2cAddress_c,&au8TestMemAddress[0], 2); 
    SerialComError = IIC_ReadBlocking(mEepromAt24C1024bwI2cAddress_c,&au8TestMemBuffer[0], 5);
    SerialComError = IIC_StopTransaction(mEepromAt24C1024bwI2cAddress_c);

    if(gIicErrNoError_c != SerialComError) 
    {
      return gEepromErrNoEepromInBus_c;
    }
    
    return gEepromNoErr_c;
 
 }
#endif

// Eeprom_Init function for Target Board based on 1323xMRB, which has 
// AT45DB021D eeprom
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)    
 eepromErr_t  Eeprom_Init (void) 
 {
    uint8_t au8TestMemBuffer[8] = {0};
    uint8_t au8Pwr2PageSizeBuffer[] = {mEepromCmdPwrTwoPageSizeSeq_c};
    spiErr_t SerialComError = gSpiErrNoError_c;
  
    // Check if SPI is previously initialized     
    /*
    if(FALSE == mEepromSerialComEnableRegister) 
    {  
      return gEepromErrSerialComNoInit_c; 
    }
    // Not implemented for this version. User must initilize before using this driver.
    */
     
    mEepromActualState = mEepromStateIdle_c;

    au8TestMemBuffer[0] = mEepromCmdReadStatusReg_c; 
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);  
    SerialComError = SPI1_WriteStringBlocking(au8TestMemBuffer,1);
    SerialComError = SPI1_ReadStringBlocking(au8TestMemBuffer,1);
    SerialComError = SPI1_DeassertPeripheralSsPin();
    
    if(gSpiErrNoError_c != SerialComError) 
    {
      return gEepromErrNoEepromInBus_c;
    } 
    
    if(!(au8TestMemBuffer[0] & mEepromStatusBusyMask_c)) // Eeprom BUSY
    {   
      return gEepromErrBusBusy_c;
    }
    
    if(!(au8TestMemBuffer[0] & mEepromPageSize264Mask_c)) // if Eeprom Page Size = 264 
    { 
      // This will be done once per every Eeprom device, because this procedure is 
      // one-time programmable. After starting this procedure the Eeprom will be 
      // busy during 2 to 4 ms. This will program the Eeprom to work with 
      // Page size = 256 bytes    
      
      SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);
      SerialComError = SPI1_WriteStringBlocking(au8Pwr2PageSizeBuffer,4);
      SerialComError = SPI1_DeassertPeripheralSsPin();
    } 
  
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL); 
    au8TestMemBuffer[0] = mEepromCmdMainMemRead_c;
    SerialComError = SPI1_WriteStringBlocking(au8TestMemBuffer,8); 
    SerialComError = SPI1_ReadStringBlocking(au8TestMemBuffer,5);
    SerialComError = SPI1_DeassertPeripheralSsPin();
    
    if(gSpiErrNoError_c != SerialComError) 
    {  
      return gEepromErrNoEepromInBus_c;
    }  
 
    return gEepromNoErr_c;    
 }
#endif 

#else // gEepromSupported_d == FALSE

eepromErr_t  Eeprom_Init (void) 
 {
    return gEepromNoErr_c;
 }

#endif

 
/************************************************************************************
*
*  Eeprom_Write
*
************************************************************************************/

#if TRUE == gEepromSupported_d 

// Eeprom_Write function for Target Board 1320xQE128EVB, which uses I2C interface with
// the external eeprom   
#if gTargetBoard_c == gMc1320xS08qe128Evb_c 
eepromErr_t  Eeprom_Write (eepromDataPackage_t* pDataPackage)
{
    iicErr_t SerialComError = gIicErrNoError_c;    
    #if(TRUE == mEepromParameterValidation_d) 
    {
    if((mEepromMaxAddress_c < pDataPackage->EepromAddress.u32Address) || \
       (0 == pDataPackage->u8DataSize) || \
       (NULL == pDataPackage->pfEepromCallback))   
    {
      return gEepromErrInvalidParam_c;
    }  
    }
    #endif

    if(mEepromStateIdle_c != mEepromActualState) 
    {
    return gEepromErrBusBusy_c;
    }

    mpfEepromCallback = pDataPackage->pfEepromCallback;
    mpu8Data = pDataPackage->pu8Data;
    mu8DataSize = pDataPackage->u8DataSize; 


    if(mEepromAt24C1024bwPageOffset_c <= pDataPackage->EepromAddress.u32Address)
    {
    mu8DeviceAddress = (mEepromAt24C1024bwI2cAddress_c | mEepromAt24C1024bwPageMask_c) ; 
    mu8EepromAddress[0] = (uint8_t)((pDataPackage->EepromAddress.u32Address - mEepromAt24C1024bwPageOffset_c) >> 8);
    mu8EepromAddress[1] = (uint8_t)(pDataPackage->EepromAddress.u32Address - mEepromAt24C1024bwPageOffset_c);
    } 
    else
    {
    if(mEepromAt24C1024bwPageOffset_c <= (pDataPackage->EepromAddress.u32Address + mu8DataSize)) 
    {
       return gEepromErrAddrOverflow_c; 
    }

    mu8DeviceAddress = mEepromAt24C1024bwI2cAddress_c;
    }

    mEepromActualState = mEepromStateDeviceAddr_c;
    mu8EepromAddress[0] = (uint8_t)((pDataPackage->EepromAddress.u32Address) >> 8);
    mu8EepromAddress[1] = (uint8_t)(pDataPackage->EepromAddress.u32Address);

    SerialComError = IIC_Write (mu8DeviceAddress, \
                              mu8EepromAddress, \
                              2, EepromWrite_Callback); 

     if(gIicErrNoError_c != SerialComError) 
    {
      (void)IIC_StopTransaction (mu8DeviceAddress);
      mEepromActualState = mEepromStateIdle_c;
       return gEepromErrNoEepromInBus_c; 
    }
   
    return gEepromNoErr_c; 
}
#endif
 
// Eeprom_Write function for Target Board based on 1323xMRB, which uses SPI Interface 
// with the external eeprom
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c) 
eepromErr_t  Eeprom_Write (eepromDataPackage_t* pDataPackage)
 {
    spiErr_t SerialComError = gSpiErrNoError_c;
    uint8_t u8EepromStatusRegister = 0;
     
    
    #if(TRUE == mEepromParameterValidation_d) 
    {
    if((mEepromMaxAddress_c < pDataPackage->EepromAddress.u32Address) || \
       (0 == pDataPackage->u8DataSize) || \
       (NULL == pDataPackage->pfEepromCallback))   
    {
      return gEepromErrInvalidParam_c;
    }  
    }
    #endif
    
    if(mEepromStateIdle_c != mEepromActualState) 
    {
      return gEepromErrBusBusy_c;
    }
    
    u8EepromStatusRegister = mEepromCmdReadStatusReg_c;
    
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);  
    SerialComError = SPI1_WriteStringBlocking(&u8EepromStatusRegister,1);
    SerialComError = SPI1_ReadStringBlocking(&u8EepromStatusRegister,1);
    SerialComError = SPI1_DeassertPeripheralSsPin();
    
    if(gSpiErrNoError_c != SerialComError) 
    {
      return gEepromErrNoEepromInBus_c;
    } 
    
    if(!(u8EepromStatusRegister & mEepromStatusBusyMask_c)) // Eeprom BUSY
    {   
      return gEepromErrBusBusy_c;
    }
 
    mpfEepromCallback = pDataPackage->pfEepromCallback;
    mpu8Data = pDataPackage->pu8Data;
    mu8DataSize = pDataPackage->u8DataSize; 
   

    
    mu8EepromAddress[1] = (uint8_t)((pDataPackage->EepromAddress.u32Address >> 16) & 0x03);
    mu8EepromAddress[2] = (uint8_t) (pDataPackage->EepromAddress.u32Address >> 8);
    mu8EepromAddress[3] = (uint8_t) pDataPackage->EepromAddress.u32Address;
    
    mu16EepromPageNumber =  (uint16_t)((pDataPackage->EepromAddress.u32Address >> 8) & 0x03FF);
    
    if((mEepromAT45DB021DPageSize_c - mu8EepromAddress[3]) < mu8DataSize) 
    {
       return gEepromErrAddrOverflow_c;
    }
 
    mEepromActualState = mEepromStateWrMem2Buffer;
    mu8EepromAddress[0] = mEepromCmdMainMem2Buffer_c;
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);
    SerialComError = SPI1_WriteStringBlocking(mu8EepromAddress, 4);
    SerialComError = SPI1_DeassertPeripheralSsPin(); 
    
    if(gSpiErrNoError_c != SerialComError) 
    {
      return gEepromErrNoEepromInBus_c;
    } 
    
    
    do
    {  
      u8EepromStatusRegister = mEepromCmdReadStatusReg_c;
      SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);  
      SerialComError = SPI1_WriteStringBlocking(&u8EepromStatusRegister,1);
      SerialComError = SPI1_ReadStringBlocking(&u8EepromStatusRegister,1);
      SerialComError = SPI1_DeassertPeripheralSsPin();
      
      if(gSpiErrNoError_c != SerialComError) 
      {
        return gEepromErrNoEepromInBus_c;
      } 
    }while(!(u8EepromStatusRegister & mEepromStatusBusyMask_c));
/*
     */  
    // Delay??
    mu8EepromAddress[0] = mEepromCmdBufferWrite_c;
    mEepromActualState = mEepromStateDeviceAddr_c;
    SerialComError = SPI1_AssertPeripheralSsPin(1,EepromWrite_Callback);
    SerialComError = SPI1_WriteString(mu8EepromAddress, 4);
 
                        
    if(gSpiErrNoError_c != SerialComError) 
    {
      (void)SPI1_DeassertPeripheralSsPin();
      mEepromActualState = mEepromStateIdle_c;    
       return gEepromErrNoEepromInBus_c; 
    }
   
    return gEepromNoErr_c;   
 }
#endif 
   
#else // gEepromSupported_d == FALSE

eepromErr_t  Eeprom_Write (eepromDataPackage_t* pDataPackage) 
 {
    (void)pDataPackage;
    return gEepromNoErr_c;
 }

#endif                
 
 
 /************************************************************************************
*
*  Eeprom_Read
*
************************************************************************************/

#if TRUE == gEepromSupported_d 

// Eeprom_Read function for Target Board 1320xQE128EVB, which uses I2C interface with
// the external eeprom 
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
eepromErr_t Eeprom_Read (eepromDataPackage_t* pDataPackage)
 {    
    iicErr_t SerialComError; 
    
    #if(TRUE == mEepromParameterValidation_d) 
    {
    if((mEepromMaxAddress_c < pDataPackage->EepromAddress.u32Address) || \
       (0 == pDataPackage->u8DataSize) || \
       (NULL == pDataPackage->pfEepromCallback))   
    {
      return gEepromErrInvalidParam_c;
    }  
    }
    #endif
    
    if(mEepromStateIdle_c != mEepromActualState) 
    {
      return gEepromErrBusBusy_c;
    }
 
    mpfEepromCallback = pDataPackage->pfEepromCallback;
    mpu8Data = pDataPackage->pu8Data;
    mu8DataSize = pDataPackage->u8DataSize;

    if(mEepromAt24C1024bwPageOffset_c <= pDataPackage->EepromAddress.u32Address)
    {
      mu8DeviceAddress = (mEepromAt24C1024bwI2cAddress_c | mEepromAt24C1024bwPageMask_c) ; 
      mu8EepromAddress[0] = (uint8_t)((pDataPackage->EepromAddress.u32Address - mEepromAt24C1024bwPageOffset_c) >> 8);
      mu8EepromAddress[1] = (uint8_t)(pDataPackage->EepromAddress.u32Address - mEepromAt24C1024bwPageOffset_c);
    } 
    else
    {
      if(mEepromAt24C1024bwPageOffset_c <= (pDataPackage->EepromAddress.u32Address + mu8DataSize)) 
      {
         return gEepromErrAddrOverflow_c; 
      }
      
      mu8DeviceAddress = mEepromAt24C1024bwI2cAddress_c;
      mu8EepromAddress[0] = (uint8_t)((pDataPackage->EepromAddress.u32Address) >> 8);
      mu8EepromAddress[1] = (uint8_t)(pDataPackage->EepromAddress.u32Address);
    }
    
    
    mEepromActualState = mEepromStateDeviceAddr_c;
    
    SerialComError = IIC_Write (mu8DeviceAddress, \
                                mu8EepromAddress, \
                                2, EepromRead_Callback); 

    if(gIicErrNoError_c != SerialComError)
    {
       (void)IIC_StopTransaction (mu8DeviceAddress);
       mEepromActualState = mEepromStateIdle_c;
       return gEepromErrNoEepromInBus_c; 
    }
   
    return gEepromNoErr_c; 
     
 }
#endif
 
// Eeprom_Read function for Target Board based on 1323xMRB, which uses SPI Interface 
// with the external eeprom 
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
eepromErr_t Eeprom_Read (eepromDataPackage_t* pDataPackage) 
{
    spiErr_t SerialComError = gSpiErrNoError_c;
    uint8_t u8EepromStatusRegister = 0;
    
    
    #if(TRUE == mEepromParameterValidation_d) 
    {
    if((mEepromMaxAddress_c < pDataPackage->EepromAddress.u32Address) || \
       (0 == pDataPackage->u8DataSize) || \
       (NULL == pDataPackage->pfEepromCallback))   
    {
      return gEepromErrInvalidParam_c;
    }  
    }
    #endif
    
    if(mEepromStateIdle_c != mEepromActualState) 
    {
      return gEepromErrBusBusy_c;
    }
 
    u8EepromStatusRegister = mEepromCmdReadStatusReg_c;
    
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);  
    SerialComError = SPI1_WriteStringBlocking(&u8EepromStatusRegister,1);
    SerialComError = SPI1_ReadStringBlocking(&u8EepromStatusRegister,1);
    SerialComError = SPI1_DeassertPeripheralSsPin();
    
    if(gSpiErrNoError_c != SerialComError) 
    {
      return gEepromErrNoEepromInBus_c;
    } 
    
    if(!(u8EepromStatusRegister & mEepromStatusBusyMask_c)) // Eeprom BUSY
    {   
      return gEepromErrBusBusy_c;
    }
 
    mpfEepromCallback = pDataPackage->pfEepromCallback;
    mpu8Data = pDataPackage->pu8Data;
    mu8DataSize = pDataPackage->u8DataSize; 
    
  
    mu8EepromAddress[0] = mEepromCmdMainMemRead_c;
    mu8EepromAddress[1] = (uint8_t)((pDataPackage->EepromAddress.u32Address >> 16) & 0x03);
    mu8EepromAddress[2] = (uint8_t) (pDataPackage->EepromAddress.u32Address >> 8);
    mu8EepromAddress[3] = (uint8_t) pDataPackage->EepromAddress.u32Address;
    
    if((mEepromAT45DB021DPageSize_c - mu8EepromAddress[3]) < mu8DataSize) 
    {
       return gEepromErrAddrOverflow_c;
    }
    
    mEepromActualState = mEepromStateDeviceAddr_c;
    SerialComError = SPI1_AssertPeripheralSsPin(1,EepromRead_Callback);
    SerialComError = SPI1_WriteString(mu8EepromAddress, 8);
      
                
    if(gSpiErrNoError_c != SerialComError)  
    {
       (void)SPI1_DeassertPeripheralSsPin();
       mEepromActualState = mEepromStateIdle_c;
       return gEepromErrNoEepromInBus_c; 
    }
   
    return gEepromNoErr_c; 
 }
 #endif
 
#else // gEepromSupported_d == FALSE

eepromErr_t  Eeprom_Read (eepromDataPackage_t* pDataPackage) 
 {
    (void)pDataPackage;
    return gEepromNoErr_c;
 }

#endif  
 
 /************************************************************************************
*
*  Eeprom_Erase
*
************************************************************************************/
#if TRUE == gEepromSupported_d 

// Eeprom_Erase function for Target Board 1320xQE128EVB
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
eepromErr_t  Eeprom_Erase (void) 
 {
    /* Eeprom At24C1024bwI2c does not require to erase before writting.
       Erase procedure may be done by writting 0xFF data in the memory.
       The user may use Eeprom_Write to achieve this. */
    return gEepromErrNotSupported_c;
 }
 #endif


// Eeprom_Erase function for Target Board based on 1323xMRB
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
eepromErr_t  Eeprom_Erase (void) 
 {
    uint8_t au8EraseMemBuffer[] = {mEepromCmdChipEraseSeq_c};
    spiErr_t SerialComError = gSpiErrNoError_c;
    
    /* After sending the erase chip sequence, the Eeprom will be busy during
       3.6 to 6 seconds. The user must have this in mind because any other Eeprom
       API will return BusBusy error while the Eeprom finishes the erase procedure.*/
    
    SerialComError = SPI1_AssertPeripheralSsPin(1,NULL);
    SerialComError = SPI1_WriteStringBlocking(au8EraseMemBuffer,4);  
    SerialComError = SPI1_DeassertPeripheralSsPin();
    
    if(gSpiErrNoError_c != SerialComError)  
    {
       return gEepromErrNoEepromInBus_c; 
    }
    
    return gEepromNoErr_c; 
    
 }
 
 #endif 

#else // gEepromSupported_d == FALSE

eepromErr_t  Eeprom_Erase (void)  
 {
    return gEepromNoErr_c;
 }

#endif



/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/

#if TRUE == gEepromSupported_d

  #ifdef MEMORY_MODEL_BANKED
    #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
    #pragma CODE_SEG DEFAULT
  #endif

 /************************************************************************************
*
*  EepromWrite_IicCallback
*
************************************************************************************/

// Callback function for Target Board 1320xQE128EVB
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
void  EepromWrite_Callback (iicResult_t IicWriteResult) 
{
    iicErr_t SerialComError = gIicErrNoError_c;
    bool_t  fBusBusyErr = 0;

    if (gIicTxRxSuccess_c == IicWriteResult) 
    {
       switch (mEepromActualState) 
       {
        case mEepromStateDeviceAddr_c:
            // call to Write data
             SerialComError = IIC_Write (mu8DeviceAddress, \
                                         mpu8Data, mu8DataSize, EepromWrite_Callback);
             mEepromActualState = mEepromStateBusy_c; 
        break;
        case mEepromStateBusy_c:
             SerialComError = IIC_StopTransaction (mu8DeviceAddress);
             if(gIicErrNoError_c == SerialComError) 
             { 
               mpfEepromCallback(gEepromNoErr_c); 
             }  
             mEepromActualState = mEepromStateIdle_c;             
        break;

        case mEepromStateIdle_c:
        case mEepromStateMax_c:
              // STATE MACHINE ERROR
        break;
       }
    } 
    else
    {
         fBusBusyErr = 1;
         mEepromActualState = mEepromStateIdle_c;
    } 
    
    if(gIicErrNoError_c != SerialComError)  
    {
        // EepromCallback with EepromNotInBus error as parameter 
        fBusBusyErr = 1;
    }
    
    if(fBusBusyErr) 
    {  
      SerialComError = IIC_StopTransaction (mu8DeviceAddress);
      mpfEepromCallback(gEepromErrBusBusy_c);
      mEepromActualState = mEepromStateIdle_c;
    }  

}
#endif

// Callback function for Target Board 1323xMRB
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
void  EepromWrite_Callback (void)
 {  
    spiErr_t SerialComError = gSpiErrNoError_c;

    bool_t  fBusBusyErr = 0;

    switch (mEepromActualState) 
    {
      case mEepromStateWrMem2Buffer:
            /*
           mu8EepromAddress[0] = mEepromCmdBufferWrite_c;
           mEepromActualState = mEepromStateDeviceAddr_c;
           SerialComError = SPI1_AssertPeripheralSsPin(1,EepromWrite_Callback);
           SerialComError = SPI1_WriteString(mu8EepromAddress, 4);*/
      break;
      case mEepromStateDeviceAddr_c:
          // call to Write data      
           SerialComError = SPI1_WriteString(mpu8Data, mu8DataSize);
           mEepromActualState = mEepromStateBusy_c; 
      break;
      case mEepromStateBusy_c:
    
           SerialComError = SPI1_DeassertPeripheralSsPin();
           
           SerialComError = SPI1_AssertPeripheralSsPin(1,EepromWrite_Callback);
           
           mu8EepromAddress[0] = mEepromCmdMainMemWrite_c;
           mu8EepromAddress[1] = (uint8_t)((mu16EepromPageNumber >> 8) & 0x03);
           mu8EepromAddress[2] = (uint8_t)mu16EepromPageNumber;
           
           SerialComError = SPI1_WriteString(mu8EepromAddress,4);
                         
           if(gSpiErrNoError_c != SerialComError)  
           { 
              fBusBusyErr = 1; 
           }
           
           mEepromActualState = mEepromStateWrBuffer_c;
      break;
      case mEepromStateWrBuffer_c:
      
           SerialComError = SPI1_DeassertPeripheralSsPin();
           if(gSpiErrNoError_c == SerialComError)  
           { 
             mpfEepromCallback(gEepromNoErr_c); 
           }  
           mEepromActualState = mEepromStateIdle_c;
      break;

      case mEepromStateIdle_c:
      case mEepromStateMax_c:
            // STATE MACHINE ERROR
      break;
    }      
  
    if(gSpiErrNoError_c != SerialComError)  
    {
        // EepromCallback with EepromNotInBus error as parameter 
        fBusBusyErr = 1;
    }
    if(fBusBusyErr) 
    {  
      SerialComError = SPI1_DeassertPeripheralSsPin();
      mpfEepromCallback(gEepromErrBusBusy_c);
      mEepromActualState = mEepromStateIdle_c;
    }   
      
 }
 #endif
 
  /************************************************************************************
*
*  EepromRead_IicCallback
*
************************************************************************************/

// Callback function for Target Board 1320xQE128EVB
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
void  EepromRead_Callback (iicResult_t IicReadResult) 
{

    iicErr_t SerialComError = gIicErrNoError_c;    
    bool_t  fBusBusyErr = 0;

    if (gIicTxRxSuccess_c == IicReadResult) 
    {
     switch (mEepromActualState) 
     {                       
       
       case mEepromStateDeviceAddr_c:
          // call to Read data
           SerialComError = IIC_Read (mu8DeviceAddress, \
                                       mpu8Data, mu8DataSize, EepromRead_Callback);
           mEepromActualState = mEepromStateBusy_c; 
      break;
      case mEepromStateBusy_c:
           SerialComError = IIC_StopTransaction (mu8DeviceAddress);
           if(gIicErrNoError_c == SerialComError) 
           { 
             mpfEepromCallback(gEepromNoErr_c); 
           } 
           mEepromActualState = mEepromStateIdle_c; 
      break;
      
      case mEepromStateIdle_c:
      case mEepromStateMax_c:
            // STATE MACHINE ERROR 
      break;
     }
     
    } 
    else
    {
       fBusBusyErr = 1;
       mEepromActualState = mEepromStateIdle_c;
    }

    if(gIicErrNoError_c != SerialComError) 
    { 
        fBusBusyErr = 1;
    }

    if(fBusBusyErr) 
    {  
      SerialComError = IIC_StopTransaction (mu8DeviceAddress);
      mpfEepromCallback(gEepromErrBusBusy_c);
      mEepromActualState = mEepromStateIdle_c;
    } 
     
}
#endif

// Callback function for Target Board 1323xMRB
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
void  EepromRead_Callback (void)
 {    

    spiErr_t SerialComError = gSpiErrNoError_c;

    bool_t  fBusBusyErr = 0;


    switch (mEepromActualState) 
    {
    case mEepromStateDeviceAddr_c:
        // call to Write data      
         SerialComError = SPI1_ReadString(mpu8Data, mu8DataSize);
         mEepromActualState = mEepromStateBusy_c; 
    break;
    case mEepromStateBusy_c:
         SerialComError = SPI1_DeassertPeripheralSsPin();
         if(gSpiErrNoError_c == SerialComError) 
         { 
           mpfEepromCallback(gEepromNoErr_c); 
         }
         mEepromActualState = mEepromStateIdle_c;  
    break;

    case mEepromStateIdle_c:
    case mEepromStateMax_c:
          // STATE MACHINE ERROR
    break;
    }  
    
    if(gSpiErrNoError_c != SerialComError) 
    { 
        fBusBusyErr = 1;
    }

    if(fBusBusyErr) 
    {
      SerialComError = SPI1_DeassertPeripheralSsPin();
      mpfEepromCallback(gEepromErrBusBusy_c);
      mEepromActualState = mEepromStateIdle_c;
    }  
    
 }
#endif 

#pragma CODE_SEG DEFAULT 

#endif // gEepromSupported_d

/************************************************************************************
*************************************************************************************
* Private Debug stuff
*************************************************************************************
************************************************************************************/



/* End of EEPROM.c */
