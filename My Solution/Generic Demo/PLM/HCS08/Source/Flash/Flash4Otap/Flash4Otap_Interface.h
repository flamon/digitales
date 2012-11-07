/************************************************************************************
* Flash for OTAP interface header. 
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


#ifndef __FLASH4OTAP_INTERFACE_H__               
  #define __FLASH4OTAP_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "SMAC_config.h"

#if(TRUE == gOtapSupported_d)
  

/************************************************************************************
*************************************************************************************
* Public Constants
*************************************************************************************
************************************************************************************/


#define gFlash4OTAPParamAndInitValidation_d FALSE


#define gFlash4OTAPCmdBlankCheck_c     (0x05)
#define gFlash4OTAPCmdProgram_c        (0x20)
#define gFlash4OTAPCmdBurst_c          (0x25)                                
#define gFlash4OTAPCmdErasePage_c      (0x40)
#define gFlash4OTAPCmdMassErase_c      (0x41)


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/


typedef enum Flash4OTAPClkValues_tag
{
  gFlash4OTAPClk20MHz_c = 0,
  gFlash4OTAPClk16MHz_c,
  gFlash4OTAPClk10MHz_c,
  gFlash4OTAPClk8MHz_c,
  gFlash4OTAPClk4MHz_c,
  gFlash4OTAPClk2MHz_c,
  gFlash4OTAPClk1MHz_c,
  gFlash4OTAPClk200kHz_c,
  gFlash4OTAPClk100kHz_c,
  gFlash4OTAPClkMax_c  
} Flash4OTAPClkValues_t;



typedef enum Flash4OTAPErr_tag
{
  gFlash4OTAPNoErr_c = 0,
  gFlash4OTAPErrInvalidParam_c,
  gFlash4OTAPErrBlankCheck_c,
  gFlash4OTAPErrWriteProtection_c,  
  gFlash4OTAPErrOncePerRst_c,
  gFlash4OTAPErrModuleBusy_c,
  gFlash4OTAPErrAccess_c,
  gFlash4OTAPErrMax_c
} Flash4OTAPErr_t;


/************************************************************************************
*************************************************************************************
* Macro definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* Flash4OTAP_BlankCheck() entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Verify" or 
*              "Blank Check" Flash Command.
*
************************************************************************************/

#define Flash4OTAP_BlankCheck() \
        Flash4OTAP_CmdHandler ( (uint32_t) NULL, NULL, (uint16_t) 1, gFlash4OTAPCmdBlankCheck_c) 

/************************************************************************************
*
* Flash4OTAP_WriteByte(u32Address, u8Data) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Program"  
*              Flash Command. The user must set the u32Address and u8Data parameters.
*
************************************************************************************/

#define Flash4OTAP_WriteByte(u32Address, u8Data) Flash4OTAP_CmdHandler ( (uint32_t) u32Address, (uint8_t*) &u8Data, (uint16_t) 1, gFlash4OTAPCmdProgram_c)

/************************************************************************************
*
* Flash4OTAP_WriteString(u32Address, pu8DataPointer, u16DataSize) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Burst"  
*              Flash Command. The user must set the u32Address, u8Data, and u16DataSize
*              parameters.
*
************************************************************************************/

#define Flash4OTAP_WriteString(u32Address, pu8DataPointer, u16DataSize) \
        Flash4OTAP_CmdHandler ( (uint32_t) u32Address, (uint8_t*) pu8DataPointer, (uint16_t) u16DataSize, gFlash4OTAPCmdBurst_c)

/************************************************************************************
*
* Flash4OTAP_ErasePage(u32Address) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Erase"  
*              Flash Command. The user must set the u32Address parameter.
*
************************************************************************************/

#define Flash4OTAP_ErasePage(u32Address) \
        Flash4OTAP_CmdHandler ( (uint32_t) u32Address, NULL, (uint16_t) 1, gFlash4OTAPCmdErasePage_c)

/************************************************************************************
*
* Flash4OTAP_MassErase() entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Mass Erase"  
*              Flash Command.
*
************************************************************************************/

