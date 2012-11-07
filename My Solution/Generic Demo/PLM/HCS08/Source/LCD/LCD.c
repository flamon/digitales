/*****************************************************************************
* Source file for Lcd driver.
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

#include "LCD.h"

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/
#if(TRUE == gLcdSupported_d) 
    /* Lcd Driver Status */
    static lcdState_t      mLcdStatus = mLcdStateNotInit_c;  
    /* Lcd String to be written to Lcd*/
    static uint8_t         *mu8LcdString;     
    static uint8_t         mu8RsState;
    static uint8_t         mu8LcdStringLength;
    static bool_t          mu8LcdStrFlag;
    static bool_t          mbLcdBlockingFlag;
    LcdCallback_t          gpfLcdCallBack;  
    #if (gMc1323xRcm_c == gTargetBoard_c)
      static uint8_t         mu8LcdSpiCounter;
    #endif
 
#endif 
/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Lcd_Init
************************************************************************************/
#if(TRUE == gLcdSupported_d) 
lcdErrors_t Lcd_Init
(
 LcdCallback_t   pfUserCallBack
)
{    
      mu8LcdString = NULL;
      mbLcdBlockingFlag = TRUE; 
      mLcdStatus = mLcdStateNotInit_c;          
      #if (gMc1321xNcb_c == gTargetBoard_c)
      /*To clear the Lcd_Enable pin on the MC1321x NCB and enable the display*/    
        LcdEnable();                   
      #endif        
      Delay_ms( mWait40mSec_c );             
      #if (gTargetBoard_c == gMc1320xS08qe128Evb_c || gTargetBoard_c == gMc1321xNcb_c)        
         LcdClear_E_RS_RW();                   
         Delay_us( mWait125uSec_c ); 
         /*Return Home*/             
         LcdWriteCmd(mLcdRsCmd_c, 0x02);         
         Delay_ms( mWait10mSec_c );
      #endif           
      Delay_us( mWait125uSec_c ); 
      
      /*Function Set*/ 
      LcdWriteCmd(mLcdRsCmd_c,0x20);
      Delay_us( mWait125uSec_c );                           
      
      /*Function Set*/       
      LcdWriteCmd(mLcdRsCmd_c,0x20);             
      Delay_us( mWait125uSec_c );  
            
      /*Display Clear*/             
      LcdWriteCmd(mLcdRsCmd_c, 0x01);        
      Delay_ms( mWait10mSec_c );
      
      /*Entry Mode Set*/     
      LcdWriteCmd(mLcdRsCmd_c, 0x06);        
      Delay_us( mWait125uSec_c ); 
            
      /*Display ON/OFF control*/  
      LcdWriteCmd(mLcdRsCmd_c,0x0E);   
      Delay_us( mWait125uSec_c );                                 
      
      mLcdStatus = mLcdStateIdle_c;
      /*Sets the LCD callback */
      gpfLcdCallBack = pfUserCallBack;                  
      mu8LcdStrFlag = TRUE;      
      if(NULL != gpfLcdCallBack)
       {
        gpfLcdCallBack(gLcdErrNoError_c);                       
       } 
      return gLcdErrNoError_c;                                
}
#else
lcdErrors_t Lcd_Init(LcdCallback_t   pfUserCallBack)
{
  (void) pfUserCallBack;
  return gLcdErrNoError_c;
}
#endif  // gLcdSupported_d


