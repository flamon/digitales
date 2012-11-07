/******************************************************************************
* Public header file for the GPIO driver.
*
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/


 
#ifndef _GPIO_INTERFACE_H_
#define _GPIO_INTERFACE_H_


#include "derivative.h"
#include "EmbeddedTypes.h"
#include "PortConfig.h"

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/

typedef union bitByte_tag{
 uint8_t port;
 struct{
   uint8_t bit0: 1;
   uint8_t bit1: 1;
   uint8_t bit2: 1;
   uint8_t bit3: 1;
   uint8_t bit4: 1;
   uint8_t bit5: 1;
   uint8_t bit6: 1;
   uint8_t bit7: 1;
 }bit;

}bitByte_t;

typedef enum gpioLevel_tag{
  gGpioPinStateInactive_c = 0,
  gGpioPinStateActive_c,
  gGpioPinStateMax_c
} gpioLevel_t;


typedef enum gpioError_tag{
  gGpioErrNoError_c = 0,
  gGpioErrInvalidParameter_c,
  gGpioErrMax_c
} gpioErr_t;

typedef enum gpioPin_tag{
  
  gGpioNoGpioMask_c         =    0,
  gGpioPin0Mask_c           = 1<<0,
  gGpioPin1Mask_c           = 1<<1,
  gGpioPin2Mask_c           = 1<<2,
  gGpioPin3Mask_c           = 1<<3,
  gGpioPin4Mask_c           = 1<<4,
  gGpioPin5Mask_c           = 1<<5,
  gGpioPin6Mask_c           = 1<<6,
  gGpioPin7Mask_c           = 1<<7,
  gGpioMaxMask_c  
} gpioPin_t;

 typedef struct gpioRuntimeConfig_tag{
  bitByte_t Direction;
  bitByte_t PullDown;
  bitByte_t SlewRate;
  bitByte_t DriveStrength;
  bitByte_t Logic;
 }gpioRuntimeConfig_t; 
 

//******************************************************************************

#ifdef gMcs08qe128Platform_d

typedef enum gpioPort_tag{
  
  gGpioPortA_c = 0, 
  gGpioPortB_c,
  gGpioPortC_c,
  gGpioPortD_c,
  gGpioPortE_c,
  gGpioPortF_c,
  gGpioPortG_c,
  gGpioPortH_c,
  gGpioPortJ_c,
  gGpioPortMax_c
} gpioPort_t;
 
#endif 




//******************************************************************************
#ifdef gMc1323xPlatform_d 
 
 typedef enum gpioPort_tag{
  
  gGpioPortA_c = 0, 
  gGpioPortB_c,
  gGpioPortC_c,
  gGpioPortD_c,
  gGpioPortMax_c
} gpioPort_t;
 
#endif 

#ifdef gMcs08Gt60Platform_d 
//******************************************************************************

typedef enum gpioPort_tag{
  
  gGpioPortA_c = 0, 
  gGpioPortB_c,
  gGpioPortC_c,
  gGpioPortD_c,
  gGpioPortE_c,
  gGpioPortF_c,
  gGpioPortG_c,
  gGpioPortMax_c
} gpioPort_t;
 
#endif  


 
 
 /******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/

#define gPortDisable_c        0x00
#define gPortEnable_c         0xFF
#define gPortInput_c          0x00
#define gPortOutput_c         0xFF
#define gPortInverse_c        0xFF 
#define gPortDirect_c         0x00

#define gPinDisable_c         FALSE
#define gPinEnable_c          TRUE
#define gPinInput_c           FALSE
#define gPinOutput_c          TRUE
#define gPinInverse_c         TRUE 
#define gPinDirect_c          FALSE


/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
*******************************************************************************/


/******************************************************************************
* Gpio_Init - initializes the GPIO module's variables and configures the GPIO
* ports with the specified default values
*
* Interface assumptions:
*   NONE
*
* return value:
*   
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_Init(void);


/******************************************************************************
* Gpio_PortWrite - writes a value onthe selected port
*
* Interface assumptions:
*   A correct Port value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_PortWrite
(
  gpioPort_t, // Port to write
  uint8_t     // Value to write
);                


/******************************************************************************
* Gpio_ConfigPort - configures the selected port in runtime
*
* Interface assumptions:
*    A correct Port value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_ConfigPort
(
  gpioPort_t,  // Port where the function is intended to act
  gpioRuntimeConfig_t  // Configuration parameters 
); 


/******************************************************************************
* Gpio_PinSet - sets a selected pin of any port available 
*
* Interface assumptions:
*    A correct Port and pin value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_PinSet
(
  gpioPort_t, // Port where the function is intended to act
  gpioPin_t   // Pin to set
);


/******************************************************************************
* Gpio_PinClear -  clears a selected pin of any port available 
*
* Interface assumptions:
*    A correct Port and pin value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_PinClear
(
  gpioPort_t, // Port where the function is intended to act
  gpioPin_t   // Pin to clear
);


/******************************************************************************
* Gpio_PinToggle - toggles a selected pin of any port available
*
* Interface assumptions:
*    A correct Port and pin value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern void Gpio_PinToggle
(
  gpioPort_t, // Port where the function is intended to act
  gpioPin_t   // Pin to toggle
);


/******************************************************************************
* Gpio_PinValue returns - the state of the selected pin of any port available
*
* Interface assumptions:
*    A correct Port and pin value must be given
*
* return value:
*   gGpioErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
 extern gpioLevel_t Gpio_PinValue
(
  gpioPort_t,  // Port where the function is intended to act
  gpioPin_t    // Pin to read
);


/******************************************************************************
* Gpio_PortValue - returns the value of any port available
*
* Interface assumptions:
*    A correct Port value must be given
*
* return value:
*   uint8_t - The value of the port
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern bitByte_t Gpio_PortValue
(
  gpioPort_t  // Port to read
); 


#endif //_GPIO_INTERFACE_H_


