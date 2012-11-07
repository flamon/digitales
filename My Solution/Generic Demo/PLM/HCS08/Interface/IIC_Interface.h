/*****************************************************************************
* IIC Interface header
* 
* Copyright (c) 2010, Freescale, Inc. All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/


#ifndef __IIC_INTERFACE_H__               
  #define __IIC_INTERFACE_H__

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "PortConfig.h"     // include board specific defines
#include "PLM_Config.h"

/************************************************************************************
*************************************************************************************
* Interface Constants
*************************************************************************************
************************************************************************************/

#ifndef gIicSupported_d
 #define gIicSupported_d     FALSE
#endif


#ifndef gIicMode_d
 #define gIicMode_d     gInterruption_c
#endif

#define mIicMaxSlaveAddres_c 0x7f                 

#define gIicPoll_c      gPolling_c                
#define gIicInterrupt_c gInterruption_c                

/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

typedef enum iicResult_tag{
    gIicTxRxSuccess_c = 0,
    gIicTxNoAck_c,	
    gIicMaxResult_c
}iicResult_t;

/*IIC callback pointer definition
  This callback function will be called when all the asked bytes have been sent trougth 
  the IIC bus, It's called in the IIC interrupt context.
  The function shall be placed on the non-banked flash section. 
*/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near iicCallback_t)(iicResult_t);
#else
  typedef void(* __near iicCallback_t)(iicResult_t);
#endif

typedef enum iicErr_tag{
  gIicErrNoError_c = 0,
  gIicErrInvalidParameter_c,
  gIicErrModuleBusy_c,
  gIicErrInvalidCondition_c,
  gIicErrNoAck_c,
  gIicErrMax_c
}iicErr_t;


/* Comstats used to set the IIC baud rate
-	IIC baud rate = bus speed (Hz)/(MultiplierFactor * SCL divider)
-	SDA hold time = bus period (s) * MultiplierFactor * SDA hold value
-	SCL Start hold time = bus period (s) * MultiplierFactor * SCL Start hold value
-	SCL Stop hold time = bus period (s) * MultiplierFactor * SCL Stop hold value
*/
typedef enum iicMultiplierFactors_tag{
    gIicMul_1_c    = 0x00,                 
    gIicMul_2_c    = 0x01,                 
    gIicMul_4_c    = 0x02,                 
    gIicMulMax_c   = 0x03                 
} iicMultiplierFactors_t;

