/******************************************************************************
* Public header file for the UART/SCI driver.
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/
#ifndef _UART_INTERFACE_H_
#define _UART_INTERFACE_H_

/* Include files section */
#include "EmbeddedTypes.h"
#include "derivative.h"
#include "PLM_Config.h"


/******************************************************************************
*******************************************************************************
* UART/SCI Module configuration definitions
*******************************************************************************
*******************************************************************************/

#ifndef gUartSupported_d   
  #define gUartSupported_d   FALSE
#endif

#define gUartTxMode_d   gInterruption_c

#define gUart1_Enabled_d FALSE
#define gUart2_Enabled_d TRUE
#define gUart1_EnableHWFlowControl_d FALSE
#define gUart2_EnableHWFlowControl_d FALSE

#if (FALSE == gUart1_Enabled_d) && (FALSE == gUart2_Enabled_d)
  #ifdef gMc1323xPlatform_d
    #undef  gUart1_Enabled_d
    #define  gUart1_Enabled_d     FALSE
  #endif 
  #ifdef gMcs08qe128Platform_d 
    #undef  gUart2_Enabled_d
    #define  gUart2_Enabled_d     TRUE
  #endif
  #ifdef gMcs08Gt60Platform_d
    #undef  gUart2_Enabled_d
    #define  gUart2_Enabled_d     TRUE
  #endif
#endif


#ifdef gMc1323xPlatform_d
 #undef  gUart2_Enabled_d
 #define gUart2_Enabled_d  TRUE
#endif

/******************************************************************************
*******************************************************************************
* Public type definitions
*******************************************************************************
*******************************************************************************/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near uartTxCallback_t)(void);
  typedef void __near(* __near uartRxCallback_t)(uint8_t rxConditions);
#else
  typedef void (* __near uartTxCallback_t)(void);
  typedef void (* __near uartRxCallback_t)(uint8_t rxConditions);
#endif

typedef enum uartErr_tag{
  gUartErrNoError_c = 0,                 
  gUartErrBuffFull_c,
  gUartErrHwError_c,
  gUartErrInvalidParameter_c,
  gUartErrLoopTestFail_c,   
  gUartErrNoNewData_c,   
  gUartErrNoTokenFound_c,   
  gUartErrTxOnProgress_c,   
  gUartErrMax_c
} uartErr_t;

typedef enum uartParity_tag{
  gUartParityNone_c = 0,
  gUartParityOdd_c,
  gUartParityEven_c,
  gUartParityMax_c
}uartParity_t;

#define g8DataBits_c  (8)
#define g9DataBits_c  (9)

typedef enum uartPortNumber_tag{
  gUartStrOffset_c = 0,
#if gUart1_Enabled_d
  gUart1Offset_c,
#endif
#if gUart2_Enabled_d
  gUart2Offset_c,
#endif
  gUartMaxOffset_c
}uartPortNumber_t;

#if gUart1_Enabled_d
 #define gUart1Index_c    (gUart1Offset_c-1)
#endif
#if gUart2_Enabled_d
 #define gUart2Index_c    (gUart2Offset_c-1)
#endif
#define gUartMaxIndex_c   (gUartMaxOffset_c-1)


typedef struct uartConfigSet_tag{
  uint16_t      baudRate;
  uint8_t       dataBits;
  uartParity_t  parity;
}uartConfigSet_t;


/******************************************************************************
*******************************************************************************
* Public macros
*******************************************************************************
*******************************************************************************/


/* Baud Rate Modulo Divisor */
#define Uart_BrVal(baudrate) (gSystemClock_d/(baudrate*16))

#define gUARTBaudRate110_c      Uart_BrVal(110)
#define gUARTBaudRate300_c      Uart_BrVal(300)
#define gUARTBaudRate1200_c     Uart_BrVal(1200)
#define gUARTBaudRate2400_c     Uart_BrVal(2400)
#define gUARTBaudRate4800_c     Uart_BrVal(4800)
#define gUARTBaudRate9600_c     Uart_BrVal(9600)
#define gUARTBaudRate19200_c    Uart_BrVal(19200)
#define gUARTBaudRate38400_c    Uart_BrVal(38400)
#define gUARTBaudRate57600_c    Uart_BrVal(57600)
#define gUARTBaudRate115200_c   Uart_BrVal(115200)

