/************************************************************************************
* This module contains the PLM basic configuration.
*
*
* (c) Copyright 2010, Freescale, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/

#ifndef _PLM_CONFIG_H_
#define _PLM_CONFIG_H_

/* Defines to enable/disable modules support */ 

#define gLEDSupported_d             TRUE                      
#define gKeyboardSupported_d        FALSE                 
#define gBuzzerSupported_d          TRUE
#define gLPMSupported_d             FALSE
#define gAsmSupported_d             TRUE
#define gAdcSupported_d             FALSE
#define gCmtSupported_d             FALSE
#define gKbiSupported_d             TRUE
#define gFlashSupported_d           TRUE
#define gEepromSupported_d          FALSE
#define gAccelerometerSupported_d   FALSE
#define gIicSupported_d             TRUE
#define gSpiSupported_d             TRUE
#define gUartSupported_d            TRUE  
#define gRtiSupported_d             TRUE 
#define gTimerSupported_d           TRUE
#define gLvdSupported_d             TRUE
#define gLcdSupported_d             TRUE
#define gTouchpadSupported_d        FALSE


  
#define gPolling_c        0
#define gInterruption_c   1


#endif                       /* _PLM_CONFIG_H_ */


