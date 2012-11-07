/************************************************************************************
* Source file for MC1323x platform ASM driver, and software security driver
* for HCS08GT/GB  and HCS08QE
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
************************************************************************************/

#include "ASM.h"

#if gAsmSupported_d == TRUE

/************************************************************************************
*************************************************************************************
* Private macros
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Private prototypes
*************************************************************************************
************************************************************************************/

asmErr_t mCipherDecipher(void);

#ifdef gMcs08Gt60Platform_d
  void mAesAddKeyByteSubShiftKeyExpand(void);
  uint8_t mxTime(uint8_t x);
#endif

#ifdef gMcs08qe128Platform_d
  void mAesAddKeyByteSubShiftKeyExpand(void);
  uint8_t mxTime(uint8_t x);
#endif


/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/

        
/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

#ifdef gMcs08Gt60Platform_d

  #define mSBox_c(x) mBox_c[x] 

  const uint8_t mBox_c[256] = 
  {
   99, 124, 119, 123, 242, 107, 111, 197,
   48,   1, 103,  43, 254, 215, 171, 118,
  202, 130, 201, 125, 250,  89,  71, 240,
  173, 212, 162, 175, 156, 164, 114, 192,
  183, 253, 147,  38,  54,  63, 247, 204,
   52, 165, 229, 241, 113, 216,  49,  21,
    4, 199,  35, 195,  24, 150,   5, 154,
    7,  18, 128, 226, 235,  39, 178, 117,
    9, 131,  44,  26,  27, 110,  90, 160,
   82,  59, 214, 179,  41, 227,  47, 132,
   83, 209,   0, 237,  32, 252, 177,  91,
  106, 203, 190,  57,  74,  76,  88, 207,
  208, 239, 170, 251,  67,  77,  51, 133,
   69, 249,   2, 127,  80,  60, 159, 168,
   81, 163,  64, 143, 146, 157,  56, 245,
  188, 182, 218,  33,  16, 255, 243, 210,
  205,  12,  19, 236,  95, 151,  68,  23,
  196, 167, 126,  61, 100,  93,  25, 115,
   96, 129,  79, 220,  34,  42, 144, 136,
   70, 238, 184,  20, 222,  94,  11, 219,
  224,  50,  58,  10,  73,   6,  36,  92,
  194, 211, 172,  98, 145, 149, 228, 121,
  231, 200,  55, 109, 141, 213,  78, 169,
  108,  86, 244, 234, 101, 122, 174,   8,
  186, 120,  37,  46,  28, 166, 180, 198,
  232, 221, 116,  31,  75, 189, 139, 138,
  112,  62, 181, 102,  72,   3, 246,  14,
   97,  53,  87, 185, 134, 193,  29, 158,
  225, 248, 152,  17, 105, 217, 142, 148,
  155,  30, 135, 233, 206,  85,  40, 223,
  140, 161, 137,  13, 191, 230,  66, 104,
   65, 153,  45,  15, 176,  84, 187,  22
  };
 

  uint8_t                * mpMacAccPreload;
  static uint8_t         * mpCtr;
  static uint8_t         * mpKey;
  static uint8_t           mAesTmp;
  static uint8_t           mAesTmp2;
  static uint8_t           mAesRound;
  static uint8_t           mAesState[16];
  static uint8_t           mAesTempKey[16]; 

#endif


