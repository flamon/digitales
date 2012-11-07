/*****************************************************************************
* Radio Interface header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __RADIO_INTERFACE_H__               
  #define __RADIO_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "SPI_Interface.h"
#include "IRQ_Interface.h"
#include "PortConfig.h"

/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/
extern uint16_t gu16RadioIrqStatus;

/************************************************************************************
*************************************************************************************
* Interface macro definitions
*************************************************************************************
************************************************************************************/

/*Control Signals configuration*/
/*Attn pin*/
#define gRadioAttnPort_c            gAttnPort_c
#define gRadioAttnPinMask_c         gAttnPinMask_c

/*RxTxEnable pin*/
#define gRadioRxTxEnablePort_c      gRxTxEnablePort_c
#define gRadioRxTxEnablePinMask_c   gRxTxEnablePinMask_c

/*Reset pin*/
#define gRadioResetRadioPort_c      gResetRadioPort_c
#define gRadioResetRadioPinMask_c   gResetRadioPinMask_c

/*Status Signals configuration*/
/*OutOfIdle Pin*/
#define gRadioOutOfIdlePort_c       gOutOfIdlePort_c
#define gRadioOutOfIdlePinMask_c    gOutOfIdlePinMask_c

/*CrcValid Pin*/
#define gRadioCrcValidPort_c        gCrcValidPort_c
#define gRadioCrcValidPinMask_c     gCrcValidPinMask_c


/*Macros to manage de Radio Control Signals*/
#define Radio_AssertReset()          (void)Gpio_PinSet(gRadioResetRadioPort_c, gRadioResetRadioPinMask_c)   
#define Radio_DeassertReset()        (void)Gpio_PinClear(gRadioResetRadioPort_c, gRadioResetRadioPinMask_c) 

#define Radio_AssertAttentionPin()   (void)Gpio_PinSet(gRadioAttnPort_c, gRadioAttnPinMask_c)    
#define Radio_DeassertAttentionPin() (void)Gpio_PinClear(gRadioAttnPort_c, gRadioAttnPinMask_c)

#define Radio_AssertTxRxEnable()     (void)Gpio_PinSet(gRadioRxTxEnablePort_c, gRadioRxTxEnablePinMask_c)
#define Radio_DeassertTxRxEnable()   (void)Gpio_PinClear(gRadioRxTxEnablePort_c, gRadioRxTxEnablePinMask_c)

/*Macros to Read de Radio status Signals*/
#define Radio_GetOutOfIdlePinState() Gpio_PinValue(gRadioOutOfIdlePort_c, gRadioOutOfIdlePinMask_c)

#define Radio_GetCrcValidPinState()  Gpio_PinValue(gRadioCrcValidPort_c, gRadioCrcValidPinMask_c)

/*Radio event flags on the Radio's IRQ status register*/
#define gRadioIrqFlagPllLockIrq_c  (1<<15)
#define gRadioIrqFlagRamAddrErr_c  (1<<14)
#define gRadioIrqFlagArbBusyErr_c  (1<<13)
#define gRadioIrqFlagStrmDataErr_c (1<<12)
#define gRadioIrqFlagAttnIrq_c     (1<<10)
#define gRadioIrqFlagDozeIrq_c     (1<<9)
#define gRadioIrqFlagTmr1Irq_c     (1<<8)
#define gRadioIrqFlagRxRcvdIrq_c   (1<<7)
#define gRadioIrqFlagTxSentIrq_c   (1<<6)
#define gRadioIrqFlagCcaIrq_c      (1<<5)
#define gRadioIrqFlagTmr3Irq_c     (1<<4)
#define gRadioIrqFlagTmr4Irq_c     (1<<3)
#define gRadioIrqFlagTmr2Irq_c     (1<<2)
#define gRadioIrqFlagCca_c         (1<<1)
#define gRadioIrqFlagCrcValid_c    (1<<0)
#define gRadioIrqNoFlags_c         (0)

#define Radio_IrqStatusRegValue() (gu16RadioIrqStatus)

