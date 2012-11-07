/*****************************************************************************
* SPI Interface header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __SPI_INTERFACE_H__               
  #define __SPI_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "GPIO_interface.h"
#include "PLM_Config.h"
#include "PortConfig.h"


/************************************************************************************
*************************************************************************************
* Interface macro definitions
*************************************************************************************
************************************************************************************/
#ifndef gSpiSupported_d
#define gSpiSupported_d FALSE
#endif

#define gSpiPoll_d       gPolling_c
#define gSpiInterrupt_d  gInterruption_c

/*Defines to enable or disable the SPI */
#define gSpiUsePort1_d TRUE
#define gSpiUsePort2_d FALSE

#ifdef gMc1323xPlatform_d
 #undef  gSpiUsePort1_d
 #define gSpiUsePort1_d  TRUE
 #undef  gSpiUsePort2_d
 #define gSpiUsePort2_d  FALSE
#endif

#define gSpi1Mode_d         gInterruption_c
#define gSpi2Mode_d         gInterruption_c

#define mSpiIntEnable_c       TRUE
#define mSpiIntDisable_c      FALSE


/*SPI 1 Config*/
#if gSpiUsePort1_d

  #define gSpi1PeripheralsOnTheBus_c  2 

  /*SPI1 Peripheral 1 SS pin*/
  #define gSpi1UsePeripheral1_d        gUsePeripheral1_c  //set FALSE to non to use it
  #define gSpi1Peripheral1SsPort_c     gPeripheral1Port_c
  #define gSpi1Peripheral1SsPinMask_c  gPeripheral1PinMask_c
  /*SPI1 Peripheral 2 SS pin*/
  #define gSpi1UsePeripheral2_d        gUsePeripheral2_c //set FALSE to non to use it
  #define gSpi1Peripheral2SsPort_c     gPeripheral2Port_c
  #define gSpi1Peripheral2SsPinMask_c  gPeripheral2PinMask_c



  //#define gSpi1Mode_d gSpiInterrupt_d //can be gSpiPoll_c or
                                      //gSpiInterrupt_c

  #define SPI1_SetCallback(NewCallback)  gpfSpi1CallBack = NewCallback
  #define mSpi1RxIenBit        SPI1C1_SPIE
  #define mSpi1TxIenBit        SPI1C1_SPTIE
#endif /*gSpiUsePort1_d*/
                                   
                                   
                                   
/*SPI 2 Config*/
#if gSpiUsePort2_d

  #define gSpi2PeripheralsOnTheBus_c  1

  /*SPI2 Peripheral 1 SS pin*/
  #define gSpi2UsePeripheral1_d        FALSE //set FALSE to non to use it
  #define gSpi2Peripheral1SsPort_c     
  #define gSpi2Peripheral1SsPinMask_c  
  /*SPI2 Peripheral 2 SS pin*/
  #define gSpi2UsePeripheral2_d        FALSE //set FALSE to non to use it
  #define gSpi2Peripheral2SsPort_c     
  #define gSpi2Peripheral2SsPinMask_c  



  //#define gSpi2Mode_d gSpiInterrupt_d //can be gSpiPoll_d or
                                      //gSpiInterrupt_d

  #define SPI2_SetCallback(NewCallback)  gpfSpi2CallBack = NewCallback                                   
  
  #define mSpi2RxIenBit        (SPI2C1_SPIE)
  #define mSpi2TxIenBit        (SPI2C1_SPTIE)

#endif /*gSpiUsePort2_d*/

#define mSpiEnableRxInterrupt(port) mSpi##port##RxIenBit = mSpiIntEnable_c
#define mSpiDisableRxInterrupt(port) mSpi##port##RxIenBit = mSpiIntDisable_c

#define mSpiEnableTxInterrupt(port) mSpi##port##TxIenBit = mSpiIntEnable_c
#define mSpiDisableTxInterrupt(port) mSpi##port##TxIenBit = mSpiIntDisable_c


#if gSpiUsePort1_d

#define mSpi1EnableRxInterrupt()  mSpiEnableRxInterrupt(1)
#define mSpi1DisableRxInterrupt()  mSpiDisableRxInterrupt(1)

