/******************************************************************************
* Public header file for the MCU initialization driver for SMAC.
*
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/

#ifndef _MCU_INIT_H_
#define _MCU_INIT_H_

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/

 /******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
*******************************************************************************/

/******************************************************************************
* MCUInit - initializes the MCU's write once registers
*
* Interface assumptions:
*   NONE
*
* return value:
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void MCUInit(void);

/******************************************************************************
* MCU_SetInternalClock8Mhz - Sets the internal MCU clock to 8Mhz
*
* Interface assumptions:
*   NONE
*
* return value:
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void MCU_SetInternalClock8Mhz(void);

/******************************************************************************
* MCU_UseExternalClock - Sets the external clock to be used to generate the 
*                        bus clock.
*
* Interface assumptions:
*   NONE
*
* return value:
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void MCU_UseExternalClock(void);

/******************************************************************************
* MCU_UseMcuClock - Sets the Internal clock to be used to generate the 
*                        bus clock.
*
* Interface assumptions:
*   NONE
*
* return value:
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void MCU_UseMcuClock();

#endif /*_MCU_INIT_H_*/