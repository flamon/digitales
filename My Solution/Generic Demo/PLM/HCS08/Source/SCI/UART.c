/******************************************************************************
* Source file for UART/SCI driver.
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*                               
*******************************************************************************/

/*Include files secction*/
#include "UART.h"
#pragma MESSAGE DISABLE C2705
#pragma MESSAGE DISABLE C4001

#if (gUartNumOfPorts_c > 0)

/******************************************************************************
*******************************************************************************
* Private defines and macros
*******************************************************************************
*******************************************************************************/
#define Get_Number_Of_FreeBytes(buffer, portNum)\
  do{\
    if(((mUartRxBufferIndx[portNum]).iLatestData) >= ((mUartTxBufferIndx[portNum]).iStartOfData)){ \
      buffer = ((mUartRxBufferIndx[portNum]).iLatestData) - ((mUartTxBufferIndx[portNum]).iStartOfData);\
    } else {\
      buffer = (gUartRxBuffSize_c - ((mUartTxBufferIndx[portNum]).iStartOfData)) + ((mUartRxBufferIndx[portNum]).iLatestData);\
    }\
  }while(FALSE)


/******************************************************************************
*******************************************************************************
* Private constants
*******************************************************************************
*******************************************************************************/
#if TRUE == gUartSupported_d 
 const uint8_t* maUartStartOfModule[gUartMaxIndex_c] = 
#if gUart1_Enabled_d
 #if gUart2_Enabled_d
                {&SCI1BDH, &SCI2BDH };
 #else
                {&SCI1BDH};
 #endif
#else
 #if gUart2_Enabled_d
                {&SCI2BDH };
 #endif
#endif

#endif //gUartSupported_d


/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
*******************************************************************************/
#if TRUE == gUartSupported_d
static bool_t mSciModuleHwErrors;
static bool_t mIsTxPending[gUartNumOfPorts_c];
static uint8_t mUartRxBuff[gUartNumOfPorts_c][gUartRxBuffSize_c];
static uint8_t mUartTxBuff[gUartNumOfPorts_c][gUartTxBuffSize_c];

static uartBufferIndex_t mUartRxBufferIndx[gUartNumOfPorts_c];
static uartBufferIndex_t mUartTxBufferIndx[gUartNumOfPorts_c];

static uartRxCallback_t mUartRxCbFn[gUartNumOfPorts_c];
static uartTxCallback_t mUartTxCbFn[gUartNumOfPorts_c];

static uint8_t mUartRxCbFlags[gUartNumOfPorts_c];
static uint8_t mUartTxNumOfFreeBytes[gUartNumOfPorts_c];
#endif //gUartSupported_d


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/