#define mSpi1EnableTxInterrupt()  mSpiEnableTxInterrupt(1)
#define mSpi1DisableTxInterrupt()  mSpiDisableTxInterrupt(1)

#endif /*gSpiUsePort1_d*/
 
#if gSpiUsePort2_d

#define mSpi2EnableRxInterrupt()  mSpiEnableRxInterrupt(2)
#define mSpi2DisableRxInterrupt()  mSpiDisableRxInterrupt(2)

#define mSpi2EnableTxInterrupt()  mSpiEnableTxInterrupt(2)
#define mSpi2DisableTxInterrupt()  mSpiDisableTxInterrupt(2)

#endif /*gSpiUsePort2_d*/


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/
typedef enum spiResult_tag {
    gSpiTxSuccess_c = 0,                 
    gSpiRxSuccess_c,
    gSpiRxNoData_c,	
    gSpiMaxResult_c
} spiResult_t;

//typedef void(*spiCallback_t)(void);
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near spiCallback_t)(void);
#else
  typedef void(* __near spiCallback_t)(void);
#endif


typedef enum spiErr_tag {
  gSpiErrNoError_c = 0,           //No error
  gSpiErrNullPointer_c,           //Passed Pointer is Null
  gSpiErrNoCallback_c,            //Passed Callback's pointer is Null
  gSpiErrInvalidParameter_c,      //Passed parameter out of range
  gSpiErrTransferInProgress_c,    //Module Transmitting or Receiving
  gSpiErrModuleBusy_c,            //Peripheral previously asserted
  gSpiErrNoPinAsserted_c,         //No peripheral previously asserted
  gSpiErrMax_c
} spiErr_t;


/*The Divisor to use depends on the desired BaudRate:
Divisor = BusClock / Desired Baudrate

For instance if the busclock is 8Mhz and the desired baudrate is 4Mhz
then:  

Divisor = 8Mhz / 4Mhz = 2

Then in this case you should use  gSpiBaudDivisor_2_c as paramenter for 
SPIn_Init() to have a 4Mhz baudrate.
*/
typedef enum spiBaudRateDivisor_tag {
    gSpiBaudDivisor_2_c    = 0x00,                 
    gSpiBaudDivisor_4_c    = 0x01,                 
    gSpiBaudDivisor_6_c    = 0x20,                 
    gSpiBaudDivisor_8_c    = 0x02,                 
    gSpiBaudDivisor_10_c   = 0x40,                 
    gSpiBaudDivisor_12_c   = 0x21,                 
    gSpiBaudDivisor_14_c   = 0x60,                 
    gSpiBaudDivisor_16_c   = 0x03,                 
    gSpiBaudDivisor_20_c   = 0x41,                 
    gSpiBaudDivisor_24_c   = 0x43,                 
    gSpiBaudDivisor_28_c   = 0x61,                 
    gSpiBaudDivisor_32_c   = 0x04,                 
    gSpiBaudDivisor_40_c   = 0x42,                 
    gSpiBaudDivisor_48_c   = 0x23,                 
    gSpiBaudDivisor_56_c   = 0x62,                 
    gSpiBaudDivisor_64_c   = 0x05,                 
    gSpiBaudDivisor_80_c   = 0x43,                 
    gSpiBaudDivisor_96_c   = 0x24,                 
    gSpiBaudDivisor_112_c  = 0x63,                 
    gSpiBaudDivisor_128_c  = 0x06,                 
    gSpiBaudDivisor_160_c  = 0x44,                 
    gSpiBaudDivisor_192_c  = 0x25,                 
    gSpiBaudDivisor_224_c  = 0x64,                 
    gSpiBaudDivisor_256_c  = 0x07,                 
    gSpiBaudDivisor_320_c  = 0x45,                 
    gSpiBaudDivisor_384_c  = 0x26,                 
    gSpiBaudDivisor_448_c  = 0x65,                 
    gSpiBaudDivisor_512_c  = 0x17,                 
    gSpiBaudDivisor_640_c  = 0x46,                 
    gSpiBaudDivisor_768_c  = 0x27,                 
    gSpiBaudDivisor_896_c  = 0x66,                 
    gSpiBaudDivisor_1024_c = 0x37,                 
    gSpiBaudDivisor_1280_c = 0x47,                 
    gSpiBaudDivisor_1536_c = 0x57,                 
    gSpiBaudDivisor_1792_c = 0x67,                 
    gSpiBaudDivisor_2048_c = 0x77,                 
    gSpiBaudDivisor_Max_c  
} spiBaudRateDivisor_t;


