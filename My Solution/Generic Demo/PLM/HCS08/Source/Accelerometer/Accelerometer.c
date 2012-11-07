/************************************************************************************
* Accelerometer driver implementation. 
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


#include "Accelerometer.h"


/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#if TRUE == gAccelerometerSupported_d

static accelCallback_t    mgAccelCallback = NULL;
static accelComStatus_t   mgAccelCurrentStatus = gAccelComStatusNoInit_c;

#if gTargetBoard_c == gMc1323xRcm_c
static uint8_t            mgReadBuffer[3];
static accelData_t*       mgXYZValues;
#endif

#endif //gAccelerometerSupported_d

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

#if TRUE == gAccelerometerSupported_d

 accelData_t   gAccelCalibrationValues;
 
#endif //gAccelerometerSupported_d

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
*  Accel_Init
*
************************************************************************************/
#if TRUE == gAccelerometerSupported_d

#if gTargetBoard_c == gMc1323xRcm_c     /* Only for MC1323xRCM board */
accelErr_t  Accel_Init (void)
{
  iicErr_t SerialComError = gIicErrNoError_c; 
  uint8_t au8TempBuffer[2] ={0};
  uint8_t u8TempCounter;
  
  
  au8TempBuffer[0] = mAccelModeReg_c;
  au8TempBuffer[1] = mAccelConfigValue4ActiveMode_c;
  
  SerialComError = IIC_WriteBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 2); 
  SerialComError = IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
  
  
  DelayMs(15);   //Delay ~ 12ms + 1/ODR 
  
  // Initalize calibration values
  gAccelCalibrationValues.XValue.AxisByte = 0;
  gAccelCalibrationValues.YValue.AxisByte = 0;
  gAccelCalibrationValues.ZValue.AxisByte = 0;
  
  // Read initial X value
  do
  {  
    u8TempCounter = 0;
    au8TempBuffer[0] = mAccelXValueReg_c;
    SerialComError = IIC_WriteBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);   
    SerialComError = IIC_ReadBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);
    SerialComError = IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
    gAccelCalibrationValues.XValue.AxisByte = au8TempBuffer[0];
    u8TempCounter++;
  }while((au8TempBuffer[0] & mAccelRegAlertmask_c) && (u8TempCounter != 3)); 
  
  if(4 <= u8TempCounter)
  {
    return gAccelErrInterfComError_c;
  }
  
  DelayMs(1);
  // Read initial Y value
  do
  {  
    u8TempCounter = 0;
    au8TempBuffer[0] = mAccelYValueReg_c;
    SerialComError = IIC_WriteBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);   
    SerialComError = IIC_ReadBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);
    SerialComError = IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
    gAccelCalibrationValues.YValue.AxisByte = au8TempBuffer[0];
    u8TempCounter++;
  }while((au8TempBuffer[0] & mAccelRegAlertmask_c) && (u8TempCounter != 3));
  
  if(4 <= u8TempCounter)
  {
    return gAccelErrInterfComError_c;
  }
  
  DelayMs(1);
  // Read initial Z value
  do
  {  
    u8TempCounter = 0;
    au8TempBuffer[0] = mAccelZValueReg_c;
    SerialComError = IIC_WriteBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);   
    SerialComError = IIC_ReadBlocking(mAccelmma7660fcI2cAddress_c,&au8TempBuffer[0], 1);
    SerialComError = IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
    gAccelCalibrationValues.ZValue.AxisByte = au8TempBuffer[0];
    u8TempCounter++;
  }while((au8TempBuffer[0] & mAccelRegAlertmask_c) && (u8TempCounter != 3));
  
  if(4 <= u8TempCounter)
  {
    return gAccelErrInterfComError_c;
  }
    
    
  if(gIicErrNoError_c != SerialComError) 
  {
    return gAccelErrInterfComError_c;
  }

  mgAccelCurrentStatus = gAccelComStatusIdle_c;
  return gAccelNoError_c;
}
#endif

#if gTargetBoard_c == gMc1321xSrb_c       /* Only for MC1321xSRB board */
accelErr_t  Accel_Init (void)
{
  adcErrors_t TempInterfaceErr;
  uint16_t u16TempBuffer = 0;
  
  mAccel_PowerSaveOFF();
  
  (void) Accel_ConfigGravity (gAccelGRange1p5g_c);
  
  DelayMs(1);
  
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelXoutAdc_c, &u16TempBuffer); 
  gAccelCalibrationValues.XValue = (uint8_t) (u16TempBuffer);
  
  DelayMs(1);
  
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelYoutAdc_c, &u16TempBuffer);
  gAccelCalibrationValues.YValue = (uint8_t) (u16TempBuffer);
  
  DelayMs(1);
  
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelZoutAdc_c, &u16TempBuffer);
  gAccelCalibrationValues.ZValue = (uint8_t) (u16TempBuffer);
  
  if(gAdcErrNoError_c != TempInterfaceErr)
  {
    return gAccelErrInterfComError_c;
  }
  
  return gAccelNoError_c;
}
#endif