#define Flash4OTAP_MassErase() \
        Flash4OTAP_CmdHandler ( (uint32_t) NULL,NULL, (uint16_t) 1, gFlash4OTAPCmdMassErase_c)



/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/



/************************************************************************************
*
* Flash4OTAP_CmdHandler
*
* Description: This API handles the commands supported by Flash module. It needs to 
*              execute from RAM memory, in order to achieve write, erase, or verify 
*              commands. 
*
* Interface assumptions: 
* 
* Return value:  Flash4OTAPErr_t type
*         gFlash4OTAPErrSuccess_c          - API Operation was completed succesfully. 
*         gFlash4OTAPErrInvalidParam_c     - The user introduced a parameter not supported 
*                                       by the API.
*         gFlash4OTAPErrBlankCheck_c       - If the Blank Check command detected that 
*                                       Flash is not erased. 
*         gFlash4OTAPErrWriteProtection_c  - The procedure could not complete because 
*                                       Flash protection was enabled.
*         gFlash4OTAPErrModuleBusy_c       - The Flash module is busy.
*
************************************************************************************/



#pragma CODE_SEG FLASH4OTAP_ROUTINES 

Flash4OTAPErr_t Flash4OTAP_CmdHandler
(
  uint32_t u32FlashAddress,  // IN: This parameter defines the start address on Flash  
                             // that will be used for the command (erase, write, blank  
                             // check or verify) procedure. 
                               
  uint8_t* pu8FlashData,     // IN: This parameter points to the Data array that will  
                             // be written into the Flash memory. It is only used for
                             // a writing related command (program and burst). It is  
                             // ignored for other commands.
                             
  uint16_t u16FlashDataSize, // IN: This parameter is only used for a writing more 
                             // than 1 byte in Flash using burst command. The value of 
                             // this parameter must be 0x01 for other commands.

  uint8_t u8FlashCmd         // IN: This parameter defines which Flash command will be 
                             // executed by the API.
);

#pragma CODE_SEG OFLC_ROUTINES


/************************************************************************************
*
* Flash4OTAPCopyInRAM 
*
* Description: This function copies Flash_CmdHandler routine from FLASH to RAM memory
*
* Interface assumptions: 
* 
* Return value:  None
*                
************************************************************************************/


void Flash4OTAPCopyInRAM
(
  void
);


/************************************************************************************
*
* Flash4OTAP_Init 
*
* Description: This function is called to initialize the Flash module. It configures 
*              the Flash clock, clears Status flags, and copies program code for 
*              driving Flash module into RAM memory. 
*
* Interface assumptions: 
* 
*
* Return value:  Flash4OTAPErr_t type
*         gFlash4OTAPSuccess_c         - API Operation was completed succesfully.
*         gFlash4OTAPErrInvalidParam_c - The user introduced a parameter not supported by 
*                                   the API.
*         gFlash4OTAPErrOncePerRst_c   - Flash4OTAP_Init() may be called once after reset. 
*                                   Other attempt to call this Flash_Init will generate 
*                                   this error.
*
************************************************************************************/


Flash4OTAPErr_t Flash4OTAP_Init 
(
  Flash4OTAPClkValues_t FlashClkValue    // IN: This enumeration parameter describes the Bus 
                                    // Clock value configured by the user. Depending 
                                    // on this parameter, the driver configures the 
                                    // Flash module divisor to achieve the required 
                                    // clock to drive the Flash module.
);


/************************************************************************************
*
* Flash4OTAP_Read 
*
* Description: 
*
* Interface assumptions: 
* 
* Return value:  
*                
************************************************************************************/


Flash4OTAPErr_t Flash4OTAP_Read
(
   uint32_t u32FlashAddress,       //IN:
   uint8_t* pu8FlashData          //OUT:
);


#pragma CODE_SEG DEFAULT


#endif /*(TRUE == gOtapSupported_d)*/
#endif //__FLASH4OTAP_INTERFACE_H__

