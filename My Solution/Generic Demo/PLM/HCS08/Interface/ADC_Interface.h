/*****************************************************************************
* ADC Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __ADC_INTERFACE_H__               
  #define __ADC_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "PortConfig.h"     // include board specific defines
#include "board_config.h"
#include "PLM_config.h"

/************************************************************************************
*************************************************************************************
* Interface Constants
*************************************************************************************
************************************************************************************/

#ifndef gAdcSupported_d 
    #define gAdcSupported_d FALSE
#endif    

#define gAdcPoll_c gPolling_c
#define gAdcInterrupt_c gInterruption_c

#define gAdcMode_d gAdcInterrupt_c //can be gAdcPoll_c or
                                   //gAdcInterrupt_c


#if defined(gMcs08Gt60Platform_d)   
  #define gAdcChannelPinMask_0_c   (1<<0)
  #define gAdcChannelPinMask_1_c   (1<<1)
  #define gAdcChannelPinMask_2_c   (1<<2)
  #define gAdcChannelPinMask_3_c   (1<<3)
  #define gAdcChannelPinMask_4_c   (1<<4)
  #define gAdcChannelPinMask_5_c   (1<<5)
  #define gAdcChannelPinMask_6_c   (1<<6)
  #define gAdcChannelPinMask_7_c   (1<<7)
#elif defined(gMcs08qe128Platform_d)
  #define gAdcChannelPinMask_0_c   (1<<0)
  #define gAdcChannelPinMask_1_c   (1<<1)
  #define gAdcChannelPinMask_2_c   (1<<2)
  #define gAdcChannelPinMask_3_c   (1<<3)
  #define gAdcChannelPinMask_4_c   (1<<4)
  #define gAdcChannelPinMask_5_c   (1<<5)
  #define gAdcChannelPinMask_6_c   (1<<6)
  #define gAdcChannelPinMask_7_c   (1<<7)
  #define gAdcChannelPinMask_8_c   (1<<8)
  #define gAdcChannelPinMask_9_c   (1<<9)
  #define gAdcChannelPinMask_10_c (1<<10)
  #define gAdcChannelPinMask_11_c (1<<11)
  #define gAdcChannelPinMask_12_c (1<<12)
  #define gAdcChannelPinMask_13_c (1<<13)
  #define gAdcChannelPinMask_14_c (1<<14)
  #define gAdcChannelPinMask_15_c (1<<15)
  #define gAdcChannelPinMask_16_c (1<<16)
  #define gAdcChannelPinMask_17_c (1<<17)
  #define gAdcChannelPinMask_18_c (1<<18)
  #define gAdcChannelPinMask_19_c (1<<19)
  #define gAdcChannelPinMask_20_c (1<<20)
  #define gAdcChannelPinMask_21_c (1<<21)
  #define gAdcChannelPinMask_22_c (1<<22)
  #define gAdcChannelPinMask_23_c (1<<23)
#endif

#if defined(gMc1323xPlatform_d)                  /* This is a dummy define. There is no ADC in MC1323x platform */
  #define gAdcChannelPinMask_0_c   (1<<0)
  #define gAdcChannelPinMask_1_c   (1<<1)
  #define gAdcChannelPinMask_2_c   (1<<2)
  #define gAdcChannelPinMask_3_c   (1<<3)
  #define gAdcChannelPinMask_4_c   (1<<4)
  #define gAdcChannelPinMask_5_c   (1<<5)
  #define gAdcChannelPinMask_6_c   (1<<6)
  #define gAdcChannelPinMask_7_c   (1<<7)
  #define gAdcChannelPinMask_0_c   (1<<0)
  #define gAdcChannelPinMask_1_c   (1<<1)
  #define gAdcChannelPinMask_2_c   (1<<2)
  #define gAdcChannelPinMask_3_c   (1<<3)
  #define gAdcChannelPinMask_4_c   (1<<4)
  #define gAdcChannelPinMask_5_c   (1<<5)
  #define gAdcChannelPinMask_6_c   (1<<6)
  #define gAdcChannelPinMask_7_c   (1<<7)
  #define gAdcChannelPinMask_8_c   (1<<8)
  #define gAdcChannelPinMask_9_c   (1<<9)
  #define gAdcChannelPinMask_10_c (1<<10)
  #define gAdcChannelPinMask_11_c (1<<11)
  #define gAdcChannelPinMask_12_c (1<<12)
  #define gAdcChannelPinMask_13_c (1<<13)
  #define gAdcChannelPinMask_14_c (1<<14)
  #define gAdcChannelPinMask_15_c (1<<15)
  #define gAdcChannelPinMask_16_c (1<<16)
  #define gAdcChannelPinMask_17_c (1<<17)
  #define gAdcChannelPinMask_18_c (1<<18)
  #define gAdcChannelPinMask_19_c (1<<19)
  #define gAdcChannelPinMask_20_c (1<<20)
  #define gAdcChannelPinMask_21_c (1<<21)
  #define gAdcChannelPinMask_22_c (1<<22)
  #define gAdcChannelPinMask_23_c (1<<23)
