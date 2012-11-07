         /************************************************************************************
* EEPROM configuration header. 
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

#ifndef __EEPROM4OTAP_H__               
#define __EEPROM4OTAP_H__

#include "Eeprom4OTAP_Interface.h"
#include "SMAC_config.h"
#if(TRUE == gOtapSupported_d)


/************************************************************************************
*************************************************************************************
* Private Constants
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
#define	mEepromOTAPAt24C1024bwI2cAddress_c	(0x50)
#define mEepromOTAPAt24C1024bwPageMask_c    (0x01)
#define mEepromOTAPAt24C1024bwPageOffset_c  (0x10000)
#define	mEepromOTAPMaxAddress_c             (0x20000)   // AT24C1024B 512 pages of 256 bytes each

#define Iic4OTAP_StopTransaction()   (IIC1C &= ~(0x40|0x20|0x10))

#endif

/* MRB, RCM, REM */
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
#define	mEepromOTAPMaxAddress_c     (0x40000)   // AT45DB021D 1024 pages of 256 bytes each
#define mEepromOTAPAT45DB021DPageSize_c  (0xFF)
#define mEepromOTAPCmdChipEraseSeq_c       0xc7,0x94,0x80,0x9A
#define mEepromOTAPCmdPwrTwoPageSizeSeq_c  0x3D,0x2A,0x80,0xA6
#define mEepromOTAPCmdMainMemRead_c   (0xD2)
#define mEepromOTAPCmdMainMemWrite_c  (0x83)   // Write with built-in erase
#define mEepromOTAPCmdBufferWrite_c   (0x84)
#define mEepromOTAPCmdReadStatusReg_c (0xD7)
#define mEepromOTAPCmdMainMem2Buffer_c (0x53)


#define mSPI1OTAP_AssertPeripheralSsPin()     (PTCD_PTCD5 = 0)
#define mSPI1OTAP_DeassertPeripheralSsPin()   (PTCD_PTCD5 = 1)

#define mOTAPDummyByte_c (0x00)

#endif


/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
#pragma CODE_SEG OFLC_ROUTINES

#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
void Spi4OTAP_Write (uint8_t* pu8Data, uint8_t u8Size);
void Spi4OTAP_Read (uint8_t* pu8Data, uint8_t u8Size);
void Spi4OTAP_Init (void);
#endif

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
void Iic4OTAP_Write (uint8_t u8SlaveAddress, uint8_t* pu8TxString, uint8_t u8StringLenght, bool_t fStartTx);
void Iic4OTAP_Read (uint8_t u8SlaveAddress, uint8_t* pu8RxString, uint8_t u8StringLenght, bool_t fStartTx);
#endif

void Delay4OTAP_us (uint16_t u16us); 

#pragma CODE_SEG DEFAULT

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#define mEepromOTAPStatusBusyMask_c     (0x80)
#define mEepromOTAPPageSize264Mask_c    (0x01)



#endif /*(TRUE == gOtapSupported_d)*/

#endif  //__EEPROM4OTAP_H__ 


