/******************************************************************************
* Source file for GPIO driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* Freescale Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*   
*******************************************************************************/

 
 #include "GPIO.h"

 
/******************************************************************************
*******************************************************************************
* Private type definitions
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private defines and macros
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private constants
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private prototypes
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Public memory declarations
*******************************************************************************
*******************************************************************************/

/******************************************************************************
*******************************************************************************
* Private memory declarations
*******************************************************************************
*******************************************************************************/

// Init Values


#ifdef gMcs08qe128Platform_d
//******************************************************************************

static uint8_t* gpGpioPortData[gNumberOfGpioPorts_c] = {&PTAD,&PTBD,&PTCD,&PTDD,&PTED,&PTFD,&PTGD,&PTHD,&PTJD};
static uint8_t* gpGpioPullDown[gNumberOfGpioPorts_c] = {&PTAPE,&PTBPE,&PTCPE,&PTDPE,&PTEPE,&PTFPE,&PTGPE,&PTHPE,&PTJPE};
static uint8_t  gGpioPortLogic[gNumberOfGpioPorts_c]; 
  
//                                               Data   Direction
 
 gpioDataHandling_t const GpioDataHandling[] = {
                                                {gPTADValue_c,  gPTADDValue_c},
                                                {gPTBDValue_c,  gPTBDDValue_c},
                                                {gPTCDValue_c,  gPTCDDValue_c},
                                                {gPTDDValue_c,  gPTDDDValue_c},
                                                {gPTEDValue_c,  gPTEDDValue_c},
                                                {gPTFDValue_c,  gPTFDDValue_c},
                                                {gPTGDValue_c,  gPTGDDValue_c},
                                                {gPTHDValue_c,  gPTHDDValue_c},
                                                {gPTJDValue_c,  gPTJDDValue_c}
                                               };
 
//                                               PullDown  SlewRate  DriveStrength
 
 gpioConfigPort_t const GpioConfigPort[] =     { 
                                                {gPTAPEValue_c, gPTASEValue_c, gPTADSValue_c},
                                                {gPTBPEValue_c, gPTBSEValue_c, gPTBDSValue_c},
                                                {gPTCPEValue_c, gPTCSEValue_c, gPTCDSValue_c},
                                                {gPTDPEValue_c, gPTDSEValue_c, gPTDDSValue_c},
                                                {gPTEPEValue_c, gPTESEValue_c, gPTEDSValue_c},
                                                {gPTFPEValue_c, gPTFSEValue_c, gPTFDSValue_c},
                                                {gPTGPEValue_c, gPTGSEValue_c, gPTGDSValue_c},
                                                {gPTHPEValue_c, gPTHSEValue_c, gPTHDSValue_c},
                                                {gPTJPEValue_c, gPTJSEValue_c, gPTJDSValue_c}
                                               };
     
 
//                                               Logic    
                                            
uint8_t const GpioLogicConfig[] =              { 
                                                gPTALogicValue_c,
                                                gPTBLogicValue_c,
                                                gPTCLogicValue_c,
                                                gPTDLogicValue_c,
                                                gPTELogicValue_c,
                                                gPTFLogicValue_c,
                                                gPTGLogicValue_c,
                                                gPTHLogicValue_c,
                                                gPTJLogicValue_c
                                               };  
#endif
  
#ifdef gMc1323xPlatform_d  
//******************************************************************************

static  uint8_t* gpGpioPortData[gNumberOfGpioPorts_c] = {&PTAD,&PTBD,&PTCD,&PTDD};
static  uint8_t* gpGpioPullDown[gNumberOfGpioPorts_c] = {&PTAPE,&PTBPE,&PTCPE,&PTDPE};
static  uint8_t  gGpioPortLogic[gNumberOfGpioPorts_c]; 
  
//                                               Data   Direction
 
 gpioDataHandling_t const GpioDataHandling[] = {
                                                {gPTADValue_c,  gPTADDValue_c},
                                                {gPTBDValue_c,  gPTBDDValue_c},
                                                {gPTCDValue_c,  gPTCDDValue_c},
                                                {gPTDDValue_c,  gPTDDDValue_c}
                                               };
 