#endif


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

/*ADC callback pointer definition
  This callback function will be called at the end of each ADC conversion from the 
  ADC interrupt context.
  The function shall be placed on the non-banked flash section. 
  The conversion result will be reported on the callback parameter, It will be always
  right aligned
*/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near adcCallback_t)(uint16_t);
#else
  typedef void(* __near adcCallback_t)(uint16_t);
#endif

typedef enum adcErrors_tag {
  gAdcErrNoError_c = 0,
  gAdcErrInvalidParam_c,
  gAdcErrModuleBusy_c,
  gAdcErrNotInit_c,
  gAdcErrMax_c
} adcErrors_t;

#if defined(gMcs08Gt60Platform_d)   

  typedef enum adcChannel_tag {
    gAdcChannel_0_c = 0,                 
    gAdcChannel_1_c,
    gAdcChannel_2_c,	
    gAdcChannel_3_c,	
    gAdcChannel_4_c,	
    gAdcChannel_5_c,	
    gAdcChannel_6_c,	
    gAdcChannel_7_c,	
    gAdcMaxChannel_c
  }adcChannel_t;

#elif defined(gMcs08qe128Platform_d)

  typedef enum adcChannel_tag {
    gAdcChannel_0_c = 0,                 
    gAdcChannel_1_c,
    gAdcChannel_2_c,	
    gAdcChannel_3_c,	
    gAdcChannel_4_c,	
    gAdcChannel_5_c,	
    gAdcChannel_6_c,	
    gAdcChannel_7_c,	
    gAdcChannel_8_c,	
    gAdcChannel_9_c,	
    gAdcChannel_10_c,	
    gAdcChannel_11_c,	
    gAdcChannel_12_c,	
    gAdcChannel_13_c,	
    gAdcChannel_14_c,	
    gAdcChannel_15_c,	
    gAdcChannel_16_c,	
    gAdcChannel_17_c,	
    gAdcChannel_18_c,	
    gAdcChannel_19_c,	
    gAdcChannel_20_c,	
    gAdcChannel_21_c,	
    gAdcChannel_22_c,	
    gAdcChannel_23_c,	
    gAdcMaxChannel_c
  }adcChannel_t;

#endif

#if defined(gMc1323xPlatform_d)

  typedef enum adcChannel_tag {             /* This is a dummy define. There is no ADC in MC1323x platform */
    gAdcChannel_0_c = 0,                 
    gAdcChannel_1_c,
    gAdcChannel_2_c,	
    gAdcChannel_3_c,	
    gAdcChannel_4_c,	
    gAdcChannel_5_c,	
    gAdcChannel_6_c,	
    gAdcChannel_7_c,	
    gAdcChannel_8_c,	
    gAdcChannel_9_c,	
    gAdcChannel_10_c,	
    gAdcChannel_11_c,	
    gAdcChannel_12_c,	
    gAdcChannel_13_c,	
    gAdcChannel_14_c,	
    gAdcChannel_15_c,	
    gAdcChannel_16_c,	
    gAdcChannel_17_c,	
    gAdcChannel_18_c,	
    gAdcChannel_19_c,	
    gAdcChannel_20_c,	
    gAdcChannel_21_c,	
    gAdcChannel_22_c,	
    gAdcChannel_23_c,	
    gAdcMaxChannel_c
  }adcChannel_t;

