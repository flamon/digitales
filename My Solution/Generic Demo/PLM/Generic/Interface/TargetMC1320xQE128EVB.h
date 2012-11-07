/******************************************************************************
* This header file is for the MC1320x QE128 EVB port mappings.
*
* (c) Copyright 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
******************************************************************************/
#ifdef TARGET_MC1320

#ifndef _TARGET_QE128_EVB_
#define _TARGET_QE128_EVB_
#include "derivative.h"
#include "EmbeddedTypes.h"


/************************************************************************************
*
* Define Board LCD port mapping 
*
************************************************************************************/
  #define gPC1602Fx_c        0
  #define gCFAH1602x_c       1
  #define gCFAG12832x_c      2
  #define gLCDNone_c         3
  
  #define gLCDModule_c       gCFAH1602x_c
  
  #define gLcdRegSelectPin_c 0x20                    
  #define gLcdEnablePin_c    0x40  
  #define gLcdReadWritePin_c 0x80  
  #define gLcdCtrlPort_c     0x03
  #define gLcdDataPort_c     0x05

/************************************************************************************
*
* Define Board LED port mapping 
*
************************************************************************************/
  
  #define gLedPort    gGpioPortE_c
  #define gLed1Pin_c  0x08
  #define gLed2Pin_c  0x10
  #define gLed3Pin_c  0x20
  #define gLed4Pin_c  0x40
  #define gLedMask_c  (gLed1Pin_c | gLed2Pin_c | gLed3Pin_c | gLed4Pin_c)

  #define   gLED1_c   gLedPort,gLed1Pin_c
  #define   gLED2_c   gLedPort,gLed2Pin_c
  #define   gLED3_c   gLedPort,gLed3Pin_c
  #define   gLED4_c   gLedPort,gLed4Pin_c
  

/************************************************************************************
*
* Define Board EEPROM port mapping 
*
************************************************************************************/


  #define gEepromWPPinMask_c          (0x20)
  #define gEepromWriteProtectPin_c    gGpioPortC_c,gEepromWPPinMask_c   

/************************************************************************************
*
* Define Board Switch mapping 
*
************************************************************************************/
  #define gSwitchPort   gGpioPortD_c
  #define gSwitch1Pin_c 0x01
  #define gSwitch2Pin_c 0x02
  #define gSwitch3Pin_c 0x04
  #define gSwitch4Pin_c 0x08
  #define gSwitchMask   (gSwitch1Pin_c | gSwitch2Pin_c |  gSwitch3Pin_c | gSwitch4Pin_c)

  #define gSwitchKbiModule_c gKbi2_c

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
  #define gBuzzerPort_c gGpioPortE_c
  #define gBuzzerPin_c  gGpioPin2Mask_c
  
/************************************************************************************
*
* SPI Pins
*
************************************************************************************/

#define gUsePeripheral1_c         TRUE
#define gPeripheral1Port_c        gGpioPortB_c
#define gPeripheral1PinMask_c     0x20

#define gUsePeripheral2_c         FALSE

/************************************************************************************
*
* Define Board Radio Pins mapping 
*
************************************************************************************/

  #define gAttnPort_c           gGpioPortC_c
  #define gAttnPinMask_c        0x02
 
  #define gRxTxEnablePort_c     gGpioPortC_c
  #define gRxTxEnablePinMask_c  0x04
 
 
  #define gResetRadioPort_c     gGpioPortC_c
  #define gResetRadioPinMask_c  0x01

  #define gOutOfIdlePort_c      gGpioPortC_c
  #define gOutOfIdlePinMask_c   0x08

  
  #define gCrcValidPort_c       gGpioPortC_c
  #define gCrcValidPinMask_c    0x10


  #define gAntSwPort            gGpioPortC_c 
  #define gAntSwMask_c          0x40         

  #define gUseExternalAntennaSwitch_c TRUE



  #define gMC1319xSPIPort        PTBD
  
  #define gMC1319xSpiSsMask_c    0x20
  #define gMC1319xSpiMisoMask_c  0x08
  #define gMC1319xSpiMosiMask_c  0x04
  #define gMC1319xSpiSpsckMask_c 0x20
  

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
                                       gPinLevelLow_c,           /* Port A, Pin 5 -> IRQ */ \
                                       gPinLevelLow_c,           /* Port A, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 2 -> SCL */ \
                                       gPinLevelLow_c,           /* Port A, Pin 1 -> SDA */ \
                                       gPinLevelLow_c            /* Port A, Pin 0 */ \
                                     )
