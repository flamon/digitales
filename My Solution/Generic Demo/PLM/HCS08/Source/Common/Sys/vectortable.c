/************************************************************************************
* This module contains the Interrupt vector table
*
*
* (c) Copyright 2009, Freescale, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/

#include "PLM_Config.h"
#include "IRQ_Interface.h"

#if (TRUE == gSpiSupported_d)
  #include "SPI_Interface.h"
#endif
#if (TRUE == gUartSupported_d)
  #include "UART_Interface.h"
#endif 
#if (TRUE == gAdcSupported_d)  //defined(gMcs08Gt60Platform_d) || defined(gMcs08qe128Platform_d)
  #include "ADC_Interface.h"
#endif
#if (TRUE == gIicSupported_d)
  #include "IIC_Interface.h"
#endif
#if (TRUE == gRtiSupported_d)
  #include "RTI_Interface.h"
#endif
#if (TRUE == gTimerSupported_d)
  #include "Timer_Interface.h"
#endif
#if (TRUE == gKbiSupported_d)
  #include "KBI_Interface.h"
#endif
#if (TRUE == gLvdSupported_d)
  #include "LVD_Interface.h"
#endif
#if (TRUE == gCmtSupported_d)
  #include "CMT_Interface.h"
#endif
#include "OTAP_Interface.h"
#include "SMAC_config.h"

/***********************************************************************************/
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near ISR_func_t)(void);
#else
  typedef void(* __near ISR_func_t)(void);
#endif

/***********************************************************************************/

#pragma CODE_SEG ISR_SEGMENT
  INTERRUPT_KEYWORD void UnimplementedISR(void)                                             
  {                                                                              
    for(;;);
  }

#ifdef gMc1323xPlatform_d
  extern INTERRUPT_KEYWORD void PhyHandlerRx_ISR(void);
  extern INTERRUPT_KEYWORD void PhyHandlerTx_ISR(void);
  extern INTERRUPT_KEYWORD void PhyHandlerTmr_ISR(void);
  extern INTERRUPT_KEYWORD void PhyHandlerRxWtrmrk_ISR(void);
#endif

extern void _Startup (void);
#pragma CODE_SEG DEFAULT  


/***********************************************************************************/