/******************************************************************************
* Uart_Init 
*
* Initializes UART module's variables and configure the SCI ports with the 
* specified default values:
*      Loop Mode off, SCI Clocks continue to run in wait mode, 
*      start bit + 8 data bits, Idle line wakeup, no hardware parity checking
*      gUartDefaultBaud_c
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_Init(void){
  uint8_t i;
  uint8_t j;
  uartModule_t *tmpUart;
  
  mSciModuleHwErrors = FALSE;

  for(i=0; i<gUartNumOfPorts_c; i++) {

    mIsTxPending[i] = FALSE;
    tmpUart = (uartModule_t *)(maUartStartOfModule[i]);
    tmpUart->SCIxBDH = (gUartDefaultBaud_c >> 8);
    tmpUart->SCIxBDL = (gUartDefaultBaud_c & 0xFF);
    tmpUart->SCIxC1 = mSCI1C1_Init_c;
    tmpUart->SCIxC3 = mSCI1C3_Init_c;
    tmpUart->SCIxC2 = mSCI1C2_Init_c;

    (mUartRxBufferIndx[i]).iLatestData = 0;
    (mUartRxBufferIndx[i]).iStartOfData = 0;
    (mUartTxBufferIndx[i]).iLatestData = 0;
    (mUartTxBufferIndx[i]).iStartOfData = 0;

    for(j=0; j<gUartRxBuffSize_c; j++)
      mUartRxBuff[i][j] = 0;
    
    for(j=0; j<gUartTxBuffSize_c; j++)
      mUartTxBuff[i][j] = 0;

    mUartTxNumOfFreeBytes[i] = gUartTxBuffSize_c;

    mUartRxCbFn[i] = NULL;
    mUartTxCbFn[i] = NULL;
  }

#if gUart1_Enabled_d
  #if gUart1_EnableHWFlowControl_d
    Uart1_EnableFlow();
  #endif
#endif

#if gUart2_Enabled_d
  #if gUart2_EnableHWFlowControl_d
    Uart2_EnableFlow();
  #endif
#endif

  return Uart_ClearErrors();
}
#else

uartErr_t Uart_Init(void)
{
  return gUartErrNoError_c;
}

#endif //gUartSupported_d

/******************************************************************************
* Uart_ClearErrors 
*
* Clears error bits on the SCI1 & SCI2 Status 1 Reg.  This function is called 
* by Uart_Init
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_ClearErrors(void){
  uint8_t count;
  uint8_t dummy;

#if gUart1_Enabled_d
  count = 0;
  while (SCI1S1 & (mSCIxS1_OR_c | mSCIxS1_NF_c | mSCIxS1_FE_c | mSCIxS1_PF_c)) {
    dummy = SCI1D;
    count++;
    if(gUartMaxHwTimeOut_c < count) {
      mSciModuleHwErrors = TRUE;
      return  gUartErrHwError_c;
    }
  }
#endif

#if gUart2_Enabled_d
  count = 0;
  while (SCI2S1 & (mSCIxS1_OR_c | mSCIxS1_NF_c | mSCIxS1_FE_c | mSCIxS1_PF_c)) {
    dummy = SCI2D;
    count++;
    if(gUartMaxHwTimeOut_c < count){
      mSciModuleHwErrors = TRUE;
      return  gUartErrHwError_c;
    }
  }
#endif

  return gUartErrNoError_c;
}
#else

uartErr_t Uart_ClearErrors(void)
{
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_Configure 
*
* Configures the specified SCI "port" with the specified "pUartSettings". 
* This function configures: 
*                       Baudrate, Number of data bits, Flow control and Parity
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_Configure(uartPortNumber_t port, uartConfigSet_t * pUartSettings){

  uartModule_t *tmpUart;

  if(gUartMaxIndex_c <= port)
    return gUartErrInvalidParameter_c;
  
  tmpUart = (uartModule_t *)(maUartStartOfModule[port]);

  if(NULL ==  pUartSettings)
    return gUartErrInvalidParameter_c;

  /* Set Data bits */
  if(g8DataBits_c == (pUartSettings->dataBits))
    tmpUart->SCIxC1 &= (~mSCIxC1_M_c);
  else if(g9DataBits_c == (pUartSettings->dataBits))
    tmpUart->SCIxC1 |= mSCIxC1_M_c;
  else
    return gUartErrInvalidParameter_c;

  /* Set Parity */
  if(gUartParityNone_c == (pUartSettings->parity))
    tmpUart->SCIxC1 &= (~mSCIxC1_PE_c);
  else if(gUartParityOdd_c == (pUartSettings->parity))
    tmpUart->SCIxC1 |= (mSCIxC1_PE_c|mSCIxC1_PT_c);
  else if(gUartParityEven_c == (pUartSettings->parity)) {
    tmpUart->SCIxC1 |= mSCIxC1_PE_c;
    tmpUart->SCIxC1 &= (~mSCIxC1_PT_c);    
  } else
    return gUartErrInvalidParameter_c;

  /* Set Baud Rate */
  tmpUart->SCIxBDH = ((pUartSettings->baudRate) >> 8);
  tmpUart->SCIxBDL = ((pUartSettings->baudRate) & 0xFF);

  return gUartErrNoError_c;
}
#else