#define gPTAPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port A, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 6 */ \
                                       gPinPullUpEnable_c,       /* Port A, Pin 5 -> IRQ */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 2 -> SCL */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 1 -> SDA */ \
                                       gPinPullUpDisable_c       /* Port A, Pin 0 */ \
                                      )
#define gPTADDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port A, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 6 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 5 -> IRQ */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 3 */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 2 -> SCL */ \
                                       gPinDirectionInput_c,     /* Port A, Pin 1 -> SDA */ \
                                       gPinDirectionOutput_c     /* Port A, Pin 0 */ \
                                     )
#define gPTADSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 5 -> IRQ */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 2 -> SCL */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 1 -> SDA */ \
                                       gPinDriveStrengthLow_c    /* Port A, Pin 0 */ \
                                     ) 
#define gPTASEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port A, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 5 -> IRQ */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 2 -> SCL */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 1 -> SDA */ \
                                       gPinSlewRateDisable_c     /* Port A, Pin 0 */ \
                                     ) 
                                              
#define gPTALogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port A, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 5 -> IRQ */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 2 -> SCL */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 1 -> SDA */ \
                                       gPinLogicDirect_c         /* Port A, Pin 0 */ \
                                     ) 
                                                                                          
/* Port B. Add pins for application if needed */  
#define gPTBDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port B, Pin 7 -> EXTAL */ \
                                       gPinLevelLow_c,           /* Port B, Pin 6 */ \
                                       gPinLevelHigh_c,          /* Port B, Pin 5 -> SPI1 SS */ \
                                       gPinLevelLow_c,           /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinLevelHigh_c,          /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinLevelLow_c,           /* Port B, Pin 2 -> SPI1 CLK */ \
                                       gPinLevelLow_c,           /* Port B, Pin 1 -> SCI1 TX */ \
                                       gPinLevelLow_c            /* Port B, Pin 0 -> SCI1 RX */ \
                                     )
#define gPTBPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port B, Pin 7 -> EXTAL */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 5 -> SPI1 SS */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 2 -> SPI1 CLK */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 1 -> SCI1 TX */ \
                                       gPinPullUpDisable_c       /* Port B, Pin 0 -> SCI1 RX */ \
                                              )
#define gPTBDDValue_c       PortSetup(\
                                       gPinDirectionInput_c,     /* Port B, Pin 7 -> EXTAL */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 5 -> SPI1 SS */ \
                                       gPinDirectionInput_c,     /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 2 -> SPI1 CLK */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 1 -> SCI1 TX */ \
                                       gPinDirectionInput_c      /* Port B, Pin 0 -> SCI1 RX */ \
                                     )
#define gPTBDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 7 -> EXTAL */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 6 */ \
                                       gPinDriveStrengthHigh_c,  /* Port B, Pin 5 -> SPI1 SS (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 2 -> SPI1 CLK */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 1 -> SCI1 TX */ \
                                       gPinDriveStrengthLow_c    /* Port B, Pin 0 -> SCI1 RX */ \
                                     )                                             
#define gPTBSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port B, Pin 7 -> EXTAL */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 5 -> SPI1 SS (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 2 -> SPI1 CLK */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 1 -> SCI1 TX */ \
                                       gPinSlewRateDisable_c     /* Port B, Pin 0 -> SCI1 RX */ \
                                     ) 
                                              
#define gPTBLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port B, Pin 7 -> EXTAL */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 6 */ \
                                       gPinLogicInverse_c,       /* Port B, Pin 5 -> SPI1 SS (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 4 -> SPI1 MISO */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 3 -> SPI1 MOSI */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 2 -> SPI1 CLK  */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 1 -> SCI1 TX  */ \
                                       gPinLogicDirect_c         /* Port B, Pin 0 -> SCI1 RX */ \
                                     ) 
                                                                                                                               