//                                               PullDown  SlewRate  DriveStrength
 
 gpioConfigPort_t const GpioConfigPort[] =     { 
                                                {gPTAPEValue_c, gPTASEValue_c, gPTADSValue_c},
                                                {gPTBPEValue_c, gPTBSEValue_c, gPTBDSValue_c},
                                                {gPTCPEValue_c, gPTCSEValue_c, gPTCDSValue_c},
                                                {gPTDPEValue_c, gPTDSEValue_c, gPTDDSValue_c}
                                               };
     
 
//                                               Logic    
                                            
uint8_t const GpioLogicConfig[] =              { 
                                                gPTALogicValue_c,
                                                gPTBLogicValue_c,
                                                gPTCLogicValue_c,
                                                gPTDLogicValue_c
                                               };  
#endif




#ifdef gMcs08Gt60Platform_d  
//******************************************************************************

static  uint8_t* gpGpioPortData[gNumberOfGpioPorts_c] = {&PTAD,&PTBD,&PTCD,&PTDD,&PTED,&PTFD,&PTGD};
static  uint8_t* gpGpioPullDown[gNumberOfGpioPorts_c] = {&PTAPE,&PTBPE,&PTCPE,&PTDPE,&PTEPE,&PTFPE,&PTGPE};
static  uint8_t  gGpioPortLogic[gNumberOfGpioPorts_c]; 
  
//                                               Data   Direction
 
 gpioDataHandling_t const GpioDataHandling[] = {
                                                {gPTADValue_c, 0x00, 0x00,  gPTADDValue_c},
                                                {gPTBDValue_c, 0x00, 0x00,  gPTBDDValue_c},
                                                {gPTCDValue_c, 0x00, 0x00,  gPTCDDValue_c},
                                                {gPTDDValue_c, 0x00, 0x00,  gPTDDDValue_c},
                                                {gPTEDValue_c, 0x00, 0x00,  gPTEDDValue_c},
                                                {gPTFDValue_c, 0x00, 0x00,  gPTFDDValue_c},
                                                {gPTGDValue_c, 0x00, 0x00,  gPTGDDValue_c}
                                               };
 
//                                               PullDown  SlewRate  
 
 gpioConfigPort_t const GpioConfigPort[] =     { 
                                                {gPTAPEValue_c, gPTASEValue_c},
                                                {gPTBPEValue_c, gPTBSEValue_c},
                                                {gPTCPEValue_c, gPTCSEValue_c},
                                                {gPTDPEValue_c, gPTDSEValue_c},
                                                {gPTEPEValue_c, gPTESEValue_c},
                                                {gPTFPEValue_c, gPTFSEValue_c},
                                                {gPTGPEValue_c, gPTGSEValue_c},
                                               
                                               };
     
 
//                                               Logic    
                                            
uint8_t const GpioLogicConfig[] =              { 
                                                gPTALogicValue_c,
                                                gPTBLogicValue_c,
                                                gPTCLogicValue_c,
                                                gPTDLogicValue_c,
                                                gPTELogicValue_c,
                                                gPTFLogicValue_c,
                                                gPTGLogicValue_c,
                                               };  
#endif 


/******************************************************************************
*******************************************************************************
* Public functions
*******************************************************************************
*******************************************************************************/


/************************************************************************************
*  Gpio_ConfigPort
*
* The purpose of this function is to configure the posible IO options of a selected  
* GPIO port specified as a parameter
*
************************************************************************************/ 

void Gpio_ConfigPort(gpioPort_t Port, gpioRuntimeConfig_t Config){
 
 gpioConfigUnion_t pTmpPortPtr;
  
 pTmpPortPtr.dataHandling = (gpioDataHandling_t*)gpGpioPortData[Port];
 (pTmpPortPtr.dataHandling)->Direction = Config.Direction.port;

 pTmpPortPtr.configPort = (gpioConfigPort_t*)gpGpioPullDown[Port];
 (pTmpPortPtr.configPort)->PullDown = Config.PullDown.port;
 (pTmpPortPtr.configPort)->SlewRate = Config.SlewRate.port;
 
 #ifndef gMcs08Gt60Platform_d
 
 (pTmpPortPtr.configPort)->DriveStrength = Config.DriveStrength.port;

 #endif
 
 gGpioPortLogic[Port] = Config.Logic.port;

 } 
 
 
 
