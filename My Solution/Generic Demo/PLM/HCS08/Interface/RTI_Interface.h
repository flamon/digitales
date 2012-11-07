/************************************************************************************
* Public header file for the Real Timer Interrupt driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
#ifndef _RTI_INTERFACE_H_
#define _RTI_INTERFACE_H_


#include  "EmbeddedTypes.h"
#include  "derivative.h"
#include  "PLM_Config.h"

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/
#ifndef gRtiSupported_d
#define gRtiSupported_d FALSE
#endif

#define gRtiMode_d         gPolling_c

/*
#define  gRtiPoll_c       0
#define  gRtiInterrupt_c  1
*/
/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

typedef enum rtiErr_tag{
  gRtiErrNoError_c = 0,                 
  gRtiErrRtiIsNotConfig_c,   
  gRtiErrNullPointer_c,    
  gRtiErrInvalidParameter_c,    
  gRtiErrMax_c
} rtiErr_t;


typedef enum rti1kHzIntClkPeriod_tag{
  gRti1kHzIntClkPeriod8ms_c = 1,
  gRti1kHzIntClkPeriod32ms_c,
  gRti1kHzIntClkPeriod64ms_c,
  gRti1kHzIntClkPeriod128ms_c,
  gRti1kHzIntClkPeriod256ms_c,
  gRti1kHzIntClkPeriod512ms_c,
  gRti1kHzIntClkPeriod1d024s_c,
#ifndef gMcs08Gt60Platform_d 
  gRti1kHzIntClkPeriod1ms_c,
  gRti1kHzIntClkPeriod2ms_c,
  gRti1kHzIntClkPeriod4ms_c,
  gRti1kHzIntClkPeriod10ms_c,
  gRti1kHzIntClkPeriod16ms_c,
  gRti1kHzIntClkPeriod100ms_c,
  gRti1kHzIntClkPeriod500ms_c,
  gRti1kHzIntClkPeriod1s_c, 
#endif 
  gRti1kHzIntClkMaxPeriod_c
} rti1kHzIntClkPeriod_t;



#ifdef gMcs08Gt60Platform_d
 typedef enum rtiClkSrc_tag{
    gRtiIntClkSrc1kHz_c = 0,
    gRtiExtClkSrc16Mhz_c,
    gRtiMaxClkSrc_c
  } rtiClkSrc_t;
  
  typedef enum rti16MHzExtClkPeriod_tag{
    gRti16MHzExtClkPeriod64us_c = 1,
    gRti16MHzExtClkPeriod128us_c,
    gRti16MHzExtClkPeriod256us_c,
    gRti16MHzExtClkPeriod512us_c,
    gRti16MHzExtClkPeriod1024us_c,
    gRti16MHzExtClkPeriod2d048ms_c,
    gRti16MHzExtClkPeriod4d096ms_c,
    gRti16MHzExtClkMaxPeriod_c
  } rti16MHzExtClkPeriod_t;

  typedef union rtiTimePeriod_tag{
    rti1kHzIntClkPeriod_t   rti1kHzIntClkPeriod;
    rti16MHzExtClkPeriod_t  rti16MHzExtClkPeriod;
  } rtiTimePeriod_t;	
#endif



