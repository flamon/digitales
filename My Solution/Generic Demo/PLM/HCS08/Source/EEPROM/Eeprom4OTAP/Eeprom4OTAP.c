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

#include "Eeprom4OTAP.h"
#include "SMAC_config.h"

#if(TRUE == gOtapSupported_d)



/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
                                          

static uint8_t   mu8EepromAddress[8] = {0};
static uint8_t  mgTemporal = 0;

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
static uint8_t   mu8DeviceAddress;
#endif

#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
static uint16_t	 mu16EepromPageNumber = 0;
#endif

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/
#pragma CODE_SEG OFLC_ROUTINES


/************************************************************************************
*
*  Eeprom4OTAP_Init
*
************************************************************************************/

// Eeprom_Init function for Target Board 1320xQE128EVB, which has AT24C1024B eeprom  
#if gTargetBoard_c == gMc1320xS08qe128Evb_c 
 eepromOTAPErr_t  Eeprom4OTAP_Init (void) 
 {
    uint8_t au8TestMemAddress[2] = {0x00, 0x00};
    uint8_t au8TestMemBuffer[5] = {0}; 
    
    //Init PTC5 pin port as output to drive the write protection signal
    PTCDD_PTCDD5 = 1;
    
    // Init IIC
    IIC1C |= (0x80);
    IIC1F = ((0x00<<6)|0x18);   // mIic100KhzBaudInitParameters_c = gIicMul_1_c, gIicICR_18_c 
      
    (void)Iic4OTAP_Write(mEepromOTAPAt24C1024bwI2cAddress_c,&au8TestMemAddress[0], 2, TRUE); 
    (void)Iic4OTAP_Read(mEepromOTAPAt24C1024bwI2cAddress_c,&au8TestMemBuffer[0], 5, TRUE);
    Iic4OTAP_StopTransaction(); 

    return gEepromOTAPNoErr_c;
 
 }
#endif

// Eeprom_Init function for Target Board based on 1323xMRB, which has 
// AT45DB021D eeprom
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)    
 eepromOTAPErr_t  Eeprom4OTAP_Init (void) 
 {
    uint8_t au8TestMemBuffer[8] = {0};
    uint8_t au8Pwr2PageSizeBuffer[] = {mEepromOTAPCmdPwrTwoPageSizeSeq_c};
  
    //Init PTC5 pin port as output to drive the SPI SS signal
    PTCDD_PTCDD5 = 1;
  
    Spi4OTAP_Init ();
 
    mSPI1OTAP_AssertPeripheralSsPin();   
    // SPI Write the Status Register number.
    au8TestMemBuffer[0] = mEepromOTAPCmdReadStatusReg_c;
    Spi4OTAP_Write(au8TestMemBuffer, 1);
    // Read the Status Register
    Spi4OTAP_Read(au8TestMemBuffer, 1);
    mSPI1OTAP_DeassertPeripheralSsPin();
 
    // Check if Eeprom Busy
    if(!(au8TestMemBuffer[0] & mEepromOTAPStatusBusyMask_c)) 
    {   
      return gEepromOTAPErrBusBusy_c;
    }
    
    if(!(au8TestMemBuffer[0] & mEepromOTAPPageSize264Mask_c)) // if Eeprom Page Size = 264 
    { 
      // This will be done once per every Eeprom device, because this procedure is 
      // one-time programmable. After starting this procedure the Eeprom will be 
      // busy during 2 to 4 ms. This will program the Eeprom to work with 
      // Page size = 256 bytes    
      
      mSPI1OTAP_AssertPeripheralSsPin(); 
      Spi4OTAP_Write(au8Pwr2PageSizeBuffer, 4);
      mSPI1OTAP_DeassertPeripheralSsPin();
    } 
  
    mSPI1OTAP_AssertPeripheralSsPin(); 
    au8TestMemBuffer[0] = mEepromOTAPCmdMainMemRead_c;  
    Spi4OTAP_Write(au8TestMemBuffer, 8); 
    Spi4OTAP_Read(au8TestMemBuffer, 5);
    mSPI1OTAP_DeassertPeripheralSsPin();
    
    return gEepromOTAPNoErr_c;    
 }
#endif 

 
/************************************************************************************
*
*  Eeprom4OTAP_Write
*
************************************************************************************/

