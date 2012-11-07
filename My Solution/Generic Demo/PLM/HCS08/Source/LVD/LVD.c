#include "derivative.h"
#include "EmbeddedTypes.h"
#include "LVD.h"

#if TRUE == gLvdSupported_d  
static lvCallback_t LvwCllbk;
static lvCallback_t LvdCllbk;
#endif //gLvdSupported_d

/************************************************************************************
*  Lvd_Init
*
* The purpose of this function is to configure the posible options of the LVD moduel 
* 
*
************************************************************************************/ 

#if TRUE == gLvdSupported_d  
lvdError_t Lvd_Init
(
lvCallback_t LvwCallback,
lvCallback_t LvdCallback,
LvdControl1_t LvdControlConfig1,
LvdControl2_t LvdControlConfig2
)

{

  if((LvwCallback == NULL) || (LvdCallback == NULL)){
    
    return gLvdErrorInvalidParameter_c;
  
  }

  LvdReg1 =  LvdControlConfig1.Port;
  LvdReg2 =  LvdControlConfig2.Port;

  LvwCllbk = LvwCallback;
  LvdCllbk = LvdCallback;

  return gLvdErrorNoError_c;

}
#else
lvdError_t Lvd_Init (lvCallback_t LvwCallback, lvCallback_t LvdCallback, LvdControl1_t LvdControlConfig1, LvdControl2_t LvdControlConfig2)
{     
  (void)LvwCallback;
  (void)LvdCallback;
  (void)LvdControlConfig1;
  (void)LvdControlConfig2;
  return gLvdErrorNoError_c;
}

#endif //gLvdSupported_d 
 
 
/************************************************************************************
*  Lvd_VoltageStatus
*
* The purpose of this function is to return the current status of the voltage values  
*
*
************************************************************************************/ 
#if TRUE == gLvdSupported_d   
lvdVoltage_t Lvd_VoltageStatus(void){

    
  if (gLowVoltageDetectFlag){
        
      return gLvdVoltageDetetcFlagActive_c;
  
  } 

  if (gLowVoltageWarningFlag){
    
      return gLvdVoltageWarningFlagActive_c;

  }
  
  return gLvdVoltageNoFlagsActive_c;
  
}
#else
lvdVoltage_t Lvd_VoltageStatus(void)
{     
  return gLvdErrorNoError_c;
}

#endif //gLvdSupported_d 
  
  
/************************************************************************************
*  LowVoltageDetect and  LowVoltageWarning Interrupt / Lvd_Task
*
*
************************************************************************************/ 
#if TRUE == gLvdSupported_d
#if gLvdMode_d == gInterruption_c 
 #pragma CODE_SEG ISR_SEGMENT
 INTERRUPT_KEYWORD void LowVoltageDetectIsr(void){
#else
  #pragma CODE_SEG DEFAULT
  void Lvd_Task(void){
#endif  
  
  if(gLowVoltageDetectFlag){
    
    Lvd_Acknowledge();

    LvdCllbk();
  }
  
  
  if(gLowVoltageWarningFlag){
  
    Lvw_Acknowledge();

    LvwCllbk();
  }

}

 #pragma CODE_SEG DEFAULT

#endif //gLvdSupported_d 
