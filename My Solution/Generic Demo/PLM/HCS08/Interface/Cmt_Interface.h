/*****************************************************************************
* Carrier Modulation Timer implementation 
*
* Copyrigtht (c) 2009, Freescale, Inc. All rights reserved
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor
*
*****************************************************************************/

#ifndef _CMT_INTERFACE_H
#define _CMT_INTERFACE_H

#include "board_config.h"
#include "PLM_config.h"

/*****************************************************************************
******************************************************************************
* Public macros
******************************************************************************
*****************************************************************************/
/* Defines whether or not the CMT module is included in the application */
#ifndef gCmtSupported_d
  #define gCmtSupported_d                  FALSE
#endif


/* Carrier frequency */
#define gCmtCarrierFrequency36kHz_c       (36000)
#define gCmtCarrierFrequency38kHz_c       (38000)
#define gCmtCarrierFrequency40kHz_c       (40000)
#define gCmtCarrierFrequency56kHz_c       (56000)

#ifndef gCmtDefaultCarrierFrequency_c
  #define gCmtDefaultCarrierFrequency_c   gCmtCarrierFrequency38kHz_c
#endif

/* Carrier duty cycle */
#define gCmtCarrierDutyCycle25_c          25
#define gCmtCarrierDutyCycle33_c          33
#define gCmtCarrierDutyCycle50_c          50
 
#ifndef gCmtDefaultCarrierDutyCycle_c
  #define gCmtDefaultCarrierDutyCycle_c   gCmtCarrierDutyCycle33_c
#endif
                   

/*****************************************************************************/
/* Logic 0 */
#define gCmtDefaultLog0MarkInMicros_c     560
#define gCmtDefaultLog0SpaceInMicros_c    560
/* Logic 1 */
#define gCmtDefaultLog1MarkInMicros_c     560
#define gCmtDefaultLog1SpaceInMicros_c    1690

/* Default bits shifting: LSB first */
#define gCmtLsbFirstDefault_c             TRUE

/* Default mode of operation: Time */
#define gCmtTimeOperModeDefault_c         TRUE

/* Define the IRO output pin polarity */
#define gCmtOutputPolarityDefault_c       TRUE //FALSE


/****************************************************************************/
/* Macros defining ticks calculation (high and low periods) for a carrier */
/* waveform parameters: frequency and duty cycle */
#define ConvertTimeToClockTicks(cmtClkFreq, cmtCarrierFreq)                   (((uint32_t) (cmtClkFreq  /  cmtCarrierFreq)))
#define ComputeCarrierHigh(cmtClkFreq, cmtCarrierFreq, cmtCarrierDutyCycle)   ((uint8_t) ((ConvertTimeToClockTicks(cmtClkFreq, cmtCarrierFreq) * cmtCarrierDutyCycle) / 100))
#define ComputeCarrierLow(cmtClkFreq, cmtCarrierFreq, cmtCarrierDutyCycle)    ((uint8_t) (ConvertTimeToClockTicks(cmtClkFreq, cmtCarrierFreq) - ComputeCarrierHigh(cmtClkFreq, cmtCarrierFreq, cmtCarrierDutyCycle)))

/* Modulation cycle: mark and space ticks calculation */ 
#define CmtModTicksForOneSecond(cmtClkFreq)       ((uint32_t) cmtClkFreq / 8)
#define CmtModTicksForOneMilliSecond(cmtClkFreq)  ((uint32_t) (CmtModTicksForOneSecond(cmtClkFreq) / 1000))

#define CmtModTicksInMilliSeconds(milliseconds, cmtClkFreq)  ((uint32_t) (milliseconds * CmtModTicksForOneMilliSecond(cmtClkFreq)))
#define CmtModTicksInMicroSeconds(microseconds, cmtClkFreq)  (((uint32_t) (microseconds * CmtModTicksInMilliSeconds(1, cmtClkFreq)) < 1000) ? 1 : ((uint32_t)(microseconds * CmtModTicksInMilliSeconds(1, cmtClkFreq)) / 1000))
                                                                  
/* Define mark and space periods in milliseconds or microseconds */
/* Milliseconds */
#define CmtModMarkTicksInMilliSeconds(markMilliseconds, cmtClkFreq)    ((uint16_t)(CmtModTicksInMilliSeconds(markMilliseconds, cmtClkFreq) - 1))
#define CmtModSpaceTicksInMilliSeconds(spaceMilliseconds, cmtClkFreq)  ((uint16_t)(CmtModTicksInMilliSeconds(spaceMilliseconds, cmtClkFreq)))
/* Microseconds */
#define CmtModMarkTicksInMicroSeconds(markMicroseconds, cmtClkFreq)    ((uint16_t)(CmtModTicksInMicroSeconds(markMicroseconds, cmtClkFreq) - 1))
#define CmtModSpaceTicksInMicroSeconds(spaceMicroseconds, cmtClkFreq)  ((uint16_t)(CmtModTicksInMicroSeconds(spaceMicroseconds, cmtClkFreq)))
 
