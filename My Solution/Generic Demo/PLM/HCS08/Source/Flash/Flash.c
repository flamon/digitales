/************************************************************************************
* Flash driver implementation. 
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


#include "Flash.h"

#pragma MESSAGE DISABLE C1805

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if TRUE == gFlashSupported_d   
#pragma DATA_SEG FLASH_ROUTINES_RAM 
volatile uint8_t FlashRamDummyData = 0; //It is just to avoid Linker warning                                    
#pragma DATA_SEG DEFAULT
            
                                          
extern uint8_t __SEG_START_FLASH_ROUTINES_RAM[];
extern uint8_t __SEG_SIZE_FLASH_ROUTINES_RAM[];

#endif
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*
* Flash_CmdHandler
*
************************************************************************************/

#pragma CODE_SEG FLASH_ROUTINES  

#if TRUE == gFlashSupported_d

FlashErr_t Flash_CmdHandler
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
    #if (TRUE == gFlashParamAndInitValidation_d)
        if(!u16FlashDataSize)   // DataSize may not be 0.
        {
          return gFlashErrInvalidParam_c;
        } 
        if(((u8FlashCmd == gFlashCmdProgram_c)||(u8FlashCmd == gFlashCmdBurst_c)) && ((&pu8FlashData) == NULL)) 
        {
          return gFlashErrInvalidParam_c;
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
          return gFlashErrAccess_c;
        }
        if (FSTAT & FSTAT_FPVIOL_MASK) 
        {
          return gFlashErrWriteProtection_c;
        }
        
    }while (--u16FlashDataSize);
   
    
    while ((FSTAT  & FSTAT_FCCF_MASK)==0);  // Wait for the last command to complete  
    
    if (u8FlashCmd == gFlashCmdBlankCheck_c)  
    {
       if (!(FSTAT & FSTAT_FBLANK_MASK))      
       {
          return gFlashErrBlankCheck_c;       
       }
    
    } 
        
    return gFlashNoErr_c;                           
}

#else // FALSE == gFlashSupported_d 
 
 FlashErr_t Flash_CmdHandler (uint32_t u32FlashAddress, uint8_t* pu8FlashData, uint16_t u16FlashDataSize, uint8_t u8FlashCmd)
 {
   (void)u32FlashAddress;
   (void)pu8FlashData;
   (void)u16FlashDataSize;
   (void)u8FlashCmd;
   return gFlashNoErr_c;
 }
 
 #endif


#pragma CODE_SEG DEFAULT

/************************************************************************************
*
* Flash_Init
*
************************************************************************************/

#if TRUE == gFlashSupported_d 

FlashErr_t Flash_Init
  (
    FlashClkValues_t FlashClkValue
  )
{ 
    #if (TRUE == gFlashParamAndInitValidation_d)
   
      if(FlashClkValue > gFlashClkMax_c)
      {
        return gFlashErrInvalidParam_c;
      }
        
      if (FCDIV & FCDIV_FDIVLD_MASK) // Check if the FDIV register was written before
      {
        return gFlashErrOncePerRst_c;
      }
    
    #endif 
   
    // Clear FACCERR and FPVIOL Flags 
    FSTAT = FSTAT | 0x30;  
    
    switch (FlashClkValue)
    {
      case gFlashClk20MHz_c:  FCDIV = 0x4C; break;
      case gFlashClk16MHz_c:  FCDIV = 0x49; break;
      case gFlashClk10MHz_c:  FCDIV = 0x31; break;
      case gFlashClk8MHz_c:   FCDIV = 39; break; 
      case gFlashClk4MHz_c:   FCDIV = 20; break;  // 19 for QE and 1323x
      case gFlashClk2MHz_c:   FCDIV = 9; break;
      case gFlashClk1MHz_c:   FCDIV = 4; break;
      case gFlashClk200kHz_c: FCDIV = 0; break;
      case gFlashClk100kHz_c: FCDIV = 0; break;
      
      default:  FCDIV = 20;break;
    }
    
    // A new command may be written in the command buffer.   
    FSTAT |= FSTAT_FCBEF_MASK;   

     
    if (FSTAT & FSTAT_FACCERR_MASK)
    {
      return gFlashErrAccess_c;
    }
    if (FSTAT & FSTAT_FPVIOL_MASK) 
    {
      return gFlashErrWriteProtection_c;
    }
    
    FlashRamDummyData = 0; //It is just to avoid Linker warning                                    
    
    FlashCopyInRAM();
   
    return gFlashNoErr_c;
 }
 
#else  // FALSE == gFlashSupported_d 
  
 FlashErr_t Flash_Init (FlashClkValues_t FlashClkValue)
 {
   (void)FlashClkValue;
   return gFlashNoErr_c;
 }
 #endif
 
 /************************************************************************************
*
* Flash_Read
*
************************************************************************************/

 #if TRUE == gFlashSupported_d 
 
 FlashErr_t Flash_Read (uint32_t u32FlashAddress,uint8_t* pu8FlashData)
 {

    #if (TRUE == gFlashParamAndInitValidation_d)
      if((u32FlashAddress > mMaxFlashAddress_c) || (NULL == pu8FlashData))
      {
        return gFlashErrInvalidParam_c;
      }
    #endif
    
    #ifdef gMcs08Gt60Platform_d         
        *pu8FlashData = *(uint8_t*)(u32FlashAddress & 0x0000FFFF);      
    #else // gMcs08qe128Platform_d || gMc1323xPlatform_d      
        LAP0 =  (uint8_t)(u32FlashAddress & 0x000000FF);
        LAP1 =  (uint8_t)((u32FlashAddress >> 8) & 0x000000FF);
        LAP2 =  (uint8_t)((u32FlashAddress >> 16)& 0x00000001);
        *pu8FlashData = LB;
    #endif
    
   return gFlashNoErr_c;
 }
 
#else   // FALSE == gFlashSupported_d 

 FlashErr_t Flash_Read (uint32_t u32FlashAddress,uint8_t* pu8FlashData)
 {

   (void)u32FlashAddress;
   (void)pu8FlashData;
   return gFlashNoErr_c;
 }
 #endif
 

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
* FlashCopyInRAM
*
************************************************************************************/

#if TRUE == gFlashSupported_d 
 
void FlashCopyInRAM(void) 
{                                  
  uint8_t *srcPtr, *dstPtr;
  uint16_t count;
  srcPtr = (uint8_t *)StartCopyInRAM;
  dstPtr = (uint8_t *)&Flash_CmdHandler;
  for (count = 0; count < (uint16_t) SizeCopyInRAM;  count++, dstPtr++, srcPtr++) 
  {
    *dstPtr = *srcPtr;
  }
}

#endif

/************************************************************************************
*************************************************************************************
* Private Debug stuff
*************************************************************************************
************************************************************************************/



/* End of Flash.c */