uartErr_t Uart_Configure(uartPortNumber_t port, uartConfigSet_t * pUartSettings)
{
  (void)port;
  (void)pUartSettings;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_GetBytesFromRxBuffer 
*
* Copies a number of bytes from the UART reception buffer to the app buffer.
* When returning from the total number of bytes currently copied are notified 
* at the IN/OUT argument "pNumOfBytes"
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_GetBytesFromRxBuffer(uint8_t *pBuf, uint8_t *pNumOfBytes, uartPortNumber_t port){
  uint8_t i,j;
  j = 0;

  /* Arguments validation */
  if( (NULL == pBuf) ||
      (gUartRxBuffSize_c < (*pNumOfBytes)) ||
      (gUartMaxIndex_c <= port) )
    return gUartErrInvalidParameter_c;
      

  /* If there is no new data then return */
  if(IsRxBufEmpty(port))
    return gUartErrNoNewData_c;
  
  /* Copy UART Rx buffer to Application buffer fill with 0 the extra bytes */
  for(i=0; i<(*pNumOfBytes); i++){
    if(!IsRxBufEmpty(port)){
      *pBuf = mUartRxBuff[port][((mUartRxBufferIndx[port]).iStartOfData)];
      Uart_Inc_RxIndx(((mUartRxBufferIndx[port]).iStartOfData));
      j = i + 1;
    }else{
      *pBuf = '\0';
    }
    pBuf++;
  }
  (*pNumOfBytes) = j;


#if (gUart1_EnableHWFlowControl_d || gUart2_EnableHWFlowControl_d)
  EnableFlowIfConditionsOk(port);
#endif


  return gUartErrNoError_c;
}
#else

uartErr_t Uart_GetBytesFromRxBuffer(uint8_t *pBuf, uint8_t *pNumOfBytes, uartPortNumber_t port)
{
  (void)pBuf;
  (void)pNumOfBytes;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_GetBytesFromRxBufferUntilToken 
*
* Copies a number of bytes from the UART reception buffer to the app buffer 
* until the token is found or UART reception buffer is empty.
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_GetBytesFromRxBufferUntilToken(uint8_t *pBuf, uint8_t token, uartPortNumber_t port){
  uint8_t iRecovery;

  /* Arguments validation */
  if( (NULL == pBuf) ||
      (gUartMaxIndex_c <= port) )
    return gUartErrInvalidParameter_c;
      

  /* If there is no new data return */
  if(IsRxBufEmpty(port))
    return gUartErrNoNewData_c;

  iRecovery = ((mUartRxBufferIndx[port]).iStartOfData);
  /* Copy UART Rx buffer to Application buffer fill with 0 the extra bytes */
  do{
    if(!IsRxBufEmpty(port)){
      *pBuf = mUartRxBuff[port][((mUartRxBufferIndx[port]).iStartOfData)];
      Uart_Inc_RxIndx(((mUartRxBufferIndx[port]).iStartOfData));
    }else{
      ((mUartRxBufferIndx[port]).iStartOfData) = iRecovery;
      return gUartErrNoTokenFound_c;
    }
    pBuf++;
  } while(token != (mUartRxBuff[port][((mUartRxBufferIndx[port]).iStartOfData)]));

  /* Remove the token from the buffer */
  Uart_Inc_RxIndx(((mUartRxBufferIndx[port]).iStartOfData));

#if (gUart1_EnableHWFlowControl_d || gUart2_EnableHWFlowControl_d)
  EnableFlowIfConditionsOk(port);
#endif

  return gUartErrNoError_c;
}
#else

uartErr_t Uart_GetBytesFromRxBufferUntilToken(uint8_t *pBuf, uint8_t token, uartPortNumber_t port)
{
  (void)pBuf;
  (void)token;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_RegisterRxCallBack 
*
* Register the function to be called upon a reception condition on the an 
* specific UART/SCI port
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_RegisterRxCallBack(uartRxCallback_t pFunc, uint8_t flags, uartPortNumber_t port){

  if((NULL == pFunc) ||
     (gUartMaxIndex_c <= port) )
    return gUartErrInvalidParameter_c;
     
  mUartRxCbFn[port] = pFunc;
  mUartRxCbFlags[port] = flags;
  
  return gUartErrNoError_c;
}
#else
uartErr_t Uart_RegisterRxCallBack(uartRxCallback_t pFunc, uint8_t flags, uartPortNumber_t port)
{
  (void)pFunc;
  (void)flags;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_RegisterTxCallBack
*
* Register the function to be called when the TX buffer has been totally
* transmitted
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_RegisterTxCallBack(uartTxCallback_t pfCallBack, uartPortNumber_t port){

  if((NULL == pfCallBack) ||
     (gUartMaxIndex_c <= port) )
    return gUartErrInvalidParameter_c;

  /* If there is an ongoing transmition do not accept the request */
  if(TRUE == mIsTxPending[port])
    return gUartErrTxOnProgress_c;

  /* Register TX callback function */
  mUartTxCbFn[port] = pfCallBack;
  
  return gUartErrNoError_c;
}
#else
uartErr_t Uart_RegisterTxCallBack(uartTxCallback_t pfCallBack, uartPortNumber_t port)
{
  (void)pfCallBack;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d


/******************************************************************************
* Uart_BlokingStringTx 
*
* Transmits a string and does not return until completing the operation
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_BlockingStringTx(uint8_t const *pBuf, uartPortNumber_t port){

  /* If there is an ongoing transmition do not accept the request */
  if(TRUE == mIsTxPending[port])
    return gUartErrTxOnProgress_c;

  /* Arguments validation */
  if(gUartMaxIndex_c <= port)
    return gUartErrInvalidParameter_c;

  while('\0' != (*pBuf)){
    (void)Uart_BlockingByteTx(*pBuf++, port);
  }


  return gUartErrNoError_c;
}
#else
uartErr_t Uart_BlockingStringTx(uint8_t const *pBuf, uartPortNumber_t port)
{
  (void)pBuf;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d

/******************************************************************************
* Uart_BlockingByteTx 
*
* Transmits a byte and return after the byte is transmitted
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_BlockingByteTx(uint8_t datum, uartPortNumber_t port){
  uartModule_t *tmpUart;
  /* If there is an ongoing transmition do not accept the request */
  if(TRUE == mIsTxPending[port])
    return gUartErrTxOnProgress_c;
  
  tmpUart = (uartModule_t *)(maUartStartOfModule[port]);

  while (! ( (tmpUart->SCIxS1) & mSCIxS1_TDRE_c) );  /* ensure Tx data buffer empty */
  tmpUart->SCIxD = datum; /* load data to SCI2 register */
  while (! ( (tmpUart->SCIxS1) & mSCIxS1_TC_c) ); /* wait for Tx complete */

  return gUartErrNoError_c;
}
#else
uartErr_t Uart_BlockingByteTx(uint8_t datum, uartPortNumber_t port)
{
  (void)datum;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d

/******************************************************************************
* Uart_StringTx 
*
* Transmits a \0 terminated string
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_StringTx(uint8_t const *pBuf, uartPortNumber_t port){
  uint8_t i;
  i = StringLen(pBuf);
  return Uart_Transmit(pBuf, i, port); 
}
#else
uartErr_t Uart_StringTx(uint8_t const *pBuf, uartPortNumber_t port)
{
  (void)pBuf;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d

/******************************************************************************
* Uart_Transmit 
*
* Transmits a number of bytes thru the UART/SCI
*
*******************************************************************************/
#if TRUE == gUartSupported_d
uartErr_t Uart_Transmit(uint8_t const *pBuf, uint8_t bufLen, uartPortNumber_t port){
   uint8_t i;
  uartModule_t *tmpUart;


  /* Arguments validation */
  if( (NULL == pBuf)               ||
      (0 == bufLen)                ||
      (gUartMaxIndex_c <= port)
    )
    return gUartErrInvalidParameter_c;

  if(bufLen > mUartTxNumOfFreeBytes[port])
    return gUartErrBuffFull_c;


  /* Disable TX interrupts before updating shared variables */
  tmpUart = (uartModule_t *)(maUartStartOfModule[port]);
#if (gInterruption_c == gUartTxMode_d)
  (tmpUart->SCIxC2) &= ~mSCIxC2_TIE_c;
#endif

  mUartTxNumOfFreeBytes[port] = (mUartTxNumOfFreeBytes[port]) - bufLen;


  /* Copy Application buffer to UART Tx buffer */
  for(i=0; i<bufLen; i++){
    mUartTxBuff[port][((mUartTxBufferIndx[port]).iLatestData)] = (*pBuf);
    Uart_Inc_TxIndx(((mUartTxBufferIndx[port]).iLatestData));
    pBuf++;
  }
  mIsTxPending[port] = TRUE;

  /* Transmit the first byte, other will be sent at ISR */
  i = (tmpUart->SCIxS1);
  tmpUart->SCIxD = mUartTxBuff[port][((mUartTxBufferIndx[port]).iStartOfData)];

  /* Restore UART TX interrupts*/
#if (gInterruption_c == gUartTxMode_d)
  (tmpUart->SCIxC2) |= mSCIxC2_TIE_c;
#endif


  return gUartErrNoError_c;
}
#else
uartErr_t Uart_Transmit(uint8_t const *pBuf, uint8_t bufLen, uartPortNumber_t port)
{
  (void)pBuf;
  (void)bufLen;
  (void)port;
  return gUartErrNoError_c;
}

#endif //gUartSupported_d

#if TRUE == gUartSupported_d

#if gUart1_Enabled_d
/******************************************************************************
* Uart1_Task
*
*
*******************************************************************************/
void Uart1_Task(void){

}
#endif


#if gUart2_Enabled_d /* Not for MC1323x */
/******************************************************************************
* Uart2_Task 
*
*
*******************************************************************************/
void Uart2_Task(void){

}
#endif



#if gUart1_Enabled_d
/******************************************************************************
* Uart1_TxIsr 
*
* Interrupt routine for TX events from UART 1.
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
#if (gInterruption_c == gUartTxMode_d)
INTERRUPT_KEYWORD void Uart1_TxIsr(void) {
#else
void Uart1_TxTask(void){
  if( (TRUE == mIsTxPending[gUart1Index_c]) &&
      (mSCIxS1_TC_c & SCI1S1) )
#endif

  {
    uint8_t dummy;

    Uart_Inc_TxIndx(((mUartTxBufferIndx[gUart1Index_c]).iStartOfData));
    (mUartTxNumOfFreeBytes[gUart1Index_c])++;

    dummy = SCI1S1;
    if(IsTxBufEmpty(gUart1Index_c)){
#if (gInterruption_c == gUartTxMode_d)
      SCI1C2 &= ~mSCIxC2_TIE_c;
#endif
      (mUartTxCbFn[gUart1Index_c])();
      mIsTxPending[gUart1Index_c] = FALSE;
    }
    else{
      SCI1D = mUartTxBuff[gUart1Index_c][((mUartTxBufferIndx[gUart1Index_c]).iStartOfData)];
    }
  }
}
#pragma CODE_SEG DEFAULT


/******************************************************************************
* Uart1_RxIsr 
*
* Interrupt routine for RX events from UART 1.
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Uart1_RxIsr(void){
  uint8_t rxFlags;
  uint8_t dummy;
  uint8_t cBytesInBuffer;

  rxFlags = gUartRxCbCodeNoFlags_c;
  /* Receive register full and no errors */
  if ((SCI1S1 & (mSCIxS1_RDRF_c | mSCIxS1_OR_c | mSCIxS1_NF_c | mSCIxS1_FE_c | mSCIxS1_PF_c)) == mSCIxS1_RDRF_c) 
  {
    mUartRxBuff[gUart1Index_c][((mUartRxBufferIndx[gUart1Index_c]).iLatestData)] = SCI1D;
    Uart_Inc_RxIndx(((mUartRxBufferIndx[gUart1Index_c]).iLatestData));
    rxFlags |= gUartRxCbCodeNewByte_c;


    Get_Number_Of_FreeBytes(cBytesInBuffer, gUart1Index_c);

    if(cBytesInBuffer > gUartRxFlowThreshold_c) 
    {
      rxFlags |= gUartRxCbCodeThreshold_c;
#if gUart1_EnableHWFlowControl_d
      Uart1_DisableFlow();
#endif
    }
    if(cBytesInBuffer == gUartRxBuffSize_c) 
    {
      rxFlags |= gUartRxCbCodeMaxBytes_c;
    }
    
    /* If any of the events match the callback's flags call it*/
    if(gUartRxCbCodeNoFlags_c != (rxFlags & (mUartRxCbFlags[gUart1Index_c]))){
      (mUartRxCbFn[gUart1Index_c])(rxFlags);
    }

  } else {
    dummy = SCI1D;
  }

}
#pragma CODE_SEG DEFAULT


/******************************************************************************
* Uart1_ErrorIsr 
*
* This ISR for SCI1 errors.
* Any code for managing SCI errors could be placed here
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Uart1_ErrorIsr(void){ 
  /* PLACE YOUR CODE HERE */
}
#pragma CODE_SEG DEFAULT


#endif /* gUart1_Enabled_d */


#if gUart2_Enabled_d /* Not for MC1323x */
/******************************************************************************
* Uart2_TxIsr 
*
* Interrupt routine for TX events from UART 2.
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
#if (gInterruption_c == gUartTxMode_d) 
INTERRUPT_KEYWORD void Uart2_TxIsr(void){
#else
void Uart2_TxTask(void){
  if( (TRUE == mIsTxPending[gUart2Index_c]) &&
      (mSCIxS1_TC_c & SCI2S1) )
#endif
 {
    uint8_t dummy;

    Uart_Inc_TxIndx(((mUartTxBufferIndx[gUart2Index_c]).iStartOfData));
    (mUartTxNumOfFreeBytes[gUart2Index_c])++;

    dummy = SCI2S1;
    if(IsTxBufEmpty(gUart2Index_c)){
#if (gInterruption_c == gUartTxMode_d) 
      SCI2C2 &= ~mSCIxC2_TIE_c;
#endif
      (mUartTxCbFn[gUart2Index_c])();
      mIsTxPending[gUart2Index_c] = FALSE;
    } else {
      SCI2D = mUartTxBuff[gUart2Index_c][((mUartTxBufferIndx[gUart2Index_c]).iStartOfData)];
    }
  }

}
#pragma CODE_SEG DEFAULT


/******************************************************************************
* Uart2_RxIsr 
*
* Interrupt routine for RX events from UART 2.
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Uart2_RxIsr(void){
  uint8_t rxFlags;
  uint8_t dummy;
  uint8_t cBytesInBuffer;

  rxFlags = gUartRxCbCodeNoFlags_c;
  /* Receive register full and no errors */
  if ((SCI2S1 & (mSCIxS1_RDRF_c | mSCIxS1_OR_c | mSCIxS1_NF_c | mSCIxS1_FE_c | mSCIxS1_PF_c)) == mSCIxS1_RDRF_c) 
  {
    mUartRxBuff[gUart2Index_c][((mUartRxBufferIndx[gUart2Index_c]).iLatestData)] = SCI2D;
    Uart_Inc_RxIndx(((mUartRxBufferIndx[gUart2Index_c]).iLatestData));
    rxFlags |= gUartRxCbCodeNewByte_c;

    Get_Number_Of_FreeBytes(cBytesInBuffer, gUart2Index_c);

    if(cBytesInBuffer > gUartRxFlowThreshold_c) 
    {
      rxFlags |= gUartRxCbCodeThreshold_c;
#if gUart2_EnableHWFlowControl_d
      Uart2_DisableFlow();
#endif
    }
    if(cBytesInBuffer == gUartRxBuffSize_c) 
    {
      rxFlags |= gUartRxCbCodeMaxBytes_c;
    }

    /* If any of the events match the callback's flags call it*/
    if(gUartRxCbCodeNoFlags_c != (rxFlags & (mUartRxCbFlags[gUart2Index_c]))){
      (mUartRxCbFn[gUart2Index_c])(rxFlags);
    }

  } else {
    dummy = SCI2D;
  }

}
#pragma CODE_SEG DEFAULT


/******************************************************************************
* Uart2_ErrorIsr 
*
* This ISR for SCI2 errors.
* Any code for managing SCI errors could be placed here
*
*******************************************************************************/
#pragma CODE_SEG ISR_SEGMENT
INTERRUPT_KEYWORD void Uart2_ErrorIsr(void){
  /* PLACE YOUR CODE HERE */
}
#pragma CODE_SEG DEFAULT


#endif /* gUart2_Enabled_d */

#endif //gUartSupported_d

/******************************************************************************
*******************************************************************************
* Private functions
*******************************************************************************
*******************************************************************************/

#if TRUE == gUartSupported_d
/******************************************************************************
* EnableFlowIfConditionsOk 
*
* This function checks if there are enought available bytes on the RX buffer
* then if that is the case enables the flow control to continue receiving
*
*******************************************************************************/
static void EnableFlowIfConditionsOk(uartPortNumber_t port){

#if (gUart1_EnableHWFlowControl_d || gUart2_EnableHWFlowControl_d)
  uint8_t cBytesInBuffer;
#else
  (void)port;
#endif

#if gUart1_EnableHWFlowControl_d 
  if(gUart1Index_c == port){
    Get_Number_Of_FreeBytes(cBytesInBuffer, gUart1Index_c);
    if(gUartRxFlowThreshold_c < cBytesInBuffer)
      Uart1_EnableFlow();
  }
#endif

#if gUart2_EnableHWFlowControl_d
  if(gUart2Index_c == port){
    Get_Number_Of_FreeBytes(cBytesInBuffer, gUart2Index_c);
    if(gUartRxFlowThreshold_c < cBytesInBuffer)
      Uart2_EnableFlow();
  }
#endif
  
}


/******************************************************************************
* StringLen 
*
* This function returns the size of a \0 terminated string, 0 in the case of
* NULL pointer
*
*******************************************************************************/
static uint8_t StringLen(uint8_t const *pBuf){
  uint8_t i;
  i = 0;
  while('\0' != *pBuf) {
    pBuf++;
    i++;
  }
  return i;
}

#endif //gUartSupported_d

/******************************************************************************/



#endif /* (gUartNumOfPorts_c > 0) */