/*****************************************************************************
******************************************************************************
* Public type definitions
******************************************************************************
*****************************************************************************/

/* Generic CMT code error */
typedef enum cmtErr_tag
{
  gCmtErrNoErr_c  = 0,
  gCmtErrInvalidParameter_c,
  gCmtErrBusy_c  
} cmtErr_t;
       
/* The pointer to the callback function */
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near cmtCallback_t)(void);
#else
  typedef void(* __near cmtCallback_t)(void);
#endif


/*****************************************************************************
******************************************************************************
* Public prototypes
******************************************************************************
*****************************************************************************/

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */ 
/* CMT Interrupt Service Routine */
extern INTERRUPT_KEYWORD void CMT_InterruptHandler(void);
#pragma CODE_SEG DEFAULT

/************************************************************************************
*
* CMT_Initialize
*
* Initialize the CMT module and hardware.
*
* Interface assumptions: 
*
* Return value: void
*
************************************************************************************/
extern void CMT_Initialize(void);

/************************************************************************************
*
* CMT_SetCarrierWaveform
*
* Function which configure the CMT registers with high and low values from 
* provided frequency and duty cycle 
*
* Interface assumptions: 
*                       The CMT times are set in CMT timer tics.
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_SetCarrierWaveform
(
 uint8_t highCount,  //IN: the active amount of time for the carrier waveform
 uint8_t lowCount    //IN: the unactive amount of time for the carrier waveform
);

/************************************************************************************
*
* CMT_SetTxCallback
*
* Provides a pointer to the callback function after finishing any Tx activity 
*
* Interface assumptions: 
*                       The callback pointer shall not be NULL
*
* Return value: void
*
************************************************************************************/
extern void CMT_SetTxCallback
(
 cmtCallback_t callback  //IN: pointer to a function to be called at the end of the 
                         //    CMT transmission.   
);

/************************************************************************************
*
* CMT_SetMarkSpaceLog0
*
* Configures the mark and space width for logical 0 bit 
*
* Interface assumptions: 
*                       markPeriod and spacePeriod can't be both zero. 
*                       The CMT times are set in CMT timer tics.
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_SetMarkSpaceLog0
(
 uint16_t markPeriod,   //IN: the amount of time for the zero's mark
 uint16_t spacePeriod   //IN: the amount of time for the zero's space
);

/************************************************************************************
*
* CMT_SetMarkSpaceLog1
*
* Configures the mark and space width for logical 1 bit 
*
* Interface assumptions: 
*                       markPeriod and spacePeriod can't be both zero. 
*                       The CMT times are set in CMT timer tics.
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_SetMarkSpaceLog1
(
 uint16_t markPeriod,  //IN: the amount of time for the one's mark
 uint16_t spacePeriod  //IN: the amount of time for the one's space
);

/************************************************************************************
*
* CMT_TxBits
*
* Transfers a number of up to 8 bits via IRO pin
*
* Interface assumptions: 
*                       bitsCount can't be zero. 
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_TxBits
(
 uint8_t data,      //IN: bits to be transmitted trough the CMT module
 uint8_t bitsCount  //IN: amount of bits to be transmitted trough the CMT module
);

/************************************************************************************
*
* CMT_TxModCycle
*
* Triggers a modulation cycle with mark/space parameters given as arguments 
*
* Interface assumptions: 
*                       markPeriod and spacePeriod can't be both zero. 
*                       The CMT times are set in CMT timer tics.
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_TxModCycle
(
 uint16_t markPeriod,  //IN: the amount of time for the cycle's mark
 uint16_t spacePeriod  //IN: the amount of time for the cycle's space
); 

/************************************************************************************
*
* CMT_Abort
*
* Aborts a CMT transmission operation if it ongoing 
*
* Interface assumptions: 
*
* Return value: void
*
************************************************************************************/
extern void CMT_Abort(void);

/************************************************************************************
*
* CMT_IsTxActive
*
* Checks if whether a transmission onto the CMT's IRO pin is ongoing 
*
* Interface assumptions: 
*
* Return value: bool_t
*
************************************************************************************/
extern bool_t CMT_IsTxActive(void);

/************************************************************************************
*
* CMT_SetModOperation
*
* Set the mode of operation: time or baseband 
*
* Interface assumptions: 
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_SetModOperation
(
 bool_t timeOperMode  //IN: TRUE for time mode, FALSE for baseband
);

/************************************************************************************
*
* CMT_SetBitsShifting
*
* Set the bits shifting mode: LSB or MSB first 
*
* Interface assumptions: 
*
* Return value: cmtErr_t
*
************************************************************************************/
extern cmtErr_t CMT_SetBitsShifting
(
 bool_t bitsLsbShifting //IN: TRUE for LSB first, FALSE for MSB first
); 

#endif /* _CMT_INTERFACE_H */