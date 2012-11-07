/************************************************************************************
* Flash4OTAP driver implementation. 
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


#include "Flash4Otap.h"
#include "SMAC_config.h"

#pragma MESSAGE DISABLE C1805

#if(TRUE == gOtapSupported_d)

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
                                          
#pragma DATA_SEG FLASH_ROUTINES_RAM 
volatile uint8_t FlashRamDummyDataOtap = 0; //It is just to avoid Linker warning                                    
#pragma DATA_SEG DEFAULT
            
                                          
extern uint8_t __SEG_START_FLASH_ROUTINES_RAM[];
extern uint8_t __SEG_SIZE_FLASH_ROUTINES_RAM[];

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* Flash4OTAP_CmdHandler
*
************************************************************************************/

#pragma CODE_SEG FLASH4OTAP_ROUTINES  

Flash4OTAPErr_t Flash4OTAP_CmdHandler
(
  uint32_t u32FlashAddress,  
  uint8_t* pu8FlashData, 
  uint16_t u16FlashDataSize, 
  uint8_t u8FlashCmd
)
{
    uint16_t u16LocalAddress;
  
    #ifdef gMcs08Gt60Platform_d       
        u16LocalAddress = (uint16_t)(u32FlashAddress);  
    #else // gMcs08qe128Platform_d || gMc1323xPlatform_d      
        PPAGE = ((u32FlashAddress & 0x0001C000)>>14);
        u16LocalAddress = ((uint16_t)(u32FlashAddress & 0x00003FFF)+0x8000);     
    #endif
    #if (TRUE == gFlash4OTAPParamAndInitValidation_d)
        if(!u16FlashDataSize)   // DataSize may not be 0.
        {
          return gFlash4OTAPErrInvalidParam_c;
        } 
        if(((u8FlashCmd == gFlash4OTAPCmdProgram_c)||(u8FlashCmd == gFlash4OTAPCmdBurst_c)) && ((&pu8FlashData) == NULL)) 
        {
          return gFlash4OTAPErrInvalidParam_c;
        }
    #endif
       
     if (FSTAT & (FSTAT_FACCERR_MASK | FSTAT_FACCERR_MASK | FSTAT_FBLANK_MASK))
    {     
    // Clear Access Error and Write Protection Error Flags
    FSTAT |= (FSTAT_FACCERR_MASK | FSTAT_FPVIOL_MASK | FSTAT_FBLANK_MASK);
    }
    do
    {
        // Wait for the Last Burst Command to complete 
        while(!(FSTAT & FSTAT_FCBEF_MASK));
         
        // Dummy write before setting the Flash command
        (*((volatile uint8_t *)(u16LocalAddress++))) = *pu8FlashData; 

        pu8FlashData++;     
    
        FCMD = u8FlashCmd;            // Configures the Flash command          
        FSTAT |= FSTAT_FCBEF_MASK;    // Clears FCBEF to Start the command 
        
        // Wait at least 4 cycles to read the Error Flags 
        _asm NOP;
        _asm NOP;
        _asm NOP;
        _asm NOP;
        

        if (FSTAT & FSTAT_FACCERR_MASK)
        {
          return gFlash4OTAPErrAccess_c;
        }
        if (FSTAT & FSTAT_FPVIOL_MASK) 
        {
          return gFlash4OTAPErrWriteProtection_c;
        }
        
    }while (--u16FlashDataSize);
   
    
    while ((FSTAT  & FSTAT_FCCF_MASK)==0);  // Wait for the last command to complete  
    
    if (u8FlashCmd == gFlash4OTAPCmdBlankCheck_c)  
    {
       if (!(FSTAT & FSTAT_FBLANK_MASK))      
       {
          return gFlash4OTAPErrBlankCheck_c;       
       }
    
    } 
        
    return gFlash4OTAPNoErr_c;                           
}

#pragma CODE_SEG OFLC_ROUTINES

/************************************************************************************
*
* Flash_Init
*
************************************************************************************/