// Eeprom_Write function for Target Board 1320xQE128EVB, which uses I2C interface with
// the external eeprom   
#if gTargetBoard_c == gMc1320xS08qe128Evb_c 
eepromOTAPErr_t  Eeprom4OTAP_Write (uint32_t EepromAddress, uint8_t* pu8Data, uint8_t u8DataSize)
{

    if(mEepromOTAPAt24C1024bwPageOffset_c <= EepromAddress)
    {
    mu8DeviceAddress = (mEepromOTAPAt24C1024bwI2cAddress_c | mEepromOTAPAt24C1024bwPageMask_c) ; 
    mu8EepromAddress[0] = (uint8_t)((EepromAddress - mEepromOTAPAt24C1024bwPageOffset_c) >> 8);
    mu8EepromAddress[1] = (uint8_t)(EepromAddress - mEepromOTAPAt24C1024bwPageOffset_c);
    } 
    else
    {
    if(mEepromOTAPAt24C1024bwPageOffset_c <= (EepromAddress + u8DataSize)) 
    {
       return gEepromOTAPErrAddrOverflow_c; 
    }

    mu8DeviceAddress = mEepromOTAPAt24C1024bwI2cAddress_c;
    
    mu8EepromAddress[0] = (uint8_t)((EepromAddress) >> 8);
    mu8EepromAddress[1] = (uint8_t)(EepromAddress);
    }
    
    (void)Iic4OTAP_Write (mu8DeviceAddress,mu8EepromAddress,2, TRUE); 

    (void)Iic4OTAP_Write (mu8DeviceAddress, pu8Data, u8DataSize, FALSE);
                                         
    Iic4OTAP_StopTransaction();
    
    return gEepromOTAPNoErr_c; 
}
#endif
 
// Eeprom_Write function for Target Board based on 1323xMRB, which uses SPI Interface 
// with the external eeprom
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c) 
eepromOTAPErr_t  Eeprom4OTAP_Write (uint32_t EepromAddress, uint8_t* pu8Data, uint8_t u8DataSize)
 {
    uint8_t u8TempRxBuffer = 0;
    uint8_t u8TempCounter = 0;
   
    mu8EepromAddress[1] = (uint8_t)((EepromAddress >> 16) & 0x03);
    mu8EepromAddress[2] = (uint8_t) (EepromAddress >> 8);
    mu8EepromAddress[3] = (uint8_t) EepromAddress;
    
    mu16EepromPageNumber =  (uint16_t)((EepromAddress >> 8) & 0x03FF);
    
    if((mEepromOTAPAT45DB021DPageSize_c - mu8EepromAddress[3]) < u8DataSize) 
    {
       return gEepromOTAPErrAddrOverflow_c;
    }
 
    //-------- Copy Page to Buffer from main memory -------
    mu8EepromAddress[0] = mEepromOTAPCmdMainMem2Buffer_c;
    mSPI1OTAP_AssertPeripheralSsPin();   
    // SPI Write the Status Register number.  
    Spi4OTAP_Write (mu8EepromAddress, 4); 
    mSPI1OTAP_DeassertPeripheralSsPin();
      
    //------------------------------------------------------
    
    //------ Check if Eeprom busy & wait until Free --------
    do
    {   
      mSPI1OTAP_AssertPeripheralSsPin();
      u8TempRxBuffer = mEepromOTAPCmdReadStatusReg_c;   
      // SPI Write the Status Register number.
      Spi4OTAP_Write (&u8TempRxBuffer, 1);
      // Read the Status Register
      Spi4OTAP_Read (&u8TempRxBuffer, 1);
      mSPI1OTAP_DeassertPeripheralSsPin();
 
    }while(!(u8TempRxBuffer & mEepromOTAPStatusBusyMask_c));
    //------------------------------------------------------
    
    //------------------ Write to buffer --------------------
      mu8EepromAddress[0] = mEepromOTAPCmdBufferWrite_c;
      mSPI1OTAP_AssertPeripheralSsPin();
      Spi4OTAP_Write (mu8EepromAddress, 4);  
      Spi4OTAP_Write (pu8Data, u8DataSize); 
      mSPI1OTAP_DeassertPeripheralSsPin();
    //------------------------------------------------------
    
    //---------- Write to Main mem from buffer -------------
    mu8EepromAddress[0] = mEepromOTAPCmdMainMemWrite_c;
    mu8EepromAddress[1] = (uint8_t)((mu16EepromPageNumber >> 8) & 0x03);
    mu8EepromAddress[2] = (uint8_t)mu16EepromPageNumber;

    mSPI1OTAP_AssertPeripheralSsPin();   
    Spi4OTAP_Write (mu8EepromAddress, 4);
    mSPI1OTAP_DeassertPeripheralSsPin();
    //------------------------------------------------------
    
    return gEepromOTAPNoErr_c;   
 }
#endif 
   
                
 
 
 /************************************************************************************
*
*  Eeprom4OTAP_Read
*
************************************************************************************/

