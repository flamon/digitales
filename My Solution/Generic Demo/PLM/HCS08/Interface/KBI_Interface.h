/******************************************************************************
* Public header file for the KBI driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/

#ifndef _KBI_INTERFACE_H_
#define _KBI_INTERFACE_H_


#include "derivative.h"
#include "EmbeddedTypes.h"
#include "board_config.h"
#include "PLM_config.h"

#ifndef gKbiSupported_d 
    #define gKbiSupported_d FALSE
#endif    

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/

typedef enum kbiPort_tag{
  gKbi1_c = 0,
  gKbi2_c,
  gKbiMax_c
}kbiPort_t;

typedef enum kbiErr_tag{
  gKbiErrorNoError_c = 0,
  gKbiErrorNullPointer_c,
  gKbiErrInvalidParamater_c
} kbiErr_t;

typedef uint8_t kbiPressed_t;

typedef union kbiControl_tag{
  uint8_t Port;
  struct{
    uint8_t TriggerByLevel: 1;
    uint8_t Interrupt: 1;
    uint8_t Acknowledge: 1;
    uint8_t InterruptFlag :1;
    uint8_t Empty :4;
  }bit;
}kbiControl_t;

typedef union kbiPinPort_tag{
  uint8_t Port;
  struct{
    uint8_t KbiPin0: 1;
    uint8_t KbiPin1: 1;
    uint8_t KbiPin2: 1;
    uint8_t KbiPin3: 1;
    uint8_t KbiPin4: 1;
    uint8_t KbiPin5: 1;
    uint8_t KbiPin6: 1;
    uint8_t KbiPin7: 1;
  } bit;

}kbiPinPort_t;

typedef struct kbiConfig_tag{
  kbiControl_t Control;
  kbiPinPort_t InterruptPin;
  kbiPinPort_t EdgeSelect;
}kbiConfig_t; 

/*KBI callback pointer definition
  This callback function will be called when one key is pressed,
  It's called in the KBI's interrupt context.
  The callback function shall be placed on the non-banked flash section. 
*/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near kbiCallback_t)(kbiPressed_t);
#else
  typedef void(* __near kbiCallback_t)(kbiPressed_t);
#endif
  
 /******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/

#define gKbiPoll_c            gPolling_c
#define gKbiInterruption_c    gInterruption_c

#define gKbiMode_d          gKbiInterruption_c

#if defined(gMcs08Gt60Platform_d)
 #define gNumberOfKbiPorts_c       1
#elif defined(gMcs08qe128Platform_d)|| defined(gMc1323xPlatform_d)
  #define gNumberOfKbiPorts_c       2
#endif  

#define gKbiDisabledPort_c        0x00
#define gKbiEnabledPort_c         0xFF
#define gKbiRisingEdgeOrHighLevelDetectPort_c 0xFF
#define gKbiFallingEdgeOrLowLevelDetectPort_c 0x00

#define gKbiDisabledPin_c         0
#define gKbiEnabledPin_c          1
#define gKbiRisingEdgeOrHighLevelDetectPin_c 1
#define gKbiFallingEdgeOrLowLevelDetectPin_c 0

#define gKbiMaskKey0_c  (1<<0)
#define gKbiMaskKey1_c  (1<<1)
#define gKbiMaskKey2_c  (1<<2)
#define gKbiMaskKey3_c  (1<<3)
#define gKbiMaskKey4_c  (1<<4)
#define gKbiMaskKey5_c  (1<<5)
#define gKbiMaskKey6_c  (1<<6)
#define gKbiMaskKey7_c  (1<<7)

#define gKbiPressedKeyNoKey_c    (0)
#define gKbiPressedKey0_c     (1<<0)
#define gKbiPressedKey1_c     (1<<1)
#define gKbiPressedKey2_c     (1<<2)
#define gKbiPressedKey3_c     (1<<3)
#define gKbiPressedKey4_c     (1<<4)
#define gKbiPressedKey5_c     (1<<5)
#define gKbiPressedKey6_c     (1<<6)
#define gKbiPressedKey7_c     (1<<7)


/******************************************************************************
*******************************************************************************
* Public memory prototypes
*******************************************************************************
*******************************************************************************/


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/


/******************************************************************************
* Kbi_Init - initializes the KBI module's variables and configures the KBI 
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
extern kbiErr_t Kbi_Init
(
  kbiCallback_t,   // Input: Callback function to start after KBI interrupt
  kbiConfig_t*,    // Input: Pointer to configuration structure 
  kbiPort_t        // Input: Port to initialize
); 


/******************************************************************************
* Kbi_Config - configures the selected port in runtime
*
* Interface assumptions:
*   A correct Port value must be given
*
* return value:
*   gKbiErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern kbiErr_t Kbi_Config
(
  kbiPort_t,       // Input: Port to configure
  kbiConfig_t*     // Input: Pointer to configuration structure
); 


/******************************************************************************
* Kbi_PortValue - returns the value of any port available
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
extern uint8_t	Kbi_PortValue
(
  kbiPort_t        // Input: port to get value from
);

 
/******************************************************************************
* Kbi_task - Polls the Kbi flags and if active calls the callback function.
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
#ifdef gMcs08qe128Platform_d
  
  #if gKbiMode_d == gKbiInterruption_c 
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void KBI1_ISR (void);
    #pragma CODE_SEG DEFAULT
  #else 
    extern void Kbi_KbiTask(void);
  #endif
#endif


#ifdef gMc1323xPlatform_d
    
  #if gKbiMode_d == gKbiInterruption_c
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void  KBI1_ISR (void);
    #pragma CODE_SEG DEFAULT
    
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void KBI2_ISR (void);
    #pragma CODE_SEG DEFAULT
  
  #else 
    extern void Kbi_KbiTask(void);
  #endif

#endif

 
 #ifdef gMcs08Gt60Platform_d

  #if gKbiMode_d == gKbiInterruption_c
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void KBI1_ISR (void);
    #pragma CODE_SEG DEFAULT
  #else 
    extern void Kbi1_KbiTask(void);
  #endif
    
#endif
 

#endif //_KBI_INTERFACE_H_