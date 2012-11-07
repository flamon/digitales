/************************************************************************************
* Mapping of the IO ports and pins.
*
* (c) Copyright 2006, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/

#ifndef _PORTCONFIG_H_
#define _PORTCONFIG_H_

#include "derivative.h"

#if gTargetBoard_c == gMc1321xNcb_c
#include "TargetMC1321xNCB.h"
#endif // gMc1321xNcb_c 

#if gTargetBoard_c == gMc1321xSrb_c
#include "TargetMC1321xSRB.h"
#endif // gMc1321xSrb_c 

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
#include "TargetMC1320xQE128EVB.h"
#endif // gMc1320xS08qe128Evb_c

#if gTargetBoard_c == gMc1323xMrb_c
#include "TargetMC1323xMRB.h"
#endif // gMc1323xRcm_c

#if gTargetBoard_c == gMc1323xRcm_c
#include "TargetMC1323xRCM.h"
#endif // gMc1323xRcm_c

#if gTargetBoard_c == gMc1323xRem_c
#include "TargetMC1323xREM.h"
#endif // gMc1323xRem_c

#if gTargetBoard_c == gUserDefine_c

  #define NotDefined 

  /* macro used for enabling Low Noise Amplifier*/
  #define EnableLNA  NotDefined
  
  /* macro used for enabling Power Amplifier*/
  #define EnablePA   NotDefined
  
  /* macro used for disabling Low Noise Amplifier*/
  #define DisableLNA NotDefined
  
  /* macro used for disabling Power Amplifier*/
  #define DisablePA NotDefined

/* BEGIN Port Index Definition */

/************************************************************************************
*
* Define Board LED port mapping
*
************************************************************************************/

#define gLedPort1   gGpioPortD_c
#define gLedPort2   gGpioPortC_c
#define  gLed1Pin_c     16
#define gLed2Pin_c    32
#define gLed3Pin_c    64
#define gLed4Pin_c    128
#define gLed5Pin_c    16


/************************************************************************************
*
* Define Board Switch mapping 
*
************************************************************************************/

#define gSwitchPort       gGpioPortA_c
#define gSwitch1Pin_c     4
#define gSwitch2Pin_c     8
#define gSwitch3Pin_c     16
#define gSwitch4Pin_c     32

/************************************************************************************
*
* Define Board Radio Pins mapping
*
************************************************************************************/

#define gAttnPort_c           gGpioPortD_c
#define gAttnPinMask_c        1

#define gRxTxEnablePort_c     gGpioPortD_c
#define gRxTxEnablePinMask_c  2

#define gResetRadioPort_c     gGpioPortD_c
#define gResetRadioPinMask_c  8

#define gOutOfIdlePort_c      gGpioPortE_c
#define gOutOfIdlePinMask_c   128

#define gCrcValidPort_c       gGpioPortE_c
#define gCrcValidPinMask_c    64

#define gAntSwPort            gGpioPortB_c
#define gAntSwMask_c       0
#define gUseExternalAntennaSwitch_c  TRUE

/************************************************************************************
*
* Uart Port setup is for choosing the flow control pins RTS and CTS.
The user can choose the port and the pin for RTS or CTS.
*
************************************************************************************/

#define gSCI1RTSPort_c     PTAD
#define gSCI1RTSPin_c      2
#define gSCI1CTSPort_c      PTAD
#define gSCI1CTSPin_c      1

#define gSCI2RTSPort_c      PTAD
#define gSCI2RTSPin_c       64

#define gSCI2CTSPort_c      PTAD
#define gSCI2CTSPin_c       128

/************************************************************************************
*
* Port Registers
*
************************************************************************************/


#define gPTADValue_c      0
#define gPTAPEValue_c     60
#define gPTADDValue_c     66
#define gPTADSValue_c     0

#define gPTBDValue_c      0
#define gPTBPEValue_c     0
#define gPTBDDValue_c     0
#define gPTBDSValue_c     0

#define gPTCDValue_c      16
#define gPTCPEValue_c     0
#define gPTCDDValue_c     16
#define gPTCDSValue_c     0


#define gPTDDValue_c      242
#define gPTDPEValue_c     0
#define gPTDDDValue_c     251
#define gPTDDSValue_c     0

#define gPTEDValue_c      20
#define gPTEPEValue_c     0
#define gPTEDDValue_c     27
#define gPTEDSValue_c     0

#define gPTFDValue_c      0
#define gPTFPEValue_c     0
#define gPTFDDValue_c     0
#define gPTFDSValue_c     0

#define gPTGDValue_c      0
#define gPTGPEValue_c     0
#define gPTGDDValue_c     0
#define gPTGDSValue_c     0

