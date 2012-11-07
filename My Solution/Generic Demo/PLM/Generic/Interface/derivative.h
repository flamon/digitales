/************************************************************************************
* Public header file used to include the specific platform header file.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/

#ifndef __DERIVATIVE_H__
#define __DERIVATIVE_H__ 
                                 
#include "board_config.h"
#include "IrqControlLib.h"
/* Include the derivative-specific header file */

#ifdef MEMORY_MODEL_BANKED
  #define ISR_SEGMENT __NEAR_SEG NON_BANKED
#else
  #define ISR_SEGMENT DEFAULT
#endif

#if defined(gMcs08Gt60Platform_d)
  #include <MC9S08GB60.h>
  #ifndef INTERRUPT_KEYWORD   
    #define INTERRUPT_KEYWORD interrupt
  #endif
  #ifndef gSystemClock_d
   #define gSystemClock_d     8000000           /* 8 MHz. */
  #endif
#endif

#if defined(gMc1321xPlatform_d)
  #include <MC13213.h>  
  #ifndef INTERRUPT_KEYWORD 
    #define INTERRUPT_KEYWORD interrupt
  #endif
  #ifndef gSystemClock_d
   #define gSystemClock_d     8000000           /* 8 MHz. */
  #endif
#endif

#if defined(gMcs08qe128Platform_d)
  #ifndef INTERRUPT_KEYWORD 
    #define INTERRUPT_KEYWORD __interrupt
  #endif
  #include <mc9s08qe128.h>
  #ifndef gSystemClock_d
   #define gSystemClock_d     8000000           /* 8 MHz. */
  #endif
#endif

#if defined(gMc1323xPlatform_d)
  #ifndef INTERRUPT_KEYWORD 
    #define INTERRUPT_KEYWORD __interrupt
  #endif
  #include "Mc1323xRegs.h"
  #ifndef gSystemClock_d
   #define gSystemClock_d     16000000           /* 16 MHz. */
  #endif
#endif

#ifdef gUserDefinePlatform_d

  #if (gMcuMcs08GbGt60_c == gTargetBasedOn_c)	
	  #include <MC9S08GB60.h>
	  #ifndef INTERRUPT_KEYWORD   
		#define INTERRUPT_KEYWORD interrupt
	  #endif
	  #ifndef gSystemClock_d
	   #define gSystemClock_d     8000000           /* 8 MHz. */
	  #endif
  #endif

  #if (gMcuMcs08qe128_c == gTargetBasedOn_c)
    #ifndef INTERRUPT_KEYWORD 
      #define INTERRUPT_KEYWORD __interrupt
    #endif
    #include <mc9s08qe128.h>
    #ifndef gSystemClock_d
     #define gSystemClock_d     8000000           /* 8 MHz. */
    #endif
  #endif

  #if (gMcuMc1323x_c == gTargetBasedOn_c)
    #ifndef INTERRUPT_KEYWORD 
      #define INTERRUPT_KEYWORD __interrupt
    #endif
    #include "Mc1323xRegs.h"
    #ifndef gSystemClock_d
     #define gSystemClock_d     16000000           /* 16 MHz. */
    #endif
   #endif

#endif



#endif /* __DERIVATIVE_H__ */




