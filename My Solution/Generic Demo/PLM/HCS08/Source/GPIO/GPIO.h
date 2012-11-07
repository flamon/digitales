/******************************************************************************
* Private header file for the GPIO driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/
#ifndef _GPIO_H_
#define _GPIO_H_

#include "GPIO_Interface.h"

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
*******************************************************************************/


#ifdef gMcs08Gt60Platform_d  
 
 typedef struct gpioDataHandling_tag{
  uint8_t Data;
  uint8_t Dummy1;
  uint8_t Dummy2;
  uint8_t Direction;
} gpioDataHandling_t;

typedef struct gpioConfigPort_tag{
  uint8_t PullDown;
  uint8_t SlewRate;
} gpioConfigPort_t; 


#else


typedef struct gpioDataHandling_tag{
  uint8_t Data;
  uint8_t Direction;
} gpioDataHandling_t;

typedef struct gpioConfigPort_tag{
  uint8_t PullDown;
  uint8_t SlewRate;
  uint8_t DriveStrength;
} gpioConfigPort_t; 

#endif




typedef union gpioConfigUnion_tag{  // This union is used in order 
  gpioDataHandling_t* dataHandling; // to use the same variable for
  gpioConfigPort_t* configPort;     // two different functions
} gpioConfigUnion_t;

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
*******************************************************************************/

  #ifdef gMcs08qe128Platform_d

    #define gNumberOfGpioPorts_c  9

  #endif



  #ifdef gMc1323xPlatform_d

    #define gNumberOfGpioPorts_c  4

  #endif



  #ifdef gMcs08Gt60Platform_d

    #define gNumberOfGpioPorts_c  7

  #endif

#endif

/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public memory declarations
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
*******************************************************************************/