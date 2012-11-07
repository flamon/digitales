/*****************************************************************************
* Buzzer Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __BUZZER_INTERFACE_H__               
  #define __BUZZER_INTERFACE_H__
 
#include "derivative.h"     
#include "EmbeddedTypes.h"  
#include "PortConfig.h" 
#include "app_config.h"
#include "Timer_Interface.h"
#include "board_config.h"
#include "GPIO_Interface.h"
#include "PLM_config.h"

#ifndef gBuzzerSupported_d
    #define gBuzzerSupported_d FALSE
#endif



/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef enum buzzerVolume_tag
{
  gBuzzerVolume1_c,
  gBuzzerVolume2_c,
  gBuzzerVolume3_c,
  gBuzzerVolume4_c,
  gBuzzerVolume5_c,
  gBuzzerVolume6_c,
  gBuzzerVolumeMaxVolume_c  
}buzzerVolume_t;

typedef enum buzzerTone_tag
{
  gBuzzerTone1_c,
  gBuzzerTone2_c,
  gBuzzerTone3_c,
  gBuzzerTone4_c,
  gBuzzerTone5_c,
  gBuzzerTone6_c,
  gBuzzerTone7_c,
  gBuzzerTone8_c,
  gBuzzerTone9_c,
  gBuzzerTone10_c,
  gToneMaxTone_c  
}buzzerTone_t;


/************************************************************************************
*************************************************************************************
* Interface Constants
*************************************************************************************
************************************************************************************/

#define gShortBeepLength_c (1)       //Parameter in seconds

#define gLongBeepLength_c (3)        //Parameter in seconds

#ifdef gMc1323xPlatform_d
  #define gBuzzerTimer_c gTmr2_c
  #define gBuzzerTimerChannel_c gTmrChannel0_c
  #define gBuzzerTimerInterruptSource_c gTmrChannel0Event_c
#endif

#ifdef gMcs08qe128Platform_d
  #define gBuzzerTimer_c gTmr3_c   
  #define gBuzzerTimerChannel_c gTmrChannel4_c
  #define gBuzzerTimerInterruptSource_c gTmrChannel4Event_c
#endif  

#ifdef gMcs08Gt60Platform_d
  #define gBuzzerTimer_c gTmr1_c 
  #define gBuzzerTimerChannel_c gTmrChannel2_c
  #define gBuzzerTimerInterruptSource_c gTmrChannel2Event_c
#endif




/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/

extern bool_t gBuzzerBusy;        /*This global variable is set to TRUE when the
                                   buzzer is beeping. It is FALSE by default.*/

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/

/******************************************************************************
* Buzzer_Init - This is the initialization function for the driver, and must 
*               be called prior to any other buzzer function.
*               It configures the timer used depending on the platform and 
*               initializes some variables, such as the default start tone
*               and volume.
*
* Parameters passed in:
*   Void
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/

extern void Buzzer_Init(void);

/******************************************************************************
* Buzzer_ShortBeep - This function makes the buzzer to do a beep sound of 1 
*                    second length by default.
*                    The length of the short beep can be changed with the
*                    gShortBeepLength_c constant.
*
* Parameters passed in:
*   Void
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/

extern void Buzzer_ShortBeep(void);

/******************************************************************************
* Buzzer_LongBeep - This function makes the buzzer to do a beep sound of 3 
*                   second length by default.
*                   The length of the long beep can be changed with the
*                   gLongBeepLength_c constant.
*
* Parameters passed in:
*   Void
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/

extern void Buzzer_LongBeep(void);          


/******************************************************************************
* Buzzer_Beep - With this function the user can configure a sound passing as 
*               parameters the length in seconds (just round quantities), the 
*               tone desired and the specific volume for that beep.
*               After executing this function, all the beeps will return to the 
*               global tone and volume (default if not set differently before).
*
*
* Parameters passed in:
*   uint8_t beepSecs          Length in seconds of the beep.
*   buzzerTone_t beepTone     Tone desired.
*   buzzerVolume_t beepVol    Volume desired.
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/

extern void Buzzer_Beep(uint8_t beepSecs, buzzerTone_t beepTone, buzzerVolume_t beepVol);

/******************************************************************************
* Buzzer_SetBeepTone - This function changes the global tone to the one given 
*                      as parameter. After this function is executed, the
*                      short and long beeps will have this tone.
*
* Parameters passed in:
*  buzzerTone_t beepTone    Tone desired.
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/

extern void Buzzer_SetBeepTone(buzzerTone_t beepTone);

/******************************************************************************
* Buzzer_SetBeepVolume - This function changes the global beep volume to the 
*                        one given as parameter. After this function is 
*                        executed, the short and long beeps will have this 
*                        volume.
*
* Parameters passed in:
*  buzzerVolume_t beepVol    Volume desired.
*
* Interface assumptions:
*   None
*
* Return value:   
*   Void
*
*
*******************************************************************************/


extern void Buzzer_SetBeepVolume(buzzerVolume_t beepVol);

#endif    //__BUZZER_INTERFACE_H__