#define gRadioResetReg_c        0x00     /* Register 00 */
#define gRadioRxPktRAMReg_c     0x01     /* Register 01 */
#define gRadioTxPktRAMReg_c     0x02     /* Register 02 */
#define gRadioTxPktCtlReg_c     0x03     /* Register 03 */
#define gRadioCCAThreshReg_c    0x04     /* Register 04 */
#define gRadioIRQMaskReg_c      0x05     /* Register 05 */
#define gRadioControlAReg_c     0x06     /* Register 06 */
#define gRadioControlBReg_c     0x07     /* Register 07 */
#define gRadioPAEnableReg_c     0x08     /* Register 08 */
#define gRadioControlCReg_c     0x09     /* Register 09 */
#define gRadioCLKOCtlReg_c      0x0A     /* Register 0A */
#define gRadioGPIODirReg_c      0x0B     /* Register 0B */
#define gRadioGPIODataOutReg_c  0x0C     /* Register 0C */
#define gRadioLO1IntDivReg_c    0x0F     /* Register 0F */
#define gRadioLO1NumReg_c       0x10     /* Register 10 */
#define gRadioRegister0x11_c    0x11     /* Register 11 */
#define gRadioPALvlReg_c        0x12     /* Register 12 */
#define gRadioTmrCmp1AReg_c     0x1B     /* Register 1B */
#define gRadioTmrCmp1BReg_c     0x1C     /* Register 1C */
#define gRadioTmrCmp2AReg_c     0x1D     /* Register 1D */
#define gRadioTmrCmp2BReg_c     0x1E     /* Register 1E */
#define gRadioTmrCmp3AReg_c     0x1F     /* Register 1F */
#define gRadioTmrCmp3BReg_c     0x20     /* Register 20 */
#define gRadioTmrCmp4AReg_c     0x21     /* Register 21 */
#define gRadioTmrCmp4BReg_c     0x22     /* Register 22 */
#define gRadioTC2PrimeReg_c     0x23     /* Register 23 */
#define gRadioIRQStatusReg_c    0x24     /* Register 24 */
#define gRadioRSTIndReg_c       0x25     /* Register 25 */
#define gRadioCurrentTimeAReg_c 0x26     /* Register 26 */
#define gRadioCurrentTimeBReg_c 0x27     /* Register 27 */
#define gRadioGPIODataInReg_c   0x28     /* Register 28 */
#define gRadioChipIdReg_c       0x2C     /* Register 2C */
#define gRadioRXStatusReg_c     0x2D     /* Register 2D */
#define gRadioTimestampAReg_c   0x2E     /* Register 2E */
#define gRadioTimestampBReg_c   0x2F     /* Register 2F */
#define gRadioBEREnableReg_c    0x30     /* Register 30 */
#define gRadioPSMModeReg_c      0x31     /* Register 31 */
#define gRadioRegister0x34_c    0x34     /* Register 34 */

#define gRadioMaxDataLenght_c    128


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

/*Type used for the event and process Radio callbacks*/
//typedef void(*radioCallback_t)(void);
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near radioCallback_t)(void);
#else
  typedef void(* __near radioCallback_t)(void);
#endif

/*Possible return values for the Radio driver API functions*/
typedef enum radioError_tag{
  gRadioErrNoError_c,
  gRadioErrInvalidParameter_c,
  gRadioErrRadioBussy_c,
  gRadioErrMaxError_c
}radioError_t;

/*Radio Regiter's data type*/
typedef struct radioRegister_tag{
uint8_t   address;
uint16_t  u16RegisterValue; 
}radioRegister_t;


typedef enum {
  gRadioNoBlock_d  = 0,
  gRadioAllowToBlock_d
}radioUtilBlock_t;

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
* At the end of this function the radio signals are as follows:
*     Attn          Deasserted
*     RxTxEnenable  Deasserted
*     Reset         Asserted 
*
* Interface assumptions: 
*   This function must be called prior to any other call to the Radio driver 
*   interface functions
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK
*                gRadioErrInvalidParameter_c    radioEventCallback is a NULL pointer
*
************************************************************************************/
radioError_t Radio_DriverInit
(
  radioCallback_t   radioEventCallback //IN, Pointer to the Event callback
);
             
