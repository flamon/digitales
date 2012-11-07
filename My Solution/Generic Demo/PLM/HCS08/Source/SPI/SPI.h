/*****************************************************************************
* SPI Configuration header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef __SPI_H__               
  #define __SPI_H__

#include "SPI_Interface.h"

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

/*SPI module states*/
typedef enum spiStatus_tag {
  mSpiIdle_c,
  mSpiPeripheralAsserted_c,
  mSpiTransmittingData_c,
  mSpiReceivingData_c
}spiStatus_t;

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

/* SPI 1 Registers */

#if gSpiUsePort1_d

  #define mSpi1BaudReg         SPI1BR
  #define mSpi1Ctr1            SPI1C1
  #define mSpi1RxIenBit        SPI1C1_SPIE
  #define mSpi1TxIenBit        SPI1C1_SPTIE
  #define mSpi1Ctr2            SPI1C2
  #define mSpi1DataReg         SPI1D
  #define mSpi1StatusReg       SPI1S
  #define mSpi1RxFlag          SPI1S_SPRF
  #define mSpi1TxFlag          SPI1S_SPTEF
  #define mSpi1IntVector_c     VectorNumber_Vspi1

  /* SPIE=0, SPE=1, SPTIE=0, MSTR=1, CPOL=0, CPHA=0, SSOE=0, LSBFE=0 */
  #define mSpi1Ctrl1RegVal_c 0b01010000;  
  /* MODFEN=0, BIDIROE=0, SPISWAI=0, SPC0=0 */
  #define mSpi1Ctrl2RegVal_c 0;  


#endif /*gSpiUsePort1_d*/

/* SPI 2 Registers */

#if gSpiUsePort2_d

  #define mSpi2BaudReg         (SPI2BR)
  #define mSpi2Ctr1            (SPI2C1)
  #define mSpi2RxIenBit        (SPI2C1_SPIE)
  #define mSpi2TxIenBit        (SPI2C1_SPTIE)
  #define mSpi2Ctr2            (SPI2C2)
  #define mSpi2DataReg         (SPI2D)
  #define mSpi2StatusReg       (SPI2S)
  #define mSpi2RxFlag          (SPI2S_SPRF)
  #define mSpi2TxFlag          (SPI2S_SPTEF)
  #define mSpi2IntVector_c     VectorNumber_Vspi2

  /* SPIE=0, SPE=1, SPTIE=0, MSTR=1, CPOL=0, CPHA=0, SSOE=0, LSBFE=0 */
  #define mSpi2Ctrl1RegVal_c 0b01010000;  
  /* MODFEN=0, BIDIROE=0, SPISWAI=0, SPC0=0 */
  #define mSpi2Ctrl2RegVal_c 0;  

#endif /*gSpiUsePort2_d*/

#define mDummyByte_c          0x00 

#define mSpiIntEnable_c       TRUE
#define mSpiIntDisable_c      FALSE


/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/

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


#endif  //__SPI_H__ 

  