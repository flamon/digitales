 /************************************************************************************
* Accelerometer configuration header. 
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* Freescale Semiconductor Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/

#ifndef _ACCELEROMETER_H_               
#define _ACCELEROMETER_H_

#include "Accelerometer_Interface.h"

#if gTargetBoard_c == gMc1323xRcm_c
#include "IIC_Interface.h"
#endif

#if gTargetBoard_c == gMc1321xSrb_c
#include "GPIO_Interface.h"
#include "ADC_Interface.h"
#endif



/************************************************************************************
*************************************************************************************
* Private Constants
*************************************************************************************
************************************************************************************/

#ifndef gAccelParameterValidation_d
#define gAccelParameterValidation_d TRUE 
#endif


#if gTargetBoard_c == gMc1323xRcm_c
#define	mAccelmma7660fcI2cAddress_c 	  (0x4C)  // (100 1100) + R/W bit
#define mAccelModeReg_c                 (0x07)
#define mAccelConfigValue4ActiveMode_c  (0xE1)  // Mode active, auto-awake and auto-sleep
                                                // disabled, preescaler = 16, INT output
                                                // with push-pull and active in High.
#define mAccelInterrupSetupReg_c        (0x06) 
#define mAccelXValueReg_c               (0x00)  
#define mAccelYValueReg_c               (0x01)
#define mAccelZValueReg_c               (0x02)
#define mAccelRegAlertmask_c            (0x40)                                         
                                              
#endif

#if gTargetBoard_c == gMc1321xSrb_c

#define	mAccelXoutAdc_c    (gAdcChannel_2_c)
#define	mAccelYoutAdc_c    (gAdcChannel_3_c)
#define	mAccelZoutAdc_c    (gAdcChannel_4_c)

#define mAccelXYZBitMapMask_c   (0x0000001C)
                                                                              
#endif


/************************************************************************************
*************************************************************************************
* Private Type definitions
*************************************************************************************
************************************************************************************/

typedef enum accelComStatus_tag
{
  gAccelComStatusNoInit_c = 0,
  gAccelComStatusIdle_c,
  gAccelComStatusWrtAddress_c,
  gAccelComStatusReadBusy_c, 
  gAccelComStatusMax_c
} accelComStatus_t;


/************************************************************************************
*************************************************************************************
* Private definitions
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1321xSrb_c
#define mAccel_ClearGSel1pin()    Gpio_PinClear(gGpioPortB_c,gGpioPin1Mask_c)
#define mAccel_SetGSel1pin()      Gpio_PinSet(gGpioPortB_c,gGpioPin1Mask_c)
#define mAccel_ClearGSel2pin()    Gpio_PinClear(gGpioPortB_c,gGpioPin0Mask_c)
#define mAccel_SetGSel2pin()      Gpio_PinSet(gGpioPortB_c,gGpioPin0Mask_c)

#define mAccel_PowerSaveON()      Gpio_PinClear(gGpioPortB_c,gGpioPin5Mask_c) 
#define mAccel_PowerSaveOFF()     Gpio_PinSet(gGpioPortB_c,gGpioPin5Mask_c)  
#endif

/************************************************************************************
*************************************************************************************
* Private Prototypes
*************************************************************************************
************************************************************************************/
#if TRUE == gAccelerometerSupported_d

 #ifdef MEMORY_MODEL_BANKED
    #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
    #pragma CODE_SEG DEFAULT
  #endif

// Callback function for Target Board MC1323xRCM
#if gTargetBoard_c == gMc1323xRcm_c
void  AccelWrite_IicCallback (iicResult_t IicWriteResult);
void  AccelRead_IicCallback (iicResult_t IicWriteResult);
#endif 

#pragma CODE_SEG DEFAULT

#endif
/************************************************************************************
*************************************************************************************
* Module macro definitions
*************************************************************************************
************************************************************************************/



#endif  //_ACCELEROMETER_H_ 