/************************************************************************************
* Gpio_Init settings
*
* The purpose of this function is to enable or disable GPIO settings such as port direction, 
* pull-ups, slew rate control 
*
************************************************************************************/ 
 
 void Gpio_Init()
 {
  
  uint8_t PortCount;
  gpioConfigUnion_t pTmpPortPtr;
    
  for(PortCount = gGpioPortA_c; PortCount < gGpioPortMax_c; PortCount++){  
  
    pTmpPortPtr.dataHandling = (gpioDataHandling_t*)gpGpioPortData[PortCount];
    (pTmpPortPtr.dataHandling)->Data = GpioDataHandling[PortCount].Data;
    (pTmpPortPtr.dataHandling)->Direction = GpioDataHandling[PortCount].Direction;
    
    pTmpPortPtr.configPort = (gpioConfigPort_t*)gpGpioPullDown[PortCount];
    (pTmpPortPtr.configPort)->PullDown = GpioConfigPort[PortCount].PullDown;
    (pTmpPortPtr.configPort)->SlewRate = GpioConfigPort[PortCount].SlewRate;
    
    #ifndef gMcs08Gt60Platform_d
    
    (pTmpPortPtr.configPort)->DriveStrength = GpioConfigPort[PortCount].DriveStrength;
    
    #endif
    
    gGpioPortLogic[PortCount] = GpioLogicConfig[PortCount];
  
  } 
 
 }
 

/************************************************************************************
*  Gpio_PortValue
*
* The purpose of this function is to read the value of a selected GPIO port 
* specified as a parameter
*
************************************************************************************/  
 
bitByte_t Gpio_PortValue(gpioPort_t Port){
   
   bitByte_t TempValueHandling;
   
   TempValueHandling.port = (*gpGpioPortData[Port]) ^ (gGpioPortLogic[Port]);
   
   return TempValueHandling;

 }
 

 /************************************************************************************
* Gpio_ReadPort
*
* The purpose of this function is to read the value of a selected pin on a selected 
* GPIO port with both values specified as a parameter.
*
************************************************************************************/ 
 
 
 gpioLevel_t Gpio_PinValue(gpioPort_t Port, gpioPin_t Pin){
 
   uint8_t PortValue;
   
   PortValue = (*gpGpioPortData[Port]) ^ (gGpioPortLogic[Port]);
   
   PortValue &= Pin;
   
   if(PortValue){
         
    return gGpioPinStateActive_c;
   
   } 
     
   else{
      
    return gGpioPinStateInactive_c;
     
   }
   
 }
 
 
 /************************************************************************************
* Gpio_WritePort
*
* The purpose of this function is to write a value on a selected GPIO port 
* specified as a parameter
*
************************************************************************************/ 
 
 void Gpio_PortWrite(gpioPort_t Port, uint8_t Value){
    
   *gpGpioPortData[Port] = Value ^ gGpioPortLogic[Port];
   
 }
 
 
 /************************************************************************************
* Gpio_SetPin
*
* The purpose of this function is to set a selected pin on a selected 
* GPIO port with both values specified as a parameter.
*
************************************************************************************/ 
 
 
 void Gpio_PinSet(gpioPort_t Port, gpioPin_t Pin){
  
  if(Pin & gGpioPortLogic[Port]){
    
    (*gpGpioPortData[Port]) &= ~Pin;
   
  } else{
  
    (*gpGpioPortData[Port])|= Pin;
  
  }
   
 }
 
 /************************************************************************************
* Gpio_ClearPin
*
* The purpose of this function is to clear a selected pin on a selected 
* GPIO port with both values specified as a parameter.
*
************************************************************************************/ 
 
 
 void Gpio_PinClear(gpioPort_t Port, gpioPin_t Pin){
   
   if(Pin & gGpioPortLogic[Port]){
    
    *gpGpioPortData[Port] |= Pin;
   
   } else{
  
    *gpGpioPortData[Port] &= ~Pin; 
    
   }
  
 }
 
 /************************************************************************************
* Gpio_TogglePin
*
* The purpose of this function is to toggle a selected pin on a selected 
* GPIO port with both values specified as a parameter.
*
************************************************************************************/ 
 
 
 void Gpio_PinToggle(gpioPort_t Port, gpioPin_t Pin){
  
    *gpGpioPortData[Port] ^= Pin;
 }
  
  

  
  
  
    
    
    
    
    
    
    