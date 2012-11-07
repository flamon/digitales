/*****************************************************************************
* Low Power Modes driver configuration header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _LPM_H_               
#define _LPM_H_

#include "LPM_Interface.h"


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

#define _Stop asm ( stop )

#define _Wait asm ( wait )
  
#define _Nop  asm ( nop )



#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)
  typedef enum lowPowerMode_tag {
    mRunMode_c,
    mLPRunMode_c,
    mMaxLowPowerMode_c
  }lowPowerMode_t;
  

#endif



#endif  //__LPM_H__ 