/************************************************************************************
* Lcd_Config
************************************************************************************/
#if(TRUE == gLcdSupported_d) 
lcdErrors_t Lcd_Config
(
  bool_t LcdDisplayOn, bool_t LcdCursorOn, bool_t LcdBlinkOn
) 
{ 
    #if (TRUE == gLcdParametersValidation_d)
      if(mLcdStateIdle_c != mLcdStatus)
       {    
        return gLcdErrBusBusy_c;
       }    
    #endif 
    mbLcdBlockingFlag = TRUE;      
    mLcdStatus = mLcdStateTransmittingData_c; 
    /*Display ON/OFF, Cursor ON/OFF, Char Blink ON/OFF*/
    LcdWriteCmd(mLcdRsCmd_c,(0x08 | LcdDisplayOn << 2 | LcdCursorOn < 1 | LcdBlinkOn )); 
    #if (gTargetBoard_c == gMc1320xS08qe128Evb_c || gTargetBoard_c == gMc1321xNcb_c)
        Delay_us( mWait125uSec_c );        
        LcdWriteCmd(mLcdRsCmd_c,0x28); 
        Delay_us( mWait125uSec_c );        
        LcdWriteCmd(mLcdRsCmd_c,0x28);   
       // Delay_us( mWait125uSec_c );        
    #endif           
    mLcdStatus = mLcdStateIdle_c;     
    if(NULL != gpfLcdCallBack)
    {
     gpfLcdCallBack(gLcdErrNoError_c);                       
    }  
    return gLcdErrNoError_c;                                      
}
#else
lcdErrors_t Lcd_Config(bool_t LcdDisplayOn, bool_t LcdCursorOn, bool_t LcdBlinkOn)
{
    (void) LcdDisplayOn;
    (void) LcdCursorOn;
    (void) LcdBlinkOn;
    return gLcdErrNoError_c;
}
#endif // gLcdSupported_d 


/************************************************************************************
* Lcd_WriteChar
************************************************************************************/
#if(TRUE == gLcdSupported_d) 
lcdErrors_t Lcd_WriteChar( uint8_t u8line,  uint8_t u8CharToWrite)
{     
   mu8LcdStrFlag = FALSE;
   return Lcd_WriteStringBlocking(u8line, &u8CharToWrite);     
}
#else
lcdErrors_t Lcd_WriteChar( uint8_t u8line,  uint8_t u8CharToWrite)
{
  
   (void) u8line;
   (void) u8CharToWrite;  
   return gLcdErrNoError_c;
}
#endif // gLcdSupported_d 

/************************************************************************************
* Lcd_WriteString
************************************************************************************/
#if(TRUE == gLcdSupported_d)
lcdErrors_t Lcd_WriteStringNonBlocking
( 
  uint8_t u8line,  
  uint8_t *pString
)
{
     #if (TRUE == gLcdParametersValidation_d)
      if(mLcdStateIdle_c != mLcdStatus)
       {    
        return gLcdErrBusBusy_c;
       }    
     #endif 
     mbLcdBlockingFlag = FALSE; 
     mLcdStatus = mLcdStateTransmittingData_c;   
     mu8LcdString = pString;     
     if(mu8LcdStrFlag)
      {  
        mu8LcdStringLength = (uint8_t) strlen(mu8LcdString) ;      
      }
     else
      {  
        mu8LcdStringLength = mLcdCharSize_c;
      }      
     #if (TRUE == gLcdParametersValidation_d)          
       if((u8line > mSecondLine_c) || (mu8LcdStringLength > mLcdMaxChars_c))
       {  
        mLcdStatus = mLcdStateIdle_c;
        return gLcdErrInvalidParameter_c;    
       }
     #endif   
     if(mSecondLine_c == u8line)
      {  
      
        LcdSelectSecondLine();
      }
     else
      {  
        LcdSelectFirstLine();
      }   
      #if (gMc1323xRcm_c == gTargetBoard_c)
        mu8LcdSpiCounter =  mu8LcdStringLength * 2;
        LcdWriteCmdNonBlocking(mLcdRsData_c, *mu8LcdString);         
      #else
        while(mLcdStateTransmittingData_c == mLcdStatus) 
         {
          if(mu8LcdStringLength>0) 
          {
            LcdWriteCmd(mLcdRsData_c, *mu8LcdString++);   
            mu8LcdStringLength--;
          } 
          else 
          {
            mLcdStatus = mLcdStateIdle_c;
            mu8LcdStrFlag = TRUE;
          }  
         }
      #endif                                            
     if(NULL != gpfLcdCallBack)
      {                      
       gpfLcdCallBack(gLcdErrNoError_c);                       
      }
     return gLcdErrNoError_c;    
}
#else
lcdErrors_t Lcd_WriteStringNonBlocking (uint8_t u8line, uint8_t *pString)
{
  (void) u8line;
  (void) pString;  
  return gLcdErrNoError_c;
}
#endif // gLcdSupported_d