typedef enum iicClockRates_tag{
  gIicICR_00_c = 0x00,/* SCL divider:   20, SDA Hold Value:   7, SCL Hold Start Value:    6, SDA Hold Stop Value:   11 */
  gIicICR_01_c = 0x01,/* SCL divider:   22, SDA Hold Value:   7, SCL Hold Start Value:    7, SDA Hold Stop Value:   12 */
  gIicICR_02_c = 0x02,/* SCL divider:   24, SDA Hold Value:   8, SCL Hold Start Value:    8, SDA Hold Stop Value:   13 */
  gIicICR_03_c = 0x03,/* SCL divider:   26, SDA Hold Value:   8, SCL Hold Start Value:    9, SDA Hold Stop Value:   14 */
  gIicICR_04_c = 0x04,/* SCL divider:   28, SDA Hold Value:   9, SCL Hold Start Value:   10, SDA Hold Stop Value:   15 */
  gIicICR_05_c = 0x05,/* SCL divider:   30, SDA Hold Value:   9, SCL Hold Start Value:   11, SDA Hold Stop Value:   16 */
  gIicICR_06_c = 0x06,/* SCL divider:   34, SDA Hold Value:  10, SCL Hold Start Value:   13, SDA Hold Stop Value:   18 */
  gIicICR_07_c = 0x07,/* SCL divider:   40, SDA Hold Value:  10, SCL Hold Start Value:   16, SDA Hold Stop Value:   21 */
  gIicICR_08_c = 0x08,/* SCL divider:   28, SDA Hold Value:   7, SCL Hold Start Value:   10, SDA Hold Stop Value:   15 */
  gIicICR_09_c = 0x09,/* SCL divider:   32, SDA Hold Value:   7, SCL Hold Start Value:   12, SDA Hold Stop Value:   17 */
  gIicICR_0A_c = 0x0A,/* SCL divider:   36, SDA Hold Value:   9, SCL Hold Start Value:   14, SDA Hold Stop Value:   19 */
  gIicICR_0B_c = 0x0B,/* SCL divider:   40, SDA Hold Value:   9, SCL Hold Start Value:   16, SDA Hold Stop Value:   21 */
  gIicICR_0C_c = 0x0C,/* SCL divider:   44, SDA Hold Value:  11, SCL Hold Start Value:   18, SDA Hold Stop Value:   23 */
  gIicICR_0D_c = 0x0D,/* SCL divider:   48, SDA Hold Value:  11, SCL Hold Start Value:   20, SDA Hold Stop Value:   25 */
  gIicICR_0E_c = 0x0E,/* SCL divider:   56, SDA Hold Value:  13, SCL Hold Start Value:   24, SDA Hold Stop Value:   29 */
  gIicICR_0F_c = 0x0F,/* SCL divider:   68, SDA Hold Value:  13, SCL Hold Start Value:   30, SDA Hold Stop Value:   35 */
  gIicICR_10_c = 0x10,/* SCL divider:   48, SDA Hold Value:   9, SCL Hold Start Value:   18, SDA Hold Stop Value:   25 */
  gIicICR_11_c = 0x11,/* SCL divider:   56, SDA Hold Value:   9, SCL Hold Start Value:   22, SDA Hold Stop Value:   29 */
  gIicICR_12_c = 0x12,/* SCL divider:   64, SDA Hold Value:  13, SCL Hold Start Value:   26, SDA Hold Stop Value:   33 */
  gIicICR_13_c = 0x13,/* SCL divider:   72, SDA Hold Value:  13, SCL Hold Start Value:   30, SDA Hold Stop Value:   37 */
  gIicICR_14_c = 0x14,/* SCL divider:   80, SDA Hold Value:  17, SCL Hold Start Value:   34, SDA Hold Stop Value:   41 */
  gIicICR_15_c = 0x15,/* SCL divider:   88, SDA Hold Value:  17, SCL Hold Start Value:   38, SDA Hold Stop Value:   45 */
  gIicICR_16_c = 0x16,/* SCL divider:  104, SDA Hold Value:  21, SCL Hold Start Value:   46, SDA Hold Stop Value:   53 */
  gIicICR_17_c = 0x17,/* SCL divider:  128, SDA Hold Value:  21, SCL Hold Start Value:   58, SDA Hold Stop Value:   65 */
  gIicICR_18_c = 0x18,/* SCL divider:   80, SDA Hold Value:   9, SCL Hold Start Value:   38, SDA Hold Stop Value:   41 */
  gIicICR_19_c = 0x19,/* SCL divider:   96, SDA Hold Value:   9, SCL Hold Start Value:   46, SDA Hold Stop Value:   49 */
  gIicICR_1A_c = 0x1A,/* SCL divider:  112, SDA Hold Value:  17, SCL Hold Start Value:   54, SDA Hold Stop Value:   57 */
  gIicICR_1B_c = 0x1B,/* SCL divider:  128, SDA Hold Value:  17, SCL Hold Start Value:   62, SDA Hold Stop Value:   65 */
  gIicICR_1C_c = 0x1C,/* SCL divider:  144, SDA Hold Value:  25, SCL Hold Start Value:   70, SDA Hold Stop Value:   73 */
  gIicICR_1D_c = 0x1D,/* SCL divider:  160, SDA Hold Value:  25, SCL Hold Start Value:   78, SDA Hold Stop Value:   81 */ 
  gIicICR_1E_c = 0x1E,/* SCL divider:  192, SDA Hold Value:  33, SCL Hold Start Value:   94, SDA Hold Stop Value:   97 */
  gIicICR_1F_c = 0x1F,/* SCL divider:  240, SDA Hold Value:  33, SCL Hold Start Value:  118, SDA Hold Stop Value:  121 */
  gIicICR_20_c = 0x20,/* SCL divider:  160, SDA Hold Value:  17, SCL Hold Start Value:   78, SDA Hold Stop Value:   81 */
  gIicICR_21_c = 0x21,/* SCL divider:  192, SDA Hold Value:  17, SCL Hold Start Value:   94, SDA Hold Stop Value:   97 */
  gIicICR_22_c = 0x22,/* SCL divider:  224, SDA Hold Value:  33, SCL Hold Start Value:  110, SDA Hold Stop Value:  113 */
  gIicICR_23_c = 0x23,/* SCL divider:  256, SDA Hold Value:  33, SCL Hold Start Value:  126, SDA Hold Stop Value:  129 */
  gIicICR_24_c = 0x24,/* SCL divider:  288, SDA Hold Value:  49, SCL Hold Start Value:  142, SDA Hold Stop Value:  145 */
  gIicICR_25_c = 0x25,/* SCL divider:  320, SDA Hold Value:  49, SCL Hold Start Value:  158, SDA Hold Stop Value:  161 */
  gIicICR_26_c = 0x26,/* SCL divider:  384, SDA Hold Value:  65, SCL Hold Start Value:  190, SDA Hold Stop Value:  193 */
  gIicICR_27_c = 0x27,/* SCL divider:  480, SDA Hold Value:  65, SCL Hold Start Value:  238, SDA Hold Stop Value:  241 */
  gIicICR_28_c = 0x28,/* SCL divider:  320, SDA Hold Value:  33, SCL Hold Start Value:  158, SDA Hold Stop Value:  161 */
  gIicICR_29_c = 0x29,/* SCL divider:  384, SDA Hold Value:  33, SCL Hold Start Value:  190, SDA Hold Stop Value:  193 */
  gIicICR_2A_c = 0x2A,/* SCL divider:  448, SDA Hold Value:  65, SCL Hold Start Value:  222, SDA Hold Stop Value:  225 */  
  gIicICR_2B_c = 0x2B,/* SCL divider:  512, SDA Hold Value:  65, SCL Hold Start Value:  254, SDA Hold Stop Value:  257 */
  gIicICR_2C_c = 0x2C,/* SCL divider:  576, SDA Hold Value:  97, SCL Hold Start Value:  286, SDA Hold Stop Value:  289 */
  gIicICR_2D_c = 0x2D,/* SCL divider:  640, SDA Hold Value:  97, SCL Hold Start Value:  318, SDA Hold Stop Value:  321 */
  gIicICR_2E_c = 0x2E,/* SCL divider:  768, SDA Hold Value: 129, SCL Hold Start Value:  382, SDA Hold Stop Value:  385 */
  gIicICR_2F_c = 0x2F,/* SCL divider:  960, SDA Hold Value: 129, SCL Hold Start Value:  478, SDA Hold Stop Value:  481 */
  gIicICR_30_c = 0x30,/* SCL divider:  640, SDA Hold Value:  65, SCL Hold Start Value:  318, SDA Hold Stop Value:  321 */
  gIicICR_31_c = 0x31,/* SCL divider:  768, SDA Hold Value:  65, SCL Hold Start Value:  382, SDA Hold Stop Value:  385 */
  gIicICR_32_c = 0x32,/* SCL divider:  896, SDA Hold Value: 129, SCL Hold Start Value:  446, SDA Hold Stop Value:  449 */
  gIicICR_33_c = 0x33,/* SCL divider: 1024, SDA Hold Value: 129, SCL Hold Start Value:  510, SDA Hold Stop Value:  513 */
  gIicICR_34_c = 0x34,/* SCL divider: 1152, SDA Hold Value: 193, SCL Hold Start Value:  574, SDA Hold Stop Value:  577 */
  gIicICR_35_c = 0x35,/* SCL divider: 1280, SDA Hold Value: 193, SCL Hold Start Value:  638, SDA Hold Stop Value:  641 */
  gIicICR_36_c = 0x36,/* SCL divider: 1536, SDA Hold Value: 257, SCL Hold Start Value:  766, SDA Hold Stop Value:  769 */
  gIicICR_37_c = 0x37,/* SCL divider: 1920, SDA Hold Value: 257, SCL Hold Start Value:  958, SDA Hold Stop Value:  961 */
  gIicICR_38_c = 0x38,/* SCL divider: 1280, SDA Hold Value: 129, SCL Hold Start Value:  638, SDA Hold Stop Value:  641 */
  gIicICR_39_c = 0x39,/* SCL divider: 1536, SDA Hold Value: 129, SCL Hold Start Value:  766, SDA Hold Stop Value:  769 */
  gIicICR_3A_c = 0x3A,/* SCL divider: 1792, SDA Hold Value: 257, SCL Hold Start Value:  894, SDA Hold Stop Value:  897 */
  gIicICR_3B_c = 0x3B,/* SCL divider: 2048, SDA Hold Value: 257, SCL Hold Start Value: 1022, SDA Hold Stop Value: 1025 */
  gIicICR_3C_c = 0x3C,/* SCL divider: 2304, SDA Hold Value: 385, SCL Hold Start Value: 1150, SDA Hold Stop Value: 1153 */
  gIicICR_3D_c = 0x3D,/* SCL divider: 2560, SDA Hold Value: 385, SCL Hold Start Value: 1278, SDA Hold Stop Value: 1281 */
  gIicICR_3E_c = 0x3E,/* SCL divider: 3072, SDA Hold Value: 513, SCL Hold Start Value: 1534, SDA Hold Stop Value: 1537 */
  gIicICR_3F_c = 0x3F,/* SCL divider: 3840, SDA Hold Value: 513, SCL Hold Start Value: 1918, SDA Hold Stop Value: 1921 */
  gIicICR_Max_c = 0x40
}iicClockRates_t;

