/************************************************************************************
* This header file is for the MC1323x MRB port mappings.
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/
#ifdef TARGET_MC1323

#ifndef _TARGET_MC1323x_MRB_
#define _TARGET_MC1323x_MRB_
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

  #define gLedPort1    gGpioPortA_c
  #define gLed1Pin_c   0x01
  #define gLed2Pin_c   0x10
  #define gLedMask1    (gLed1Pin_c | gLed2Pin_c)
  #define gLedPort2    gGpioPortD_c
  #define gLed3Pin_c   0x01
  #define gLed4Pin_c   0x02
  #define gLedMask2_c  (gLed3Pin_c | gLed4Pin_c)

  #define gLED1_c      gLedPort1,gLed1Pin_c
  #define gLED2_c      gLedPort1,gLed2Pin_c
  #define gLED3_c      gLedPort2,gLed3Pin_c
  #define gLED4_c      gLedPort2,gLed4Pin_c



/************************************************************************************
*
* Define Board Switch mapping 
*
************************************************************************************/

  // Switches defines columns
  #define gSwitchColmnPort    gGpioPortB_c
  #define gSwitchColmn1_c     0x01
  #define gSwitchColmn2_c     0x02
  #define gSwitchColmn3_c     0x04
  #define gSwitchColmn4_c     0x08
  #define gSwitchColmnMask_c  (gSwitchColmn1_c | gSwitchColmn2_c | gSwitchColmn3_c | gSwitchColmn4_c)
                            
  // Switches defines rows
  #define gSwitchRowPort      gGpioPortC_c
  #define gSwitchRow1_c       0x01          
  #define gSwitchRow2_c       0x02
  #define gSwitchRowMask_c    (gSwitchRow1_c | gSwitchRow2_c )

  #define gSwitchKbiModule_c  gKbi1_c
 
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
  #define gBuzzerPort_c gGpioPortA_c
  #define gBuzzerPin_c  gGpioPin1Mask_c
  
/************************************************************************************
*
* SPI Pins
*
************************************************************************************/

#define gUsePeripheral1_c         FALSE

#define gUsePeripheral2_c         FALSE

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
                                       gPinLevelLow_c,           /* Port A, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port A, Pin 1 */ \
                                       gPinLevelLow_c            /* Port A, Pin 0 */ \
                                     )
#define gPTAPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port A, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port A, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port A, Pin 0 */ \
                                      )
#define gPTADDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port A, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port A, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port A, Pin 0 */ \
                                     )
#define gPTADSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port A, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port A, Pin 0 */ \
                                     ) 
#define gPTASEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port A, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port A, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port A, Pin 0 */ \
                                     ) 
                                              
#define gPTALogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port A, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port A, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port A, Pin 0 */ \
                                     ) 
                                     

/* Port B. Add pins for application if needed */  
#define gPTBDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port B, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port B, Pin 1 */ \
                                       gPinLevelLow_c            /* Port B, Pin 0 */ \
                                     )
#define gPTBPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port B, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port B, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port B, Pin 0 */ \
                                     )
#define gPTBDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port B, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port B, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port B, Pin 0 */ \
                                     )
#define gPTBDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 4 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 3 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 2 */ \
                                       gPinDriveStrengthLow_c,   /* Port B, Pin 1 */ \
                                       gPinDriveStrengthLow_c    /* Port B, Pin 0 */ \
                                     )                                             
#define gPTBSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port B, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port B, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port B, Pin 0 */ \
                                     ) 
                                              
#define gPTBLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port B, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port B, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port B, Pin 0 */ \
                                     )
                                     
                                     
/* Port C. Add pins for application if needed */                                                                                         
#define gPTCDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port C, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 1 */ \
                                       gPinLevelLow_c            /* Port C, Pin 0 */ \
                                     )
#define gPTCPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port C, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port C, Pin 0 */ \
                                     )
#define gPTCDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port C, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port C, Pin 0 */ \
                                     )
#define gPTCDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 4 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 3 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 2 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 1 */ \
                                       gPinDriveStrengthLow_c   /* Port C, Pin 0 */ \
                                     )                                               
#define gPTCSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port C, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port C, Pin 0 */ \
                                     ) 
                                              
#define gPTCLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port C, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port C, Pin 0 */ \
                                     ) 
                                     
     
/* Port D. Add pins for application if needed */  
#define gPTDDValue_c        PortSetup(\
                                       gPinLevelLow_c,           /* Port C, Pin 7 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 6 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 5 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 4 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 3 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 2 */ \
                                       gPinLevelLow_c,           /* Port C, Pin 1 */ \
                                       gPinLevelLow_c            /* Port C, Pin 0 */ \
                                     )
#define gPTDPEValue_c       PortSetup(\
                                       gPinPullUpDisable_c,      /* Port C, Pin 7 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 6 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 5 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 4 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 3 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 2 */ \
                                       gPinPullUpDisable_c,      /* Port C, Pin 1 */ \
                                       gPinPullUpDisable_c       /* Port C, Pin 0 */ \
                                     )
#define gPTDDDValue_c       PortSetup(\
                                       gPinDirectionOutput_c,    /* Port C, Pin 7 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 6 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 5 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 4 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 3 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 2 */ \
                                       gPinDirectionOutput_c,    /* Port C, Pin 1 */ \
                                       gPinDirectionOutput_c     /* Port C, Pin 0 */ \
                                     )
#define gPTDDSValue_c       PortSetup(\
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 7 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 6 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 5 */ \
                                       gPinDriveStrengthLow_c,   /* Port C, Pin 4 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 3 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 2 */ \
                                       gPinDriveStrengthLow_c,  /* Port C, Pin 1 */ \
                                       gPinDriveStrengthLow_c   /* Port C, Pin 0 */ \
                                     )                                               
#define gPTDSEValue_c       PortSetup(\
                                       gPinSlewRateDisable_c,    /* Port C, Pin 7 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 6 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 5 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 4 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 3 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 2 */ \
                                       gPinSlewRateDisable_c,    /* Port C, Pin 1 */ \
                                       gPinSlewRateDisable_c     /* Port C, Pin 0 */ \
                                     ) 
                                              
#define gPTDLogicValue_c    PortSetup(\
                                       gPinLogicDirect_c,        /* Port C, Pin 7 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 6 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 5 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 4 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 3 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 2 */ \
                                       gPinLogicDirect_c,        /* Port C, Pin 1 */ \
                                       gPinLogicDirect_c         /* Port C, Pin 0 */ \
                                     ) 
    

#endif /* _TARGET_MC1323x_MRB_ */
#endif
