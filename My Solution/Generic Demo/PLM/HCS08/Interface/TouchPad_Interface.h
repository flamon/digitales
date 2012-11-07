/*****************************************************************************
* Touchpad driver Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef __TOUCHPAD_INTERFACE_H__               
  #define __TOUCHPAD_INTERFACE_H__
  
#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "PortConfig.h"     // include board specific defines
#include "IIC_Interface.h"
#include "GPIO_Interface.h"
#include "board_config.h"
#include "PLM_config.h"

#ifndef gTouchpadSupported_d 
    #define gTouchpadSupported_d FALSE
#endif 

/************************************************************************************
*************************************************************************************
* Interface Constants
*************************************************************************************
************************************************************************************/

typedef enum touchpadEventType_tag{
  gTouchpadBusError_c = 0,
  gTouchpadGpioEvent_c,
  gTouchpadFingerPositionEvent_c,
  gTouchpadPinchGestureEvent_c,
  gTouchpadFlickGestureEvent_c,
  gTouchpadEarlyTapGestureEvent_c,
  gTouchpadDoubleTapGestureEvent_c,
  gTouchpadTapAndHoldGestureEvent_c,
  gTouchpadSingleTapGestureEvent_c,
  gTouchpadDevStatusEvent_c,
  gTouchpadFlashEvent_c,
  gTouchpadEventMax_c
}touchpadEventType_t;

typedef enum touchpadErrors_tag{
  gErrNoError_c = 0,
  gErrInvalidParameter_c,
  gErrBusError_c,
  gErrMaxError_c
}touchpadErrors_t;

/*
Each finger has two status bits to describe its state. The encoding of the bits is: 
  00(0) The finger is not present. 
  01(1) The finger is present and the positional information associated with it is accurate. 
  10(2) The finger is present, however the positional information associated with it may be 
        inaccurate. 
  11(3) Reserved. 
  
  The next enumeration can be used to decode the finger states.
*/
typedef enum touchpadFingerStates_tag
{
  gFingerNotPresent_c = 0,
  gFingerPresentAccurateInfo_c,
  gFingerPresentNotAccurateInfo_c,
  gFingerReserved_c  
};

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

typedef union touchpadGpioEventReport_tag 
{
  uint8_t u8Byte;
  struct
  {
    uint8_t gpio0State :1;
    uint8_t gpio1State :1;
    uint8_t Unused     :6;  
  }GpioStates; 
}touchpadGpioEventReport_t;

typedef struct touchpadFingerPositionEventReport_tag
{
  union
  {
    uint8_t u8Byte; 
    struct 
    {
      uint8_t Finger0State :2;
      uint8_t Finger1State :2;
      uint8_t Unused       :4;   
    }States;
  }FingersState;
  /*Finger 0 specific data*/
  uint16_t u16Finger0XPosition;
  uint16_t u16Finger0YPosition;
  union
  {
    uint8_t u8Byte; 
    struct 
    {
      uint8_t X :4;
      uint8_t Y :4;
    }Wides;
  }Finger0Wide;
  uint8_t u8Finger0ZPosition;
  /*Finger 1 specific data*/
  uint16_t u16Finger1XPosition;
  uint16_t u16Finger1YPosition;
  union
  {
    uint8_t u8Byte; 
    struct 
    {
      uint8_t X :4;
      uint8_t Y :4;
    }Wides;
  }Finger1Wide;
  uint8_t u8Finger1ZPosition;
}touchpadFingerPositionEventReport_t;

typedef int8_t touchpadPinchGestureEventReport_t;


typedef struct touchpadFlickGestureEventReport_tag
{
  int8_t  i8XFlickDistance;
  int8_t  i8YFlickDistance;
  uint8_t uFlickTime;
}touchpadFlickGestureEventReport_t;

typedef struct touchpadDevStatusEventReport_tag
{
  uint8_t u8DeviceStatus;
  uint8_t u8InterruptStatus;
}touchpadDevStatusEventReport_t;

typedef struct touchpadFlashEventReport_tag
{
  uint16_t u16BlockNumber;
  uint8_t  au8BlockData[16];
  uint8_t  u8FlashControl;
}touchpadFlashEventReport_t;

typedef union touchpadEventDataReport_tag
{
  touchpadGpioEventReport_t           gpioReport;
  touchpadFingerPositionEventReport_t fingerPosReport;
  touchpadPinchGestureEventReport_t   pinchReport;
  touchpadFlickGestureEventReport_t   flickReport;
  touchpadDevStatusEventReport_t      devStatusReport;
  touchpadFlashEventReport_t          flashReport;
}touchpadEventDataReport_t;

typedef struct touchpadEvent_tag
{
  touchpadEventType_t EventType;
  touchpadEventDataReport_t EventDataReport;
}touchpadEvent_t;

#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near touchpadCallback_t)(touchpadEvent_t*);
#else
  typedef void(* __near touchpadCallback_t)(touchpadEvent_t*);
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* Touchpad_DriverInit
*
* Initialize the Touchpad driver and touchpad module.
*
* Interface assumptions: 
*   the IIC and KBI drivers have been initialized before calling Touchpad_DriverInit
*
*
* Return value: touchpadErrors_t
*               gErrInvalidParameter_c: One of the passed configuration parameters
*                                       is out of range. 
*               gErrBusError_c:         The IIC driving is getting problems
*               gErrMaxError_c:         Everything is OK.
*
************************************************************************************/
extern touchpadErrors_t Touchpad_DriverInit
(
 touchpadCallback_t userCallBack,//IN: Pointer to the function that shall be 
                                 //    called when touchpad events
 gpioPort_t AttnPort,            //IN: Mcu GPIO port where the touchpad's attention 
                                 //    line is tied.
 gpioPin_t AttnPin               //IN: Mcu GPIO pin in the AtthPort where the 
                                 //    touchpad's attention line is tied.
);

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
* The function is called to start the touchpad events poll and handle, this process 
* is made asynchronously and the result is reported on the touchpad callback.
*
* Interface assumptions: 
*   the Touchpad_DriverInit have been called.
*
*
* Return value: void
*
************************************************************************************/
extern void Touchpad_EventHandler(void);
#pragma CODE_SEG DEFAULT

#endif //__TOUCHPAD_INTERFACE_H__               
