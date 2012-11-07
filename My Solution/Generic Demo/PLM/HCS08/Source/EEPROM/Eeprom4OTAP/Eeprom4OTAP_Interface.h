                         /************************************************************************************
* EEPROM interface header. 
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


#ifndef __EEPROM4OTAP_INTERFACE_H__               
  #define __EEPROM4OTAP_INTERFACE_H__ 

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "SMAC_config.h" 

#if(TRUE == gOtapSupported_d)



#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
#include "Mc1323xRegs.h"
#endif

/************************************************************************************
*************************************************************************************
* Public Constants
*************************************************************************************
************************************************************************************/


typedef enum eepromOTAPErr_tag
{
  gEepromOTAPNoErr_c = 0,
  gEepromOTAPErrNoEepromInBus_c,
  gEepromOTAPErrBusBusy_c,
  gEepromOTAPErrAddrOverflow_c,
  gEepromOTAPErrNotSupported_c,  
  gEepromOTAPErrMax_c
} eepromOTAPErr_t;




/************************************************************************************
*************************************************************************************
* Macro definitions
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
 #define  Eeprom4OTAP_EnableWriteProtect()       PTCD_PTCD5 = 1;
                                                 
 #define  Eeprom4OTAP_DisableWriteProtect()      PTCD_PTCD5 = 0;
                                                                                               
                                                 
#endif


#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
 /* Write Protection pin is connected to Vcc in 1323xMRB, and then no HW protection
    is used. If the user wants to protect the memory, He'll need to perform SW
    protection. Please refer to At45db021d datasheet.  */
 #define  Eeprom_EnableWriteProtect()       // Not implemented   
 #define  Eeprom_DisableWriteProtect()      // Not implemented  
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
#pragma CODE_SEG OFLC_ROUTINES


/************************************************************************************
*
* Eeprom_Init
*
* Description:
*
* Interface assumptions:
* 
* Return value: 
*
************************************************************************************/

extern eepromOTAPErr_t  Eeprom4OTAP_Init 
(
  void
);

/************************************************************************************
*
* Eeprom_Write
*
* Description:
*
* Interface assumptions: AT45DB021D (for 1323xMRB) and AT24C1024B (for 1320xQE128) are  
* external Eeprom with memory pages of 256 bytes. If the user wants to write in 2 
* different pages, the Eeprom_Write function must be called twice: once for each page. 
*
* Return value:
*
************************************************************************************/


extern eepromOTAPErr_t  Eeprom4OTAP_Write 
(
  uint32_t EepromAddress, 
  uint8_t* pu8Data, 
  uint8_t u8DataSize
);

/************************************************************************************
*
* Eeprom_Read
*
* Description: 
*
* Interface assumptions: AT45DB021D (for 1323xMRB) and AT24C1024B (for 1320xQE128) are  
* external Eeprom with memory pages of 256 bytes. If the user wants to read from 2 
* different pages, the Eeprom_Read function must be called twice: once for each page.
* 
* Return value: 
*      
************************************************************************************/


extern eepromOTAPErr_t  Eeprom4OTAP_Read 
(
  uint32_t EepromAddress, 
  uint8_t* pu8Data, 
  uint8_t u8DataSize
);

/************************************************************************************
*
* Eeprom_Erase
*
* Description: 
*
* Interface assumptions:
* 
* Return value:        
*.
************************************************************************************/

extern eepromOTAPErr_t  Eeprom4OTAP_Erase
(
  void
);


#pragma CODE_SEG DEFAULT

#endif /*(TRUE == gOtapSupported_d)*/
#endif //__EEPROM4OTAP_INTERFACE_H__