#ifdef gMcs08Gt60Platform_d 
  const ISR_func_t ISR_vectors[] @0xFFC0 =
  {
    UnimplementedISR,       // vector 31   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 30   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 29   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 28   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 27   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 26   - NOT A REAL VECTOR
/*------------------RTI interrupt vector------------------------*/   
#if (TRUE == gRtiSupported_d)    
  #if (gRtiMode_d == gInterruption_c)  
      Rti_Isr,              // vector 25   - RTI
  #else    
      UnimplementedISR,     // vector 25   - RTI
  #endif
#else
  UnimplementedISR,         // vector 25   - RTI
#endif 
/*------------------IIC interrupt vector------------------------*/ 
#if (TRUE == gIicSupported_d)   
  #if (gIicMode_d == gInterruption_c) 
      IIC_Isr,              // vector 24   - IIC 
  #else    
      UnimplementedISR,     // vector 24   - IIC 
  #endif
#else    
      UnimplementedISR,     // vector 24   - IIC 
#endif
/*------------------ADC interrupt vector------------------------*/  
#if (TRUE == gAdcSupported_d)  
  #if (gAdcMode_d == gAdcInterrupt_c) 
      ADC_Isr,              // vector 23   - ADC
  #else
      UnimplementedISR,     // vector 23   - ADC
  #endif  
#else
    UnimplementedISR,       // vector 23   - ADC
#endif 
/*------------------KBI interrupt vector------------------------*/  
#if (TRUE == gKbiSupported_d) 
  #if (gKbiMode_d == gInterruption_c)  
      KBI1_ISR,             // vector 22   - KBI
  #else    
      UnimplementedISR,     // vector 22   - KBI
  #endif
#else    
    UnimplementedISR,       // vector 22   - KBI
#endif
/*------------------UART2 interrupt vectors----------------------*/ 
#if (TRUE == gUartSupported_d)
  #if (gUart2_Enabled_d) 
    #if (gInterruption_c == gUartTxMode_d)
      Uart2_TxIsr,          // vector 21   - SCI2 TX
    #else                     
      UnimplementedISR,     // vector 21   - SCI2 TX
    #endif  
      Uart2_RxIsr,          // vector 20   - SCI2 RX
      Uart2_ErrorIsr,       // vector 19   - SCI2 Error
  #else
      UnimplementedISR,     // vector 21   - SCI2 TX
      UnimplementedISR,     // vector 20   - SCI2 RX
      UnimplementedISR,     // vector 19   - SCI2 Error
  #endif
#else    
    UnimplementedISR,       // vector 21   - SCI2 TX
    UnimplementedISR,       // vector 20   - SCI2 RX
    UnimplementedISR,       // vector 19   - SCI2 Error
#endif  
/*------------------UART1 interrupt vectors----------------------*/  
#if (TRUE == gUartSupported_d) 
  #if (gUart1_Enabled_d) 
    #if (gInterruption_c == gUartTxMode_d)   
      Uart1_TxIsr,          // vector 18   - SCI1 TX
    #else
      UnimplementedISR,     // vector 18   - SCI1 TX
    #endif
      Uart1_RxIsr,          // vector 17   - SCI1 RX
      Uart1_ErrorIsr,       // vector 16   - SCI1 Error
  #else
    UnimplementedISR,       // vector 18   - SCI1 TX
    UnimplementedISR,       // vector 17   - SCI1 RX
    UnimplementedISR,       // vector 16   - SCI1 Error  
  #endif
#else    
    UnimplementedISR,       // vector 18   - SCI1 TX
    UnimplementedISR,       // vector 17   - SCI1 RX
    UnimplementedISR,       // vector 16   - SCI1 Error 
#endif 
/*------------------SPI interrupt vector-----------------------*/ 
#if (TRUE == gSpiSupported_d)
  #if (gSpiUsePort1_d && (gSpi1Mode_d == gInterruption_c))       
      SPI1_Isr,             // vector 15   - SPI
  #else
      UnimplementedISR,     // vector 15   - SPI
  #endif 
#else
  UnimplementedISR,         // vector 15   - SPI
#endif    
/*------------------TMR interrupt vectors-----------------------*/
#if (TRUE == gTimerSupported_d)  
  #if (gTimerMode_d == gInterruption_c)     
      Tmr_Tmr2Isr,          // vector 14   - TPM2 Cverflow
      Tmr_Tmr2Isr,          // vector 13   - TPM2 Channel 4
      Tmr_Tmr2Isr,          // vector 12   - TPM2 Channel 3
      Tmr_Tmr2Isr,          // vector 11   - TPM2 Channel 2
      Tmr_Tmr2Isr,          // vector 10   - TPM2 Channel 1
      Tmr_Tmr2Isr,          // vector 09   - TPM2 Channel 0
      Tmr_Tmr1Isr,          // vector 08   - TPM1 Overflow
      Tmr_Tmr1Isr,          // vector 07   - TPM1 Channel 2
      Tmr_Tmr1Isr,          // vector 06   - TPM1 Channel 1
      Tmr_Tmr1Isr,          // vector 05   - TPM1 Channel 0
  #else    
      UnimplementedISR,     // vector 14   - TPM2 Cverflow
      UnimplementedISR,     // vector 13   - TPM2 Channel 4
      UnimplementedISR,     // vector 12   - TPM2 Channel 3
      UnimplementedISR,     // vector 11   - TPM2 Channel 2
      UnimplementedISR,     // vector 10   - TPM2 Channel 1
      UnimplementedISR,     // vector 09   - TPM2 Channel 0
      UnimplementedISR,     // vector 08   - TPM1 Overflow
      UnimplementedISR,     // vector 07   - TPM1 Channel 2
      UnimplementedISR,     // vector 06   - TPM1 Channel 1
      UnimplementedISR,     // vector 05   - TPM1 Channel 0
  #endif
#else    
    UnimplementedISR,       // vector 14   - TPM2 Cverflow
    UnimplementedISR,       // vector 13   - TPM2 Channel 4
    UnimplementedISR,       // vector 12   - TPM2 Channel 3
    UnimplementedISR,       // vector 11   - TPM2 Channel 2
    UnimplementedISR,       // vector 10   - TPM2 Channel 1
    UnimplementedISR,       // vector 09   - TPM2 Channel 0
    UnimplementedISR,       // vector 08   - TPM1 Overflow
    UnimplementedISR,       // vector 07   - TPM1 Channel 2
    UnimplementedISR,       // vector 06   - TPM1 Channel 1
    UnimplementedISR,       // vector 05   - TPM1 Channel 0
#endif 
/*------------------ICG interrupt vectors-----------------------*/ 
    UnimplementedISR,       // vector 04   - ICG (FLL lock of clock)
/*------------------LVD interrupt vector-----------------------*/    
#if (TRUE == gLvdSupported_d)    
  #if (gLvdMode_d == gInterruption_c)  
      LowVoltageDetectIsr,  // vector 03   - LVD
  #else
      UnimplementedISR,     // vector 03   - LVD
  #endif    
#else
    UnimplementedISR,       // vector 03   - LVD
#endif
/*------------------IRQ interrupt vector-----------------------*/  
    IRQ_Isr,                // vector 02   - IRQ (MC1319x interrupt)
/*------------------SWI interrupt vector-----------------------*/    
    UnimplementedISR,       // vector 01   - SWI (Software Interrupt)
/*------------------Reset interrupt vector---------------------*/     
                            // vector 00   - Reset (Watchdog timer, LVD, external pin, illegal opcode)
};
#endif    

