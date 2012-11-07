 /************************************************************************************
* Flash configuration header. 
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

#ifndef _FLASH_H_               
#define _FLASH_H_

#include "Flash_Interface.h"


/************************************************************************************
*************************************************************************************
* Private Constants
*************************************************************************************
************************************************************************************/

/*
StartCopyInRAM refers to the begining of the segment FLASH_TO_RAM. 
This segment contains the Cmd_Handler routine after it has been copied to RAM.
*/

#define StartCopyInRAM __SEG_START_FLASH_ROUTINES_RAM
#define SizeCopyInRAM __SEG_SIZE_FLASH_ROUTINES_RAM

#define gFlashParamAndInitValidation_d TRUE

/************************************************************************************
*************************************************************************************
* Private Type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#ifdef gMcs08Gt60Platform_d  
  #define   FCDIV_FDIVLD_MASK     128
  #define   mMaxFlashAddress_c    0xFFFF    
#endif
   
#ifdef  gMcs08qe128Platform_d 
  #define   mMaxFlashAddress_c    0x1FFFF
#endif 
#ifdef gMc1323xPlatform_d
  #define   mMaxFlashAddress_c    0x13FFF
#endif  

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* FlashCopyInRAM 
*
* Description: This function copies Flash_CmdHandler routine from FLASH to RAM memory
*
* Interface assumptions: 
* 
* Return value:  None
*                
************************************************************************************/
#if TRUE == gFlashSupported_d 

void FlashCopyInRAM
(
  void
);

#endif

/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/



#endif  //__FLASH_H__ 