#if (gSystemClock_d == 8000000) 
  #define mIic100KhzBaudInitParameters_c gIicMul_1_c, gIicICR_18_c            
#endif

#if (gSystemClock_d == 16000000) 
  #define mIic100KhzBaudInitParameters_c gIicMul_2_c, gIicICR_18_c              
#endif


/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/
/************************************************************************************
*
* IIC_Init
*
* Initialize the IIC communication module and hardware.
*
* Interface assumptions: 
*   The function configures hard coded the next values
*
*   - Setting	                          - Hard coded Value
*  IIC Interrupt Enable	               Enabled on if gIICMode_d is defined  
*                                      as gIICInterrupt_c
*  IIC System Enable	                 Enabled
*  Master/Slave Mode Select	           Master mode
*  Transmit Acknowledge Enable	       Acknowledge enabled
*  General Call Address Enable	       General call address is disabled
*  Address Extension	                 7-bit address scheme
*
* Return value: iicErr_t
*               gIicErrInvalidParameter_c: One of the passed configuration parameters
*                                          is out of range. 
*               gAdcErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_Init
( 
  iicMultiplierFactors_t IICMultiplier, //IN, Multiplier factor needed to set the 
                                        //    desired baud rate
  iicClockRates_t        IICClockrate  //IN, Clock rates needed to set the baud rate
);

/************************************************************************************
*
* IIC_Write
*
* The function is called to send a string trough the IIC bus; the string can have a 
* maximum length of 256 bytes.
*
* Interface assumptions: 
*    The Callback pointer can not be null puinter.
*
* Return value:   iicErr_t
*                 gIicErrInvalidCondition_c: The IIC module has not been Initialized
*                 gIicErrInvalidParameter_c: One of the parameters is out of range.
*                 gIicErrModuleBusy_c:       The IIC module is not Idle.
*                 gIicErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_Write
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8TxString,     //IN, Pointer to the string to be transmitted.
  uint8_t         u8StringLenght,   //IN, String Lenght 
  iicCallback_t   pfUserCallBack   //IN, to the callback function called at the end 
                                   //    of transmitting the entrie string.
);

/************************************************************************************
*
* IIC_Read
*
* The function is called to receive a string from the IIC bus, the string can have a
* maximum length of 256 bytes.
*
* Interface assumptions: 
*    The Callback pointer can not be null puinter.
*
* Return value:   iicErr_t
*                 gIicErrInvalidCondition_c: The IIC module has not been Initialized
*                 gIicErrInvalidParameter_c: One of the parameters is out of range.
*                 gIicErrModuleBusy_c:       The IIC module is not Idle.
*                 gIicErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_Read
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8RxString,     //OUT, Pointer to the string to be received.
  uint8_t         u8StringLenght,  //IN, String Lenght 
  iicCallback_t   pfUserCallBack   //IN, to the callback function called at the end 
                                   //    of receiving the entrie string.
);

/************************************************************************************
*
* IIC_WriteBlocking
*
* The function is called to send a string trough the IIC bus in a synchronous manner;
* the string can have a maximum length of 256 bytes.
*
* Interface assumptions: 
*
* Return value:   iicErr_t
*                 gIicErrInvalidCondition_c: The IIC module has not been Initialized
*                 gIicErrInvalidParameter_c: One of the parameters is out of range.
*                 gIicErrModuleBusy_c:       The IIC module is not Idle.
*                 gIicErrNoAck_c             No acknowledge received from the slave
*                 gIicErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_WriteBlocking
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8TxString,     //IN, Pointer to the string to be transmitted.
  uint8_t         u8StringLenght   //IN, String Lenght 
);

/************************************************************************************
*
* IIC_ReadBlocking
*
* The function is called to receive a string from the IIC bus in a synchronous manner;
* the string can have a maximum length of 256 bytes.
*
* Interface assumptions: 
*
* Return value:   iicErr_t
*                 gIicErrInvalidCondition_c: The IIC module has not been Initialized
*                 gIicErrInvalidParameter_c: One of the parameters is out of range.
*                 gIicErrModuleBusy_c:       The IIC module is not Idle.
*                 gIicErrNoAck_c             No acknowledge received from the slave
*                 gIicErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_ReadBlocking
(
  uint8_t         u8SlaveAddress,  //IN, Address of one slave on the IIC bus on which 
                                   //    the Master device has a transaction.
  uint8_t*        pu8RxString,     //OUT, Pointer to the string to be received.
  uint8_t         u8StringLenght   //IN, String Lenght 
);

/************************************************************************************
*
* IIC_StopTransaction
*
* The function is called to finish the transaction between the master and one of the
* slaves on the IIC bus  generating a  STOP condition  on the bus.  This function 
* releases the module to START a new transaction with another slave.
*
* Interface assumptions: 
*
* Return value:   iicErr_t
*                 gIicErrInvalidCondition_c: The IIC module has not been Initialized
*                 gIicErrInvalidParameter_c: One of the parameters is out of range.
*                 gIicErrModuleBusy_c:       The IIC module is not Idle.
*                 gIicErrNoError_c:          Everything is OK.
*
************************************************************************************/
extern iicErr_t IIC_StopTransaction
(
  uint8_t         u8SlaveAddress  //IN, Address of one slave on the IIC bus on which 
                                  //    the Master device has a transaction.
);


/************************************************************************************
* IIC Interrupt service function
*
*
*
************************************************************************************/
#if TRUE == gIicSupported_d

#if gIicMode_d == gIicInterrupt_c   

/* Place it in NON_BANKED memory */
 #pragma CODE_SEG ISR_SEGMENT

 extern INTERRUPT_KEYWORD void IIC_Isr(void);

 #pragma CODE_SEG DEFAULT
#else
 extern void IIC_Task(void);
#endif

#endif



#endif //__IIC_INTERFACE_H__

  