/************************************************************************************
* Lcd_WriteStringBlocking
************************************************************************************/
#if(TRUE == gLcdSupported_d)
lcdErrors_t Lcd_WriteStringBlocking
( 
  uint8_t u8line, 
  uint8_t *pString
)
{ 
     #if (TRUE == gLcdParametersValidation_d)
      if(mLcdStateIdle_c != mLcdStatus)
       {    
        return gLcdErrBusBusy_c;
       }    
     #endif  
     mbLcdBlockingFlag = TRUE;   
     mLcdStatus = mLcdStateTransmittingData_c;   
     mu8LcdString = pString;     
     if(mu8LcdStrFlag)
      {  
        mu8LcdStringLength = (uint8_t) strlen(mu8LcdString) ;
      }
     else
      {  
        mu8LcdStringLength = mLcdCharSize_c;
      }
     #if (TRUE == gLcdParametersValidation_d)       
        if((u8line > mSecondLine_c) || (mu8LcdStringLength > mLcdMaxChars_c))
        {  
          mLcdStatus = mLcdStateIdle_c;
          return gLcdErrInvalidParameter_c;    
        }
     #endif  
     if(mSecondLine_c == u8line)
      {  
        LcdSelectSecondLine();
      }
     else
      {  
        LcdSelectFirstLine();
      }        
     while(mLcdStateTransmittingData_c == mLcdStatus) 
     {
        if(mu8LcdStringLength>0) 
        {
          LcdWriteCmd(mLcdRsData_c, *mu8LcdString++);   
          mu8LcdStringLength--;
        } 
        else 
        {
          mLcdStatus = mLcdStateIdle_c;
          mu8LcdStrFlag = TRUE;
        }  
     }      
     if(NULL != gpfLcdCallBack)
      {
       gpfLcdCallBack(gLcdErrNoError_c);                       
      }
     
    return gLcdErrNoError_c;  
}
#else
lcdErrors_t Lcd_WriteStringBlocking(uint8_t u8line, uint8_t *pString)
{
  (void) u8line;
  (void) pString;  
  return gLcdErrNoError_c;
}
#endif // gLcdSupported_d 

/************************************************************************************
* Lcd_ClearDisplay
************************************************************************************/
#if(TRUE == gLcdSupported_d)
lcdErrors_t Lcd_Clear
(
void
)
{ 
  mbLcdBlockingFlag = TRUE;
  #if (TRUE == gLcdParametersValidation_d)                         
    if(mLcdStateIdle_c != mLcdStatus)
    {    
     return gLcdErrBusBusy_c;
    }
  #endif 
  mLcdStatus = mLcdStateTransmittingData_c;  
  LcdWriteCmd(mLcdRsCmd_c, mLcdClrDisplay_c);
  Delay_us(mWait1500uSec_c);  
  mLcdStatus = mLcdStateIdle_c;
  if(NULL != gpfLcdCallBack)
  {
   gpfLcdCallBack(gLcdErrNoError_c);                       
  }
  return gLcdErrNoError_c;     
}
#else
lcdErrors_t Lcd_Clear(void)
{
   return gLcdErrNoError_c; 
}

#endif  //gLcdSupported_d


#if(TRUE == gLcdSupported_d)
/*************************************************************************************/
#if (gMc1323xRcm_c == gTargetBoard_c) 
static void LcdCtlToogle(void) 
{          
 (void)SPI1_AssertPeripheralSsPin(2,NULL); 
 (void)SPI1_DeassertPeripheralSsPin(); 
}
#else
#endif
/*************************************************************************************/
static void LcdSelectFirstLine
(
  void
)
{   
   LcdWriteCmd( mLcdRsCmd_c, mLcdFirstLine_c );    
}
 

/************************************************************************************/
static void  LcdSelectSecondLine
(
  void
)
{
   //LcdWriteCmd(mLcdRsCmd_c,0x20);     
   LcdWriteCmd( mLcdRsCmd_c, mLcdSecondLine_c );    
}


