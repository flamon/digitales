/*****************************************************************************
* Lcd Interface header
* 
* Copyright (c) 2009, Freescale, Inc. All rights reserved.
* 
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _Lcd_INTERFACE_H_
   #define _Lcd_INTERFACE_H_
   
/*Include files section*/
#include "derivative.h" 
#include "EmbeddedTypes.h" 
#if (gMc1320xS08qe128Evb_c == gTargetBoard_c || gMc1321xNcb_c == gTargetBoard_c) 
  #include "GPIO_interface.h"
#else if (gMc1323xRcm_c == gTargetBoard_c) 
  #include "SPI_interface.h"
#endif
#include "PortConfig.h"
#include "Utilities_Interface.h"
#include "PLM_Config.h"




/************************************************************************************
*************************************************************************************
* Public defines and macros
*************************************************************************************
************************************************************************************/ 

#ifndef gLcdSupported_d   
  #define gLcdSupported_d   FALSE
#endif

#define gLcdParametersValidation_d TRUE
 

/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/
typedef enum lcdErrors_tag{
  gLcdErrNoError_c = 0,
  gLcdErrInvalidParameter_c,
  gLcdErrBusBusy_c, 
  gLcdErrModuleNoInit_c,                                                  
  gLcdErrMaxError_c
}lcdErrors_t;

#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near LcdCallback_t)(lcdErrors_t);
#else
  typedef void(* __near LcdCallback_t)(lcdErrors_t);
#endif
 
/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Initialize the Lcd driver module and the display to begin receiving commands.
* 
* Interface assumptions:
*   This function must called first before start using the Lcd module. Also, 
* some GPIOs must configured before calling this function. This function is performed in    
* a blocking fashion.
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c      Success operation of the function.
*               gLcdErrNullPointer_c  This occur if no callback parameter passed 
*                                     (only for MC1323x platform with interrupt 
*                                      mode chosen).
*
************************************************************************************/
extern lcdErrors_t Lcd_Init
( 
  LcdCallback_t   pfUserCallBack   //IN, Pointer to the callback function provided by the user
);


/************************************************************************************
* Configure the Lcd display
* 
* Interface assumptions:
*   This function must be called after the Lcd_Init. this function configures the LCD      
* with the options shown below.
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c      Success operation of the function.
*               gLcdErrNullPointer_c  This occur if no callback parameter passed 
*                                     (only for MC1323x platform with interrupt 
*                                      mode chosen).
*
************************************************************************************/
extern lcdErrors_t Lcd_Config
( 
 bool_t LcdDisplayOn,//IN, to set Display ON or OFF
 bool_t LcdCursorOn, //IN  to set Cursor ON or OFF
 bool_t LcdBlinkOn   //IN,  to set Blink ON or OFF
);

/************************************************************************************
* Write a single char to the display in a given line in a blocking fashion.
*
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.
*
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c           Success operation of the function.
*               gLcdErrInvalidParameter_c  This occur if an invalid parameter
*                                          has been used.
*               gLcdErrModuleNoInit_c      This occur if the Lcd module was not
*                                          initialized before.
*               gLcdErrModuleBusy_c        This occur if the Lcd module is not
*                                          finish the latest operation yet. 
*
************************************************************************************/
extern lcdErrors_t Lcd_WriteChar
(
 uint8_t u8line,              //The line where the character will be write. (1 or 2) 
 uint8_t u8CharToWrite        //The character to write
);


/************************************************************************************
* Write a chars string to the display in a given line in a blocking fashion when 
*  the gMc1323xRcm_c target board is selected.
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.
*
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c           Success operation of the function.
*               gLcdErrInvalidParameter_c  This occur if an invalid parameter
*                                          has been used.
*               gLcdErrModuleNoInit_c      This occur if the Lcd module was not
*                                          initialized before.
*               gLcdErrModuleBusy_c        This occur if the Lcd module is not
*                                          finish the latest operation yet. 
*
************************************************************************************/
extern lcdErrors_t Lcd_WriteStringBlocking
( 
  uint8_t u8line,  /* Lcd line where String is to be displayed 1 or 2*/ 
  uint8_t *pString  /* Pointer to the String to be written in the Lcd*/
);



/************************************************************************************
* Write a chars string to the display in a given line in a non-blocking fashion when 
*  the gMc1323xRcm_c target board is selected.
*
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.
*
* Constrains: When this function is called is very important do not modified the
*             string bufer until this function finish its operation.
*             The data buffer must finished with '\0'
*
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c           Success operation of the function.
*               gLcdErrInvalidParameter_c  This occur if an invalid parameter
*                                          has been used.
*               gLcdErrModuleNoInit_c      This occur if the Lcd module was not
*                                          initialized before.
*               gLcdErrModuleBusy_c        This occur if the Lcd module is not
*                                          finish the latest operation yet. 
*
************************************************************************************/
extern lcdErrors_t Lcd_WriteStringNonBlocking
(
 uint8_t u8line,                   //The line where the string will be write. (1 or 2)   
 uint8_t *pStringToWrite           //The string pointer where the data is ready for writing
);   



/************************************************************************************
* Clear the display (both lines).
*
* Interface assumptions:
*   This function will be able to use if the Lcd_Init was called once before.
*
* Return value: lcdErrors_t
*
*               gLcdErrNoError_c           Success operation of the function.              
*               gLcdErrModuleNoInit_c      This occur if the Lcd module was not
*                                          initialized before.
*               gLcdErrModuleBusy_c        This occur if the Lcd module is not
*                                          finish the latest operation yet. 
*
************************************************************************************/
extern lcdErrors_t Lcd_Clear
(
void
);


#endif  /* _Lcd_INTERFACE_H_ */