/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/

/*SPI 1 Globals*/
#if gSpiUsePort1_d

extern spiCallback_t   gpfSpi1CallBack;     //SPI callback

#endif //gSpiUsePort1_d

/*SPI 2 Globals*/
#if gSpiUsePort2_d

extern spiCallback_t   gpfSpi2CallBack;     //SPI callback

#endif //gSpiUsePort2_d


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/

/*SPI 1 Functions*/
#if gSpiUsePort1_d
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/************************************************************************************
* Initialize the SPI1 communication module and hardware.
*
* Interface assumptions: 
*   The function configures hard coded the next values
*
*   - Setting	                       - Hard coded Value
*   SPI Interrupt Enable	           Enabled only if gSpinMode_d = gSpiInterrupt_d
*   SPI System Enable	               Enabled
*   SPI Transmit Interrupt Enable	   Enabled
*   Master/Slave Mode Select	       Master
*   Clock Polarity	                 Active-high
*   Clock Phase	                     at the middle of the first cycle
*   LSB First	                       Disabled
*   Master Mode-Fault Enable	       Disabled
*   SPI Stop in Wait Mode	           Disabled
*   SPI Pin Control 0	               SPI uses separate pins for data in and data out
*
* Return value:  void
*
*
************************************************************************************/
extern void SPI1_Init
( 
spiBaudRateDivisor_t SpiBaud         //IN, Desired baudrate on the SPI bus
);

/************************************************************************************
* Enable one of the peripherals on the SPI bus setting low the corresponding GPIO 
* pin. 
*
* Interface assumptions: 
*    The Callback pointer can be null puinter.
*    Once a Peripheral is asserted is necesary to deassert it in order to assert 
*   another one. 
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c      Everithing is OK.
*                gSpiErrInvalidParameter_c    The peripheral num passed is not valid.
*                gSpiErrModuleBusy_c   There is one peripheral asserted already.
*
************************************************************************************/
extern spiErr_t SPI1_AssertPeripheralSsPin
(
 uint8_t         u8PeripheralNum, //IN, Which of the peripherals on the list will be used
 spiCallback_t   pfUserCallBack   //IN, Pointer to the callback function called when a frame 
                                  //is completely received or transmitted 
); 

/************************************************************************************
* Disable the previously enabled peripheral on the SPI bus setting high the 
* corresponding GPIO pin. 
*
* Interface assumptions: 
*    There shall be a peripheral previuosly asserted and it shouldn't be transmitting
*   or receiving data.    
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
extern spiErr_t SPI1_DeassertPeripheralSsPin
(
void
); 

/************************************************************************************
* Function to send strings using the SPI bus
*
* Interface assumptions: 
*    The string's lenght to be transmitted shall not be biger than 256 bytes. 
*    Before trying to send a string one peripheral shall be asserted.
*    The SPI module doesn't create a copy of transmitting strings.
*    The transmitting strings shall not be modified until SPI driver ends transmitting.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
************************************************************************************/
extern spiErr_t SPI1_WriteString
(
uint8_t *pu8TxString,         //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght       //IN, Bufferr data length
);

/************************************************************************************
* Function to send strings using the SPI bus without interrupts in a blocking way 
*
* Interface assumptions: 
*    The string's lenght to be transmitted shall not be biger than 256 bytes. 
*    Before trying to send a string one peripheral shall be asserted.
*    The SPI module doesn't create a copy of transmitting strings.
*    The transmitting strings shall not be modified until SPI driver ends transmitting.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
************************************************************************************/
spiErr_t SPI1_WriteStringBlocking
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
);