#define Baudrate_110         gUARTBaudRate110_c
#define Baudrate_300         gUARTBaudRate300_c
#define Baudrate_1200        gUARTBaudRate1200_c
#define Baudrate_2400        gUARTBaudRate2400_c
#define Baudrate_4800        gUARTBaudRate4800_c
#define Baudrate_9600        gUARTBaudRate9600_c
#define Baudrate_19200       gUARTBaudRate19200_c
#define Baudrate_38400       gUARTBaudRate38400_c
#define Baudrate_57600       gUARTBaudRate57600_c
#define Baudrate_115200      gUARTBaudRate115200_c


#define gUartDefaultBaud_c      Baudrate_38400



#define gUartDefaultFlowControl_c  0

#if gUart1_Enabled_d
 #if gUart2_Enabled_d
  #define gUartNumOfPorts_c    2
 #else
  #define gUartNumOfPorts_c    1
 #endif
#else
 #if gUart2_Enabled_d
  #define gUartNumOfPorts_c    1
 #else
  #define gUartNumOfPorts_c    0
 #endif
#endif



#define Uart_LoopEnable(uartNumber)  (SCI##uartNumber##C1 |= 0x80)
#define Uart_LoopDisable(uartNumber) (SCI##uartNumber##C1 &= 0x7F)

#if gUart1_Enabled_d
 #define Uart1_LoopEnable()  Uart_LoopEnable(1)
 #define Uart1_LoopDisable() Uart_LoopDisable(1)
#endif

#if gUart2_Enabled_d
 #define Uart2_LoopEnable()  Uart_LoopEnable(2)
 #define Uart2_LoopDisable() Uart_LoopDisable(2)
#endif


#ifdef gMcs08qe128Platform_d
 #define gClockGatingUartMask (0x03)
 #define Uart_EnableClockGating()  (SCGC1 |= gClockGatingUartMask )
 #define Uart_DisableClockGating() (SCGC1 &= (~gClockGatingUartMask) )
#else
 #ifdef gMc1323xPlatform_d
  #define gClockGatingUartMask (0x01)
  #define Uart_EnableClockGating()  (SCGC1 |= gClockGatingUartMask )
  #define Uart_DisableClockGating() (SCGC1 &= (~gClockGatingUartMask) )
 #else
  #define Uart_EnableClockGating()
  #define Uart_DisableClockGating()
 #endif
#endif


#define Uart_GetByteFromRxBuffer(pOnyByteBuf,port) Uart_GetBytesFromRxBuffer(pOnyByteBuf,1,port)

#define BitMappedFlag(x) (1<<x)
#define gUartRxCbCodeNoFlags_c     (0)
#define gUartRxCbCodeMaxBytes_c    BitMappedFlag(0)
#define gUartRxCbCodeThreshold_c   BitMappedFlag(1)
#define gUartRxCbCodeNewByte_c     BitMappedFlag(2)
#define gUartRxCbCodeTokenFound_c  BitMappedFlag(3)


#if gUart1_Enabled_d
  #define Uart1_Configure(settings)   Uart_Configure(gUart1Index_c, settings)
  #define Uart1_GetBytesFromRxBuffer(buffer, size)\
                 Uart_GetBytesFromRxBuffer(buffer, size, gUart1Index_c)
  #define Uart1_GetBytesFromRxBufferUntilToken(buffer, t)\
                 Uart_GetBytesFromRxBufferUntilToken(buffer, t, gUart1Index_c)
  #define Uart1_RegisterRxCallBack(routine, flags)\
                 Uart_RegisterRxCallBack(routine, flags, gUart1Index_c)
  #define Uart1_RegisterTxCallBack(routine)\
                 Uart_RegisterTxCallBack(routine, gUart1Index_c)
  #define Uart1_BlockingStringTx(buffer)  Uart_BlockingStringTx(buffer, gUart1Index_c)
  #define Uart1_BlockingByteTx(b) Uart_BlockingByteTx(b, gUart1Index_c)
  #define Uart1_StringTx(buffer) Uart_StringTx(buffer, gUart1Index_c)
  #define Uart1_Transmit(buffer,size) Uart_Transmit(buffer, size, gUart1Index_c)
#endif