/******************* ISR Vectors definition for HCs08QE128 platform *******************/

#ifdef gMcs08qe128Platform_d
  const ISR_func_t ISR_vectors[] @ 0xFFC0 =
  {
#if (TRUE == gTimerSupported_d) 
  #if (gTimerMode_d == gInterruption_c)    
      Tmr_Tmr3Isr,          // vector 31   - TPM3 Overflow
      Tmr_Tmr3Isr,          // vector 30   - TPM3 Channel 5
      Tmr_Tmr3Isr,          // vector 29   - TPM3 Channel 4
      Tmr_Tmr3Isr,          // vector 28   - TPM3 Channel 3
      Tmr_Tmr3Isr,          // vector 27   - TPM3 Channel 2
      Tmr_Tmr3Isr,          // vector 26   - TPM3 Channel 1
      Tmr_Tmr3Isr,          // vector 25   - TPM3 Channel 0
  #else
      UnimplementedISR,     // vector 31   - TPM3 Overflow
      UnimplementedISR,     // vector 30   - TPM3 Channel 5
      UnimplementedISR,     // vector 29   - TPM3 Channel 4
      UnimplementedISR,     // vector 28   - TPM3 Channel 3
      UnimplementedISR,     // vector 27   - TPM3 Channel 2
      UnimplementedISR,     // vector 26   - TPM3 Channel 1
      UnimplementedISR,     // vector 25   - TPM3 Channel 0
  #endif
#else
    UnimplementedISR,       // vector 31   - TPM3 Overflow
    UnimplementedISR,       // vector 30   - TPM3 Channel 5
    UnimplementedISR,       // vector 29   - TPM3 Channel 4
    UnimplementedISR,       // vector 28   - TPM3 Channel 3
    UnimplementedISR,       // vector 27   - TPM3 Channel 2
    UnimplementedISR,       // vector 26   - TPM3 Channel 1
    UnimplementedISR,       // vector 25   - TPM3 Channel 0
#endif
/*------------------RTI interrupt vector------------------------*/
#if (TRUE == gRtiSupported_d) 
  #if (gRtiMode_d == gInterruption_c)    
      Rti_Isr,              // vector 24   - RTI
  #else
      UnimplementedISR,     // vector 24   - RTI
  #endif
#else
    UnimplementedISR,       // vector 24   - RTI
#endif
/*-----------------UART2 interrupt vector------------------------*/ 
#if (TRUE == gUartSupported_d)
  #if (gUart2_Enabled_d) 
    #if (gInterruption_c == gUartTxMode_d)
      Uart2_TxIsr,          // vector 23   - SCI2 TX
    #else                     
      UnimplementedISR,     // vector 23   - SCI2 TX
    #endif  
      Uart2_RxIsr,          // vector 22   - SCI2 RX
      Uart2_ErrorIsr,       // vector 21   - SCI2 Error
  #else
      UnimplementedISR,     // vector 23   - SCI2 TX
      UnimplementedISR,     // vector 22   - SCI2 RX
      UnimplementedISR,     // vector 21   - SCI2 Error
  #endif
#else
    UnimplementedISR,       // vector 23   - SCI2 TX
    UnimplementedISR,       // vector 22   - SCI2 RX
    UnimplementedISR,       // vector 21   - SCI2 Error
#endif
/*------------Analog Comparator interrupt vector----------------*/ 
    UnimplementedISR,       // vector 20   - Analog comparator x
/*-----------------ADC interrupt vector-------------------------*/ 
#if (TRUE == gAdcSupported_d)     
  #if (gAdcMode_d == gAdcInterrupt_c)   
      ADC_Isr,              // vector 19   - ADC
  #else
      UnimplementedISR,     // vector 19   - ADC
  #endif  
#else
    UnimplementedISR,       // vector 19   - ADC
#endif
/*-----------------KBI interrupt vector-------------------------*/ 
#if (TRUE == gKbiSupported_d)   
  #if (gKbiMode_d == gInterruption_c) 
       KBI1_ISR,            // vector 18   - KBI
  #else    
      UnimplementedISR,     // vector 18   - KBI
  #endif
#else    
    UnimplementedISR,       // vector 18   - KBI
#endif
/*-----------------IIC interrupt vector-------------------------*/ 
#if (TRUE == gIicSupported_d)   
  #if (gIicMode_d == gInterruption_c) 
      IIC_Isr,              // vector 17   - IIC 
  #else    
      UnimplementedISR,     // vector 17   - IIC 
  #endif
#else    
    UnimplementedISR,       // vector 17   - IIC 
#endif
/*-----------------UART1 interrupt vector------------------------*/ 
#if (TRUE == gUartSupported_d)
  #if (gUart1_Enabled_d)
    #if (gInterruption_c == gUartTxMode_d)    
      Uart1_TxIsr,          // vector 16   - SCI1 TX
    #else
      UnimplementedISR,     // vector 16   - SCI1 TX
    #endif
      Uart1_RxIsr,          // vector 15   - SCI1 RX
      Uart1_ErrorIsr,       // vector 14   - SCI1 Error
  #else
      UnimplementedISR,     // vector 16   - SCI1 TX
      UnimplementedISR,     // vector 15   - SCI1 RX
      UnimplementedISR,     // vector 14   - SCI1 Error  
  #endif
#else
    UnimplementedISR,       // vector 16   - SCI1 TX
    UnimplementedISR,       // vector 15   - SCI1 RX
    UnimplementedISR,       // vector 14   - SCI1 Error  
#endif
/*-----------------SPI interrupt vectors-------------------------*/ 
#if (TRUE == gSpiSupported_d) 
  #if (gSpiUsePort1_d && (gSpi1Mode_d == gInterruption_c))  
      SPI1_Isr,             // vector 13  - SPI1
  #else
      UnimplementedISR,     // vector 13  - SPI1
  #endif
  #if (gSpiUsePort2_d && (gSpi2Mode_d == gInterruption_c))
      SPI2_Isr,             // vector 12  - SPI2
  #else 
      UnimplementedISR,     // vector 12  - SPI2
  #endif   
#else 
    UnimplementedISR,       // vector 13  - SPI1
    UnimplementedISR,       // vector 12  - SPI2
#endif 
/*-----------------TMR interrupt vectors-------------------------*/ 
#if (TRUE == gTimerSupported_d) 
  #if (gTimerMode_d == gInterruption_c)    
      Tmr_Tmr2Isr,          // vector 11   - TPM2 Overflow
      Tmr_Tmr2Isr,          // vector 10   - TPM2 Channel 2
      Tmr_Tmr2Isr,          // vector 09   - TPM2 Channel 1
      Tmr_Tmr2Isr,          // vector 08   - TPM2 Channel 0        
      Tmr_Tmr1Isr,          // vector 07   - TPM1 Overflow
      Tmr_Tmr1Isr,          // vector 06   - TPM1 Channel 2
      Tmr_Tmr1Isr,          // vector 05   - TPM1 Channel 1
      Tmr_Tmr1Isr,          // vector 04   - TPM1 Channel 0
  #else
      UnimplementedISR,     // vector 11   - TPM2 Overflow
      UnimplementedISR,     // vector 10   - TPM2 Channel 2
      UnimplementedISR,     // vector 09   - TPM2 Channel 1
      UnimplementedISR,     // vector 08   - TPM2 Channel 0        
      UnimplementedISR,     // vector 07   - TPM1 Overflow
      UnimplementedISR,     // vector 06   - TPM1 Channel 2
      UnimplementedISR,     // vector 05   - TPM1 Channel 1
      UnimplementedISR,     // vector 04   - TPM1 Channel 0
  #endif
#else
    UnimplementedISR,       // vector 11   - TPM2 Overflow
    UnimplementedISR,       // vector 10   - TPM2 Channel 2
    UnimplementedISR,       // vector 09   - TPM2 Channel 1
    UnimplementedISR,       // vector 08   - TPM2 Channel 0        
    UnimplementedISR,       // vector 07   - TPM1 Overflow
    UnimplementedISR,       // vector 06   - TPM1 Channel 2
    UnimplementedISR,       // vector 05   - TPM1 Channel 1
    UnimplementedISR,       // vector 04   - TPM1 Channel 0
#endif
/*-----------------LVD interrupt vector------------------------*/ 
#if (TRUE == gLvdSupported_d) 
  #if (gLvdMode_d == gInterruption_c)   
      LowVoltageDetectIsr,  // vector 03   - LVD
  #else
      UnimplementedISR,     // vector 03   - LVD
  #endif  
#else
    UnimplementedISR,       // vector 03   - LVD
#endif  
/*------------------IRQ interrupt vector-----------------------*/  
    IRQ_Isr,                // vector 02   - IRQ (MC1319x interrupt)
/*------------------SWI interrupt vector-----------------------*/      
    UnimplementedISR,       // vector 01   - SWI (Software Interrupt)
/*------------------Reset interrupt vector---------------------*/     
                            // vector 00   - Reset (Watchdog timer, LVD, external pin, illegal opcode)
  };
