/************************************************************************************
* Public header file for the KBI driver.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/
 
 /*Include files section*/
 
 #include "KBI.h"
 
#if TRUE == gKbiSupported_d 

 /************************************************************************************
*************************************************************************************
* Public type definitions
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public macros
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public prototypes
*************************************************************************************
************************************************************************************/

/************************************************************************************
*************************************************************************************
* Public memory declarations
*************************************************************************************
************************************************************************************/


static kbiCallback_t pKbiCallback[gNumberOfKbiPorts_c]; 

#if defined(gMcs08qe128Platform_d) || defined(gMc1323xPlatform_d)
  static uint8_t* gpKbiControl[] = {&KBI1SC,&KBI2SC};
#elif defined(gMcs08Gt60Platform_d)
  static uint8_t* gpKbiControl[] = {&KBI1SC};
#endif


/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*  Kbi_Config
*
* The purpose of this function is to configure the posible KBI options of a selected  
* KBI port specified as a parameter
*
************************************************************************************/ 


 kbiErr_t	Kbi_Config(kbiPort_t mKbiPort, kbiConfig_t* mKbiConfigOption){
	  	
  	kbiConfig_t* pmTempKbiConfigPtr;
  	
  	if(mKbiPort>= gKbiMax_c){
  	
  	  return gKbiErrInvalidParamater_c;
  	
  	}
  	
  	pmTempKbiConfigPtr =(kbiConfig_t*)gpKbiControl[mKbiPort];
    pmTempKbiConfigPtr->InterruptPin.Port = mKbiConfigOption->InterruptPin.Port;
#if defined(gMcs08qe128Platform_d) || defined(gMc1323xPlatform_d)
   	pmTempKbiConfigPtr->EdgeSelect.Port = mKbiConfigOption->EdgeSelect.Port;
#elif defined(gMcs08Gt60Platform_d)
    mKbiConfigOption->Control.Port &= 0x0F;
    mKbiConfigOption->EdgeSelect.Port &= 0xF0;
    mKbiConfigOption->Control.Port |= mKbiConfigOption->EdgeSelect.Port;
#endif
	  pmTempKbiConfigPtr->Control.Port = mKbiConfigOption->Control.Port;
	  	  
	  return gKbiErrorNoError_c;
	  
	}
	
/************************************************************************************
*  	Kbi_Init
*
* The purpose of this function is to set the initial status of the KBI module  
* GPIO port specified as a parameter
*
************************************************************************************/ 


  kbiErr_t	Kbi_Init(kbiCallback_t pKbiCallbackinit, kbiConfig_t* pKbiConfig, kbiPort_t kbiPort){
      	
  	if(gKbiMax_c <= kbiPort){
  	  return gKbiErrInvalidParamater_c;
  	}
  	
  	if(NULL == pKbiCallbackinit){
  	  return gKbiErrorNullPointer_c;
  	}
  	
  	pKbiCallback[kbiPort] = pKbiCallbackinit;
    (void)Kbi_Config(kbiPort,pKbiConfig);	
    return gKbiErrorNoError_c;
  
  }

/************************************************************************************
*  Kbi_PortValue
*
* The purpose of this function is to obtain the KBI port value in order to know 
* which keys are pressed. The return value is the value of the KBI port where each 
* key is mapped to a bit therefore KBI key 7 is mapped to bit possition 7 and so on
*
************************************************************************************/  
  uint8_t	Kbi_PortValue(kbiPort_t kbiPort){

  volatile uint8_t KbiActivePort;	 	
	 	 
	 	if(kbiPort == gKbi1_c){  	
  	  KbiActivePort = ((~KbiEdgePort1) ^ Kbi1Port) & (KbiEnabledPinsPort1); 
 	  	#if defined(gMcs08Gt60Platform_d)
    	 KbiActivePort>>=2;
    	#endif 

   	  return KbiActivePort;  	
  	}
	    	
  	#if defined(gMcs08qe128Platform_d) || defined(gMc1323xPlatform_d)
  	
  	else{   	  
  	  KbiActivePort = ((~KbiEdgePort2) ^ Kbi2Port) & (KbiEnabledPinsPort2);  	
  	  return KbiActivePort;  	
  	}    
    #endif
	}
	

/************************************************************************************
* Kbi_task/Kbi interrupt
*
* The purpose of this function is, if used as Kbi_Task(), to poll
* every time it is called, the status of the Kbi flag, assert it and call the callback function. 
* When used as interrupt it asserts the Kbi flag and calls the callback. 
* 
************************************************************************************/ 