/*************************************************************************************/
static void LcdWriteCmd
( 
  uint8_t u8RsState, 
  uint8_t u8ByteToWrite
)
{ 
#if(TRUE == gLcdSupported_d) 
    mu8RsState = u8RsState;         
    #if (gMc1323xRcm_c == gTargetBoard_c) 
      LcdSetPort( u8RsState, moreSignificantNibble );
      LcdCtlToogle();
      LcdSetPort( u8RsState, lessSignificantNibble );                                           
      LcdCtlToogle();
    #else
      LcdSetPort( u8RsState, moreSignificantNibble );
      LcdSetPort( u8RsState, lessSignificantNibble );                                           
    #endif
#else
    (void) u8RsState;
    (void) u8ByteToWrite;  
#endif  
}


/*************************************************************************************/

#if (gMc1323xRcm_c == gTargetBoard_c) 
static void LcdWriteCmdNonBlocking
( 
  uint8_t u8RsState, 
  uint8_t u8ByteToWrite
)
{ 
   mu8RsState = u8RsState;  
  (void)SPI1_AssertPeripheralSsPin(2,LcdCallBack);          
  LcdSetPort( u8RsState, moreSignificantNibble );  
}
#endif

/**************************************************************************************/
static void LcdSetPort
(
  uint8_t u8RsState, 
  uint8_t u8ByteToWrite
) 
{ 
  #if (gMc1321xNcb_c== gTargetBoard_c)
  bitByte_t fLcdDataPortValue;
  #endif
  
  #if (gMc1323xRcm_c == gTargetBoard_c) 
     if (TRUE == mbLcdBlockingFlag || u8RsState == mLcdRsCmd_c ) 
     {
      /* when blocking functions */      
        if(mLcdRsData_c == u8RsState) 
        {        
          LcdWrite4BitsBlocking(u8ByteToWrite|0x10);          
        }
        else
        {        
          LcdWrite4BitsBlocking(u8ByteToWrite);    
        }
     } 
     else
     {     
       /* when non-blocking functions*/
       if(mLcdRsData_c == u8RsState) 
        {        
          LcdWrite4BitsNonBlocking(u8ByteToWrite|0x10);          
        }    
     }
  #else if (gMc1320xS08qe128Evb_c == gTargetBoard_c || gMc1321xNcb_c == gTargetBoard_c) 
    LcdToggleEN();
    if(mLcdRsData_c == u8RsState)
    {      
      LcdSetRs();
      Delay_us(mWait25uSec_c); 
    }
    else
    {      
      LcdClearRs();
      Delay_us( mWait25uSec_c ); 
    }   
     #if (gMc1321xNcb_c== gTargetBoard_c) 
       fLcdDataPortValue = Gpio_PortValue(gLcdDataPort_c);
       LcdWrite4Bits( u8ByteToWrite, fLcdDataPortValue );
     #else
       LcdWrite4Bits( u8ByteToWrite ); 
    #endif
    LcdToggleEN();          
  #endif
}

#if (gMc1323xRcm_c == gTargetBoard_c) 
  #ifdef MEMORY_MODEL_BANKED
     #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
     #pragma CODE_SEG DEFAULT
  #endif
  
/**************************************************************************************/
  void LcdCallBack
  ( 
   void
  )
  {
    (void)SPI1_DeassertPeripheralSsPin (); 
    if (mLcdStateTransmittingData_c == mLcdStatus)
      {
         mu8LcdSpiCounter--;
         if ( 0 != mu8LcdSpiCounter ) 
          {    
             if (1 == (0x01 & mu8LcdSpiCounter))/*even*/
              {
                /*send the LSB*/
                (void)SPI1_AssertPeripheralSsPin(2,LcdCallBack); 
                LcdSetPort( mLcdRsData_c, (*mu8LcdString>>4) & mLessSignificantNibbleMask_c );                   
              } 
              else /*odd*/
               {
                /*send the MSB*/
                (void)SPI1_AssertPeripheralSsPin(2,LcdCallBack);
                LcdWriteCmdNonBlocking(mLcdRsData_c, *mu8LcdString++);             
               }      
          }   
         else /*If counter = 0*/
          {
            if (mLcdStateTransmittingData_c == mLcdStatus)
              {        
               mLcdStatus = mLcdStateIdle_c;   
              }            
          }   
      }     
  } 
  #pragma CODE_SEG DEFAULT 
#endif
#endif //#(gLcdSupported_d)