#define gPTHDValue_c      0
#define gPTHPEValue_c     0
#define gPTHDDValue_c     0
#define gPTHDSValue_c     0
#define gPTJDValue_c      0
#define gPTJPEValue_c     0
#define gPTJDDValue_c     0
#define gPTJDSValue_c     0

#define gSwitchKbiModule_c  gKbi1_c
/* END Port Index Definition */

/************************************************************************************
*
* Define Board LED port mapping
*
************************************************************************************/

#define gLedMask_c  (gLed1Pin_c | gLed2Pin_c | gLed3Pin_c | gLed4Pin_c)

 #define   gLED1_c   gLedPort1,gLed1Pin_c
 #define   gLED2_c   gLedPort1,gLed2Pin_c
 #define   gLED3_c   gLedPort1,gLed3Pin_c
 #define   gLED4_c   gLedPort1,gLed4Pin_c
 #define   gLED5_c     gLedPort2,gLed5Pin_c

/************************************************************************************
*
* Define Board Switch mapping 
*
************************************************************************************/
#define gSwitchMask   (gSwitch1Pin_c | gSwitch2Pin_c |  gSwitch3Pin_c | gSwitch4Pin_c)

/************************************************************************************
*************************************************************************************
Uart Port setup is for choosing the flow control pins RTS and CTS. 
The user can choose the port and the pin for RTS or CTS.  
*************************************************************************************
************************************************************************************/

/* Set the pin direction OUTPUT*/
  #define mSETUP_SCI1RTS_OUTPUT mPORT_DD(gSCI1RTSPort_c) |= gSCI1RTSPin_c;

/* Set the pin direction INPUT*/
  #define mSETUP_SCI1CTS_INPUT mPORT_DD(gSCI1CTSPort_c) &= ~gSCI1CTSPin_c;

 /* Set the pin direction OUTPUT*/
  #define mSETUP_SCI2RTS_OUTPUT mPORT_DD(gSCI2RTSPort_c) |= gSCI2RTSPin_c;

/* Set the pin direction INPUT*/
  #define mSETUP_SCI2CTS_INPUT mPORT_DD(gSCI2CTSPort_c) &= ~gSCI2CTSPin_c;

  #define mSETUP_SCIxPINS mSETUP_SCI1RTS_OUTPUT\
                          mSETUP_SCI1CTS_INPUT\
                          mSETUP_SCI2RTS_OUTPUT\
                          mSETUP_SCI2CTS_INPUT

/************************************************************************************
*
* Port Registers
*
************************************************************************************/

#define gPTASEValue_c       0x00                                              
#define gPTALogicValue_c    0x00

#define gPTBSEValue_c       0x00                                              
#define gPTBLogicValue_c    0x00

#define gPTCSEValue_c       0x00                                              
#define gPTCLogicValue_c    0x00

#define gPTDSEValue_c       0x00                                              
#define gPTDLogicValue_c    0x00

#define gPTESEValue_c       0x00                                              
#define gPTELogicValue_c    0x00

#define gPTFSEValue_c       0x00                                              
#define gPTFLogicValue_c    0x00

#define gPTGSEValue_c       0x00                                              
#define gPTGLogicValue_c    0x00

#define gPTHSEValue_c       0x00                                              
#define gPTHLogicValue_c    0x00

#define gPTJSEValue_c       0x00                                              
#define gPTJLogicValue_c    0x00
/************************************************************************************
*
* SPI Pins
*
************************************************************************************/
#if (gMcuMcs08GbGt60_c == gTargetBasedOn_c)   

#define gUsePeripheral1_c         TRUE
#define gPeripheral1Port_c        gGpioPortB_c
#define gPeripheral1PinMask_c     0x20
#define gUsePeripheral2_c         FALSE

#elif (gMcuMcs08qe128_c == gTargetBasedOn_c)

#define gUsePeripheral1_c         TRUE
#define gPeripheral1Port_c        gGpioPortC_c
#define gPeripheral1PinMask_c     0x80 

#define gUsePeripheral2_c         TRUE
#define gPeripheral2Port_c        gGpioPortA_c
#define gPeripheral2PinMask_c     0x04 

#elif (gMcuMc1323x_c == gTargetBasedOn_c) 

#define gUsePeripheral1_c         TRUE
#define gPeripheral1Port_c        gGpioPortC_c
#define gPeripheral1PinMask_c     gGpioPin5Mask_c 

#define gUsePeripheral2_c         TRUE
#define gPeripheral2Port_c        gGpioPortA_c
#define gPeripheral2PinMask_c     0x04 

#endif

#endif /* gTarget_UserDefined_c */

#endif /* _PORTCONFIG_H_ */