#ifdef gMc1323xPlatform_d
  typedef enum rtiClkSrc_tag{
    gRtiIntClkSrc1kHz_c = 0,
    gRtiXosc2ClkSrc32Mhz_c,
    gRtiXosc1ClkSrc32d768kHz_c,
    gRtiXosc1ClkSrc32d768kHzPresc2_c,
    gRtiMaxClkSrc_c
  } rtiClkSrc_t;

  typedef enum rti32MHzExtClkPeriod_tag{
    gRti32MHzExtClkPeriod32us_c = 1,
    gRti32MHzExtClkPeriod64us_c,
    gRti32MHzExtClkPeriod128us_c,
    gRti32MHzExtClkPeriod256us_c,
    gRti32MHzExtClkPeriod512us_c,
    gRti32MHzExtClkPeriod1024us_c,
    gRti32MHzExtClkPeriod2048us_c,
    gRti32MHzExtClkPeriod31d25us_c,
    gRti32MHzExtClkPeriod62d5us_c,
    gRti32MHzExtClkPeriod156d25us_c,
    gRti32MHzExtClkPeriod312d5us_c,
    gRti32MHzExtClkPeriod625us_c,
    gRti32MHzExtClkPeriod1d5625ms_c,
    gRti32MHzExtClkPeriod3d125ms_c,
    gRti32MHzExtClkPeriod6d25ms_c,
    gRti32MHzExtClkMaxPeriod_c
  } rti32MHzExtClkPeriod_t;

  typedef enum rti32d768kHzExtClkPeriod_tag{
    gRti32d768kHzExtClkPeriod244us_c = 1,
    gRti32d768kHzExtClkPeriod976us_c,
    gRti32d768kHzExtClkPeriod1d95ms_c,
    gRti32d768kHzExtClkPeriod3d9ms_c,
    gRti32d768kHzExtClkPeriod7d8125ms_c,
    gRti32d768kHzExtClkPeriod1d5625ms_c,
    gRti32d768kHzExtClkPeriod3d125ms_c,
    gRti32d768kHzExtClkPeriod30d517us_c,
    gRti32d768kHzExtClkPeriod61d03us_c,
    gRti32d768kHzExtClkPeriod122d07us_c,
    gRti32d768kHzExtClkPeriod305d17us_c,
    gRti32d768kHzExtClkPeriod4d8828ms_c,
    gRti32d768kHzExtClkPeriod3d0517ms_c,
    gRti32d768kHzExtClkPeriod15d2588ms_c,
    gRti32d768kHzExtClkPeriod30d517ms_c,
    gRti32d768kHzExtClkMaxPeriod_c
  } rti32d768kHzExtClkPeriod_t;

  typedef enum rti32d768kHzExtClkPresc2Period_tag{
    gRti32d768kHzExtClkPresc2Period31d25ms_c = 1,
    gRti32d768kHzExtClkPresc2Period62d5ms_c,
    gRti32d768kHzExtClkPresc2Period125ms_c,
    gRti32d768kHzExtClkPresc2Period250ms_c,
    gRti32d768kHzExtClkPresc2Period500msPresc2_c,
    gRti32d768kHzExtClkPresc2Period1sPres2_c,
    gRti32d768kHzExtClkPresc2Period2s_c,
    gRti32d768kHzExtClkPresc2Period30d518ms_c,
    gRti32d768kHzExtClkPresc2Period61d035ms_c,
    gRti32d768kHzExtClkPresc2Period152d59ms_c,
    gRti32d768kHzExtClkPresc2Period305d18ms_c,
    gRti32d768kHzExtClkPresc2Period610d35ms_c,
    gRti32d768kHzExtClkPresc2Period1d5259s_c,
    gRti32d768kHzExtClkPresc2Period3d0518s_c,
    gRti32d768kHzExtClkPresc2Period6d1035s_c,
    gRti32d768kHzExtClkPresc2MaxPeriod_c
  } rti32d768kHzExtClkPresc2Period_t;

  typedef union rtiTimePeriod_tag{
    rti1kHzIntClkPeriod_t             rti1kHzIntClkPeriod;
    rti32MHzExtClkPeriod_t            rti32MHzExtClkPeriod;
    rti32d768kHzExtClkPeriod_t        rti32d768kHzExtClkPeriod;
    rti32d768kHzExtClkPresc2Period_t  rti32d768kHzExtClkPresc2Period; 
  } rtiTimePeriod_t;	
#endif