#ifdef gMcs08qe128Platform_d

  #define mSBox_c(x) mBox_c[x] 

  const uint8_t mBox_c[256] = 
  {
   99, 124, 119, 123, 242, 107, 111, 197,
   48,   1, 103,  43, 254, 215, 171, 118,
  202, 130, 201, 125, 250,  89,  71, 240,
  173, 212, 162, 175, 156, 164, 114, 192,
  183, 253, 147,  38,  54,  63, 247, 204,
   52, 165, 229, 241, 113, 216,  49,  21,
    4, 199,  35, 195,  24, 150,   5, 154,
    7,  18, 128, 226, 235,  39, 178, 117,
    9, 131,  44,  26,  27, 110,  90, 160,
   82,  59, 214, 179,  41, 227,  47, 132,
   83, 209,   0, 237,  32, 252, 177,  91,
  106, 203, 190,  57,  74,  76,  88, 207,
  208, 239, 170, 251,  67,  77,  51, 133,
   69, 249,   2, 127,  80,  60, 159, 168,
   81, 163,  64, 143, 146, 157,  56, 245,
  188, 182, 218,  33,  16, 255, 243, 210,
  205,  12,  19, 236,  95, 151,  68,  23,
  196, 167, 126,  61, 100,  93,  25, 115,
   96, 129,  79, 220,  34,  42, 144, 136,
   70, 238, 184,  20, 222,  94,  11, 219,
  224,  50,  58,  10,  73,   6,  36,  92,
  194, 211, 172,  98, 145, 149, 228, 121,
  231, 200,  55, 109, 141, 213,  78, 169,
  108,  86, 244, 234, 101, 122, 174,   8,
  186, 120,  37,  46,  28, 166, 180, 198,
  232, 221, 116,  31,  75, 189, 139, 138,
  112,  62, 181, 102,  72,   3, 246,  14,
   97,  53,  87, 185, 134, 193,  29, 158,
  225, 248, 152,  17, 105, 217, 142, 148,
  155,  30, 135, 233, 206,  85,  40, 223,
  140, 161, 137,  13, 191, 230,  66, 104,
   65, 153,  45,  15, 176,  84, 187,  22
  };
 

  uint8_t                * mpMacAccPreload;
  static uint8_t         * mpCtr;
  static uint8_t         * mpKey;
  static uint8_t           mAesTmp;
  static uint8_t           mAesTmp2;
  static uint8_t           mAesRound;
  static uint8_t           mAesState[16];
  static uint8_t           mAesTempKey[16]; 

#endif


static uint8_t         * mpData;
static uint8_t         * mpCbcMacVal;
static uint8_t           mBufferLength;
static asmAesState_t     mu8AsmState;
static asmMode_t         mCipherMode;
static asmCallbackFunc_t mCallbackFunc;
static asmAction_t       mAsmAction;
static bool_t            mCcmSecondRound;
static bool_t            mLoadCbcPrevMac;


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Asm_Init
*
* This function is called to initialize the ASM driver, ASM control registers and
* to perform the AES self-test in the MC1323x platform. For HCS08GT/GB, MC1321x, 
* and HCS08QE platforms the function only initialize the state machine.
*
************************************************************************************/


asmErr_t Asm_Init()
{   

#ifdef gMc1323xPlatform_d
 
  uint16_t timeoutCycles;
 
  mAsmCntl2Reg = (uint8_t)0x00;    
  mAsmCntl1Reg = AES_CTRL1_CLEAR | AES_CTRL1_AESMSK;

 
  /*See if the test was already performed */
  if(mAsmCntl2Reg & AES_CTRL2_TSTPAS)   
  {
    return gAsmErrNoError_c;
  }
 
  /* Initialize the AES self test */
  mAsmCntl1Reg = AES_CTRL1_START | AES_CTRL1_SELFTST | AES_CTRL1_AESMSK;
 
  /* Poll for the DONE Flag. If The DONE flag is not set on the SelfTest 
     the while loop takes 3330 cycles  */
  
  timeoutCycles = gAsmMaxSelfTestCycles; 
 
  while(!(mAsmCntl2Reg & AES_CTRL2_DONE) && (timeoutCycles--)); 
 
  if(mAsmCntl2Reg & AES_CTRL2_DONE)
  {    
    if(!(mAsmCntl2Reg & AES_CTRL2_TSTPAS))
    {
      return gAsmErrSelfTestFail_c;
    }
  }
  
  else
  {
    /* Self test timeout */
    /* Clear the SELFTST bit to get out from this mode */
    mAsmCntl1Reg = AES_CTRL1_AESMSK;
    return gAsmErrInternalTimeout_c;
  }
  
#endif
  
  mu8AsmState = gAsmStateIdle_c;  
  return gAsmErrNoError_c;
 
}

/************************************************************************************
* Asm_CipherDecipherRequest
*
* This function configures the cipher operation, set the key, counter, a preload MAC
* (Message Autentication Code) in the respective ASM registers. For platforms
* without a cipher engine it set the respective parameter in the arrays used for
* for the cipher opertation.
*
* Caution: The input data buffer is overridden with the ciphered or deciphered data.
* The buffer length must be multiple of 16 bytes or 128 bits.
*
************************************************************************************/