/************************************************************************************
* Function to read strings using the SPI
*
* Interface assumptions: 
*    The string's lenght to be received shall not be biger than 256 bytes. 
*    Before trying to receive a string one peripheral shall be asserted.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
extern spiErr_t SPI1_ReadString
(
uint8_t *pu8RxString,         //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght       //IN, Bufferr data length
);

/************************************************************************************
* Function to read strings using the SPI
*
* Interface assumptions: 
*    The string's lenght to be received shall not be biger than 256 bytes. 
*    Before trying to receive a string one peripheral shall be asserted.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
spiErr_t SPI1_ReadStringBlocking
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
); 


/************************************************************************************
* Function to send one byte using the SPI
*
* Interface assumptions: The same as SPI1_WriteString
*
* Return value:          The same as SPI1_WriteString
*
*
************************************************************************************/
extern spiErr_t SPI1_WriteByte
(
uint8_t u8TxData            //IN, Byte to be transmitted
);

/************************************************************************************
* Function to send one byte using the SPI
*
* Interface assumptions: The same as SPI1_WriteString
*
* Return value:          The same as SPI1_WriteString
*
*
************************************************************************************/
void SPI1_WriteByteBlocking
(
uint8_t u8TxData                           //IN, Byte to be transmitted
);

/************************************************************************************
* Function to Receive one byte using the SPI
*
* Interface assumptions: SPI1_ReadString
*
* Return value:          SPI1_ReadString
*
*
************************************************************************************/
extern spiErr_t SPI1_ReadByte
(
uint8_t * pu8RxData         //IN, Address to put the readed byte
);

/************************************************************************************
* Function to Receive one byte using the SPI
*
* Interface assumptions: SPI1_ReadString
*
* Return value:          SPI1_ReadString
*
*
************************************************************************************/
uint8_t SPI1_ReadByteBlocking
(
void                        //IN, Address to put the readed byte
);

#pragma CODE_SEG DEFAULT

/************************************************************************************
* SPI´s Interrupr service function
*
*
*
************************************************************************************/
#if gSpi1Mode_d == gSpiInterrupt_d   

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

 extern INTERRUPT_KEYWORD void SPI1_Isr(void);

 #pragma CODE_SEG DEFAULT
#else
 extern void SPI1_Task(void);
#endif

#endif //gSpiUsePort1_d


/*SPI 2 Functions*/
#if gSpiUsePort2_d
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/************************************************************************************
* Initialize the SPI2 communication module and hardware.
*
* Interface assumptions: 
*   The function configures hard coded the next values
*
*   - Setting	                       - Hard coded Value
*   SPI Interrupt Enable	           Enabled only if gSpinMode_d = gSpiInterrupt_d
*   SPI System Enable	               Enabled
*   SPI Transmit Interrupt Enable	   Enabled
*   Master/Slave Mode Select	       Master
*   Clock Polarity	                 Active-high
*   Clock Phase	                     at the middle of the first cycle
*   LSB First	                       Disabled
*   Master Mode-Fault Enable	       Disabled
*   SPI Stop in Wait Mode	           Disabled
*   SPI Pin Control 0	               SPI uses separate pins for data input and data output
*
* Return value:  void
*
*
************************************************************************************/
extern void SPI2_Init
( 
spiBaudRateDivisor_t SpiBaud         //IN, Desired baudrate on the SPI bus
);

/************************************************************************************
* Enable one of the peripherals on the SPI bus setting low the corresponding GPIO 
* pin. 
*
* Interface assumptions: 
*    The Callback pointer can be null puinter.
*    Once a Peripheral is asserted is necesary to deassert it in order to assert 
*   another one. 
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c      Everithing is OK.
*                gSpiErrInvalidParameter_c    The peripheral num passed is not valid.
*                gSpiErrModuleBusy_c   There is one peripheral asserted already.
*
************************************************************************************/
extern spiErr_t SPI2_AssertPeripheralSsPin
(
 uint8_t          u8PeripheralNum, //IN, which of the peripherals on the list will be used
 spiCallback_t    pfUserCallBack   //IN, Pointer to the callback function called when a frame 
                                   //is completely received or transmitted
); 