#if gUart2_Enabled_d
  #define Uart2_Configure(settings)   Uart_Configure(gUart2Index_c, settings)
  #define Uart2_GetBytesFromRxBuffer(buffer, size)\
                 Uart_GetBytesFromRxBuffer(buffer, size, gUart2Index_c)
  #define Uart2_GetBytesFromRxBufferUntilToken(buffer, t)\
                 Uart_GetBytesFromRxBufferUntilToken(buffer, t, gUart2Index_c)
  #define Uart2_RegisterRxCallBack(routine, flags)\
                 Uart_RegisterRxCallBack(routine, flags, gUart2Index_c)
  #define Uart2_RegisterTxCallBack(routine)\
                 Uart_RegisterTxCallBack(routine, gUart2Index_c)
  #define Uart2_BlockingStringTx(buffer)  Uart_BlockingStringTx(buffer, gUart2Index_c)
  #define Uart2_BlockingByteTx(b) Uart_BlockingByteTx(b, gUart2Index_c)
  #define Uart2_StringTx(buffer) Uart_StringTx(buffer, gUart2Index_c)
  #define Uart2_Transmit(buffer,size) Uart_Transmit(buffer, size, gUart2Index_c)
#endif


/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
*******************************************************************************/


/******************************************************************************
* Uart_Init initializes UART/SCI module's variables and configures the SCI
* ports with the specified default values:
*      Loop Mode off, SCI Clocks continue to run in wait mode, 
*      start bit + 8 data bits, idle line wakeup, no hardware parity checking
*      gUartDefaultBaud_c
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrHwError_c - If the error bits are not cleared after 0x80 retires
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_Init(void);



/******************************************************************************
* Uart_ClearErrors clears error bits on the SCI1 & SCI2 Status 1 Reg.  This
* function is called by Uart_Init.
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrHwError_c - If the error bits are not cleared after 0x80 retires
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_ClearErrors(void);



/******************************************************************************
* Uart_Configure configures the specified SCI "port" with the 
* specified "uartSettings".  This function configures: 
*                      Baudrate, Number of data bits, Flow control and Parity
*
*
* Interface assumptions:
*   In case of a gUartErrInvalidParameter_c error SCI configuration stay on 
*   undetermined state and the application shall configure it
*
* return value:
*   gUartErrInvalidParameter_c - If port number is not a valid one or any of 
*                                the register settings is out of range
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_Configure
(
 uartPortNumber_t port,          /* IN: UART/SCI Port number to be configured */
 uartConfigSet_t * pUartSettings  /* IN: A pointer to an structure containing 
                                        the setting for configuring the port  */
);



/******************************************************************************
* Uart_GetBytesFromRxBuffer copies a number of bytes from the UART reception 
* buffer to the app buffer.    When returning from the total number of bytes
* currently copied are notified at the IN/OUT argument "pNumOfBytes".
*
* Interface assumptions:
*   pBuf?s size shall be at least gUartRxBuffSize_c.
*
* return value:
*   gUartErrInvalidParameter_c - If the app buffer is NULL or port number is
*                                out of range
*   gUartErrNoError_c - When the function is successfully performed
*   gUartErrNoNewData_c - If UART/SCI reception buffer is empty
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_GetBytesFromRxBuffer
(
 uint8_t *pBuf,        /* OUT: Pointer to the application buffer where the data
                               will be stored */
 uint8_t *pNumOfBytes,  /* IN/OUT: When enter to the function: it is the number
                                  of bytes to be copied on the application's
                                  buffer / When leaving the function it represent
                                  the actual number of bytes copied  */
 uartPortNumber_t port /* IN: UART/SCI Port number where the bytes come from */
);



/******************************************************************************
* Uart_GetBytesFromRxBufferUntilToken
*
* Interface assumptions:
*   pBuf?s size shall be at least gUartRxBuffSize_c.
*   pBuf could be written and modified even if gUartErrNoTokenFound_c occurs
*
* return value:
*   gUartErrInvalidParameter_c - If the app buffer is NULL or port number is
*                                out of range
*   gUartErrNoError_c - When the function is successfully performed
*   gUartErrNoNewData_c - If UART/SCI reception buffer is empty
*   gUartErrNoTokenFound_c - If the specified token is not on present on 
*                            UART/SCI's reception buffer
*   
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_GetBytesFromRxBufferUntilToken
(
 uint8_t *pBuf,        /* OUT: Pointer to the application buffer where the data
                               will be stored */
 uint8_t token,        /* IN: Token to be found on the reception buffer */
 uartPortNumber_t port /* IN: UART/SCI Port number where the bytes come from */
);



/******************************************************************************
* Uart_RegisterRxCallBack registers the function to be called upon a reception
* condition on the an specific UART/SCI port.
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrInvalidParameter_c - If the callback is NULL or port number is
*                                out of range
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_RegisterRxCallBack
(
 uartRxCallback_t pFunc, /* IN: Pointer to that callback function to be executed 
                            when any of the receiving conditions is reach */
 uint8_t flags,          /* IN: Flags to indicate over which conditions the 
                                callback shall be executed.
                                                Ej. gUartRxCbCodeNewByte_c */
 uartPortNumber_t port   /* IN: UART/SCI Port number to assign the callback */
);



