/*****************************************************************************
* Source file for Utilities.
* 
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
*
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#include "Utilities_Interface.h"

#pragma MESSAGE DISABLE C5651

/************************************************************************************
* Delay_ms
*
************************************************************************************/
void Delay_ms
(
  uint16_t u16ms
) 
{                                            
  uint16_t u16ctr;                                          
  for(u16ctr = 0; u16ctr < ((gSystemClock_d/40000)*u16ms); u16ctr++){
    asm NOP; 
    asm NOP;
  }    
}

/************************************************************************************
* Delay_us
*
************************************************************************************/
void Delay_us
(
  uint16_t u16us
) 
{
  uint16_t u16ctr;                                          
  for(u16ctr = 0; u16ctr < (((gSystemClock_d*10/40000000)*u16us)/10); u16ctr++){
    asm NOP; 
    asm NOP;     
  }      
}


 
/*******************************************************************************  
* HexToAscii
*
* This function converts an hexadecimal value in ascii code.
*******************************************************************************/
uint8_t HexToAscii(uint8_t u8Hex)
{
  u8Hex &= 0x0F;
  return u8Hex + ((u8Hex <= 9) ? '0' : ('A'-10));
}



/**************************************************************************************/



/************************************************************************************
* convert_2_bcd function
*
* This function converts the received data as parameter to bcd format
*
************************************************************************************/
sixDigitBcd_t convert_2_bcd(uint16_t u16Value)
{
  sixDigitBcd_t u16RtrnVal;
  uint16_t tmp;
  u16RtrnVal.u8Units = 0;
  u16RtrnVal.u8Tens = 0;
  u16RtrnVal.u8Houndreds = 0;
  u16RtrnVal.u8Thousands = 0;
  u16RtrnVal.u8TensThousands = 0;
  u16RtrnVal.u8HoundredsThousands = 0;
  if(65535 > u16Value)
  {
    tmp = u16Value;
    if(10000 <= u16Value)
    {
      tmp = u16Value/10000;
      u16RtrnVal.u8TensThousands = (uint8_t)tmp;
      tmp = u16Value%10000;
      u16Value = tmp;
    }

    if(1000 <= u16Value)
    {
      tmp = u16Value/1000;
      u16RtrnVal.u8Thousands = (uint8_t)tmp;
      tmp = u16Value%1000;
      u16Value = tmp;
    }
    if(100 <= u16Value)
    {
      tmp = u16Value/100;
      u16RtrnVal.u8Houndreds = (uint8_t)tmp;
      tmp = u16Value%100;
      u16Value = tmp;
    }
    if(10 <= u16Value)
    {
      tmp = u16Value/10;
      u16RtrnVal.u8Tens = (uint8_t)tmp;
      tmp = u16Value%10;
      u16Value = tmp;
    }
    u16RtrnVal.u8Units = (uint8_t)tmp;
  }
  return u16RtrnVal;
} 

uint8_t HexaToAscii(uint8_t u8Hex)
{
  u8Hex &= 0x0F;
  return u8Hex + ((u8Hex <= 9) ? '0' : ('A'-10));
}

/************************************************************************************
* stringComp
*
* ...
*
************************************************************************************/
bool_t stringComp(uint8_t * au8leftString, uint8_t * au8RightString, uint8_t bytesToCompare)
{
  do
  {
  }while((*au8leftString++ == *au8RightString++) && --bytesToCompare);
  return(0 == bytesToCompare);
}


/************************************************************************************
* MemoryCpy
*
* This function copy a buffer source to a buffer destination
*
************************************************************************************/

void MemoryCpy
(
  void *pDst, // Destination buffer
  void *pSrc, // Source buffer
  uint8_t n   // Byte count
)
{
    // Code looks weird, but is quite optimal on a HCS08 CPU
  while(n) {
    *((uint8_t *)pDst) = *((uint8_t *)pSrc);
    pDst=((uint8_t *)pDst)+1;
    pSrc=((uint8_t *)pSrc)+1;
    n--;
  }
}

/************************************************************************************
* MemorySet
*
* This function set an specific value into a buffer destination
*
************************************************************************************/

void MemorySet
(
  void *pDst,    // Buffer to be reset
  uint8_t value, // Byte value
  uint8_t cnt    // Byte count
)
{
  while(cnt) {
    ((uint8_t *)pDst)[--cnt] = value;
  }
}

/**************************************************************************************/
void PrintByteOnHexFormatBlocking(uint8_t u8Byte, bool_t bPrintComma, uartPortNumber_t port)
{
  static uint8_t HexString[4] = {0};
    
  HexString[0] = HexToAscii(u8Byte>>4);
  HexString[1] = HexToAscii(u8Byte);
  if(bPrintComma){
    HexString[2] = ',';  
  }else{
    HexString[2] = 0;      
  }
  HexString[3] = 0;      
  
  (void)Uart_BlockingStringTx(HexString, port);
}


/**************************************************************************************/
void PrintWordOnDecimalFormatBlocking(uint16_t u16Dec, uint8_t u8MinDigs,bool_t bNegative, uartPortNumber_t port)
{
  static uint8_t decString[7] = {0};
  uint8_t ind,Sing;
    
  int2BCD(u16Dec, &decString[1]);
  
  Sing=0;
  decString[0]=0;
  if(u8MinDigs > 5) u8MinDigs = 5;
  while((0 == decString[Sing+1]) && (Sing < 5-u8MinDigs)){
    Sing++; 
  }
  for(ind=Sing;ind<6;ind++){
    decString[ind] += '0';  
  }
  if(bNegative){
    decString[Sing] = '-';
  }else{
    Sing++;
  }
  (void)Uart_BlockingStringTx(&decString[Sing], port);
}

/**************************************************************************************/
void int2BCD(uint16_t u16Num, uint8_t * pu8BCDOut)
{
  uint8_t cont=16;
  uint8_t aux=0;

  for (aux=0;aux<5;aux++){
    pu8BCDOut[aux]=0;
  }
  do{
    for(aux=0;aux<5;aux++) {
      pu8BCDOut[aux]<<=1;
    }
    if(u16Num&0x8000){
      pu8BCDOut[4]+=1;
    }
    u16Num<<=1;
    for(aux=4;aux>0;aux--){
      if (pu8BCDOut[aux]>=10){
        pu8BCDOut[aux-1]++;
        pu8BCDOut[aux]-=10;
      }
    }
    cont--;
  }while(cont);
}

/**************************************************************************************/
#pragma MESSAGE DISABLE C4003
uint16_t HexString2Dec16(uint8_t * au8String)
{
  uint8_t u8LocIndex=0;
  uint8_t u8LocIndex2=0;
  uint16_t u16DecValue = 0;
  
  while(au8String[u8LocIndex]){
    u8LocIndex++;
  }
  
  while(u8LocIndex--){
    if((au8String[u8LocIndex] >= '0') && (au8String[u8LocIndex] <= '9'))
     u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - '0'))<<(u8LocIndex2*4);
    else if((au8String[u8LocIndex] >= 'A') && (au8String[u8LocIndex] <= 'F')){
     u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - 'A' + 0x0A))<<(u8LocIndex2*4);    
    }else{
     u16DecValue |= ((uint16_t)(au8String[u8LocIndex] - 'a' + 0x0A))<<(u8LocIndex2*4);        
    }
    u8LocIndex2++;
  }
  
  return u16DecValue;
  
}

