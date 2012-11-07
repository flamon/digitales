/************************************************************************************
* Private header file for the Lcd driver.
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
#ifndef _LCD_H_
   #define _LCD_H_

#include "Lcd_Interface.h"



/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum LcdState_tag
{
  mLcdStateNotInit_c = 0,  
  mLcdStateIdle_c,
  mLcdStateTransmittingData_c,
  mLcdStateMaxState_c 
}lcdState_t;

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************          
************************************************************************************/

#if (gTargetBoard_c == gMc1323xRcm_c)  
  #define mLcdSecondLine_c   (0x90)  
#else 
  #define mLcdSecondLine_c   (0xC0)   
#endif

#define mLcdMaxChars_c     (0x10)
#define mLcdCharSize_c     (0x01)

#define mLcdRsCmd_c        (0x00)
#define mLcdRsData_c       (0x10)
#define mLcdClrDisplay_c   (0x01)
#define mLcdFirstLine_c    (0x80)

#define mLcdMaxLines_c	   (0x02)

#define mSecondLine_c      (2)

#define mMoreSignificantNibbleMask_c (0xF0)
#define mLessSignificantNibbleMask_c (0x0F)   

#define mWait40mSec_c      (15)
#define mWait10mSec_c      (5)

#define mWait125uSec_c     (25)
#define mWait25uSec_c      (2)
#define mWait1500uSec_c    (150)

#if (gTargetBoard_c == gMc1320xS08qe128Evb_c || gTargetBoard_c == gMc1321xNcb_c)
    #define LcdToggleEN() (void)Gpio_PinToggle(gLcdCtrlPort_c, gLcdEnablePin_c)  
    #define LcdClear_E_RS_RW() (void)Gpio_PinClear(gLcdCtrlPort_c,gLcdRegSelectPin_c);\
                            (void)Gpio_PinClear(gLcdCtrlPort_c, gLcdEnablePin_c);\
                            (void)Gpio_PinClear(gLcdCtrlPort_c, gLcdReadWritePin_c)                           
    #define LcdSetRs() (void)Gpio_PinSet(gLcdCtrlPort_c, gLcdRegSelectPin_c)  
    #define LcdClearRs() (void)Gpio_PinClear(gLcdCtrlPort_c, gLcdRegSelectPin_c)
    #if (gTargetBoard_c == gMc1320xS08qe128Evb_c)  
      #define LcdWrite4Bits(u8Data) (void)Gpio_PortWrite(gLcdDataPort_c,u8Data)
    #endif
    #if (gTargetBoard_c == gMc1321xNcb_c)    
      #define LcdWrite4Bits(u8Data,u8PortValue) (void)Gpio_PortWrite(gLcdDataPort_c,u8Data|(u8PortValue.port & 0x0F));
      #define LcdEnable()  (void)Gpio_PinClear(gLcdCtrlPort_c,gLcdTurnOnPin_c)    
      #define LcdDisable() (void)Gpio_PinSet(gLcdCtrlPort_c,gLcdTurnOnPin_c)
    #endif     
    #define moreSignificantNibble u8ByteToWrite & mMoreSignificantNibbleMask_c
    #define lessSignificantNibble (u8ByteToWrite<<4) & mMoreSignificantNibbleMask_c
#endif  

#if (gTargetBoard_c == gMc1323xRcm_c)           
    #define moreSignificantNibble (u8ByteToWrite>>4) & mLessSignificantNibbleMask_c
    #define lessSignificantNibble u8ByteToWrite & mLessSignificantNibbleMask_c   
    #define LcdWrite4BitsBlocking(u8Data) (void)SPI1_WriteByteBlocking(u8Data) 
    #define LcdWrite4BitsNonBlocking(u8Data) (void)SPI1_WriteByte(u8Data)   
#endif   

/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/
#if(TRUE == gLcdSupported_d)
/************************************************************************************
* LcdSelectFirstLine
* This function is for selecting the first line in the display.
* 
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.    
*
* Return value: None
*
************************************************************************************/
static void LcdSelectFirstLine
(
  void
);

/************************************************************************************
* LcdSelectSecondLine
* This function is for selecting the second line in the display.
* 
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.    
*
* Return value: None
*
************************************************************************************/
static void LcdSelectSecondLine
(
  void
);

/************************************************************************************
* LcdWriteCmd
* This function is for writing commands or data in the display.
* 
* Return value: None
*
************************************************************************************/
static void LcdWriteCmd
(
 uint8_t, 
 uint8_t
);

/************************************************************************************
* LcdWriteCmdNonBlocking
* This function is for writing commands or data in the display in a non-blocking way.
* 
* Return value: None
*
************************************************************************************/
static void LcdWriteCmdNonBlocking
( 
  uint8_t,
  uint8_t
);

/************************************************************************************
* LcdSetPort
* This function is for writing in nibble mode to the display.    
*
* Return value: None
*
************************************************************************************/
static void LcdSetPort
(
  uint8_t, 
  uint8_t
);


/************************************************************************************
* LcdCtlToogle
* This function is for writing in nibble mode to the display.    
*
* Return value: None
*
************************************************************************************/
static void LcdCtlToogle
(
void
);


#if (gMc1323xRcm_c == gTargetBoard_c)
  #ifdef MEMORY_MODEL_BANKED
      #pragma CODE_SEG __NEAR_SEG NON_BANKED
  #else
      #pragma CODE_SEG DEFAULT
  #endif
  /************************************************************************************
  * LcdCallBack
  * This function is the callback function sent to the SPI module.
  *
  * Return value: None
  *
  ************************************************************************************/
  static void LcdCallBack
  (
  void
  );
    
  #pragma CODE_SEG DEFAULT

#endif 

#endif //gLcdSupported_d

#endif /* _LCD_H_ */