/************************************************************************************
* This header file is for the MC1321x SRB port mappings.
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#ifdef TARGET_MC1321

#ifndef _TARGET_MC1321x_SRB_
#define _TARGET_MC1321x_SRB_
#include "derivative.h"


/************************************************************************************
*
* Define Board LCD port mapping 
*
************************************************************************************/
  #define gPC1602Fx_c   0
  #define gCFAH1602x_c  1
  #define gCFAG12832x_c 2
  #define gLCDNone_c    3
  
  #define gLCDModule_c  gLCDNone_c

/************************************************************************************
*
* Define Board LED port mapping 
*
************************************************************************************/

  #define gLedPort1   gGpioPortD_c
  #define gLed1Pin_c  0x10
  #define gLed2Pin_c  0x20
  #define gLed3Pin_c  0x40
  #define gLed4Pin_c  0x80
  #define gLedPort2   gGpioPortC_c
  #define gLed5Pin_c  0x10
  #define gLedMask_c  (gLed1Pin_c | gLed2Pin_c | gLed3Pin_c | gLed4Pin_c)

  #define gLED1_c     gLedPort1,gLed1Pin_c
  #define gLED2_c     gLedPort1,gLed2Pin_c
  #define gLED3_c     gLedPort1,gLed3Pin_c
  #define gLED4_c     gLedPort1,gLed4Pin_c
  #define gLED5_c     gLedPort2,gLed5Pin_c

/************************************************************************************
*
* Define Board Switch mapping 
*
************************************************************************************/
  #define gSwitchPort   gGpioPortA_c
  #define gSwitch1Pin_c 0x04
  #define gSwitch2Pin_c 0x08
  #define gSwitch3Pin_c 0x10
  #define gSwitch4Pin_c 0x20
  #define gSwitchMask   (gSwitch1Pin_c | gSwitch2Pin_c |  gSwitch3Pin_c | gSwitch4Pin_c)

  #define gSwitchKbiModule_c gKbi1_c
/************************************************************************************
*
* TouchPad Pins
*
************************************************************************************/

#define gTouchpadAttnPinMask_c    0x00  
 
/************************************************************************************
*
* Define Board Buzzer mapping 
*
************************************************************************************/
  #define gBuzzerPort_c gGpioPortD_c
  #define gBuzzerPin_c  gGpioPin2Mask_c

/************************************************************************************
*
* SPI Pins
*
************************************************************************************/

#define gUsePeripheral1_c         TRUE
#define gPeripheral1Port_c        gGpioPortE_c
#define gPeripheral1PinMask_c     0x04 

#define gUsePeripheral2_c         FALSE
  
/************************************************************************************
*
* Define Board Radio Pins mapping 
*
************************************************************************************/
  
  #define gAttnPort_c           gGpioPortD_c
  #define gAttnPinMask_c        0x01
 
  #define gRxTxEnablePort_c     gGpioPortD_c
  #define gRxTxEnablePinMask_c  0x02
 
 
  #define gResetRadioPort_c     gGpioPortD_c
  #define gResetRadioPinMask_c  0x08

  #define gOutOfIdlePort_c      gGpioPortE_c
  #define gOutOfIdlePinMask_c   0x80

  
  #define gCrcValidPort_c       gGpioPortE_c
  #define gCrcValidPinMask_c    0x40

  //#define gAntSwPort         gGpioPortB_c// No antenna switch port defined for Toroweap
  //#define gAntSwMask_c       0// No antenna switch port defined for Toroweap
  
  #define gUseExternalAntennaSwitch_c FALSE


 
/************************************************************************************
*
* Define Board GPIO Initializarion values
*
************************************************************************************/
  #define gPinPullUpEnable_c       0x01
  #define gPinPullUpDisable_c      0x00
  
  #define gPinDirectionOutput_c    0x01
  #define gPinDirectionInput_c     0x00
  
  #define gPinLevelHigh_c          0x01
  #define gPinLevelLow_c           0x00
  
  #define gPinDriveStrengthHigh_c  0x01
  #define gPinDriveStrengthLow_c   0x00
  
  #define gPinSlewRateEnable_c     0x01
  #define gPinSlewRateDisable_c    0x00
  
  #define gPinLogicInverse_c       0x01 
  #define gPinLogicDirect_c        0x00 
  
  #define PortSetup(b7, b6, b5, b4, b3, b2, b1, b0)    ( ((b7) << 7) | ((b6) << 6) | ((b5) << 5) | ((b4) << 4) | ((b3) << 3) | ((b2) << 2) | ((b1) << 1) | (b0))


 /* Port A. Add pins for application if needed */  
 #define gPTADValue_c       PortSetup(\
                                       gPinLevelLow_c,           /* Port A, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 5 -> SW4 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 4 -> SW3 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 3 -> SW2 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 2 -> SW1 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 1 */ \
                                       gPinLevelLow_c            /* Port A, Pin 0 */ \
                                     )
