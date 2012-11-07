 /************************************************************************************
* Flash interface header. 
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


#ifndef __FLASH_INTERFACE_H__               
  #define __FLASH_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "PLM_Config.h"  

/************************************************************************************
*************************************************************************************
* Public Constants
*************************************************************************************
************************************************************************************/

#ifndef gFlashSupported_d   
  #define gFlashSupported_d   FALSE
#endif

#define gFlashCmdBlankCheck_c     (0x05)
#define gFlashCmdProgram_c        (0x20)
#define gFlashCmdBurst_c          (0x25)                                
#define gFlashCmdErasePage_c      (0x40)
#define gFlashCmdMassErase_c      (0x41)


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/


typedef enum FlashClkValues_tag
{
  gFlashClk20MHz_c = 0,
  gFlashClk16MHz_c,
  gFlashClk10MHz_c,
  gFlashClk8MHz_c,
  gFlashClk4MHz_c,
  gFlashClk2MHz_c,
  gFlashClk1MHz_c,
  gFlashClk200kHz_c,
  gFlashClk100kHz_c,
  gFlashClkMax_c  
} FlashClkValues_t;



typedef enum FlashErr_tag
{
  gFlashNoErr_c = 0,
  gFlashErrInvalidParam_c,
  gFlashErrBlankCheck_c,
  gFlashErrWriteProtection_c,  
  gFlashErrOncePerRst_c,
  gFlashErrModuleBusy_c,
  gFlashErrAccess_c,
  gFlashErrMax_c
} FlashErr_t;


/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/



/************************************************************************************
*************************************************************************************
* Macro definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* Flash_BlankCheck() entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Verify" or 
*              "Blank Check" Flash Command.
*
************************************************************************************/

#define Flash_BlankCheck() \
        Flash_CmdHandler ( (uint32_t) NULL, NULL, (uint16_t) 1, gFlashCmdBlankCheck_c) 

/************************************************************************************
*
* Flash_WriteByte(u32Address, u8Data) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Program"  
*              Flash Command. The user must set the u32Address and u8Data parameters.
*
************************************************************************************/

#define Flash_WriteByte(u32Address, u8Data) Flash_CmdHandler ( (uint32_t) u32Address, (uint8_t*) &u8Data, (uint16_t) 1, gFlashCmdProgram_c)

/************************************************************************************
*
* Flash_WriteString(u32Address, pu8DataPointer, u16DataSize) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Burst"  
*              Flash Command. The user must set the u32Address, u8Data, and u16DataSize
*              parameters.
*
************************************************************************************/

#define Flash_WriteString(u32Address, pu8DataPointer, u16DataSize) \
        Flash_CmdHandler ( (uint32_t) u32Address, (uint8_t*) pu8DataPointer, (uint16_t) u16DataSize, gFlashCmdBurst_c)

/************************************************************************************
*
* Flash_ErasePage(u32Address) entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Erase"  
*              Flash Command. The user must set the u32Address parameter.
*
************************************************************************************/

#define Flash_ErasePage(u32Address) \
        Flash_CmdHandler ( (uint32_t) u32Address, NULL, (uint16_t) 1, gFlashCmdErasePage_c)

/************************************************************************************
*
* Flash_MassErase() entry point
*
* Description: This Macro calls the Cmd_Handler API configured to perform "Mass Erase"  
*              Flash Command.
*
************************************************************************************/

#define Flash_MassErase() \
        Flash_CmdHandler ( (uint32_t) NULL,NULL, (uint16_t) 1, gFlashCmdMassErase_c)



/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/



/************************************************************************************
*
* Flash_CmdHandler
*
* Description: This API handles the commands supported by Flash module. It needs to 
*              execute from RAM memory, in order to achieve write, erase, or verify 
*              commands. 
*
* Interface assumptions: 
* 
* Return value:  FlashErr_t type
*         gFlashErrSuccess_c          - API Operation was completed succesfully. 
*         gFlashErrInvalidParam_c     - The user introduced a parameter not supported 
*                                       by the API.
*         gFlashErrBlankCheck_c       - If the Blank Check command detected that 
*                                       Flash is not erased. 
*         gFlashErrWriteProtection_c  - The procedure could not complete because 
*                                       Flash protection was enabled.
*         gFlashErrModuleBusy_c       - The Flash module is busy.
*
************************************************************************************/



#pragma CODE_SEG FLASH_ROUTINES 
/*
#ifndef __BANKED__
 FlashErr_t  Flash_CmdHandler
#else  
 FlashErr_t near Flash_CmdHandler
#endif */

FlashErr_t Flash_CmdHandler
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

#pragma CODE_SEG DEFAULT




/************************************************************************************
*
* Flash_Init 
*
* Description: This function is called to initialize the Flash module. It configures 
*              the Flash clock, clears Status flags, and copies program code for 
*              driving Flash module into RAM memory. 
*
* Interface assumptions: 
* 
*
* Return value:  FlashErr_t type
*         gFlashSuccess_c         - API Operation was completed succesfully.
*         gFlashErrInvalidParam_c - The user introduced a parameter not supported by 
*                                   the API.
*         gFlashErrOncePerRst_c   - Flash_Init() may be called once after reset. 
*                                   Other attempt to call this Flash_Init will generate 
*                                   this error.
*
************************************************************************************/


FlashErr_t Flash_Init 
(
  FlashClkValues_t FlashClkValue    // IN: This enumeration parameter describes the Bus 
                                    // Clock value configured by the user. Depending 
                                    // on this parameter, the driver configures the 
                                    // Flash module divisor to achieve the required 
                                    // clock to drive the Flash module.
);


/************************************************************************************
*
* Flash_Read 
*
* Description: 
*
* Interface assumptions: 
* 
* Return value:  
*                
************************************************************************************/


FlashErr_t Flash_Read
(
  uint32_t u32FlashAddress,       //IN:
   uint8_t* pu8FlashData          //OUT:
);



#endif //__FLASH_INTERFACE_H__