/* Port C. Add pins for application if needed */                                                                                         
#define gPTCDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port C, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinLevelLow_c,           /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinLevelLow_c,           /* Port C, Pin 4 -> GPIO2 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 2 -> RTXEN */ \
                                       gPinLevelHigh_c,          /* Port C, Pin 1 -> ATTN */ \
                                       gPinLevelLow_c            /* Port C, Pin 0 -> RSTO */ \
                                     )
#define gPTCPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port C, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 4 -> GPIO2 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 2 -> RTXEN */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 1 -> ATTN */ \
                                       gPinPullUpDisable_c       /* Port C, Pin 0 -> RSTO */ \
                                     )
#define gPTCDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port C, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinDirectionInput_c,     /* Port C, Pin 4 -> GPIO2 */ \
                                       gPinDirectionInput_c,     /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 2 -> RTXEN */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 1 -> ATTN */ \
                                       gPinDirectionOutput_c     /* Port C, Pin 0 -> RSTO */ \
                                     )
#define gPTCDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 4 -> GPIO2 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinDriveStrengthHigh_c,  /* Port C, Pin 2 -> RTXEN (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinDriveStrengthHigh_c,  /* Port C, Pin 1 -> ATTN */ \
                                       gPinDriveStrengthHigh_c   /* Port C, Pin 0 -> RSTO */ \
                                     )                                               
#define gPTCSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port C, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 4 -> GPIO2 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 2 -> RTXEN (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 1 -> ATTN */ \
                                       gPinSlewRateDisable_c     /* Port C, Pin 0 -> RSTO */ \
                                     ) 
                                              
#define gPTCLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port C, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 6 -> TRX AntSw */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 5 -> EEPROM WP */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 4 -> GPIO1 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 3 -> GPIO1 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 2 -> RTXEN (High drive strength required for fast switching at high speed SPI) */ \
                                       gPinLogicInverse_c,       /* Port C, Pin 1 -> ATTN */ \
                                       gPinLogicInverse_c        /* Port C, Pin 0 -> RSTO */ \
                                     ) 
                            
/* Port D. Add pins for application if needed */  
#define gPTDDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port D, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 3 -> KBI4 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 2 -> KBI3 */ \
                                       gPinLevelLow_c,           /* Port D, Pin 1 -> KBI2 */ \
                                       gPinLevelLow_c            /* Port D, Pin 0 -> KBI1 */ \
                                     )
 #define gPTDPEValue_c      PortSetup(\
                                       gPinPullUpDisable_c,      /* Port D, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port D, Pin 4 */ \
                                       gPinPullUpEnable_c,       /* Port D, Pin 3 -> KBI4 */ \
                                       gPinPullUpEnable_c,       /* Port D, Pin 2 -> KBI3 */ \
                                       gPinPullUpEnable_c,       /* Port D, Pin 1 -> KBI2 */ \
                                       gPinPullUpEnable_c        /* Port D, Pin 0 -> KBI1 */ \
                                     )
#define gPTDDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port D, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port D, Pin 4 */ \
                                       gPinDirectionInput_c,     /* Port D, Pin 3 -> KBI4 */ \
                                       gPinDirectionInput_c,     /* Port D, Pin 2 -> KBI3 */ \
                                       gPinDirectionInput_c,     /* Port D, Pin 1 -> KBI2 */ \
                                       gPinDirectionInput_c      /* Port D, Pin 0 -> KBI1 */ \
                                     )
#define gPTDDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 3 -> KBI4 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 2 -> KBI3 */ \
                                       gPinDriveStrengthLow_c,   /* Port D, Pin 1 -> KBI2 */ \
                                       gPinDriveStrengthLow_c    /* Port D, Pin 0 -> KBI1 */ \
                                     )                                               
#define gPTDSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port D, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 3 -> KBI4 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 2 -> KBI3 */ \
                                       gPinSlewRateDisable_c,    /* Port D, Pin 1 -> KBI2 */ \
                                       gPinSlewRateDisable_c     /* Port D, Pin 0 -> KBI1 */ \
                                     ) 
                                              
#define gPTDLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port D, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port D, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port D, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port D, Pin 4 */ \
                                       gPinLogicInverse_c,       /* Port D, Pin 3 -> KBI4 */ \
                                       gPinLogicInverse_c,       /* Port D, Pin 2 -> KBI3 */ \
                                       gPinLogicInverse_c,       /* Port D, Pin 1 -> KBI2 */ \
                                       gPinLogicInverse_c        /* Port D, Pin 0 -> KBI1 */ \
                                     ) 