// Eeprom_Read function for Target Board 1320xQE128EVB, which uses I2C interface with
// the external eeprom 
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
eepromOTAPErr_t Eeprom4OTAP_Read (uint32_t EepromAddress, uint8_t* pu8Data, uint8_t u8DataSize)
 {    
    

    if(mEepromOTAPAt24C1024bwPageOffset_c <= EepromAddress)
    {
      mu8DeviceAddress = (mEepromOTAPAt24C1024bwI2cAddress_c | mEepromOTAPAt24C1024bwPageMask_c) ; 
      mu8EepromAddress[0] = (uint8_t)((EepromAddress - mEepromOTAPAt24C1024bwPageOffset_c) >> 8);
      mu8EepromAddress[1] = (uint8_t)(EepromAddress - mEepromOTAPAt24C1024bwPageOffset_c);
    } 
    else
    {
      if(mEepromOTAPAt24C1024bwPageOffset_c <= (EepromAddress + u8DataSize)) 
      {
         return gEepromOTAPErrAddrOverflow_c; 
      }
      
      mu8DeviceAddress = mEepromOTAPAt24C1024bwI2cAddress_c;
      mu8EepromAddress[0] = (uint8_t)((EepromAddress) >> 8);
      mu8EepromAddress[1] = (uint8_t)(EepromAddress);
    }
    
    
    (void)Iic4OTAP_Write (mu8DeviceAddress,mu8EepromAddress,2, TRUE);

    (void)Iic4OTAP_Read (mu8DeviceAddress, pu8Data, u8DataSize, TRUE);
    Iic4OTAP_StopTransaction();

    return gEepromOTAPNoErr_c; 
     
 }
#endif
 
// Eeprom_Read function for Target Board based on 1323xMRB, which uses SPI Interface 
// with the external eeprom 
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
eepromOTAPErr_t Eeprom4OTAP_Read (uint32_t EepromAddress, uint8_t* pu8Data, uint8_t u8DataSize)
{

    mu8EepromAddress[0] = mEepromOTAPCmdMainMemRead_c;
    mu8EepromAddress[1] = (uint8_t)((EepromAddress >> 16) & 0x03);
    mu8EepromAddress[2] = (uint8_t) (EepromAddress >> 8);
    mu8EepromAddress[3] = (uint8_t) EepromAddress;
    
    if((mEepromOTAPAT45DB021DPageSize_c - mu8EepromAddress[3]) < u8DataSize) 
    {
       return gEepromOTAPErrAddrOverflow_c;
    }
    
    mSPI1OTAP_AssertPeripheralSsPin(); 
    Spi4OTAP_Write (mu8EepromAddress, 8); 
    Spi4OTAP_Read (pu8Data, u8DataSize);
    mSPI1OTAP_DeassertPeripheralSsPin();

   
    return gEepromOTAPNoErr_c; 
 }
 #endif
 

 
 /************************************************************************************
*
*  Eeprom4OTAP_Erase
*
************************************************************************************/

// Eeprom_Erase function for Target Board 1320xQE128EVB
#if gTargetBoard_c == gMc1320xS08qe128Evb_c
eepromOTAPErr_t  Eeprom4OTAP_Erase (void) 
 {
    /* Eeprom At24C1024bwI2c does not require to erase before writting.
       Erase procedure may be done by writting 0xFF data in the memory.
       The user may use Eeprom_Write to achieve this. */
    return gEepromOTAPErrNotSupported_c;
 }
 #endif


// Eeprom_Erase function for Target Board based on 1323xMRB
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
eepromOTAPErr_t  Eeprom4OTAP_Erase (void) 
 {
    uint8_t au8EraseMemBuffer[] = {mEepromOTAPCmdChipEraseSeq_c};
    uint8_t u8TempRxBuffer = 0;
    
    /* After sending the erase chip sequence, the Eeprom will be busy during
       3.6 to 6 seconds. The user must have this in mind because any other Eeprom
       API will return BusBusy error while the Eeprom finishes the erase procedure.*/  
     
    mSPI1OTAP_AssertPeripheralSsPin(); 
    Spi4OTAP_Write (au8EraseMemBuffer, 4);
    mSPI1OTAP_DeassertPeripheralSsPin();
 
    //------ Check if Eeprom busy & wait until Free --------
    do
    {  
      mSPI1OTAP_AssertPeripheralSsPin();   
      u8TempRxBuffer = mEepromOTAPCmdReadStatusReg_c;
      // SPI Write the Status Register number.
      Spi4OTAP_Write (&u8TempRxBuffer, 4);
      // Read the Status Register
      Spi4OTAP_Read (&u8TempRxBuffer, 1);
      mSPI1OTAP_DeassertPeripheralSsPin();
    }while(!(u8TempRxBuffer & mEepromOTAPStatusBusyMask_c));
    //------------------------------------------------------
     
    return gEepromOTAPNoErr_c; 
    
 }
 
 #endif 