#endif



/*This divisor determinates the ADC conversion rate 
  as the (MCU Bus clock)/(selected divisor).
*/
#if defined(gMcs08Gt60Platform_d)   

  typedef enum adcConversionRateDivisor_tag {
    gAdcRateDivisor_2_c = 0,                 
    gAdcRateDivisor_4_c,                 
    gAdcRateDivisor_6_c,                 
    gAdcRateDivisor_8_c,                 
    gAdcRateDivisor_10_c,                 
    gAdcRateDivisor_12_c,                 
    gAdcRateDivisor_14_c,                 
    gAdcRateDivisor_16_c,                 
    gAdcRateDivisor_18_c,                 
    gAdcRateDivisor_20_c,                 
    gAdcRateDivisor_22_c,                 
    gAdcRateDivisor_24_c,                 
    gAdcRateDivisor_26_c,                 
    gAdcRateDivisor_28_c,                 
    gAdcRateDivisor_30_c,                 
    gAdcRateDivisor_32_c,                 
    gAdcRateMaxDivisor_c  
  } adcConversionRateDivisor_t;

#elif defined(gMcs08qe128Platform_d)

  typedef enum adcConversionRateDivisor_tag {
    gAdcRateDivisor_1_c = 0,                 
    gAdcRateDivisor_2_c,                 
    gAdcRateDivisor_4_c,                 
    gAdcRateDivisor_8_c,                 
    gAdcRateMaxDivisor_c  
  } adcConversionRateDivisor_t;

#endif

#if defined(gMc1323xPlatform_d)   

  typedef enum adcConversionRateDivisor_tag {               /* This is a dummy define. There is no ADC in MC1323x platform */
    gAdcRateDivisor_2_c = 0,                 
    gAdcRateDivisor_4_c,                 
    gAdcRateDivisor_6_c,                 
    gAdcRateDivisor_8_c,                 
    gAdcRateDivisor_10_c,                 
    gAdcRateDivisor_12_c,                 
    gAdcRateDivisor_14_c,                 
    gAdcRateDivisor_16_c,                 
    gAdcRateDivisor_18_c,                 
    gAdcRateDivisor_20_c,                 
    gAdcRateDivisor_22_c,                 
    gAdcRateDivisor_24_c,                 
    gAdcRateDivisor_26_c,                 
    gAdcRateDivisor_28_c,                 
    gAdcRateDivisor_30_c,                 
    gAdcRateDivisor_32_c,                 
    gAdcRateMaxDivisor_c  
  } adcConversionRateDivisor_t;
  
#endif  

#if defined(gMcs08Gt60Platform_d)   

  typedef enum adcConversionModes_tag {
    gAdcMode8bit_c = 0,                 
    gAdcMode10bit_c,                 
    gAdcModeMax_c  
  } adcConversionModes_t;

#elif defined(gMcs08qe128Platform_d)

  typedef enum adcConversionModes_tag {
    gAdcMode8bit_c = 0,                 
    gAdcMode12bit_c,                 
    gAdcMode10bit_c,                 
    gAdcModeMax_c  
  } adcConversionModes_t;

#endif

#if defined(gMc1323xPlatform_d)   

  typedef enum adcConversionModes_tag {                    /* This is a dummy define. There is no ADC in MC1323x platform */
    gAdcMode8bit_c = 0,                 
    gAdcMode10bit_c,                 
    gAdcModeMax_c  
  } adcConversionModes_t;
  