#define gPTAPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port A, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 6 */ \
                                       gPinPullUpEnable_c,       /* Port A, Pin 5 -> SW4 */ \
                                       gPinPullUpEnable_c,       /* Port A, Pin 5 -> SW3 */ \
                                       gPinPullUpEnable_c,       /* Port A, Pin 5 -> SW2 */ \
                                       gPinPullUpEnable_c,       /* Port A, Pin 5 -> SW1 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port A, Pin 0 */ \
                                      )
#define gPTADDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port A, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 6 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 5 -> SW4 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 4 -> SW3 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 3 -> SW2 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 2 -> SW1 */ \
                                       gPinDirectionOutput_c,     /* Port A, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port A, Pin 0 */ \
                                     )
#define gPTADSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 5 -> SW4 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 4 -> SW3 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 3 -> SW2 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 2 -> SW1 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port A, Pin 0 */ \
                                     ) 
#define gPTASEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port A, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 5 -> SW4 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 4 -> SW3 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 3 -> SW2 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 2 -> SW1 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port A, Pin 0 */ \
                                     ) 
                                              
#define gPTALogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port A, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 5 -> SW4 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 4 -> SW3 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 3 -> SW2 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 2 -> SW1 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port A, Pin 0 */ \
                                     ) 
  
                        
/* Port B. Add pins for application if needed */  
#define gPTBDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port B, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinLevelLow_c,           /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinLevelLow_c,           /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinLevelLow_c,           /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinLevelLow_c,           /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinLevelLow_c            /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                     )
#define gPTBPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port B, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinPullUpDisable_c       /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                              )
#define gPTBDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port B, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinDirectionInput_c,     /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinDirectionInput_c,     /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinDirectionInput_c,     /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinDirectionOutput_c     /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                     )
#define gPTBDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinDriveStrengthLow_c    /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                     )                                             
#define gPTBSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port B, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinSlewRateDisable_c     /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                     ) 
                                              
#define gPTBLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port B, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 5 -> ACCELEROMETER Power Save*/ \
                                       gPinLogicDirect_c,        /* Port B, Pin 4 -> ACCELEROMETER ZOUT */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 3 -> ACCELEROMETER YOUT */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 2 -> ACCELEROMETER XOUT */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 1 -> ACCELEROMETER g-Select 1 */ \
                                       gPinLogicDirect_c         /* Port B, Pin 0 -> ACCELEROMETER g-Select 2 */ \
                                     ) 
        
  
/* Port C. Add pins for application if needed */  
#define gPTCDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port C, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 4 -> LED5 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 1 */ \
                                       gPinLevelLow_c            /* Port C, Pin 0 */ \
                                     )
 #define gPTCPEValue_c      PortSetup(\
                                       gPinPullUpDisable_c,      /* Port C, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 4 -> LED5 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port C, Pin 0 */ \
                                     )
#define gPTCDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port C, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 4 -> LED5 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port C, Pin 0 */ \
                                     )
#define gPTCDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 4 -> LED5 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port C, Pin 0 */ \
                                     )                                               
#define gPTCSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port C, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 4 -> LED5 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port C, Pin 0 */ \
                                     ) 
                                              
#define gPTCLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port C, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 5 */ \
                                       gPinLogicInverse_c,        /* Port C, Pin 4 -> LED5 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port C, Pin 0 */ \
                                     ) 


/* Port D. Add pins for application if needed */                                                                                         
#define gPTDDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port D, Pin 7 -> LED4 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 6 -> LED3 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 5 -> LED2 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 4 -> LED1 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 3 -> RESET */ \
                                       gPinLevelLow_c,           /* Port D, Pin 2 -> */ \
                                       gPinLevelLow_c,           /* Port D, Pin 1 -> RTXEN */ \
                                       gPinLevelHigh_c           /* Port D, Pin 0 -> ATTN */ \
                                     )
#define gPTDPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port D, Pin 7 -> LED4 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 6 -> LED3 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 5 -> LED2 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 4 -> LED1  */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 3 -> RESET */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 2 -> */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 1 -> RTXEN */ \
                                       gPinPullUpDisable_c       /* Port D, Pin 0 -> ATTN */ \
                                     )
#define gPTDDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port D, Pin 7 -> LED4 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 6 -> LED3 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 5 -> LED2 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 4 -> LED1 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 3 -> RESET */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 2 -> */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 1 -> RTXEN */ \
                                       gPinDirectionOutput_c     /* Port D, Pin 0 -> ATTN */ \
                                     )
#define gPTDDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 7 -> LED4 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 6 -> LED3 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 5 -> LED2 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 4 -> LED1 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 3 -> RESET */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 2 -> */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 1 -> RTXEN */ \
                                       gPinDriveStrengthLow_c    /* Port D, Pin 0 -> ATTN */ \
                                     )                                               
