/*****************************************************************************
* TouchPad driver implementation.
* 
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/
#include "TouchPad.h"

#if TRUE == gTouchpadSupported_d

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
static touchpadCallback_t tpEventCallback;
static uint8_t touchData[19] = {0};
static uint8_t tpRegAddress;
static touchpadEventStates_t eventState = mTpEvStateNoInit_c;
static touchpadEvent_t eventData;

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* Touchpad_DriverInit
*
************************************************************************************/
touchpadErrors_t Touchpad_DriverInit
(
 touchpadCallback_t userCallBack, 
 gpioPort_t AttnPort, 
 gpioPin_t AttnPin
)
{
 #if (TRUE == gTouchpadParametersValidation_d)  
  if(NULL == userCallBack)
  {
    return gErrInvalidParameter_c;
  }
 #endif 
  tpEventCallback = userCallBack;
  eventState = mTpEvStateIdle_c;
  if(gGpioPinStateActive_c == Gpio_PinValue(AttnPort,AttnPin)) 
  {
    tpRegAddress = mTpIntStatusRegAdd_c;
    (void)IIC_WriteBlocking(mTouchpadIicSlaveAddress_c, &tpRegAddress, 1);
    (void)IIC_ReadBlocking(mTouchpadIicSlaveAddress_c, &tpRegAddress, 1);
    (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
  }
  
  return gErrNoError_c;
}


/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/************************************************************************************
*
* Touchpad_EventHandler
*
************************************************************************************/
void Touchpad_EventHandler(void)
{
  if(mTpEvStateIdle_c == eventState)
  {
    tpRegAddress = mTpIntStatusRegAdd_c;
    eventState = mTpEvStateWrittingInterruptStatusRegisterAddress_c;
    (void)IIC_Write(mTouchpadIicSlaveAddress_c, &tpRegAddress,1, tpEventPollCallBack);
  }
}
#pragma CODE_SEG DEFAULT

/************************************************************************************
*************************************************************************************
* private functions
*************************************************************************************
************************************************************************************/
/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/***********************************************************************************/
void tpEventPollCallBack(iicResult_t IicResult)
{
  iicCallback_t handlingEvent;
  (void)IicResult;
  
  if(mTpEvStateWrittingInterruptStatusRegisterAddress_c == eventState)
  {
    eventState = mTpEvStateReadingInterruptStatusRegisterData_c;
    (void)IIC_Read(mTouchpadIicSlaveAddress_c, touchData,1, tpEventPollCallBack);
  }
  else if(mTpEvStateReadingInterruptStatusRegisterData_c == eventState)
  {
    eventState = mTpEvStateWrittingEventDataAddress_c;
    if(touchData[0] & mIntStatusGpioMask_c)
    {
      tpRegAddress = mTpGpioDataAddress_c;
      handlingEvent = tpHandlingGpioEvent;  
    }
    else if(touchData[0] & mIntStatusAbs0Mask_c)
    {
      tpRegAddress = mTpAbs0DataAddress_c; 
      handlingEvent = tpHandlingAbs0Event;  
    }
    else if(touchData[0] & mIntStatusDevStatusMask_c)
    {
      tpRegAddress = mTpDevStatusDataAddress_c; 
      handlingEvent = tpHandlingDevStatusEvent;  
    }
  #if(TRUE == mTouchpadFlashSupportEnable_d)
    else if(touchData[0] & mIntStatusFlashMask_c)
    {
      tpRegAddress = mTpFlashDataAddress_c; 
      handlingEvent = tpHandlingFlashEvent;  
    }
  #endif    
    else /*There are not events to handle*/
    {
      (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
      eventState = mTpEvStateIdle_c;
      return;
    }
    (void)IIC_Write(mTouchpadIicSlaveAddress_c, &tpRegAddress,1, handlingEvent);
  }
}

/***********************************************************************************/
void tpHandlingGpioEvent(iicResult_t IicResult)
{
  (void)IicResult;
  if(mTpEvStateWrittingEventDataAddress_c == eventState)
  {
    eventState = mTpEvStateReadingEventData_c;
    (void)IIC_Read(mTouchpadIicSlaveAddress_c, touchData,mTpGpioDataSize_c, tpHandlingGpioEvent);
  }
  else if(mTpEvStateReadingEventData_c == eventState)
  {
    eventData.EventType = gTouchpadGpioEvent_c; 
    eventData.EventDataReport.gpioReport.u8Byte = touchData[0];
    (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
    eventState = mTpEvStateIdle_c;
    tpEventCallback(&eventData);
  }
}

/***********************************************************************************/
void tpHandlingAbs0Event(iicResult_t IicResult)
{
  (void)IicResult;
  if(mTpEvStateWrittingEventDataAddress_c == eventState)
  {
    eventState = mTpEvStateReadingEventData_c;
    (void)IIC_Read(mTouchpadIicSlaveAddress_c, touchData,mTpAbs0DataSize_c, tpHandlingAbs0Event);
  }
  else if(mTpEvStateReadingEventData_c == eventState)
  {
    if(mPinchMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadPinchGestureEvent_c; 
      eventData.EventDataReport.pinchReport = touchData[13];
    }
    else if(mFlickMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadFlickGestureEvent_c; 
      eventData.EventDataReport.flickReport.i8XFlickDistance = touchData[13];
      eventData.EventDataReport.flickReport.i8YFlickDistance = touchData[14];
      eventData.EventDataReport.flickReport.uFlickTime =       touchData[15];
    }
    else if(mEarlyTapMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadEarlyTapGestureEvent_c; 
    }
    else if(mDoubleTapMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadDoubleTapGestureEvent_c; 
    }
    else if(mTapAndHoldMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadTapAndHoldGestureEvent_c; 
    }
    else if(mSingleTapMask_c & touchData[11])
    {
      eventData.EventType = gTouchpadSingleTapGestureEvent_c; 
    }
    else
    {
      eventData.EventType = gTouchpadFingerPositionEvent_c; 
      eventData.EventDataReport.fingerPosReport.FingersState.u8Byte = touchData[0];
      eventData.EventDataReport.fingerPosReport.u16Finger0XPosition = ((uint16_t)touchData[1])<<4;
      eventData.EventDataReport.fingerPosReport.u16Finger0XPosition += (uint16_t)(touchData[3]&0x0F);
      eventData.EventDataReport.fingerPosReport.u16Finger0YPosition = ((uint16_t)touchData[2])<<4;
      eventData.EventDataReport.fingerPosReport.u16Finger0YPosition += (uint16_t)(touchData[3]>>4);
      eventData.EventDataReport.fingerPosReport.Finger0Wide.u8Byte = touchData[4];
      eventData.EventDataReport.fingerPosReport.u8Finger0ZPosition = touchData[5];

      eventData.EventDataReport.fingerPosReport.u16Finger1XPosition = ((uint16_t)touchData[6])<<4;
      eventData.EventDataReport.fingerPosReport.u16Finger1XPosition += (uint16_t)(touchData[8]&0x0F);
      eventData.EventDataReport.fingerPosReport.u16Finger1YPosition = ((uint16_t)touchData[7])<<4;
      eventData.EventDataReport.fingerPosReport.u16Finger1YPosition += (uint16_t)(touchData[8]>>4);
      eventData.EventDataReport.fingerPosReport.Finger1Wide.u8Byte = touchData[9];
      eventData.EventDataReport.fingerPosReport.u8Finger1ZPosition = touchData[10];
    }
    (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
    eventState = mTpEvStateIdle_c;
    tpEventCallback(&eventData);
  }
}

/***********************************************************************************/
void tpHandlingDevStatusEvent(iicResult_t IicResult)
{
  (void)IicResult;
  if(mTpEvStateWrittingEventDataAddress_c == eventState)
  {
    eventState = mTpEvStateReadingEventData_c;
    (void)IIC_Read(mTouchpadIicSlaveAddress_c, touchData,mTpDevStatusDataSize_c, tpHandlingDevStatusEvent);
  }
  else if(mTpEvStateReadingEventData_c == eventState)
  {
    eventData.EventType = gTouchpadDevStatusEvent_c;
    eventData.EventDataReport.devStatusReport.u8DeviceStatus = touchData[0];
    eventData.EventDataReport.devStatusReport.u8InterruptStatus = touchData[1];
    (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
    eventState = mTpEvStateIdle_c;
    tpEventCallback(&eventData);
  }
}

/***********************************************************************************/
#if(TRUE == mTouchpadFlashSupportEnable_d)
void tpHandlingFlashEvent(iicResult_t IicResult)
{
  (void)IicResult;
  if(mTpEvStateWrittingEventDataAddress_c == eventState)
  {
    eventState = mTpEvStateReadingEventData_c;
    (void)IIC_Read(mTouchpadIicSlaveAddress_c, touchData,mTpFlashDataSize_c, tpHandlingFlashEvent);
  }
  else if(mTpEvStateReadingEventData_c == eventState)
  {
    eventData.EventType = gTouchpadFlashEvent_c; 
    (void)IIC_StopTransaction(mTouchpadIicSlaveAddress_c);
    eventState = mTpEvStateIdle_c;
    tpEventCallback(&eventData);
  }
}
#endif
#pragma CODE_SEG DEFAULT

#else                  /* FALSE == gTouchpadSupported */         


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* Touchpad_DriverInit
*
************************************************************************************/
touchpadErrors_t Touchpad_DriverInit
(
 touchpadCallback_t userCallBack, 
 gpioPort_t AttnPort, 
 gpioPin_t AttnPin
)
{
  (void)userCallBack;
  (void)AttnPort;
  (void)AttnPin;
  return gErrNoError_c;
}


/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/************************************************************************************
*
* Touchpad_EventHandler
*
************************************************************************************/
void Touchpad_EventHandler(void)
{
  
}
#pragma CODE_SEG DEFAULT

#endif                        /* gTouchpadSupported_d */