#ifdef gMcs08qe128Platform_d
  typedef enum rtiClkSrc_tag{
    gRtiIntClkSrc1kHz_c = 0,
    gRtiExtClkSrc16Mhz_c,
    gRtiIntClkSrc32kHz_c,
    gRtiIntClkSrc32kHzPresc2_c,
    gRtiMaxClkSrc_c
  } rtiClkSrc_t;

  typedef enum rti16MHzExtClkPeriod_tag{
    gRti16MHzExtClkPeriod64us_c = 1,
    gRti16MHzExtClkPeriod128us_c,
    gRti16MHzExtClkPeriod256us_c,
    gRti16MHzExtClkPeriod512us_c,
    gRti16MHzExtClkPeriod1024us_c,
    gRti16MHzExtClkPeriod2d048ms_c,
    gRti16MHzExtClkPeriod4d096ms_c,
    gRti16MHzExtClkPeriod62d5us_c,
    gRti16MHzExtClkPeriod125us_c,
    gRti16MHzExtClkPeriod312d5us_c,
    gRti16MHzExtClkPeriod625us_c,
    gRti16MHzExtClkPeriod1d25ms_c,
    gRti16MHzExtClkPeriod3d125ms_c,
    gRti16MHzExtClkPeriod6d25ms_c,
    gRti16MHzExtClkPeriod12d5ms_c,
    gRti16MHzExtClkMaxPeriod_c
  } rti16MHzExtClkPeriod_t;

  typedef enum rti32kHzIntClkPeriod_tag{
    gRti32kHzIntClkPeriod250us_c = 1,
    gRti32kHzIntClkPeriod1ms_c,
    gRti32kHzIntClkPeriod2ms_c,
    gRti32kHzIntClkPeriod4ms_c,
    gRti32kHzIntClkPeriod8ms_c,
    gRti32kHzIntClkPeriod16ms_c,
    gRti32kHzIntClkPeriod32ms_c,
    gRti32kHzIntClkPeriod32d25us_c,
    gRti32kHzIntClkPeriod62d5us_c,
    gRti32kHzIntClkPeriod125us_c,
    gRti32kHzIntClkPeriod312d5us_c,
    gRti32kHzIntClkPeriod500us_c,
    gRti32kHzIntClkPeriod3d125ms_c,
    gRti32kHzIntClkPeriod15d625ms_c,
    gRti32kHzIntClkPeriod31d25ms_c,
    gRti32kHzIntClkMaxPeriod_c
  } rti32kHzIntClkPeriod_t;

  typedef enum rti32kHzIntClkPresc2Period_tag{
    gRti32kHzIntClkPresc2Period32ms_c = 1,
    gRti32kHzIntClkPresc2Period64ms_c,
    gRti32kHzIntClkPresc2Period128ms_c,
    gRti32kHzIntClkPresc2Period256ms_c,
    gRti32kHzIntClkPresc2Period512ms_c,
    gRti32kHzIntClkPresc2Period1d024s_c,
    gRti32kHzIntClkPresc2Period2d048s_c,
    gRti32kHzIntClkPresc2Period31d25ms_c,
    gRti32kHzIntClkPresc2Period62d5ms_c,
    gRti32kHzIntClkPresc2Period156d25ms_c,
    gRti32kHzIntClkPresc2Period312d5ms_c,
    gRti32kHzIntClkPresc2Period625ms_c,
    gRti32kHzIntClkPresc2Period1d5625s_c,
    gRti32kHzIntClkPresc2Period3d125s_c,
    gRti32kHzIntClkPresc2Period6d25s_c,
    gRti32kHzIntClkPresc2MaxPeriod_c
  } rti32kHzIntClkPresc2Period_t;

  typedef union rtiTimePeriod_tag{
    rti1kHzIntClkPeriod_t         rti1kHzIntClkPeriod;
    rti16MHzExtClkPeriod_t        rti16MHzExtClkPeriod;
    rti32kHzIntClkPeriod_t        rti32kHzIntClkPeriod;
    rti32kHzIntClkPresc2Period_t  rti32kHzIntClkPresc2Period; 
  } rtiTimePeriod_t;	
#endif



typedef struct rtiConfig_tag{
  rtiClkSrc_t   rtiClockSrc;
  rtiTimePeriod_t rtiTimePeriod;
#ifdef gMc1323xPlatform_d   
  uint16_t rtiCntMod;
#endif
#ifdef gMcs08qe128Platform_d
  uint8_t rtiCntMod;
#endif
} rtiConfig_t;


#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near rtiCallbackFunc_t)(void);
#else
  typedef void(* __near rtiCallbackFunc_t)(void);
#endif   


/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Initializate the Real Time Interrupt module as disable, enable the RTI interruption
* when require it and set the callback function for RTI events.
*
* The routine limitations:
*    The gRtiMode must be set previous to call this function in order to initializate
*    the module correctly.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
* Revison history:
*   date      Author    Comments
*   ------    ------    --------
*
************************************************************************************/

extern rtiErr_t Rti_Init(
rtiCallbackFunc_t pCallback //IN:Pointer to the RTI callback funtion.
);


/************************************************************************************
* Configure the RTI module setting the input clock source, frequency prescaler,
* and counter module.
* 
* Return value:
*   An rtiErr_t value with the result of the operation.
*
* Revison history:
*   date      Author    Comments
*   ------    ------    --------
*
************************************************************************************/

extern rtiErr_t Rti_SetConfig(
rtiConfig_t * pRtiConfig //IN:Pointer to a structure that contain the configuration
);


/************************************************************************************
* Obtains the current configuration in the RTI module, this configuration is returned
* in the structure pointer  
*
* The routine limitations:
*   This function returns the configuration when the module has been already 
*   configured.
*
* Return value:
*   An rtiErr_t value with the result of the operation.
*
* Revison history:
*   date      Author    Comments
*   ------    ------    --------
*
************************************************************************************/

extern rtiErr_t Rti_GetConfig(
rtiConfig_t * pRtiConfig //OUT:Pointer to a structure to set the current configuration
);


/************************************************************************************
* This function is the interrupt request routine.
*
* The routine limitations:
*   This function can operate as interrup routine when this mode is selected.
*
* Return value:
*   None.
*
* Revison history:
*   date      Author    Comments
*   ------    ------    --------
*
************************************************************************************/


#if  gRtiMode_d == gInterruption_c
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Rti_Isr(void);
#else
  void Rti_Task(void);
#endif
#endif
#pragma CODE_SEG DEFAULT

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/* _RTI_INTERFACE_H_ */
