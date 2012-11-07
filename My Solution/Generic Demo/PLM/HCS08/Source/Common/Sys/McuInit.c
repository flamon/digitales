/******************************************************************************
* Source file for the MCU initialization driver for SMAC.
*
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/

#include "derivative.h"
/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private defines and macros
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private constants
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public memory declarations
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*  MCUInit
*******************************************************************************/ 
#if defined(gMc1321xPlatform_d) | defined(gMcs08Gt60Platform_d)
void MCUInit(void)
{
  SOPT = 0x72;                /* Turn off the watchdog. */  
}
#elif defined(gMcs08qe128Platform_d)
void MCUInit(void)
{
  SOPT1 = 0x62;   
  SPMSC2 = 0x03;     
}
#elif defined(gMc1323xPlatform_d)
void MCUInit(void)
{
  SOPT1 = 0x62;   
  SPMSC2 = 0x03;      
  SOMC1 = 0x04;
  SOMC2 = gDefaultCrystalTrim_c;
}
#endif

/******************************************************************************
*  MCU_SetInternalClock8Mhz
*******************************************************************************/ 
#if defined(gMc1321xPlatform_d) | defined(gMcs08Gt60Platform_d)
void MCU_SetInternalClock8Mhz(void)
{
  ICGC1 = 0x44;                                      
  ICGFLT = 0xC0;
  ICGC2 = 0x00;                                      
  ICGTRM = *(unsigned char*far)0xFFBE; 
}
#elif defined(gMcs08qe128Platform_d)
void MCU_SetInternalClock8Mhz(void)
{
  ICSC1 = 0x04; 
  ICSC2 = 0x00; 
  while(!ICSSC_IREFST) {/* Wait until the source of reference clock is internal clock */
  }
  ICSSC &= (unsigned char)~0xE0;
  while((ICSSC & 0xC4) != 0x00) {
  }
}
#elif defined(gMc1323xPlatform_d)
void MCU_SetInternalClock8Mhz(void)
{
   /*This function is not allowed on the mc1323x platform*/
}
#endif


/******************************************************************************
*  MCU_UseExternalClock
*******************************************************************************/ 
#if defined(gMc1321xPlatform_d) | defined(gMcs08Gt60Platform_d)
void MCU_UseExternalClock(void)
{
    ICGC1 = 0x50;   
    while (!ICGS1_ERCS); /* Wait for external reference to be stable. */
    ICGC2_LOLRE = 1;
    ICGC2_MFD = 0x00;  
}
#elif defined(gMcs08qe128Platform_d)
void MCU_UseExternalClock(void) 
{
    ICSC1 = 0x80;
    ICSC2 = 0x3A;
    ICSSC = 0x60;
}
#elif defined(gMc1323xPlatform_d)
void MCU_UseExternalClock(void)
{
   /*This function is not allowed on the mc1323x platform*/
}
#endif

/******************************************************************************
*  MCU_UseMcuClock
*******************************************************************************/ 
#if defined(gMc1321xPlatform_d) | defined(gMcs08Gt60Platform_d)
void MCU_UseMcuClock() 
{
    ICGC1_RANGE = 1;        /* Range High. */
    ICGC1_OSCSTEN = 1;      /* Osc enabled in off mode. */
    ICGC1_CLKS = 0x00;      /* Self clocked. */            
}
#elif defined(gMcs08qe128Platform_d)
void MCU_UseMcuClock() //use_mcu_clock()
{
    ICSC1 = 0x03;
    ICSC2 = 0x17;
    ICSTRM = ((*((unsigned char *)(0xFFAF))) +11);
    ICSSC = 0x61;     
}
#elif defined(gMc1323xPlatform_d)
void MCU_UseMcuClock() //use_mcu_clock()
{
   /*This function is not allowed on the mc1323x platform*/
}
#endif


