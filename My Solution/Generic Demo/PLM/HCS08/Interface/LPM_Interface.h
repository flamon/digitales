/*****************************************************************************
* Low Power Modes Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __LPM_INTERFACE_H__               
  #define __LPM_INTERFACE_H__
 
#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "board_config.h"
#include "PLM_config.h"

#ifndef gLPMSupported_d 
    #define gLPMSupported_d FALSE
#endif    


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

 

/*LPM errors*/    
typedef enum lpmError_tag{
  gLpmErrorNoError_c,
  gLpmErrorTransitionNotSupported_c,
  gLpmErrorMaxError_c
}lpmError_t;

#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d) 

  typedef enum stop2WakeUpEnablingOptions_tag {
    gDisableAll_c,
    gEnable_RTC_c,
    gEnable_KBI_c,
    gEnable_RTC_KBI_c,
    gMaxEnablingOptions_c
  }stop2WakeUpEnablingOptions_t;
  
#endif

/************************************************************************************
*************************************************************************************
* Interface macros
*************************************************************************************
************************************************************************************/



#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d) 

#define LPM_WakeUpInterruptsToRunMode(Enabled) (SPMSC2_LPWUI = Enabled);           //TRUE for enabled, FALSE for disabled
#define LPM_WakeUpInterruptsToLPRunMode(Enabled) (SPMSC2_LPWUI = !Enabled);        //TRUE for enabled, FALSE for disabled
#define LPM_EnableClkPeripherals(PeripheralsMaskSCGC1, PeripheralsMaskSCGC2)       SCGC1 |= PeripheralsMaskSCGC1; \
                                                                                   SCGC2 |= PeripheralsMaskSCGC2;     
#endif

#if defined(gMc1323xPlatform_d)
/* The parameter options for this macro are:
    gDisableAll_c              RTC and KBI wakeups both disabled
    gEnable_RTC_c           RTC wakeups enabled, KBI wakeups disabled
    gEnable_KBI_c           KBI wakeups enabled, RTC wakeups disabled
    gEnable_RTC_KBI_c       RTC and KBI wakeups both enabled
*/

#define LPM_WakeUpInterruptsFromStop2Mode(Enabled_RTC_KBI) if (gDisableAll_c == Enabled_RTC_KBI){            \
                                                              SCGC2_RTC = 0;                                 \
                                                              SCGC2_KBI1 = 0;                                \
                                                              SCGC2_KBI2 = 0;                                \
                                                              }                                              \
                                                            if (gEnable_RTC_c == Enabled_RTC_KBI){           \
                                                              SCGC2_RTC = 1;                                 \
                                                              SCGC2_KBI1 = 0;                                \
                                                              SCGC2_KBI2 = 0;                                \
                                                            }                                                \
                                                            if (gEnable_KBI_c == Enabled_RTC_KBI){           \
                                                              SCGC2_RTC = 0;                                 \
                                                              SCGC2_KBI1 = 1;                                \
                                                              SCGC2_KBI2 = 1;                                \
                                                            }                                                \
                                                            if (gEnable_RTC_KBI_c == Enabled_RTC_KBI){       \
                                                              SCGC2_RTC = 1;                                 \
                                                              SCGC2_KBI1 = 1;                                \
                                                              SCGC2_KBI2 = 1;                                \
                                                            }
#endif        

#if defined(gMcs08qe128Platform_d)
#define LPM_WakeUpInterruptsFromStop2Mode(Enabled_RTC_KBI) if (gFalse_c == Enabled_RTC_KBI){                 \
                                                              SCGC2_RTC = 0;                                 \
                                                              SCGC2_KBI = 0;                                 \
                                                             }                                               \
                                                            if (gTrue_RTC_c == Enabled_RTC_KBI){             \
                                                              SCGC2_RTC = 1;                                 \
                                                              SCGC2_KBI = 0;                                 \
                                                            }                                                \
                                                            if (gTrue_KBI_c == Enabled_RTC_KBI){             \
                                                              SCGC2_RTC = 0;                                 \
                                                              SCGC2_KBI = 1;                                 \
                                                            }                                                \
                                                            if (gTrue_RTC_KBI_c == Enabled_RTC_KBI){         \
                                                              SCGC2_RTC = 1;                                 \
                                                              SCGC2_KBI = 1;                                 \
                                                            }                                                    
#endif       

#if defined(gMcs08Gt60Platform_d)

  #define LPM_WakeUpInterruptsToRunMode(Enabled) SRTISC_RTIS2 = !Enabled; \
                                                 SRTISC_RTIS1 = !Enabled; \
                                                 SRTISC_RTIS0 = !Enabled; 
#endif



                                                       
                     



/************************************************************************************
*************************************************************************************
* Interface Constants
*************************************************************************************
************************************************************************************/





/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
*  LPM_LPRunMode
*
*   This function is called to enter LPRun mode. It can only be called from run mode.
*   
*   Not supported in MC1321x.
*
************************************************************************************/
#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

  extern void LPM_LPRunMode
  (
  void
  );

#endif

/************************************************************************************
*
*  LPM_Stop3Mode
*
*   This function is called to enter Stop3 mode. 
*
*   Stop3 mode will only work in running mode and not in debug mode (all asm STOP 
*   are taken as WAIT when debugging).
*
*   IN: TRUE or FALSE if you want to keep the voltage regulator active or not 
*   during Stop3 mode    (just for MC1323x and QE128 platforms). In MC1321x platform  
*   the parameter will be ignored.
*
************************************************************************************/

extern void LPM_Stop3Mode
(
bool_t bVoltageRegulatorRemainsActive
);

/************************************************************************************
*
*  LPM_Stop2Mode
*
*  This function is called to enter Stop2 mode. 
*  
*  Return value: gErrorTransitionNotSupported_c     if the actual MCU low power 
*                                                     mode is LPRun
*
************************************************************************************/

extern lpmError_t LPM_Stop2Mode
(
void
);

/************************************************************************************
*
*  LPM_WaitMode
*
*  This function is called to enter Wait mode. For MC1323x and QE128 it validates  
*  the actual low power mode of the MCU. 
*
*  Return value: gErrorTransitionNotSupported_c     if the actual MCU low power 
*                                                     mode is LPRun (just in MC1323x 
*                                                     and QE128)
*
************************************************************************************/

extern lpmError_t LPM_WaitMode
(
void
);

/************************************************************************************
*
*  LPM_RunMode
*
*  This function is called to enter run mode and it can only be used in MC1323x and
*  QE128 platforms.
*
************************************************************************************/
#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

  extern void LPM_RunMode
  (
  void
  );

#endif

/************************************************************************************
*
*  LPM_LPWaitMode
*
*  This function is called to enter LPWait mode. 
*  Return value: gErrorTransitionNotSupported_c     if the actual MCU low power 
*                                                    mode is Run mode 
*
*  This function is not supported in MC1321x. 
*
************************************************************************************/

#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

  extern lpmError_t LPM_LPWaitMode
  (
  void
  );

#endif


#endif //__LPM_INTERFACE_H__

  