/************************************************************************************
*************************************************************************************
* Private Functions
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1320xS08qe128Evb_c

 /************************************************************************************
*
*  Iic4OTAP_Write
*
************************************************************************************/

void Iic4OTAP_Write (uint8_t u8SlaveAddress, uint8_t* pu8TxString, uint8_t u8StringLenght, bool_t fStartTx)
{


if(fStartTx)
  {  
    IIC1C |= (0x20|0x10);  
    IIC1D = ((u8SlaveAddress<<1) & 0xFE);                                       
    while(!(0x02 & IIC1S));             
    IIC1S |= 0x02;                       
  }
  
  while(u8StringLenght--)
  {
    //Delay4OTAP_us(20);
    //iicSendNextByte(*pu8TxString++);
    IIC1D = *pu8TxString++;                                       
    while(!(0x02 & IIC1S));             
    IIC1S |= 0x02;     
  }  
 
  
}


 /************************************************************************************
*
*  Iic4OTAP_Read
*
************************************************************************************/

void Iic4OTAP_Read (uint8_t u8SlaveAddress, uint8_t* pu8RxString, uint8_t u8StringLenght, bool_t fStartTx)
{
 
  if(fStartTx)
  {
    IIC1C |= (0x04);
  }
  else
  {
    IIC1C |= (0x20|0x10);
  }


  IIC1D = ((u8SlaveAddress<<1) | 0x01);                                       
  while(!(0x02 & IIC1S));             
  IIC1S |= 0x02;  
  
  if(1 == u8StringLenght) IIC1C |= 0x08;
  else IIC1C &= ~0x08;
  IIC1C &= ~0x10;
  (void)IIC1D;   /*Starts the first reception*/
  
  while(u8StringLenght)
  {
    //Delay4OTAP_us(20);
    while(!(0x02 & IIC1S));              
    IIC1S |= 0x02; 
    u8StringLenght--;                      
    if(1 == u8StringLenght) IIC1C |= 0x08;
    if(0 == u8StringLenght) IIC1C |= 0x10;
    *pu8RxString++ = IIC1D;
  }

}



#endif



#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)

 /************************************************************************************
*
*  Spi4OTAP_Write
*
************************************************************************************/

void Spi4OTAP_Write (uint8_t* pu8Data, uint8_t u8Size)
{
  uint8_t u8TempCounter; 
  
  for(u8TempCounter = 0; u8TempCounter < u8Size; u8TempCounter++)
  {
    (void)SPI1S;          
    SPI1D = *pu8Data++;
    while (!(SPI1S_SPTEF));
  }
  
}

 /************************************************************************************
*
*  Spi4OTAP_Read
*
************************************************************************************/

void Spi4OTAP_Read (uint8_t* pu8Data, uint8_t u8Size)
{
  uint8_t u8TempCounter; 
  
  for(u8TempCounter = 0; u8TempCounter < u8Size; u8TempCounter++)
  {
    (void)SPI1S;             
    SPI1D = mOTAPDummyByte_c;
    while (!(SPI1S_SPRF));
    *pu8Data++ = SPI1D;
  }
  
}

 /************************************************************************************
*
*  Spi4OTAP_Init
*
************************************************************************************/

void Spi4OTAP_Init (void)
{
  
 /*Init the SS pins in disable mode*/
  mSPI1OTAP_DeassertPeripheralSsPin();  

  (void)SPI1S;                // Clear any Interrupt Flag 
  SPI1BR    = 0x00;           // Baud Rate Divisor = 2
  SPI1C1       = 0b01010000; 
  SPI1C2       = 0;           // SS as GPIO,SPI use separate pins 

}

#endif

 /************************************************************************************
*
*  Delay4OTAP_us
*
************************************************************************************/

void Delay4OTAP_us
(
  uint16_t u16us
) 
{
  uint16_t u16ctr;
  uint16_t u16Temp;
                    
  u16Temp = (uint16_t)((((uint32_t)(gSystemClock_d*10)/40000000)*u16us)/10);                  
                                            
  for(u16ctr = 0; u16ctr < u16Temp; u16ctr++){
    asm NOP; 
    asm NOP;     
  }      
}
#pragma CODE_SEG DEFAULT
#endif /*(TRUE == gOtapSupported_d)*/

/* End of EEPROM4OTAP.c */
