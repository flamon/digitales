/******************************************************************************
* Private header file for the LVD driver.
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
#ifndef _LVD_H_
#define _LVD_H_

#include "LVD_interface.h"

/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
*******************************************************************************/

typedef enum LvdControlOptTag{
 LvdControl1_c = 0,
 LvdControl2_c,
 LvdControlMax_c
}lvdControlOpt_t;

/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
*******************************************************************************/

#define LvdReg1  (SPMSC1)

#if defined(gMcs08Gt60Platform_d)
  #define LvdReg2  (SPMSC2)

#elif defined(gMcs08qe128Platform_d)
  #define LvdReg2  (SPMSC3)

#elif defined(gMc1323xPlatform_d)
  #define LvdReg2  (SPMSC3)

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

#endif //_LVD_H_