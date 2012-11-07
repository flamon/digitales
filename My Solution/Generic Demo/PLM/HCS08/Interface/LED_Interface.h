/******************************************************************************
* Public header file for the LED driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/

#ifndef _LED_INTERFACE_H_
#define _LED_INTERFACE_H_

#include "GPIO_interface.h"
#include "EmbeddedTypes.h" 
#include "derivative.h" 
#include "board_config.h"
#include "PLM_config.h"

#ifndef gLEDSupported_d
    #define gLEDSupported_d FALSE
#endif    

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

#define   Led_On(x)        (void)Gpio_PinSet(x)
#define   Led_Off(x)       (void)Gpio_PinClear(x)
#define   Led_Toggle(x)    (void)Gpio_PinToggle(x)

#define  gLED1Mask_c        8
#define  gLED2Mask_c        4
#define  gLED3Mask_c        2
#define  gLED4Mask_c        1


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/

/******************************************************************************
* Led_ClearAll - Clears all the leds
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

extern void Led_OffAll(void);

/******************************************************************************
* Led_SetAll - Sets all the leds
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

extern void Led_OnAll(void);

/******************************************************************************
* Led_PrintValue - Writes a hex value on the LEDs
*
* Interface assumptions:
*   A value greater than 0xF shall not be used.
*
* return value:
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/

extern void Led_PrintValue
(
uint8_t  // Value to print in the Led port. It shall not be greater than 0x0F
);


#endif //_LED_INTERFHACE_H_