/* Port E. Add pins for application if needed */  
#define gPTEDValue_c        PortSetup(\
                                       gPinLevelHigh_c,           /* Port E, Pin 7 */ \
                                       gPinLevelHigh_c,           /* Port E, Pin 6 -> LED4 */ \
                                       gPinLevelHigh_c,           /* Port E, Pin 5 -> LED3 */ \
                                       gPinLevelHigh_c,           /* Port E, Pin 4 -> LED2 */ \
                                       gPinLevelHigh_c,           /* Port E, Pin 3 -> LED1 */ \
                                       gPinLevelLow_c,            /* Port E, Pin 2 -> Buzzer */ \
                                       gPinLevelHigh_c,           /* Port E, Pin 1 */ \
                                       gPinLevelHigh_c            /* Port E, Pin 0 */ \
                                     )
#define gPTEPEValue_c      PortSetup(\
                                       gPinPullUpDisable_c,      /* Port E, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 6 -> LED4 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 5 -> LED3 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 4 -> LED2 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 3 -> LED1 */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 2 -> Buzzer */ \
                                       gPinPullUpDisable_c,      /* Port E, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port E, Pin 0 */ \
                                     )
#define gPTEDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port E, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 6 -> LED4 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 5 -> LED3 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 4 -> LED2 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 3 -> LED1 */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 2 -> Buzzer */ \
                                       gPinDirectionOutput_c,    /* Port E, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port E, Pin 0 */ \
                                     )
#define gPTEDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 6 -> LED4 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 5 -> LED3 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 4 -> LED2 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 3 -> LED1 */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 2 -> Buzzer */ \
                                       gPinDriveStrengthLow_c,   /* Port E, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port E, Pin 0 */ \
                                     )                                              
#define gPTESEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port E, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 6 -> LED4 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 5 -> LED3 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 4 -> LED2 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 3 -> LED1 */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 2 -> Buzzer */ \
                                       gPinSlewRateDisable_c,    /* Port E, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port E, Pin 0 */ \
                                     ) 
                                              
#define gPTELogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port E, Pin 7 */ \
                                       gPinLogicInverse_c,        /* Port E, Pin 6 -> LED4 */ \
                                       gPinLogicInverse_c,        /* Port E, Pin 5 -> LED3 */ \
                                       gPinLogicInverse_c,        /* Port E, Pin 4 -> LED2 */ \
                                       gPinLogicInverse_c,        /* Port E, Pin 3 -> LED1 */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 2 -> Buzzer */ \
                                       gPinLogicDirect_c,        /* Port E, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port E, Pin 0 */ \
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
 
/*Port h - Not used by MC1319x driver. Available for application.*/
#define gPTHDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port H, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port H, Pin 1 */ \
                                       gPinLevelLow_c            /* Port H, Pin 0 */ \
                                     )
#define gPTHPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port H, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port H, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port H, Pin 0 */ \
                                     )
#define gPTHDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port H, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port H, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port H, Pin 0 */ \
                                     )
#define gPTHDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port H, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port H, Pin 0 */ \
                                     )
#define gPTHSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port H, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port H, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port H, Pin 0 */ \
                                     ) 
#define gPTHLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port H, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port H, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port H, Pin 0 */ \
                                     )
                                     
/*Port j - Not used by MC1319x driver. Available for application.*/                                      
                                                  
#define gPTJDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port J, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port J, Pin 1 */ \
                                       gPinLevelLow_c            /* Port J, Pin 0 */ \
                                     )
#define gPTJPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port J, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port J, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port J, Pin 0 */ \
                                     )
#define gPTJDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port J, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port J, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port J, Pin 0 */ \
                                     )
#define gPTJDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port J, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port J, Pin 0 */ \
                                     )
#define gPTJSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port J, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port J, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port J, Pin 0 */ \
                                     ) 
                                              
#define gPTJLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port J, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port J, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port J, Pin 0 */ \
                                     ) 

#endif /* _TARGET_QE128_EVB_ */
#endif
