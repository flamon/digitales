/************************************************************************************
* Header file of the security module  implemented using ASM module for MC1323x 
* platform or software AES implementation for HCS08GT/GB  and HCS08QE
*
* The file covers the interface to the Advanced Security Module (ASM).
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/

#ifndef _ASM_INTERFACE_H_
#define _ASM_INTERFACE_H_

#include "derivative.h"
#include "EmbeddedTypes.h"
#include "Utilities_Interface.h"
#include "board_config.h"
#include "PLM_config.h"

#ifndef gAsmSupported_d
    #define gAsmSupported_d FALSE
#endif

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/


#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near asmCallbackFunc_t)(void);
#else
  typedef void(* __near asmCallbackFunc_t)(void);
#endif




typedef enum asmErr_tag
{
  gAsmErrNoError_c,
  gAsmErrModuleBusy_c,
  gAsmErrSelfTestFail_c,
  gAsmErrInvalidParameter_c,
  gAsmErrSelfTestNotPerformed_c,
  gAsmErrInternalTimeout_c,
  gAsmErrNullPointer_c,
  gAsmErrErrMax_c
}asmErr_t;

typedef enum asmMode_tag
{
  gAsmModeCtr_c,
  gAsmModeCbcMac_c,
  gAsmModeCcm_c,
  gAsmModeAes_c,
  gAsmModeMax_c
}asmMode_t;
  
typedef struct asmConfig_tag 
{
  uint8_t * pKey; 
  uint8_t * pCtr; 
  uint8_t * pCbcMacVal;
  uint8_t * pResult;
  uint8_t * pMacAccPreload;
  asmMode_t CipherMode; 
  asmCallbackFunc_t pCallback;
  uint8_t u8BufferLength;
} asmConfig_t;

typedef enum asmAction_tag
{
  gAsmCipher_c,
  gAsmDecipher_c,
  gAsmActionMax_c
}asmAction_t;


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* This function is called to initialize the ASM driver, ASM control registers and to 
* perform the AES self-test.
*   
* Interface assumptions:
*   None
*   
* Return value:
*    gAsmErrNoError_c - The initialization was performed successful.
*    gAsmSelfTestFail_c - The AES self test failed. 
*    gAsmInternalTimeout_c - The internal timeout occurs after not have cipher engine
*                            response.
*
************************************************************************************/
extern asmErr_t Asm_Init(void);


/************************************************************************************
* Asm_CipherProcess
*
* This function performs the cipher operation through a state machine, it calls the 
* local function mCipherDecipher function many times as the number of blocks of 128
* bits that are going to be ciphered for MC1323x platforms. For HCS08GT/GB  and 
* HCS08QE the mCipherDecipher function is called 10 times per block of 128 bits. 
*
* Interface assumptions:
*   None
*   
* Return value:
*    gAsmErrNoError_c - The cipher process was successful.
*    gAsmInternalTimeout_c - The internal timeout occurs after not have cipher engine
*                            response.

************************************************************************************/                                                                                  
extern asmErr_t Asm_CipherProcess(void);



/************************************************************************************
* Asm_CipherDecipherRequest
*
* This function configures the cipher operation, set the key, counter, a preload MAC
* (Message Autentication Code) in the respective ASM registers. For platforms
* without a cipher engine it set the respective parameter in the arrays used for
* for the cipher opertation.
*
* Interface assumptions:
*   The input data buffer is overridded with the output data.
*   The buffer length must be multiple of 16 bytes or 128 bits.
* 
* Return value:
*    gAsmErrNoError_c - The cipher request was processed succesfully.
*    gAsmNullPointer_c - A needed pointer was set as NULL;
*    gAsmSelfTestNotPerformed_c - When the self test was not performed previouslly
*    gAsmInternalTimeout_c -  Internal timeout occurs after not have cipher engine
*                            answer.
*    gAsmErrModuleBusy_c - The state machine was not in Idle state.
*    gAsmErrInvalidParameter_c - The buffer length was not multiple of 16 bytes  or
*                                the aes mode is not valid.
*
*
************************************************************************************/
extern asmErr_t Asm_CipherDecipherRequest(
  asmConfig_t   asmConfig,    //IN: Pointer to the configuration structure
  uint8_t     * pData,        //IN_OUT: Pointer to the input and output buffer data
  asmAction_t   asmAction     //IN: Indicates if the process is encryption or decryption.
);             
                            



#endif /* _ASM_INTERFACE_H_ */