/******************************************************************************
* Uart_RegisterTxCallBack
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrInvalidParameter_c - If the callback is NULL or port number is
*                                out of range
*   gUartErrNoError_c - When the function is successfully performed
*   gUartErrTxOnProgress_c - If a transmit is on going, then it is not 
*                            allowed to update its callback function
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_RegisterTxCallBack
(
 uartTxCallback_t pfCallBack, /* IN: Pointer to that callback function to be 
                                     executed when the transmit id finished */
 uartPortNumber_t port        /* IN: UART/SCI Port number to assign the callback */
);


/******************************************************************************
* Uart_BlockingStringTx transmits a \0 terminated string and return after the 
* string is transmitted
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrInvalidParameter_c - If port number is out of range
*   gUartErrNoError_c - When the function is successfully performed
*   gUartErrTxOnProgress_c - If a transmit is on going, then it is not 
*                            allowed to transmit
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_BlockingStringTx
(
 uint8_t const *pBuf,  /* IN: Pointer to the buffer that is required to transmit */
 uartPortNumber_t port /* IN: Port number to use   */
);


/******************************************************************************
* Uart_BlockingByteTx transmits a byte and return after the byte is transmitted
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrNoError_c - When the function is successfully performed
*   gUartErrTxOnProgress_c - If a transmit is on going, then it is not 
*                            allowed to transmit
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_BlockingByteTx
(
 uint8_t datum,        /* IN: The byte to transmit */
 uartPortNumber_t port /* IN: Port number to use   */
);


/******************************************************************************
* Uart_StringTx transmits a \0 terminated string
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrBuffFull_c - If UART TX buffer does not have enough room to store 
*                        the string to be transmitted
*   gUartErrInvalidParameter_c - If string's length is zero or string points 
*                                to NULL or port number is out of range
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_StringTx
(
 uint8_t const *pBuf,  /* IN: Pointer to the buffer that is required to be 
                               transmited */ 
 uartPortNumber_t port /* IN: UART/SCI Port number to use */
);



/******************************************************************************
* Uart_Transmit transmits a number of bytes thru the UART/SCI
*
* Interface assumptions:
*   NONE
*
* return value:
*   gUartErrBuffFull_c - If UART TX buffer does not have enough room to store 
*                        the requested data
*   gUartErrInvalidParameter_c - If length is zero or the argument pointing 
*                                to the buffer is NULL or port number is out
*                                of range
*   gUartErrNoError_c - When the function is successfully performed
*
* Effects on global data:
*   NONE
*
*******************************************************************************/
extern uartErr_t Uart_Transmit
(
 uint8_t const *pBuf,   /* IN: Pointer to the buffer that is required to be 
                               transmited */ 
 uint8_t bufLen,        /* IN: Number of bytes to be transmitted */
 uartPortNumber_t port  /* IN: UART/SCI Port number to use */
);

/*******************************************************************************/

#if gUart1_Enabled_d
#pragma CODE_SEG ISR_SEGMENT
#if (gPolling_c == gUartTxMode_d)
 extern void Uart1_TxTask(void);
#else
 extern INTERRUPT_KEYWORD void Uart1_TxIsr(void);
#endif
extern INTERRUPT_KEYWORD void Uart1_RxIsr(void);
extern INTERRUPT_KEYWORD void Uart1_ErrorIsr(void);
#pragma CODE_SEG DEFAULT

#endif /* gUart1_Enabled_d */

/*******************************************************************************/

#if gUart2_Enabled_d

#pragma CODE_SEG ISR_SEGMENT
#if (gPolling_c == gUartTxMode_d)
 extern void Uart2_TxTask(void);                         // - Not for MC1323x -
#else
 extern INTERRUPT_KEYWORD void Uart2_TxIsr(void);      // - Not for MC1323x -
#endif
extern INTERRUPT_KEYWORD void Uart2_RxIsr(void);      // - Not for MC1323x -
extern INTERRUPT_KEYWORD void Uart2_ErrorIsr(void);   // - Not for MC1323x -
#pragma CODE_SEG DEFAULT

#endif /* gUart2_Enabled_d */

/*******************************************************************************/

#endif  /* _UART_INTERFACE_H_ */