#endif  

/******************* ISR Vectors definition for MC1323x platform *******************/

#ifdef gMc1323xPlatform_d
  const ISR_func_t ISR_vectors[] @ 0xFFC0=
  {
        
    UnimplementedISR,       // vector 31   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 30   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 29   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 28   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 27   - NOT A REAL VECTOR
    UnimplementedISR,       // vector 26   - NOT A REAL VECTOR
/*------------------RTI interrupt vector------------------------*/    
#if (TRUE == gRtiSupported_d)    
  #if (gRtiMode_d == gInterruption_c)       
      Rti_Isr,              // vector 25   - RTI
  #else
      UnimplementedISR,     // vector 25   - RTI
  #endif
#else
    UnimplementedISR,       // vector 25   - RTI
#endif
/*-----------------KBI interrupt vector-------------------------*/ 
#if (TRUE == gKbiSupported_d)
  #if (gKbiMode_d == gInterruption_c)    
      KBI1_ISR,             // vector 24   - KBI1 
      KBI2_ISR,             // vector 23   - KBI2 
  #else
      UnimplementedISR,     // vector 24   - KBI1 
      UnimplementedISR,     // vector 23   - KBI2
  #endif
#else
    UnimplementedISR,       // vector 24   - KBI1 
    UnimplementedISR,       // vector 23   - KBI2
#endif
/*-----------------IIC interrupt vector-------------------------*/ 
#if (TRUE == gIicSupported_d)
  #if (gIicMode_d == gInterruption_c) 
      IIC_Isr,              // vector 22   - IIC 
  #else    
      UnimplementedISR,     // vector 22   - IIC 
  #endif
#else    
    UnimplementedISR,       // vector 22   - IIC 
#endif
/*-----------------CMT interrupt vector-------------------------*/
#if (TRUE == gCmtSupported_d) 
  CMT_InterruptHandler,     // vector 21   - CMT
#else
  UnimplementedISR,         // vector 21   - CMT
#endif    
/*-----------------UART1 interrupt vector-----------------------*/      
#if (TRUE == gUartSupported_d)
  #if (gUart1_Enabled_d)
    #if (gInterruption_c == gUartTxMode_d)   
      Uart1_TxIsr,          // vector 20   - SCI TX
    #else    
      UnimplementedISR,     // vector 20   - SCI TX
    #endif
      Uart1_RxIsr,          // vector 19   - SCI RX
      Uart1_ErrorIsr,       // vector 18   - SCI Error
  #else
      UnimplementedISR,     // vector 20   - SCI TX
      UnimplementedISR,     // vector 19   - SCI RX
      UnimplementedISR,     // vector 18   - SCI Error  
  #endif
#else
    UnimplementedISR,       // vector 20   - SCI TX
    UnimplementedISR,       // vector 19   - SCI RX
    UnimplementedISR,       // vector 18   - SCI Error  
#endif
/*------------------SPI interrupt vector-----------------------*/  
#if (TRUE == gSpiSupported_d)
  #if (gSpiUsePort1_d && (gSpi1Mode_d == gInterruption_c))     
      SPI1_Isr,             // vector 17   - SPI
  #else
      UnimplementedISR,     // vector 17   - SPI
  #endif
#else
    UnimplementedISR,       // vector 17   - SPI
#endif  
/*------------------TMR interrupt vector-----------------------*/    
#if (TRUE == gTimerSupported_d)
  #if (gTimerMode_d == gInterruption_c)       
      Tmr_Tmr4Isr,          // vector 16   - TPM4 Overflow
      Tmr_Tmr4Isr,          // vector 15   - TPM4 Channel 0
      Tmr_Tmr3Isr,          // vector 14   - TPM3 Overflow
      Tmr_Tmr3Isr,          // vector 13   - TPM3 Channel 0
      Tmr_Tmr2Isr,          // vector 12   - TPM2 Overflow
      Tmr_Tmr2Isr,          // vector 11   - TPM2 Channel 0
      Tmr_Tmr1Isr,          // vector 10   - TPM1 Overflow
      Tmr_Tmr1Isr,          // vector 09   - TPM1 Channel 0
  #else
      UnimplementedISR,     // vector 16   - TPM4 Overflow  
      UnimplementedISR,     // vector 15   - TPM4 Channel 0  
      UnimplementedISR,     // vector 14   - TPM3 Overflow  
      UnimplementedISR,     // vector 13   - TPM3 Channel 0  
      UnimplementedISR,     // vector 12   - TPM2 Overflow  
      UnimplementedISR,     // vector 11   - TPM2 Channel 0  
      UnimplementedISR,     // vector 10   - TPM1 Overflow  
      UnimplementedISR,     // vector 09   - TPM1 Channel 0  
  #endif 
#else
    UnimplementedISR,       // vector 16   - TPM4 Overflow
    UnimplementedISR,       // vector 15   - TPM4 Channel 0
    UnimplementedISR,       // vector 14   - TPM3 Overflow
    UnimplementedISR,       // vector 13   - TPM3 Channel 0
    UnimplementedISR,       // vector 12   - TPM2 Overflow
    UnimplementedISR,       // vector 11   - TPM2 Channel 0
    UnimplementedISR,       // vector 10   - TPM1 Overflow
    UnimplementedISR,       // vector 09   - TPM1 Channel 0
#endif 
/*------------------ASM interrupt vector-----------------------*/  
    UnimplementedISR,       // vector 08   - ASM  
/*------------------PHY interrupt vectors----------------------*/     
    PhyHandlerRxWtrmrk_ISR, // vector 07   - PHY RX WTR MRK
    PhyHandlerTmr_ISR,      // vector 06   - PHY TIMERS
    PhyHandlerTx_ISR,       // vector 05   - PHY TX
    PhyHandlerRx_ISR,       // vector 04   - PHY RX
/*------------------LVD interrupt vector-----------------------*/      
#if (TRUE == gLvdSupported_d)   
  #if (gLvdMode_d == gInterruption_c)      
      LowVoltageDetectIsr,  // vector 03   - LVD
  #else
      UnimplementedISR,     // vector 03   - LVD
  #endif
#else
    UnimplementedISR,       // vector 03   - LVD
#endif
/*------------------IRQ interrupt vector-----------------------*/  
    UnimplementedISR,       // vector 02   - IRQ
/*------------------SWI interrupt vector-----------------------*/       
    UnimplementedISR,       // vector 01   - SWI (Software Interrupt)
/*------------------Reset interrupt vector---------------------*/     
                            // vector 00   - Reset (Watchdog timer, LVD, external pin, illegal opcode)
  };
#endif

/***********************************************************************************/

#pragma CONST_SEG RESET_VECTOR_SECTION

#if(TRUE == gOtapSupported_d)
  const ISR_func_t Reset_vector[] = {Otap_Startup};    // Vector 0    Reset vector
#else
  const ISR_func_t Reset_vector[] = {_Startup};    // Vector 0    Reset vector
#endif

#pragma CONST_SEG DEFAULT



/***********************************************************************************/