#else  // FALSE == gAccelerometerSupported_d

accelErr_t  Accel_Init (void)
{
   return gAccelNoError_c;
}

#endif //gAccelerometerSupported_d
 
/************************************************************************************
*
*  Accel_GetXYZMeasurements
*
************************************************************************************/
#if TRUE == gAccelerometerSupported_d

#if gTargetBoard_c == gMc1323xRcm_c         /* Only for MC1323xRCM board */
accelErr_t Accel_GetXYZMeasurements (accelData_t* XYZValues, accelCallback_t AccelCallback) 
{
  iicErr_t SerialComError = gIicErrNoError_c;
  uint8_t au8TempBuffer = 0;

  #if(TRUE == gAccelParameterValidation_d) 
  if((NULL == XYZValues) || (NULL == AccelCallback))
  {
    return gAccelErrInvalidParam_c;
  }
  #endif  
  
  if( gAccelComStatusIdle_c != mgAccelCurrentStatus) 
  {
    return gAccelErrDriverBusy_c;  
  }
  
  mgXYZValues = XYZValues;
  mgAccelCallback = AccelCallback;     
  au8TempBuffer = mAccelXValueReg_c;
  
  SerialComError = IIC_Write (mAccelmma7660fcI2cAddress_c,&au8TempBuffer, 1,AccelWrite_IicCallback);

  if(gIicErrNoError_c != SerialComError) 
  {
    (void)IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
    return gAccelErrInterfComError_c;
  } 
  else                                
  {
    mgAccelCurrentStatus = gAccelComStatusWrtAddress_c; 
  }
  
  return gAccelNoError_c;
}
#endif

#if gTargetBoard_c == gMc1321xSrb_c     /* Only for MC1321xSRB board */
accelErr_t Accel_GetXYZMeasurements (accelData_t* XYZValues, accelCallback_t AccelCallback)
{
  adcErrors_t TempInterfaceErr;
  uint16_t    u16TempBuffer;    
    
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelXoutAdc_c, &u16TempBuffer); 
  XYZValues->XValue = (uint8_t) (u16TempBuffer);
  
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelYoutAdc_c, &u16TempBuffer);
  XYZValues->YValue = (uint8_t) (u16TempBuffer);
  
  TempInterfaceErr = ADC_SetSingleConversionBlocking (mAccelZoutAdc_c, &u16TempBuffer);
  XYZValues->ZValue = (uint8_t) (u16TempBuffer);
  
  if(gAdcErrNoError_c != TempInterfaceErr)
  {
    return gAccelErrInterfComError_c;
  }
  else
  {
    AccelCallback(gAccelNoError_c);
    return gAccelNoError_c;
  }
 
}
#endif

#else  // FALSE == gAccelerometerSupported_d

accelErr_t Accel_GetXYZMeasurements (accelData_t* XYZValues, accelCallback_t AccelCallback)
{
   (void)XYZValues;
   (void)AccelCallback;
   return gAccelNoError_c;
}

#endif //gAccelerometerSupported_d
 
 
 /************************************************************************************
*
*  Accel_ConfigGravity
*
************************************************************************************/
#if TRUE == gAccelerometerSupported_d

#if gTargetBoard_c == gMc1321xSrb_c      /* Only for MC1321xSRB board */
accelErr_t Accel_ConfigGravity (accelGRange_t AccelGRange)
{
  #if(TRUE == gAccelParameterValidation_d) 
  if(gAccelGRangeMax_c <= AccelGRange)
  {
    return gAccelErrInvalidParam_c;
  }
  #endif
  
  switch (AccelGRange)
  {
    case gAccelGRange1p5g_c:
      mAccel_ClearGSel1pin();
      mAccel_ClearGSel2pin();
    break;
    case gAccelGRange2g_c:
      mAccel_SetGSel1pin();
      mAccel_ClearGSel2pin();
    break;
    case gAccelGRange4g_c:
      mAccel_ClearGSel1pin();
      mAccel_SetGSel2pin();
    break;
    case gAccelGRange6g_c:
      mAccel_SetGSel1pin();
      mAccel_SetGSel2pin();
    break;
    case gAccelGRangeMax_c:
      mAccel_ClearGSel1pin();
      mAccel_ClearGSel2pin();
    break;
  }

  return gAccelNoError_c;
}
#endif 

