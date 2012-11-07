/************************************************************************************
* Header file for physical layer
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/

#ifndef _SIMPLE_PHY_H_
#define _SIMPLE_PHY_H_

#include "derivative.h"

/************************************************************************************
*************************************************************************************
* Includes
*************************************************************************************
************************************************************************************/
#if defined(gMcs08Gt60Platform_d)
  #include "PhyMC1320X_Interface.h"

#elif defined(gMcs08qe128Platform_d)
  #include "PhyMC1320X_Interface.h"

#elif defined(gMc1323xPlatform_d)
  #include "PhyMC1323X.h"

  #define PhySetRadioTimerPrescaler(Value) PP_PHY_FRMREVTMR = (((uint8_t)Value << 5) & (7<<5)) 
  #define PhyEnableRadioInterrupts()       PP_PHY_CTL2 &= (~PP_PHY_CTL2_TRCV_MSK_BIT)
  #define PhyDisableRadioInterrupts()      PP_PHY_CTL2 |= (PP_PHY_CTL2_TRCV_MSK_BIT)
  #define PhyPlmeSetClockOutRate(x)        ((void)x, gPhySuccess_c)
  #define PhyAdjustCcaOffsetCmp(x)         PP_PHY_CCAOFSCMP = x
  #define PhyXtalAdjust(x)                 SOMC2 = x;
  #define PhyGetRfIcVersion(Buffptr)       Buffptr[0] = SDIDH;\
                                           Buffptr[1] = SDIDL;
#elif defined(gUserDefinePlatform_d)

  #if (gXcvrMc1320x_c == gTargetXcvr_c || gXcvrMc1321x_c == gTargetXcvr_c)
	#include "PhyMC1320X_Interface.h"
  #elif (gXcvrMc1323x_c == gTargetXcvr_c) 
    #include "PhyMC1323X.h" 
  #endif

#endif

#endif /* _PHY_H_ */