/************************************************************************************
* Function to Write on the Radio's SPI registers
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The writeCallback is not mandatory, to avoid the callback usage set NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrRadioBusy_c           The radio state is not Idle
*
************************************************************************************/
radioError_t Radio_WriteRegister
(
  uint8_t           regToWrite,     //IN, Addres of the Radio register to be writted  
  uint16_t          u16RegValue,    //IN, value to write on the register
  radioCallback_t   writeCallback   /*IN, Callback function to be called at the end  
                                          of the register write*/
); 

/************************************************************************************
* Function to Write on the Radio's SPI registers
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The writeCallback is not mandatory, to avoid the callback usage set NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrRadioBusy_c           The radio state is not Idle
*
************************************************************************************/
radioError_t Radio_WriteRegisterBlocking
(
  uint8_t           regToWrite,     //IN, Addres of the Radio register to be writted  
  uint16_t          u16RegValue    //IN, value to write on the register
);


/************************************************************************************
* Function to Read from the Radio's SPI registers
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The readCallback is not mandatory, to avoid the callback usage set it to NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrRadioBusy_c           The radio state is not Idle
*
************************************************************************************/
radioError_t Radio_ReadRegister
(
  uint8_t           regToRead,       //IN, Addres of the Radio register to be read  
  uint16_t*         pu16RegValue,   //IN, pointer to store the register value
  radioCallback_t   readCallback    /*IN, Callback function to be called at the end  
                                          of the register read*/
);

/************************************************************************************
* Function to Read from the Radio's SPI registers
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The readCallback is not mandatory, to avoid the callback usage set it to NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrRadioBusy_c           The radio state is not Idle
*
************************************************************************************/
radioError_t Radio_ReadRegisterBlocking
(
  uint8_t           regToRead,      //IN, Addres of the Radio register to be read  
  uint16_t*         pu16RegValue   //OUT, pointer to store the register value
);


/************************************************************************************
* Function to write data into one of the Radio Tx buffers
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The String to write is in Big endian mode.
*    The writeCallback is not mandatory, to avoid the callback usage set it to NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrInvalidParameter_c    pu8WrittingString is a NULL pointer 
*                                               or u8StringLenght is bigger  than
*                                               gRadioMaxDataLenght_c  
*                gRadioErrRadioBusy_c           The radio state is not Idle
*
************************************************************************************/
radioError_t Radio_WriteTxBuffer
(
  uint8_t *         pu8WrittingString,
  uint8_t           u8StringLenght,
  radioCallback_t   writeCallback,    /*IN, Callback function to be called at the end  
                                            of the buffer write*/
  radioUtilBlock_t  allowToBlock      /*IN, Specifies if the function shall wait until 
                                            the buffer is writted*/
); 

/************************************************************************************
* Function to read data from the Radio Rx buffer
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*    The readCallback is not mandatory, to avoid the callback usage set it to NULL
*
* Return value:  radioError_t
*
*                gRadioErrNoError_c             Everithing is OK.
*                gRadioErrInvalidParameter_c    pu8ReadingString is a NULL pointer 
*                                               or u8StringLenght is bigger  than
*                                               gRadioMaxDataLenght_c  
*                gRadioErrRadioBusy_c           The radio state is not Idle
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
); 

/************************************************************************************
* Function to know if the Radio driver is on Idle state
*
* Interface assumptions: 
*    The Radio_DriverInit shall be called before this function.
*
* Return value:  bool_t
*
*                TRUE     The radio state is Idle
*                FALSE    The radio state is not Idle
*
************************************************************************************/
bool_t Radio_IsRadioIdle(void);

/************************************************************************************
* ...
*
* Interface assumptions: 
*    ...
*
* Return value:  ...
*
*                ...
*                ...
*
************************************************************************************/
void Radio_EnableRadioInterrupts(void);

/************************************************************************************
* ...
*
* Interface assumptions: 
*    ...
*
* Return value:  ...
*
*                ...
*                ...
*
************************************************************************************/
void Radio_DisableRadioInterrupts(void);

#pragma CODE_SEG DEFAULT

#endif // __RADIO_INTERFACE_H__               