/************************************************************************************
* Disable the previously enabled peripheral on the SPI bus setting high the 
* corresponding GPIO pin. 
*
* Interface assumptions: 
*    There shall be a peripheral previuosly asserted and it shouldn't be transmitting
*   or receiving data.    
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
extern spiErr_t SPI2_DeassertPeripheralSsPin
(
void
); 

/************************************************************************************
* Function to send strings using the SPI bus
*
* Interface assumptions: 
*    The string's lenght to be transmitted shall not be biger than 256 bytes. 
*    Before trying to send a string one peripheral shall be asserted.
*    The SPI module doesn't create a copy of transmitting strings.
*    The transmitting strings shall not be modified until SPI driver ends transmitting.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
************************************************************************************/
extern spiErr_t SPI2_WriteString
(
uint8_t *pu8TxString,         //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght       //IN, Bufferr data length
);

/************************************************************************************
* Function to send strings using the SPI bus without interrupts in a blocking way 
*
* Interface assumptions: 
*    The string's lenght to be transmitted shall not be biger than 256 bytes. 
*    Before trying to send a string one peripheral shall be asserted.
*    The SPI module doesn't create a copy of transmitting strings.
*    The transmitting strings shall not be modified until SPI driver ends transmitting.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
************************************************************************************/
spiErr_t SPI2_WriteStringBlocking
(
uint8_t *pu8TxString,                      //IN, Pointer to the buffer to be transmitted
uint8_t  u8StringLenght                    //IN, Bufferr data length
);

/************************************************************************************
* Function to read strings using the SPI
*
* Interface assumptions: 
*    The string's lenght to be received shall not be biger than 256 bytes. 
*    Before trying to receive a string one peripheral shall be asserted.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
extern spiErr_t SPI2_ReadString
(
uint8_t *pu8RxString,         //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght       //IN, Bufferr data length
);

/************************************************************************************
* Function to read strings using the SPI
*
* Interface assumptions: 
*    The string's lenght to be received shall not be biger than 256 bytes. 
*    Before trying to receive a string one peripheral shall be asserted.
*
* Return value:  spiErr_t
*
*                gSpiErrNoError_c             Everithing is OK.
*                gSpiErrNoPinAsserted_c       no peripheral has been aseerted 
*                                             previously.
*                gSpiErrTransferInProgress_c  There is one peripheral asserted already
*                                             but it is receiving or transmittin data
*
*
************************************************************************************/
spiErr_t SPI2_ReadStringBlocking
(
uint8_t *pu8RxString,                      //IN, Pointer to the buffer to write the received data
uint8_t  u8StringLenght                    //IN, Bufferr data length
); 

/************************************************************************************
* Function to send one byte using the SPI
*
* Interface assumptions: The same as SPI1_WriteString
*
* Return value:          The same as SPI1_WriteString
*
*
************************************************************************************/
extern spiErr_t SPI2_WriteByte
(
uint8_t u8TxData              //IN, Byte to be transmitted
);

/************************************************************************************
* Function to send one byte using the SPI
*
* Interface assumptions: The same as SPI1_WriteString
*
* Return value:          None
*
*
************************************************************************************/
void SPI2_WriteByteBlocking
(
uint8_t u8TxData                           //IN, Byte to be transmitted
);

/************************************************************************************
* Function to Receive one byte using the SPI
*
* Interface assumptions: SPI1_ReadString
*
* Return value:          SPI1_ReadString
*
*
************************************************************************************/
extern spiErr_t SPI2_ReadByte
(
uint8_t * pu8RxData           //IN, Address to put the readed byte
);

/************************************************************************************
* Function to Receive one byte using the SPI
*
* Interface assumptions: SPI1_ReadString
*
* Return value:          None
*
*
************************************************************************************/
uint8_t SPI2_ReadByteBlocking
(
void                        //IN, Address to put the readed byte
);

#pragma CODE_SEG DEFAULT

/************************************************************************************
* SPI´s Interrupr service function
*
*
*
************************************************************************************/
#if gSpi2Mode_d == gSpiInterrupt_d   

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

 extern INTERRUPT_KEYWORD void SPI2_Isr(void);

 #pragma CODE_SEG DEFAULT
#else
 extern void SPI2_Task(void);
#endif

#endif //gSpiUsePort2_d


#endif //__SPI_INTERFACE_H__

  