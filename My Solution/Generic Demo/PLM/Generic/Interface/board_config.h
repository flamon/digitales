/************************************************************************************
* Board Parameters Configuration
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
 
#ifndef __BOARD_CONFIG_H__
#define __BOARD_CONFIG_H__


/* Borads */
#define  gMc1321xSrb_c          0
#define  gMc1321xNcb_c          1
#define  gMc1320xS08qe128Evb_c  2
#define  gMc1323xMrb_c          3
#define  gMc1323xRcm_c          4
#define  gMc1323xRem_c          5
#define  gUserDefine_c          6

  
/* MCUs */
#define  gMcuMcs08GbGt60_c      0
#define  gMcuMc1321x_c          1
#define  gMcuMcs08qe128_c       2
#define  gMcuMc1323x_c          3


/* Transceivers */
#define  gXcvrMc1319x_c         0
#define  gXcvrMc1320x_c         1
#define  gXcvrMc1321x_c         2
#define  gXcvrMc1323x_c         3


/* BEGIN Target Board Definition */

#define gTargetBoard_c          gMc1320xS08qe128Evb_c


#define gUserTransceiverType_d   gXcvrMc1320x_c
/* END Target Board Definition */


#define gTargetXcvr_c  gUserTransceiverType_d


/* NCB, SRB, */
#if (gTargetBoard_c == gMc1321xSrb_c || gTargetBoard_c == gMc1321xNcb_c)
 #define gMcs08Gt60Platform_d  
#endif /* gTargetBoard_c == gMc1321xSrb_c || gTargetBoard_c == gMc1321xSrb_c */


/* MC1320x + QE128 */
#if (gTargetBoard_c == gMc1320xS08qe128Evb_c)
 #define gMcs08qe128Platform_d  
#endif /* gTargetBoard_c == gMc1320xS08qe128Evb_c */

/* MRB, RCM, REM */
#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
 #define gMc1323xPlatform_d  
 #define gDefaultCrystalTrim_c 0x77
#endif /* gTargetBoard_c == gMc1323xMrb_c || gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c */

/* User defined target */
#if (gTargetBoard_c == gUserDefine_c)
 #define gUserDefinePlatform_d 

 #if (gMcuMcs08GbGt60_c == gTargetBasedOn_c)	
   #define gMcs08Gt60Platform_d
 #endif

 #if (gMcuMcs08qe128_c == gTargetBasedOn_c)
   #define gMcs08qe128Platform_d
 #endif

 #if (gMcuMc1323x_c == gTargetBasedOn_c)
   #define gMc1323xPlatform_d  
   #define gDefaultCrystalTrim_c 0x77
 #endif

#endif /* gTargetBoard_c == gUserDefine_c */



#endif /* __BOARD_CONFIG_H__ */

