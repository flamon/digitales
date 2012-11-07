/******************************************************************************
* Public header file for the GPIO driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/
 
#ifndef _LVD_INTERFACE_H_
#define _LVD_INTERFACE_H_

#include "derivative.h"
#include "EmbeddedTypes.h"
#include  "PLM_Config.h"

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/

#ifndef gLvdSupported_d   
  #define gLvdSupported_d   FALSE
#endif

#define gLvdMode_d     gInterruption_c   //    //gPolling_c

/*LVD callback pointer definition
  It's called in the LVD interrupt context.
  The function shall be placed on the non-banked flash section. 
*/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near lvCallback_t)(void);
#else
  typedef void(* __near lvCallback_t)(void);
#endif

typedef struct LvdControlBit1_tag{

  uint8_t BGBE: 1;
  uint8_t DoNotUse1: 1;
  uint8_t LvdEnable: 1;
  uint8_t LvdStopEnable :1;
  uint8_t LvdResetEnable :1;
  uint8_t LvdInterruptEnable :1;
  uint8_t LvdAck :1;
  uint8_t LvdFlag :1;

}LvdControlBit1_t;


typedef struct LvdControlBit2_tag{

  uint8_t DoNotUse1: 1;
  uint8_t DoNotUse2: 1;
  uint8_t DoNotUse3: 1;
  uint8_t LvwInterruptEnable: 1;
  uint8_t LvwVoltageThresholdSelect :1;
  uint8_t LvdVoltageThresholdSelect :1;
  uint8_t LvwAck :1;
  uint8_t LvwFlag :1;

}LvdControlBit2_t;


typedef union LvdControl1_tag{

  uint8_t Port;
  LvdControlBit1_t Bit;

}LvdControl1_t;


typedef union LvdControl2_tag{

  uint8_t Port;
  LvdControlBit2_t Bit;

}LvdControl2_t;


typedef enum LvdVoltage_tag{
 gLvdVoltageDetetcFlagActive_c = 0,
 gLvdVoltageWarningFlagActive_c,
 gLvdVoltageNoFlagsActive_c,
 gLvdVoltageMax_c
}lvdVoltage_t;

typedef enum LvdError_tag{
 gLvdErrorNoError_c = 0,
 gLvdErrorInvalidParameter_c,
 gLvdErrorMax_c
}lvdError_t;


 /******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/


#define  gEnableBit_c   1 
#define  gDisableBit_c  0 
#define  gHigh_c     1
#define  gLow_c      0

#define gLowVoltageDetectFlag    (SPMSC1_LVDF)
#define Lvd_Acknowledge()        (SPMSC1_LVDACK = 1) 
#define Lvd_IntEnable()          (SPMSC1_LVDIE = 1) 
#define Lvd_IntDisable()         (SPMSC1_LVDIE = 0) 

#ifdef gMcs08Gt60Platform_d 
  #define gLowVoltageWarningFlag   (SPMSC2_LVWF)
  #define gLVWThresholdHigh        (SPMSC2_LVDV)
  #define gLVDThresholdHigh        (SPMSC2_LVWV)
  #define Lvw_Acknowledge()        (SPMSC2_LVWACK = 1)
  #define Lvw_IntEnable()          
  #define Lvw_IntDisable()         
#else
  #define gLowVoltageWarningFlag   (SPMSC3_LVWF)
  #define gLVWThresholdHigh        (SPMSC3_LVDV)
  #define gLVDThresholdHigh        (SPMSC3_LVWV)
  #define Lvw_Acknowledge()        (SPMSC3_LVWACK = 1)
  #define Lvw_IntEnable()          (SPMSC3_LVWIE = 1)
  #define Lvw_IntDisable()         (SPMSC3_LVWIE = 0)
#endif


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/

/******************************************************************************
* Lvd_Init - initializes the LVD module's variables and configures the LVD
* with the specified values
*
* Interface assumptions:
*   NONE
*
* return value:
*   
*   LvdErrorNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/

extern lvdError_t Lvd_Init(
  lvCallback_t,   // Input: Callback for low power warning
  lvCallback_t,   // Input: Callback for low power detect
  LvdControl1_t,  // Input: Control variable 1 
  LvdControl2_t   // Input: Control variable 2 
);
 
/******************************************************************************
* Lvd_VoltageStatus - Returns the voltage supply status
*
* Interface assumptions:
*   NONE
*
* return value:
*   
*  gLvdVoltageDetetcFlagActive_c
*  gLvdVoltageWarningFlagActive_c
* gLvdVoltageNoFlagsActive_c
*
* Effects on global data:
*   NONE
*
*******************************************************************************/

extern lvdVoltage_t Lvd_VoltageStatus(void);

/******************************************************************************
* LowVoltageDetect / Lvd_Task - This function calls a callback when an low voltage detect
*or low voltage warning is found. Can be used as an interrupt or by polling.
*
* Interface assumptions:
*   NONE
*
* return value:
*   
*   NONE
*
* Effects on global data:
*   NONE
*
*******************************************************************************/

#if gLvdMode_d == gInterruption_c 

  #pragma CODE_SEG ISR_SEGMENT
  
   extern INTERRUPT_KEYWORD void LowVoltageDetectIsr (void);
   
  #pragma CODE_SEG DEFAULT
#else
  extern void Lvd_Task(void);
#endif //gLvdMode_d  

#endif //_LVD_INTERFACE_H_