#define gPTDSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port D, Pin 7 -> LED4 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 6 -> LED3  */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 5 -> LED2 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 4 -> LED1 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 3 -> RESET */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 2 -> */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 1 -> RTXEN */ \
                                       gPinSlewRateDisable_c     /* Port D, Pin 0 -> ATTN */ \
                                     ) 
                                              
#define gPTDLogicValue_c    PortSetup(\
                                       gPinLogicInverse_c,        /* Port D, Pin 7 -> LED4 */ \
                                       gPinLogicInverse_c,        /* Port D, Pin 6 -> LED3  */ \
                                       gPinLogicInverse_c,        /* Port D, Pin 5 -> LED2 */ \
                                       gPinLogicInverse_c,        /* Port D, Pin 4 -> LED1  */ \
                                       gPinLogicInverse_c,       /* Port D, Pin 3 -> RESET */ \
                                       gPinLogicDirect_c,        /* Port D, Pin 2 -> */ \
                                       gPinLogicDirect_c,        /* Port D, Pin 1 -> RTXEN */ \
                                       gPinLogicInverse_c        /* Port D, Pin 0 -> ATTN */ \
                                     ) 

  
/* Port E - Used by MC1319x driver. Add pins for application if needed*/
#define gPTEDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinLevelLow_c,           /* Port E, Pin 6 -> GPIO2 */ \
                                       gPinLevelLow_c,           /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinLevelHigh_c,          /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinLevelLow_c,           /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinLevelHigh_c,          /* Port E, Pin 2 -> SPI SS */ \
                                       gPinLevelLow_c,           /* Port E, Pin 1 -> SCI RX */ \
                                       gPinLevelLow_c            /* Port E, Pin 0 -> SCI TX */ \
                                     )
#define gPTEPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 6 -> GPIO2 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 2 -> SPI SS */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 1 -> SCI RX */ \
                                       gPinPullUpDisable_c       /* Port E, Pin 0 -> SCI TX */ \
                                              )
#define gPTEDDValue_c       PortSetup(\
                                       gPinDirectionInput_c,     /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinDirectionInput_c,     /* Port E, Pin 6 -> GPIO2 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinDirectionInput_c,     /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 2 -> SPI SS */ \
                                       gPinDirectionInput_c,     /* Port E, Pin 1 -> SCI RX */ \
                                       gPinDirectionOutput_c     /* Port E, Pin 0 -> SCI TX */ \
                                     )
#define gPTEDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 6 -> GPIO2 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 2 -> SPI SS */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 1 -> SCI RX */ \
                                       gPinDriveStrengthLow_c    /* Port E, Pin 0 -> SCI TX */ \
                                     )                                             
#define gPTESEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 6 -> GPIO1 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 2 -> SPI1 CLK */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 1 -> SCI1 TX */ \
                                       gPinSlewRateDisable_c     /* Port E, Pin 0 -> SCI1 RX */ \
                                     ) 
                                              
#define gPTELogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port E, Pin 7 -> GPIO1 */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 6 -> GPIO1 */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 5 -> SPI CLK */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 4 -> SPI MOSI */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 3 -> SPI MISO */ \
                                       gPinLogicInverse_c,       /* Port E, Pin 2 -> SPI SS  */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 1 -> SPI RX  */ \
                                       gPinLogicDirect_c         /* Port E, Pin 0 -> SPI TX */ \
                                     ) 
                                       
  
/* Port F - Not used by MC1319x driver. Available for application.*/
#define gPTFDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port F, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port F, Pin 1 */ \
                                       gPinLevelLow_c            /* Port F, Pin 0 */ \
                                     )
#define gPTFPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port F, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port F, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port F, Pin 0 */ \
                                     )
#define gPTFDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port F, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port F, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port F, Pin 0 */ \
                                     )
#define gPTFDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port F, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port F, Pin 0 */ \
                                     )
#define gPTFSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port F, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port F, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port F, Pin 0 */ \
                                     )                                                                        
#define gPTFLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port F, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port F, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port F, Pin 0 */ \
                                     ) 
                           

/* Port G - Not used by MC1319x driver. Available for application.*/
#define gPTGDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port G, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port G, Pin 1 */ \
                                       gPinLevelLow_c            /* Port G, Pin 0 */ \
                                     )
#define gPTGPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port G, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port G, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port G, Pin 0 */ \
                                     )
#define gPTGDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port G, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port G, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port G, Pin 0 */ \
                                     )
#define gPTGDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port G, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port G, Pin 0 */ \
                                     )
#define gPTGSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port G, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port G, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port G, Pin 0 */ \
                                     ) 
                                              
#define gPTGLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port G, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port G, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port G, Pin 0 */ \
                                     ) 
    						              


#endif /* _TARGET_MC1321x_SRB_ */
#endif
