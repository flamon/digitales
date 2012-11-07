/************************************************************************************
* Header file for Utilities.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*************************************************************************************/
#ifndef _UTILITIES_INTERFACE_H_
#define _UTILITIES_INTERFACE_H_
   
/* Include files section */   
#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "UART_Interface.h" // include UART functionality

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/


typedef struct sixDigitBcd_tag{
  uint8_t u8Units:4;
  uint8_t u8Tens:4;
  uint8_t u8Houndreds:4;
  uint8_t u8Thousands:4;
  uint8_t u8TensThousands:4;
  uint8_t u8HoundredsThousands:4;
}sixDigitBcd_t;


/******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/


#pragma MESSAGE DISABLE C4001
/*Blocking Delay, Max Milis = 2^16/(gSystemClock_d/40000)*/
#define StringCopy(pSourcePtr, pDestPtr, stringlength)\
  do{\
    uint8_t u8f;\
    for(u8f = 0; u8f < stringlength;u8f++) *(pDestPtr+u8f) = *(pSourcePtr+u8f);\
  }while(0)

#if gSystemClock_d == 8000000
  #define mMilisecondsAdjust_c 307
#elif gSystemClock_d == 16000000
  #define mMilisecondsAdjust_c 614
#endif
/*
  Max 200ms in the GB60 and QE128 platforms
      100ms in the MC1323x platforms
*/
#define DelayMs(milis)\
  do{\
    uint16_t u16f;\
    for(u16f = milis*mMilisecondsAdjust_c; u16f > 0; u16f--)\
    {                                                       \
      __asm NOP;                                            \
    }                                                       \
  }while(0)


#define LOW_POWER_WHILE() _asm wait

/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
*******************************************************************************/


/************************************************************************************
*
* Delay_ms
*
* Description: Blocking mili-seconds Delay that uses gSystemClock_d as clock reference. 
*
* Interface assumptions: none
* 
* Return value:  none
*
************************************************************************************/
extern void Delay_ms
(
  uint16_t     /* IN: Delay value in ms */
);

/************************************************************************************
*
* Delay_us
*
* Description:Blocking micro-seconds Delay that uses gSystemClock_d as clock reference. 
*
* Interface assumptions: none
* 
* Return value:  none
*
************************************************************************************/
extern void Delay_us
(
  uint16_t      /* IN: Delay value in us */
);

/************************************************************************************
*
* convert_2_bcd
*
* Description: This function converts the received data as parameter to bcd format  
*
* Interface assumptions: none
* 
* Return value:   sixDigitBcd_t -> bcd format value
*
************************************************************************************/

extern sixDigitBcd_t convert_2_bcd
(
  uint16_t u16Value  /* IN: Value to convert */
);

/************************************************************************************
*
* HexToAscii
*
* Description:  This function converts an hexadecimal value in ascii code.
*
* Interface assumptions: none
* 
* Return value: uint8_t -> Ascii value 
*
************************************************************************************/


extern uint8_t HexToAscii
(
  uint8_t u8Hex  /* IN: Value to convert */
);

/************************************************************************************
*
* stringComp
*
* Description:  
*
* Interface assumptions: 
* 
* Return value: bool_t 
*
************************************************************************************/


extern bool_t stringComp
(
  uint8_t * au8leftString, 
  uint8_t * au8RightString, 
  uint8_t bytesToCompare
);

/************************************************************************************
*
* MemoryCpy
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/


extern void MemoryCpy
(
  void *pDst, /* Pointer to Destination buffer */ 
  void *pSrc, /* Source buffer */
  uint8_t n   /* Byte count */
);

/************************************************************************************
*
* MemorySet
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/


extern void MemorySet
(
  void *pDst,    // Buffer to be reset
  uint8_t value, // Byte value
  uint8_t cnt    // Byte count
);

/************************************************************************************
*
* PrintWordOnDecimalFormatBlocking
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/

void PrintWordOnDecimalFormatBlocking
(
  uint16_t u16Dec, 
  uint8_t u8MinDigs,
  bool_t bNegative, 
  uartPortNumber_t port
);

/************************************************************************************
*
* PrintByteOnHexFormatBlocking
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/


void PrintByteOnHexFormatBlocking
(
  uint8_t u8Byte, 
  bool_t bPrintComma, 
  uartPortNumber_t port
);

/************************************************************************************
*
* int2BCD
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/

void int2BCD
(
  uint16_t u16Num, 
  uint8_t * pu8BCDOut
);

/************************************************************************************
*
* HexString2Dec16
*
* Description:  
*
* Interface assumptions: 
* 
* Return value:  
*
************************************************************************************/

uint16_t HexString2Dec16
(
  uint8_t * au8String
);


#endif /* _UTILITIES_H_ */