#endif  


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
* ADC_Init
*
* Initialize the ADC communication module and hardware.
*
* Interface assumptions: 
*   The function configures hard coded the next values
*
*   - Setting	                          - Hard coded Value
*   ADC Interrupt Enable	              Enabled on if gAdcMode_d = gAdcInterrupt_d
*   ADC System Enable	                  Enabled
*   ADC Conversion Trigger Select	      Software trigger
*   ADC Compare Function	              Disabled
*   ADC Low Power configuration	        Low power configuration
*   ADC Long Sample Time configuration	Short sample time
*   ADC Input Clock	                    Bus clock
*   ADC Data Justification Mode	        Right-justified Mode
*
* Return value:  adcErrors_t
*                 gAdcErrInvalidParam_c: One of the passed configuration parameters
*                                        is out of range. 
*                 gAdcErrNoError_c:      Everything is OK.
*
************************************************************************************/
extern adcErrors_t ADC_Init
( 
  adcConversionRateDivisor_t adcRateDivisor, //IN, Busclock divisor to set the 
                                             //    conversion rate
  adcConversionModes_t       adcMode,        //IN, select 8, 10 or 12 bits 
  uint32_t                   u32AdcPinSelect //IN, Bitmap to select the ADC pins
                                             //    to be used
);

/************************************************************************************
*
* ADC_SetSingleConversion
*
* Starts a single conversion on the ADC module, it will be performed asynchronously
* and the conversion result will be reported on the conversion callback. 
*
* Interface assumptions: 
*    The Callback pointer can not be null puinter.
*
* Return value:  adcErrors_t
*                 gAdcErrNoError_c:      Everything is OK.
*                 gAdcErrInvalidParam_c: One of the parameters is out of range.
*                 gAdcErrModuleBusy_c:   The ADC module is not Idle.
*
************************************************************************************/
extern adcErrors_t ADC_SetSingleConversion
(
 adcChannel_t    adcChannel,  //IN, On which ADC channel will be performed the 
                              //conversion.
 adcCallback_t   convCallBack //OUT,to the Event callback that shall be called at 
                              //the end of the conversion.  
); 

/************************************************************************************
*
* ADC_SetSingleConversionBlocking
*
* Performs a single conversion on the ADC module, it will be performed synchonously 
* and the conversion result will be reported at the function return on the memory 
* pointed by pu16ConvResult. 
*
* Interface assumptions: 
*    The pu16ConvResult pointer can not be null puinter.
*
* Return value:  adcErrors_t
*                 gAdcErrNoError_c:      Everything is OK.
*                 gAdcErrInvalidParam_c: One of the parameters is out of range.
*                 gAdcErrModuleBusy_c:   The ADC module is not Idle.
*
************************************************************************************/
extern adcErrors_t ADC_SetSingleConversionBlocking
(
 adcChannel_t    adcChannel,    //IN, On which ADC channel will be performed the 
                                //conversion.
 uint16_t *      pu16ConvResult //IN, Pointer to the memory where the conversion 
                                //result shall be copied.  
); 


/************************************************************************************
*
* ADC_SetContinuousConversions
*
* Starts a continuous conversion sequence on the ADC module, it will be performed 
* asynchronously and each conversion result will be reported on the conversion.
* callback. 
*
* Interface assumptions: 
*    The Callback pointer can not be null puinter.
*
* Return value:  adcErrors_t
*                 gAdcErrNoError_c:      Everything is OK.
*                 gAdcErrInvalidParam_c: One of the parameters is out of range.
*                 gAdcErrModuleBusy_c:   The ADC module is not Idle.
*
************************************************************************************/
extern adcErrors_t ADC_SetContinuousConversions
(
 adcChannel_t    adcChannel,    //IN, On which ADC channel will be performed the 
                                //conversion. 
 adcCallback_t   convCallBack   //OUT,to the Event callback that shall be called at 
                                //the end of the conversion.
); 


/************************************************************************************
*
* ADC_StopConversion
*
* Stop any single or continuous conversion sequence on the ADC module, 
* It will leave the ADC registers in the initial value as set on the init function.
*
* Interface assumptions: 
*
* Return value:  void
*
************************************************************************************/
extern void ADC_StopConversion
(
void
); 

/************************************************************************************
* ADC Interrupt service function
*
*
*
************************************************************************************/
#if gAdcMode_d == gAdcInterrupt_c   

/* Place it in NON_BANKED memory */
 #pragma CODE_SEG ISR_SEGMENT

 extern INTERRUPT_KEYWORD void ADC_Isr(void);

 #pragma CODE_SEG DEFAULT
#else
 extern void ADC_Task(void);
#endif




#endif //__ADC_INTERFACE_H__

  