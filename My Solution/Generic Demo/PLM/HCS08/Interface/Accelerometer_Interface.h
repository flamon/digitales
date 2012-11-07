/************************************************************************************
* Accelerometer interface header. 
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

                   
#ifndef __ACCELEROMETER_INTERFACE_H__               
  #define __ACCELEROMETER_INTERFACE_H__ 

#include "PortConfig.h"
#include "PLM_Config.h"
#include "derivative.h"           /* include peripheral declarations */
#include "EmbeddedTypes.h"        /* include special data types */
#include "Utilities_Interface.h"

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

#ifndef gAccelerometerSupported_d   
  #define gAccelerometerSupported_d   FALSE
#endif

typedef enum accelErr_tag
{
  gAccelNoError_c = 0,
  gAccelErrInvalidParam_c,
  gAccelErrInterfComError_c,
  gAccelErrDriverBusy_c,
  gAccelErrDataCorrupted_c,
  gAccelErrMax_c
} accelErr_t;

#if gTargetBoard_c == gMc1323xRcm_c     /* Only for MC1323xRCM board */
  typedef union axisValue_tag 
  {
   uint8_t  AxisByte;
   struct
   {
     uint8_t DataBits   :5;
     uint8_t BitSign    :1;
     uint8_t BitAlert   :1;
     uint8_t BitNotused :1;
   }AxisBits;
  }axisValue_t;
#else                                  /* Type Definition for all other boards */ 
  typedef  uint8_t   axisValue_t;
#endif
 
typedef struct accelData_tag
{
  axisValue_t XValue;
  axisValue_t YValue;
  axisValue_t ZValue;
} accelData_t;


#if gTargetBoard_c == gMc1321xSrb_c     /* Only for MC1321xSRB board */
  typedef enum accelGRange_tag
  {
    gAccelGRange1p5g_c = 0,   //800mV/g
    gAccelGRange2g_c,         //600mV/g
    gAccelGRange4g_c,         //300mV/g
    gAccelGRange6g_c,         //200mV/g
    gAccelGRangeMax_c
  } accelGRange_t;
#elif gTargetBoard_c == gMc1323xRcm_c   /* Only for MC1323xRCM board */
  typedef enum accelGRange_tag
  {
    gAccelGRangeDefault_c = 0,    // +/- 1.6g range in 6 bit (21.33 count/g)
    gAccelGRangeMax_c
  } accelGRange_t;
#else
  typedef uint8_t accelGRange_t;        /* Not used for all other boards */
#endif

#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near accelCallback_t)(accelErr_t);
#else
  typedef void(* __near accelCallback_t)(accelErr_t);
#endif

/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/

#if TRUE == gAccelerometerSupported_d
 extern accelData_t gAccelCalibrationValues;
#endif 


/************************************************************************************
*************************************************************************************
* Macro definitions
*************************************************************************************
************************************************************************************/



/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
* Accel_Init
*
* Description: This function initializes the Accelerometer driver by validating if 
* the communication with the external accelerometer is working while reading the 
* calibration initial values. 
*
* Interface assumptions: If the application targets 1323xRCM, the user must enable 
* I2C module interface. If the application targets !321xSRB, the user must enable ADC 
* module with the following function and parameters recommendation:   
*  ADC_Init(gAdcRateDivisor_4_c, gAdcMode8bit_c,0x0000001C)     
* Please refer to ADC_Interface.h
* 
* Return value:  accelErr_t 
*   gAccelNoError_c - Function operation was completed succesfully.
*   gAccelErrSerialComNoInit_c - The I2C was not prevously configured prior to call 
*     this function
*   return gAccelErrSeriaComError_c - I2C communication error
*
************************************************************************************/

extern accelErr_t Accel_Init (void);

/************************************************************************************
*
* Accel_GetXYZMeasurements
*
* Description: This function triggers the acquisition of data from the accelerometer.
*
* Interface assumptions:  This function will work properly only if the accelerometer
* driver was previosly initialized by using Accel_Init. This should be done only once.
* 
* Return value: accelErr_t 
*   gAccelNoError_c - Function operation was completed succesfully.
*   gAccelErrInvalidParam_c - An invalid IN/OUT parameter has been detected.
*   gAccelErrSeriaComError_c - I2C communication error 
*   gAccelErrDriverBusy_c - The driver is busy.
*.
************************************************************************************/

extern accelErr_t Accel_GetXYZMeasurements 
(
  accelData_t* XYZValues,         // OUT: This parameter points to a structure where the 
                                  // X, Y, and Z measuremetns will be stored.
  accelCallback_t accelCallback   // OUT: Callback for the Accel_GetXYZMeasurements process.                           
);

/************************************************************************************
*
* Accel_ConfigGravity
*
* Description: This function configures the Gravity Range for the measurements obtained 
* from the accelerometer .
*
* Interface assumptions: This function only works for 1321xSRB target
* 
* Return value: accelErr_t
* gAccelNoError_c - Function operation was completed succesfully. 
* gAccelErrInvalidParam_c - The user introduced a parameter not supported by the  function.            
*.
************************************************************************************/

#if gTargetBoard_c == gMc1321xSrb_c   /* Only for MC1321xSRB board */
 extern accelErr_t Accel_ConfigGravity 
 (
   accelGRange_t  AccelGRange    // IN: Parameter to configure the Gravity value
 );
#endif


#endif //__ACCELEROMETER_INTERFACE_H__