Flash4OTAPErr_t Flash4OTAP_Init
  (
    Flash4OTAPClkValues_t FlashClkValue
  )
{ 
    #if (TRUE == gFlash4OTAPParamAndInitValidation_d)
   
      if(FlashClkValue > gFlash4OTAPClkMax_c)
      {
        return gFlash4OTAPErrInvalidParam_c;
      }
        
      if (FCDIV & FCDIV_FDIVLD_MASK) // Check if the FDIV register was written before
      {
        return gFlash4OTAPErrOncePerRst_c;
      }
    
    #endif 
   
    // Clear FACCERR and FPVIOL Flags 
    FSTAT = FSTAT | 0x30;  
    
    switch (FlashClkValue)
    {
      case gFlash4OTAPClk20MHz_c:  FCDIV = 0x4C; break;
      case gFlash4OTAPClk16MHz_c:  FCDIV = 0x49; break;
      case gFlash4OTAPClk10MHz_c:  FCDIV = 0x31; break;
      case gFlash4OTAPClk8MHz_c:   FCDIV = 39; break; 
      case gFlash4OTAPClk4MHz_c:   FCDIV = 20; break;  // 19 for QE and 1323x
      case gFlash4OTAPClk2MHz_c:   FCDIV = 9; break;
      case gFlash4OTAPClk1MHz_c:   FCDIV = 4; break;
      case gFlash4OTAPClk200kHz_c: FCDIV = 0; break;
      case gFlash4OTAPClk100kHz_c: FCDIV = 0; break;
      
      default:  FCDIV = 20;break;
    }
    
    // A new command may be written in the command buffer.   
    FSTAT |= FSTAT_FCBEF_MASK;   

     
    if (FSTAT & FSTAT_FACCERR_MASK)
    {
      return gFlash4OTAPErrAccess_c;
    }
    if (FSTAT & FSTAT_FPVIOL_MASK) 
    {
      return gFlash4OTAPErrWriteProtection_c;
    }
  
    FlashRamDummyDataOtap = 0; //It is just to avoid Linker warning                                    
    Flash4OTAPCopyInRAM();
   
    return gFlash4OTAPNoErr_c;
 }
 
 /************************************************************************************
*
* Flash4OTAP_Read
*
************************************************************************************/
 
 Flash4OTAPErr_t Flash4OTAP_Read (uint32_t u32FlashAddress,uint8_t* pu8FlashData)
 {

    #if (TRUE == gFlash4OTAPParamAndInitValidation_d)
      if((u32FlashAddress > mMaxFlashAddress_c) || (NULL == pu8FlashData))
      {
        return gFlash4OTAPErrInvalidParam_c;
      }
    #endif
    
    #ifdef gMcs08Gt60Platform_d         
        *pu8FlashData = *(uint16_t*)u32FlashAddress;      
    #else // gMcs08qe128Platform_d || gMc1323xPlatform_d      
        LAP0 =  (uint8_t)(u32FlashAddress & 0x000000FF);
        LAP1 =  (uint8_t)((u32FlashAddress >> 8) & 0x000000FF);
        LAP2 =  (uint8_t)((u32FlashAddress >> 16)& 0x00000001);
        *pu8FlashData = LB;
    #endif
    
   return gFlash4OTAPNoErr_c;
 }
 

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
* Flash4OTAPCopyInRAM
*
************************************************************************************/

void Flash4OTAPCopyInRAM(void) 
{                                  
  uint8_t *srcPtr, *dstPtr;
  uint16_t count;
  srcPtr = (uint8_t *)StartCopyInRAM4OTAP;
  dstPtr = (uint8_t *)&Flash4OTAP_CmdHandler;
  for (count = 0; count < (uint16_t) SizeCopyInRAM4OTAP;  count++, dstPtr++, srcPtr++) 
  {
    *dstPtr = *srcPtr;
  }
}


#pragma CODE_SEG DEFAULT

#endif /*(TRUE == gOtapSupported_d)*/

/* End of Flash4Otap.c */
