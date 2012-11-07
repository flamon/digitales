/******************************************************************************
* Private header file for the KBI driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/

#ifndef _KBI_H_
#define _KBI_H_
 
 
 #include "KBI_interface.h"
  
 
/******************************************************************************
*******************************************************************************
* Private macros
*******************************************************************************
*******************************************************************************/
 
 #ifdef gMcs08qe128Platform_d
   #define Kbi1Port                 (((PTBD & 0x0F)<<4) | (PTAD & 0x0F))
   #define Kbi2Port                 (PTDD)
   #define Kbi1Acknowledge()        (KBI1SC_KBACK = 1)
   #define Kbi2Acknowledge()        (KBI2SC_KBACK = 1)
   #define Kbi1Flag                 (KBI1SC_KBF) 
   #define Kbi2Flag                 (KBI2SC_KBF) 
   #define KbiEnabledPinsPort1      (KBI1PE)
   #define KbiEnabledPinsPort2      (KBI2PE)
   #define KbiEdgePort1             (KBI1ES)
   #define KbiEdgePort2             (KBI2ES)
 #endif
 
 #ifdef gMc1323xPlatform_d
   #define Kbi1Port                 (PTBD) 
   #define Kbi2Port                 (PTCD & 0x0F)
   #define Kbi1Acknowledge()        (KBI1SC_KBACK = 1)
   #define Kbi2Acknowledge()        (KBI2SC_KBACK = 1)
   #define Kbi1Flag                 (KBI1SC_KBF) 
   #define Kbi2Flag                 (KBI2SC_KBF) 
   #define KbiEnabledPinsPort1      (KBI1PE)
   #define KbiEnabledPinsPort2      (KBI2PE)
   #define KbiEdgePort1             (KBI1ES)
   #define KbiEdgePort2             (KBI2ES)
 #endif
 
 #ifdef gMcs08Gt60Platform_d
   #define Kbi1Port                 (PTAD)
   #define Kbi1Acknowledge()        (KBI1SC_KBACK = 1)
   #define Kbi1Flag                 (KBI1SC_KBF) 
   #define KbiEnabledPinsPort1      (KBI1PE)
   #define KbiEdgePort1             (0x00)
 #endif
 

#endif //_KBI_H_