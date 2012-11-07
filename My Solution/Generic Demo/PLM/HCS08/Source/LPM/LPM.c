/*****************************************************************************                   
* Low Power Modes driver implementation.
* Driver to manage the power modes available in the MCU
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#include "LPM.h"
#include "LPM_Interface.h"



/************************************************************************************
*************************************************************************************
* Interface macros
*************************************************************************************
************************************************************************************/                                         



#if gLPMSupported_d == TRUE
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)
static lowPowerMode_t mActualLowPowerMode = mRunMode_c;
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
*  LPM_LPRunMode
*
************************************************************************************/
#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

  void LPM_LPRunMode
  (
  void
  )
  {

    mActualLowPowerMode = mLPRunMode_c;
    
    if(1 == SPMSC1_LVDE) 
    {
      SPMSC1_LVDSE = 0;
    }
      
    SPMSC2_PPDC = 0;
    SPMSC2_LPR = 1;
    
  }

#endif

/************************************************************************************
*
*  LPM_Stop3Mode
*
************************************************************************************/

void LPM_Stop3Mode
(
bool_t bVoltageRegulatorRemainsActive
)
{

  #if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)
    SPMSC1_LVDE = bVoltageRegulatorRemainsActive;           //Voltage regulator remains active OR NOT during stop mode
    SPMSC1_LVDSE = bVoltageRegulatorRemainsActive;
    SPMSC2_PPDC = 0;
    _Stop;
  #endif  
  
  #if defined(gMcs08Gt60Platform_d)
    (void)bVoltageRegulatorRemainsActive;
    SPMSC1_LVDE = 1;
    SPMSC1_LVDSE = 1;
    _Stop;
  #endif 

}

/************************************************************************************
*
*  LPM_Stop2Mode
*
************************************************************************************/

lpmError_t LPM_Stop2Mode
(
void
)
{

#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)
  if(mLPRunMode_c == mActualLowPowerMode)
  {
    return gLpmErrorTransitionNotSupported_c;
  }
  else
  {                 
    
      SPMSC1_LVDSE = 0;
      SPMSC2_LPR   = 0;
      _Nop;
      _Nop;
      _Nop;
      SPMSC2_PPDC = 1;                          
      _Stop;     
  }
#endif

#if defined(gMcs08Gt60Platform_d)
  SPMSC1_LVDSE = 0;
  _Stop;
#endif

    return gLpmErrorNoError_c;

}

/************************************************************************************
*
*  LPM_WaitMode
*
************************************************************************************/

lpmError_t LPM_WaitMode
(
void
)
{
  #if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)
    if(mLPRunMode_c == mActualLowPowerMode)
    {
      return gLpmErrorTransitionNotSupported_c;
    } 
    else
    {                 
      _Wait;
    }
  #endif

  #if defined(gMcs08Gt60Platform_d)
    _Wait;
  #endif  
  
  return gLpmErrorNoError_c;

}

#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

/************************************************************************************
*
*  LPM_RunMode
*
************************************************************************************/

void LPM_RunMode
(
void
)
{
  SPMSC2_LPR = 0;    
  mActualLowPowerMode = mRunMode_c;

}

/************************************************************************************
*
*  LPM_LPWaitMode
*
************************************************************************************/


lpmError_t LPM_LPWaitMode
(
void
)
{
  
  if(mRunMode_c == mActualLowPowerMode)
  {
    return gLpmErrorTransitionNotSupported_c;
  } 
  else
  {
    _Wait;
  }
  
  return gLpmErrorNoError_c;

}

#endif

#else                      /* If gLPMSupported_d == FALSE     */

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
*  LPM_LPRunMode
*
************************************************************************************/
#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

  void LPM_LPRunMode
  (
  void
  )
  {

        
  }

#endif

/************************************************************************************
*
*  LPM_Stop3Mode
*
************************************************************************************/

void LPM_Stop3Mode
(
bool_t bVoltageRegulatorRemainsActive
)
{

    (void)bVoltageRegulatorRemainsActive;        
}

/************************************************************************************
*
*  LPM_Stop2Mode
*
************************************************************************************/

lpmError_t LPM_Stop2Mode
(
void
)
{

    return gLpmErrorNoError_c;

}

/************************************************************************************
*
*  LPM_WaitMode
*
************************************************************************************/

lpmError_t LPM_WaitMode
(
void
)
{
    return gLpmErrorNoError_c; 
}

#if defined(gMc1323xPlatform_d) || defined(gMcs08qe128Platform_d)

/************************************************************************************
*
*  LPM_RunMode
*
************************************************************************************/

void LPM_RunMode
(
void
)
{
 
}

/************************************************************************************
*
*  LPM_LPWaitMode
*
************************************************************************************/


lpmError_t LPM_LPWaitMode
(
void
)
{
  
    return gLpmErrorNoError_c;  
}

#endif

#endif