#else  // FALSE == gAccelerometerSupported_d

accelErr_t Accel_ConfigGravity (accelGRange_t AccelGRange)
{
   (void)AccelGRange;
   return gAccelNoError_c;
}

#endif //gAccelerometerSupported_d

/************************************************************************************
*************************************************************************************
* Private functions
*************************************************************************************
************************************************************************************/
#if TRUE == gAccelerometerSupported_d

 #ifdef MEMORY_MODEL_BANKED
    #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
    #pragma CODE_SEG DEFAULT
  #endif

 /************************************************************************************
*
*  AccelWrite_IicCallback
*
************************************************************************************/

// Callback function for Target Board MC1323xRCM
#if gTargetBoard_c == gMc1323xRcm_c         /* Only for MC1323xRCM board */
void  AccelWrite_IicCallback (iicResult_t IicWriteResult) 
{
   bool_t  fTempErr = 0;
   iicErr_t SerialComError = gIicErrNoError_c;

   
   if(gIicTxRxSuccess_c == IicWriteResult) 
   {
    switch (mgAccelCurrentStatus) 
    {
      case gAccelComStatusWrtAddress_c:
        SerialComError = IIC_Read (mAccelmma7660fcI2cAddress_c,&mgReadBuffer[0], 3,AccelRead_IicCallback);
        mgAccelCurrentStatus = gAccelComStatusReadBusy_c;
        if(gIicErrNoError_c != SerialComError)
        {
          fTempErr = 1;
        }
      break;
      case gAccelComStatusReadBusy_c:
      case gAccelComStatusNoInit_c:
      case gAccelComStatusIdle_c:
      case gAccelComStatusMax_c:
        // ERROR
        fTempErr = 1;
      break;
    }
   }
   else 
   {
    fTempErr = 1;
   }
   
   if(fTempErr)
   {
      mgAccelCurrentStatus = gAccelComStatusIdle_c;
      (void)IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
      mgAccelCallback(gAccelErrInterfComError_c);
   }
   
   
}

 /************************************************************************************
*
*  AccelRead_IicCallback
*
************************************************************************************/

void  AccelRead_IicCallback (iicResult_t IicWriteResult) 
{
   bool_t  fTempErr = 0;
   iicErr_t SerialComError = gIicErrNoError_c;

   
   if(gIicTxRxSuccess_c == IicWriteResult) 
   {
    switch (mgAccelCurrentStatus) 
    {
      case gAccelComStatusReadBusy_c:
        if((mgReadBuffer[0] | mgReadBuffer[1] | mgReadBuffer[2]) & mAccelRegAlertmask_c) 
        {
          // Measurement with errors, generate error so the user try the measurement again
          mgAccelCurrentStatus = gAccelComStatusIdle_c;
          mgAccelCallback(gAccelErrDataCorrupted_c);
          return; 
        }
        mgXYZValues->XValue.AxisByte = mgReadBuffer[0];
        mgXYZValues->YValue.AxisByte = mgReadBuffer[1];
        mgXYZValues->ZValue.AxisByte = mgReadBuffer[2];
        SerialComError = IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
        mgAccelCurrentStatus = gAccelComStatusIdle_c;
        if(gIicErrNoError_c != SerialComError)
        {
          fTempErr = 1;
        }  
        else
        {
          mgAccelCallback(gAccelNoError_c);
        }
      break; 
      
      case gAccelComStatusNoInit_c:
      case gAccelComStatusIdle_c:
      case gAccelComStatusWrtAddress_c:
      case gAccelComStatusMax_c: 
        //ERROR
        fTempErr = 1;
      break;
    }
   }
   else 
   {
    fTempErr = 1;
   }
   
   if(fTempErr)
   {
      mgAccelCurrentStatus = gAccelComStatusIdle_c;
      (void)IIC_StopTransaction(mAccelmma7660fcI2cAddress_c);
      //Callback with error as parameter
      mgAccelCallback(gAccelErrInterfComError_c);
   }
   
}
#endif 
#pragma CODE_SEG DEFAULT 

#endif

/************************************************************************************
*************************************************************************************
* Private Debug stuff
*************************************************************************************
************************************************************************************/



/* End of Accelerometer.c */