#ifdef gMcs08qe128Platform_d
  
  #if gKbiMode_d == gKbiInterruption_c 
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD void KBI1_ISR (void){
  #else 
    void Kbi_KbiTask(void){
  #endif
    
    if(Kbi1Flag){
        
      Kbi1Acknowledge();
      pKbiCallback[gKbi1_c](Kbi_PortValue(gKbi1_c));
    } 
    
    else if (Kbi2Flag){
      
      Kbi2Acknowledge();
      pKbiCallback[gKbi2_c](Kbi_PortValue(gKbi2_c));
    }
  }
    #pragma CODE_SEG DEFAULT
  
#endif



#ifdef gMc1323xPlatform_d

  
  #if gKbiMode_d == gKbiInterruption_c 
      #pragma CODE_SEG ISR_SEGMENT
      INTERRUPT_KEYWORD void KBI1_ISR (void){
    
      Kbi1Acknowledge();
      pKbiCallback[gKbi1_c](Kbi_PortValue(gKbi1_c));

  }
      #pragma CODE_SEG DEFAULT
  
     #pragma CODE_SEG ISR_SEGMENT
     INTERRUPT_KEYWORD void KBI2_ISR (void){
    
    
     Kbi2Acknowledge();
     pKbiCallback[gKbi2_c](Kbi_PortValue(gKbi2_c));
   
  }
     #pragma CODE_SEG DEFAULT
   
  #else 
    
    void Kbi_KbiTask(void){
  
    Kbi1Acknowledge();
    pKbiCallback[gKbi1_c](Kbi_PortValue(gKbi1_c));
      
    Kbi2Acknowledge();
    pKbiCallback[gKbi2_c](Kbi_PortValue(gKbi2_c));  
    }
  
  #endif



#endif




#ifdef gMcs08Gt60Platform_d

//interrupt VectorNumber_Vkeyboard void KBI1_ISR (void){
  #if gKbiMode_d == gKbiInterruption_c 
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void KBI1_ISR (void){
  #else 
    void Kbi1_KbiTask(void){
  #endif
  
    Kbi1Acknowledge();
    pKbiCallback[gKbi1_c](Kbi_PortValue(gKbi1_c));
   
}
    #pragma CODE_SEG DEFAULT
  
#endif


#else                            /* FALSE == gKbiSupported_d */



/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*  Kbi_Config
*
* The purpose of this function is to configure the posible KBI options of a selected  
* KBI port specified as a parameter
*
************************************************************************************/ 


 kbiErr_t	Kbi_Config(kbiPort_t mKbiPort, kbiConfig_t* mKbiConfigOption){
	  	
      (void)mKbiPort;
      (void)mKbiConfigOption;
	  return gKbiErrorNoError_c;
	  
	}
	
/************************************************************************************
*  	Kbi_Init
*
* The purpose of this function is to set the initial status of the KBI module  
* GPIO port specified as a parameter
*
************************************************************************************/ 


  kbiErr_t	Kbi_Init(kbiCallback_t pKbiCallbackinit, kbiConfig_t* pKbiConfig, kbiPort_t kbiPort){
     
     (void)pKbiCallbackinit;
     (void)pKbiConfig;
     (void)kbiPort; 	
     return gKbiErrorNoError_c;
  
  }

/************************************************************************************
*  Kbi_PortValue
*
* The purpose of this function is to obtain the KBI port value in order to know 
* which keys are pressed. The return value is the value of the KBI port where each 
* key is mapped to a bit therefore KBI key 7 is mapped to bit possition 7 and so on
*
************************************************************************************/  
	
	
  uint8_t	Kbi_PortValue(kbiPort_t kbiPort){

    (void)kbiPort;
    return 0;
    
	}
	

/************************************************************************************
* Kbi_task/Kbi interrupt
*
* The purpose of this function is, if used as Kbi_Task(), to poll
* every time it is called, the status of the Kbi flag, assert it and call the callback function. 
* When used as interrupt it asserts the Kbi flag and calls the callback. 
* 
************************************************************************************/ 

#ifdef gMcs08qe128Platform_d
  
  #if gKbiMode_d == gKbiInterruption_c 
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD void KBI1_ISR (void){
  #else 
    void Kbi_KbiTask(void){
  #endif
    
    }
    #pragma CODE_SEG DEFAULT
  
#endif



#ifdef gMc1323xPlatform_d

  
  #if gKbiMode_d == gKbiInterruption_c 
      #pragma CODE_SEG ISR_SEGMENT
      INTERRUPT_KEYWORD void KBI1_ISR (void){

  }
      #pragma CODE_SEG DEFAULT
  
     #pragma CODE_SEG ISR_SEGMENT
     INTERRUPT_KEYWORD void KBI2_ISR (void){
   
  }
     #pragma CODE_SEG DEFAULT
   
  #else 
    
    void Kbi_KbiTask(void){
    }
  
  #endif



#endif




#ifdef gMcs08Gt60Platform_d

//interrupt VectorNumber_Vkeyboard void KBI1_ISR (void){
  #if gKbiMode_d == gKbiInterruption_c 
    #pragma CODE_SEG ISR_SEGMENT
    INTERRUPT_KEYWORD  void KBI1_ISR (void){
  #else 
    void Kbi1_KbiTask(void){
  #endif
}
    #pragma CODE_SEG DEFAULT
  
#endif




#endif