asmErr_t Asm_CipherDecipherRequest(
asmConfig_t asmConfig, 
uint8_t * pData, 
asmAction_t asmAction)
{
                                                                       
  if(gAsmStateIdle_c == mu8AsmState) 
  {
    if( (pData == NULL) || (NULL == asmConfig.pCallback) || (NULL == asmConfig.pKey))
    {    
      return gAsmErrNullPointer_c;
    }
     
    if( (NULL == asmConfig.pCtr) && (gAsmModeCtr_c == asmConfig.CipherMode) )
    {
      return gAsmErrNullPointer_c;
    }
  
    if( (NULL == asmConfig.pCtr) && (gAsmModeCcm_c == asmConfig.CipherMode) )
    {
      return gAsmErrNullPointer_c;
    } 
 
    if( (NULL == asmConfig.pCbcMacVal) && ((gAsmModeCbcMac_c == asmConfig.CipherMode) ||\
        (gAsmModeCcm_c == asmConfig.CipherMode)) )
    {
      return gAsmErrNullPointer_c;
    } 
  
    if( (gAsmModeMax_c <= asmConfig.CipherMode) || (0 != (asmConfig.u8BufferLength % mAsmBlockSize_c)) )
    {   
      return gAsmErrInvalidParameter_c;
    }
  
 
    else
    { 
      mpData = pData;
      mCipherMode = asmConfig.CipherMode;
      mBufferLength = asmConfig.u8BufferLength; 
      mAsmAction = asmAction;
      mCallbackFunc = asmConfig.pCallback;
      mpCbcMacVal = asmConfig.pCbcMacVal;
      mCcmSecondRound = FALSE;
      mLoadCbcPrevMac = FALSE;
     
#ifdef gMc1323xPlatform_d
   
      if(!(mAsmCntl2Reg & AES_CTRL2_TSTPAS))
      {   
        return gAsmErrSelfTestNotPerformed_c;   
      } 

      /* Clear previous AES encryption/decryption modes.  Clear the ASM memory*/
      mAsmCntl1Reg = (uint8_t)AES_CTRL1_CLEAR | AES_CTRL1_AESMSK; 
  
      /*Load the MAC preload if necessary. The ASM memory is already
        cleared if a MAC preload was not provided*/ 
      if(((gAsmModeCbcMac_c == mCipherMode) || (gAsmModeCcm_c == mCipherMode)) && \
          (NULL != asmConfig.pMacAccPreload))
      {
        mAsmCntl2Reg = (uint8_t)((mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | \
                       (uint8_t)AES_CTRL2_REGTYPE_MAC);
        MemoryCpy((void*)&mAsmD0Reg, asmConfig.pMacAccPreload, mAsmBlockSize_c);
        mAsmCntl1Reg |= AES_CTRL1_LOADMAC;  
      }
    
      /* Set the AES engine encryption/decryption key */
      mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | (uint8_t)AES_CTRL2_REGTYPE_KEY;
      MemoryCpy((void*)&mAsmD0Reg, asmConfig.pKey, mAsmBlockSize_c);
    
    
      /* Set the AES engine encryption/decryption mode */
      if((gAsmModeCbcMac_c == mCipherMode) || ((gAsmModeCcm_c == mCipherMode) && \
         (gAsmCipher_c == mAsmAction)))
      {
        mAsmCntl1Reg |= AES_CTRL1_CBC; 
      }
  
      if((gAsmModeCtr_c == mCipherMode) || (gAsmModeCcm_c == mCipherMode))
      {
        mAsmCntl1Reg |= AES_CTRL1_CTR;
      }
  
      if(gAsmModeAes_c == mCipherMode)
      {
        mAsmCntl1Reg |= AES_CTRL1_AES;
      } 
    
      /* Load the counter if necessary */
      if((gAsmModeCtr_c == mCipherMode )|| (gAsmModeCcm_c == mCipherMode)) 
      {
        mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | AES_CTRL2_REGTYPE_CTR;
        if(asmConfig.pCtr != NULL)
        {        
          MemoryCpy((void*)&mAsmD0Reg, asmConfig.pCtr, mAsmBlockSize_c);
        }  
        else
        {
          MemorySet((void*)&mAsmD0Reg, 0x00, mAsmBlockSize_c);
        }
      }   
  
#else

    mpKey = asmConfig.pKey;
    mpCtr = asmConfig.pCtr;
    mpMacAccPreload = asmConfig.pMacAccPreload;
    mAesRound = mFirstRound_c;
 
#endif

      mu8AsmState = gAsmStateCiphering_c;
      
      return gAsmErrNoError_c;   
    }
  }
    
  else
  {
    return gAsmErrModuleBusy_c;
  }
    
}


/************************************************************************************
* Asm_CipherProcess
*
* This function performs the cipher operation through a state machine, it calls the 
* local function mCipherDecipher function many times as the number of blocks of 128
* bits that are going to be ciphered for MC1323x platforms. For HCS08GT/GB  and 
* HCS08QE the mCipherDecipher function is called 8 times per block of 128 bits. 
*
************************************************************************************/


asmErr_t Asm_CipherProcess(void)
{ 

  asmErr_t asmError; 
  
  if(gAsmStateCiphering_c == mu8AsmState)
  {
    asmError = mCipherDecipher();
    if(gAsmErrNoError_c != asmError)
    {
      mu8AsmState = gAsmStateFinal_c; 
    }
  } 
  
  else if(gAsmStateFinishBlockCipher_c == mu8AsmState)
  {
    mpData = mpData + mAsmBlockSize_c;
    mBufferLength = mBufferLength - mAsmBlockSize_c;

    if(mBufferLength > 0)
    {
      mu8AsmState = gAsmStateCiphering_c;
      if((gAsmModeCbcMac_c == mCipherMode) || (gAsmModeCcm_c == mCipherMode))
      {
        mLoadCbcPrevMac = TRUE;
      } 
      
    } 
    else
    {
      mu8AsmState = gAsmStateFinal_c;
    }
    asmError = gAsmErrNoError_c;
  } 
  
  else if (gAsmStateFinal_c == mu8AsmState) 
  {
    mu8AsmState = gAsmStateIdle_c;
    mCallbackFunc();
  }
  
  
  return asmError;

}


/************************************************************************************
* mCipherDecipher
*
* The local mCipherDecipher function that cipher or decipher a block of 128 bits 
* using the ASM encryption engine.
*
************************************************************************************/

#ifdef gMc1323xPlatform_d

asmErr_t mCipherDecipher(void)
{
  
  uint8_t timeoutCycles;
  
  if(mCcmSecondRound)
  {
    mAsmCntl1Reg &= ~AES_CTRL1_CTR; //Clear CTR mode    
    mAsmCntl1Reg |= AES_CTRL1_CBC;  //Set CBC mode   
  }
  
  /*Load MAC from previous block encryption*/
  if(mLoadCbcPrevMac) 
  {
    mAsmCntl2Reg = (uint8_t)((mAsmCntl2Reg & mAsmCntl1RegModesMask_c) |\
                   (uint8_t)AES_CTRL2_REGTYPE_MAC);
    MemoryCpy((void*)&mAsmD0Reg, mpCbcMacVal, mAsmBlockSize_c);
    mAsmCntl1Reg |= AES_CTRL1_LOADMAC;
    mLoadCbcPrevMac = FALSE;  
  }

  /* Load data to cipher in Data Register*/
  mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | (uint8_t)AES_CTRL2_REGTYPE_DATA; 
  MemoryCpy((void*)&mAsmD0Reg, mpData, mAsmBlockSize_c);
   
  mAsmCntl1Reg |= AES_CTRL1_START; 
  
  timeoutCycles = gAsmMaxAesEngineCycles;
  while(!(mAsmCntl2Reg & AES_CTRL2_IRQFLAG) && (timeoutCycles--));
  
  if(!(mAsmCntl2Reg & AES_CTRL2_IRQFLAG))
  {    
    /* AES Operation timeout. Clear the AES mode bits */
    mAsmCntl1Reg = AES_CTRL1_AESMSK;
    return gAsmErrInternalTimeout_c; 
  }
  
  /* clear the interrupt flag */
  mAsmCntl2Reg |= AES_CTRL2_IRQFLAG;
  
  /* Cipher Operation completed succesfully. Copy the result in the provided location */
  if(is_cbc_mode_selected())
  {
    mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | (uint8_t)AES_CTRL2_REGTYPE_CBCRES;
    MemoryCpy(mpCbcMacVal, (void*)&mAsmD0Reg, mAsmBlockSize_c);
  }
  
  if(is_ctr_mode_selected())
  {      
      mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | (uint8_t)AES_CTRL2_REGTYPE_CTRRES;
      MemoryCpy(mpData, (void*)&mAsmD0Reg, mAsmBlockSize_c);
  } 
  
  else if(gAsmModeAes_c == mCipherMode)  
  {
    mAsmCntl2Reg = (mAsmCntl2Reg & mAsmCntl1RegModesMask_c) | (uint8_t)AES_CTRL2_REGTYPE_AESRES;
    MemoryCpy(mpData, (void*)&mAsmD0Reg, mAsmBlockSize_c);
  }
  
  
  if((gAsmModeCcm_c == mCipherMode) && (gAsmDecipher_c == mAsmAction))
  {
     if(mCcmSecondRound){
       mCcmSecondRound = FALSE;
       mu8AsmState = gAsmStateFinishBlockCipher_c;
     } 
     else
     {
       mCcmSecondRound = TRUE;
       mu8AsmState = gAsmStateCiphering_c;
     }
  } 
  else 
  {    
    mu8AsmState = gAsmStateFinishBlockCipher_c;
  }
  
  return gAsmErrNoError_c;
}

#else  //Other hcs08 platforms without hardware cipher engine

/************************************************************************************
* mxTime
*
* This function is part of the mix column process.
*
************************************************************************************/

uint8_t mxTime(uint8_t x)
{
  if(x & 0x80){
    return ((x << 1) ^ 0x1B); 
  } 
  else
  {
    return (x << 1);
  }
}


/************************************************************************************
* mAesAddKeyByteSubShiftKeyExpand
*
* This function executes the subBytes, ShiftRows and expand key steps in the AES cipher
* process.
*
************************************************************************************/

void mAesAddKeyByteSubShiftKeyExpand(void)
{
	/* Add round key */
  /* S-Box lookup */
  /* Transformation */
  /* Expand key */

  mAesState[0] = mSBox_c(mAesState[0]^mAesTempKey[0]);
  mAesState[4] = mSBox_c(mAesState[4]^mAesTempKey[4]);
  mAesState[8] = mSBox_c(mAesState[8]^mAesTempKey[8]);
  mAesState[12] = mSBox_c(mAesState[12]^mAesTempKey[12]);

  mAesTmp = mAesState[1]^mAesTempKey[1];
  mAesState[1] = mSBox_c(mAesState[5]^mAesTempKey[5]);
  mAesState[5] = mSBox_c(mAesState[9]^mAesTempKey[9]);
  mAesState[9] = mSBox_c(mAesState[13]^mAesTempKey[13]);
  mAesState[13] = mSBox_c(mAesTmp);

  mAesTmp = mAesState[2]^mAesTempKey[2];
  mAesState[2] = mSBox_c(mAesState[10]^mAesTempKey[10]);
  mAesState[10] = mSBox_c(mAesTmp);
  mAesTmp = mAesState[6]^mAesTempKey[6];
  mAesState[6] = mSBox_c(mAesState[14]^mAesTempKey[14]);
  mAesState[14] = mSBox_c(mAesTmp);

  mAesTmp = mAesState[15]^mAesTempKey[15];
  mAesState[15] = mSBox_c(mAesState[11]^mAesTempKey[11]);
  mAesState[11] = mSBox_c(mAesState[7]^mAesTempKey[7]);
  mAesState[7] = mSBox_c(mAesState[3]^mAesTempKey[3]);
  mAesState[3] = mSBox_c(mAesTmp);

  /* Expand key */
  
  mAesTempKey[0] ^= mSBox_c(mAesTempKey[13]) ^ mAesRound;
  mAesTempKey[4] ^= mAesTempKey[0];
  mAesTempKey[8] ^= mAesTempKey[4];
  mAesTempKey[3] ^=  mSBox_c(mAesTempKey[12]);
  mAesTempKey[12] ^= mAesTempKey[8];
  mAesTempKey[1] ^= mSBox_c(mAesTempKey[14]);
  mAesTempKey[5] ^= mAesTempKey[1];
  mAesTempKey[9] ^= mAesTempKey[5];
  mAesTempKey[13] ^= mAesTempKey[9];
  mAesTempKey[2] ^= mSBox_c(mAesTempKey[15]);
  mAesTempKey[6] ^= mAesTempKey[2];
  mAesTempKey[10] ^= mAesTempKey[6];
  mAesTempKey[14] ^= mAesTempKey[10];
  mAesTempKey[7] ^= mAesTempKey[3];
  mAesTempKey[11] ^= mAesTempKey[7];
  mAesTempKey[15] ^= mAesTempKey[11]; 
 
}

/************************************************************************************
* mAesMixColumn
*
* This function executes the mix columns step in the AES cipher process.
*
************************************************************************************/
void mAesMixColumn(void)
{
  uint8_t u8Count;
  
  for(u8Count=0;u8Count<mAsmBlockSize_c;u8Count = u8Count + 4){
	  mAesTmp = mAesState[0+u8Count];
	  mAesTmp2 = mAesState[0+u8Count] ^ mAesState[1+u8Count] ^ mAesState[2+u8Count] ^ mAesState[3+u8Count];
	  mAesState[0+u8Count] ^= mxTime(mAesState[0+u8Count] ^ mAesState[1+u8Count]) ^ mAesTmp2;
	  mAesState[1+u8Count] ^= mxTime(mAesState[1+u8Count] ^ mAesState[2+u8Count]) ^ mAesTmp2;
	  mAesState[2+u8Count] ^= mxTime(mAesState[2+u8Count] ^ mAesState[3+u8Count]) ^ mAesTmp2;
	  mAesState[3+u8Count] ^= mxTime(mAesState[3+u8Count] ^ mAesTmp) ^ mAesTmp2;
  } 

}

/************************************************************************************
* mCipherDecipher
*
* Software implementation of the AES (Advanced Encryption Standard) cipher operation
* and cipher and decipher CTR, CBC and CCM  modes. Only the cipher AES process is 
* implemented. AES decipher function is not implemented.
*
************************************************************************************/

asmErr_t mCipherDecipher(void)
{
  uint8_t  u8Count;
  uint8_t  u8TempResult;
  
  if( mFirstRound_c == mAesRound ){
    
    for(u8Count=0; u8Count < mAsmBlockSize_c; u8Count++)
    {     
      
      mAesTempKey[u8Count] = mpKey[u8Count];
      
      if(gAsmModeAes_c == mCipherMode)
      {
        mAesState[u8Count] = mpData[u8Count];
      } 
      
      else if(((gAsmModeCbcMac_c == mCipherMode) || (gAsmModeCcm_c == mCipherMode)) && \
               (mLoadCbcPrevMac)) 
      {
        mAesState[u8Count] = mpData[u8Count] ^ mpCbcMacVal[u8Count]; 
      } 
      
      else if(is_ctr_mode_selected())
      {
         mAesState[u8Count] = mpCtr[u8Count];
      } 
     
      else if(is_cbc_mode_selected())
      {
        if(NULL != mpMacAccPreload) 
        {
          mAesState[u8Count] = mpData[u8Count] ^ mpMacAccPreload[u8Count];
        } 
        else
        {
          mAesState[u8Count] = mpData[u8Count];
        }
        
      }
      
    }
    
    mLoadCbcPrevMac = FALSE; 
  }
  
  if(mAesLastRound_c != mAesRound)
  {
    mAesAddKeyByteSubShiftKeyExpand();
    mAesMixColumn();
    
    if( mEighthRound_c == mAesRound )
    {
      mAesRound = mNinthRound_c;       
    } 
     
    else if( mNinthRound_c == mAesRound)
    {
      mAesRound = mAesLastRound_c;
    } 
    else{
      mAesRound = mAesRound << 1;
    }
  } 
   
  else   // Last round
  { 
    mAesAddKeyByteSubShiftKeyExpand();
    for(u8Count=0; u8Count < mAsmBlockSize_c; u8Count++)
    {
      
      u8TempResult = mAesState[u8Count] ^ mAesTempKey[u8Count];
      
      if(is_ctr_mode_selected())
      {
        mpData[u8Count] = mpData[u8Count] ^ u8TempResult;
      } 
      
      else if( is_cbc_mode_selected() )
      {
        mpCbcMacVal[u8Count] = u8TempResult;
      } 
      
      else if (gAsmModeAes_c == mCipherMode){
        mpData[u8Count] = u8TempResult;
      }
    }
    
    if((gAsmModeCcm_c == mCipherMode))
    {
       if(mCcmSecondRound){
         mCcmSecondRound = FALSE;
         mu8AsmState = gAsmStateFinishBlockCipher_c;
       } 
       else
       {
         mCcmSecondRound = TRUE;
         mu8AsmState = gAsmStateCiphering_c;
       }
    } 
    else 
    {    
      mu8AsmState = gAsmStateFinishBlockCipher_c;
    }
    
    mAesRound = mFirstRound_c;
  }
  return gAsmErrNoError_c;    
}

#endif

#else                            /* gAsmSupported_d == FALSE */

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Asm_Init
*
* This function is called to initialize the ASM driver, ASM control registers and
* to perform the AES self-test in the MC1323x platform. For HCS08GT/GB, MC1321x, 
* and HCS08QE platforms the function only initialize the state machine.
*
************************************************************************************/


asmErr_t Asm_Init()
{   

    return gAsmErrNoError_c; 
}

/************************************************************************************
* Asm_CipherDecipherRequest
*
* This function configures the cipher operation, set the key, counter, a preload MAC
* (Message Autentication Code) in the respective ASM registers. For platforms
* without a cipher engine it set the respective parameter in the arrays used for
* for the cipher opertation.
*
* Caution: The input data buffer is overridden with the ciphered or deciphered data.
* The buffer length must be multiple of 16 bytes or 128 bits.
*
************************************************************************************/

asmErr_t Asm_CipherDecipherRequest(
asmConfig_t asmConfig, 
uint8_t * pData, 
asmAction_t asmAction)
{
    (void)asmConfig;
    (void)pData;
    (void)asmAction;                                                                       
    return gAsmErrNoError_c;   
    
}


/************************************************************************************
* Asm_CipherProcess
*
* This function performs the cipher operation through a state machine, it calls the 
* local function mCipherDecipher function many times as the number of blocks of 128
* bits that are going to be ciphered for MC1323x platforms. For HCS08GT/GB  and 
* HCS08QE the mCipherDecipher function is called 8 times per block of 128 bits. 
*
************************************************************************************/


asmErr_t Asm_CipherProcess(void)
{ 
  return gAsmErrNoError_c;

}


/************************************************************************************
* mCipherDecipher
*
* The local mCipherDecipher function that cipher or decipher a block of 128 bits 
* using the ASM encryption engine.
*
************************************************************************************/

#ifdef gMc1323xPlatform_d

asmErr_t mCipherDecipher(void)
{
  return gAsmErrNoError_c;
}

#else  //Other hcs08 platforms without hardware cipher engine

/************************************************************************************
* mxTime
*
* This function is part of the mix column process.
*
************************************************************************************/

uint8_t mxTime(uint8_t x)
{
  (void)x;
  return 0;
}


/************************************************************************************
* mAesAddKeyByteSubShiftKeyExpand
*
* This function executes the subBytes, ShiftRows and expand key steps in the AES cipher
* process.
*
************************************************************************************/

void mAesAddKeyByteSubShiftKeyExpand(void)
{

 
}

/************************************************************************************
* mAesMixColumn
*
* This function executes the mix columns step in the AES cipher process.
*
************************************************************************************/
void mAesMixColumn(void)
{
  
}

/************************************************************************************
* mCipherDecipher
*
* Software implementation of the AES (Advanced Encryption Standard) cipher operation
* and cipher and decipher CTR, CBC and CCM  modes. Only the cipher AES process is 
* implemented. AES decipher function is not implemented.
*
************************************************************************************/

asmErr_t mCipherDecipher(void)
{
  return gAsmErrNoError_c;    
}

#endif    



#endif



