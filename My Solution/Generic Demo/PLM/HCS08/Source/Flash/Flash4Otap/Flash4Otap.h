/************************************************************************************
* Flash4OTAP configuration header. 
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

#ifndef _FLASH4OTAP_H_               
#define _FLASH4OTAP_H_

#include "Flash4OTAP_Interface.h"
#include "SMAC_config.h"

#if(TRUE == gOtapSupported_d)


/************************************************************************************
*************************************************************************************
* Private Constants
*************************************************************************************
************************************************************************************/

/*
StartCopyInRAM refers to the begining of the segment FLASH_TO_RAM. 
This segment contains the Cmd_Handler routine after it has been copied to RAM.
*/

#define StartCopyInRAM4OTAP __SEG_START_FLASH_ROUTINES_RAM
#define SizeCopyInRAM4OTAP __SEG_SIZE_FLASH_ROUTINES_RAM


/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#ifdef gMcs08Gt60Platform_d  
  #define   FCDIV_FDIVLD_MASK     128
  #define   mMaxFlash4OTAPAddress_c    0xFFFF    
#endif
   
#ifdef  gMcs08qe128Platform_d 
  #define   mMaxFlash4OTAPAddress_c    0x1FFFF
#endif 
#ifdef gMc1323xPlatform_d
  #define   mMaxFlash4OTAPAddress_c    0x13FFF
#endif  



#endif /*(TRUE == gOtapSupported_d)*/

#endif  //__FLASH4OTAP_H__ 


