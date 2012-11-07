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

#ifndef _EEPROM_H_               
#define _EEPROM_H_

#include "Eeprom_Interface.h"

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
#include "IIC_Interface.h"
#endif

#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)    
#include "SPI_Interface.h"
#endif 

/************************************************************************************
*************************************************************************************
* Private Constants
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
#define	mEepromAt24C1024bwI2cAddress_c	(0x50)
#define mEepromAt24C1024bwPageMask_c    (0x01)
#define mEepromAt24C1024bwPageOffset_c  (0x10000)
#define	mEepromMaxAddress_c             (0x20000)   // AT24C1024B 512 pages of 256 bytes each
#define mEepromSerialComEnableRegister       IIC1C_IICEN
#endif

/* MRB, RCM, REM */
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
#define	mEepromMaxAddress_c     (0x40000)   // AT45DB021D 1024 pages of 256 bytes each
#define mEepromAT45DB021DPageSize_c  (0xFF)
#define mEepromSerialComEnableRegister        SPIC1_SPE
#define mEepromCmdChipEraseSeq_c       0xc7,0x94,0x80,0x9A
#define mEepromCmdPwrTwoPageSizeSeq_c  0x3D,0x2A,0x80,0xA6
#define mEepromCmdMainMemRead_c   (0xD2)
#define mEepromCmdMainMemWrite_c  (0x83)   // Write with built-in erase
#define mEepromCmdBufferWrite_c   (0x84)
#define mEepromCmdReadStatusReg_c (0xD7)
#define mEepromCmdMainMem2Buffer_c (0x53)

#endif


#ifndef mEepromParameterValidation_d
#define mEepromParameterValidation_d    TRUE //Can be TRUE or FALSE
#endif



/************************************************************************************
*************************************************************************************
* Private Type definitions
*************************************************************************************
************************************************************************************/

typedef enum EepromStateMachine_tag  
{
  mEepromStateIdle_c = 0,
  mEepromStateDeviceAddr_c,
  mEepromStateBusy_c,
  #if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
  mEepromStateWrBuffer_c,
  mEepromStateWrMem2Buffer,
  #endif  
  mEepromStateMax_c
}EepromStateMachine_t;


/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#define mEepromStatusBusyMask_c     (0x80)
#define mEepromPageSize264Mask_c    (0x01)


/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/

#if TRUE == gEepromSupported_d

  #ifdef MEMORY_MODEL_BANKED
    #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
    #pragma CODE_SEG DEFAULT
  #endif
  
  #if gTargetBoard_c == gMc1320xS08qe128Evb_c
  void  EepromWrite_Callback (iicResult_t IicWriteResult);
  void  EepromRead_Callback (iicResult_t IicReadResult); 
  #endif
  
  #if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
  void  EepromWrite_Callback (void);
  void  EepromRead_Callback (void);
  #endif 
  
   
  #pragma CODE_SEG DEFAULT

#endif // gEepromSupported_d

/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/



#endif  //__EEPROM_H__ 


