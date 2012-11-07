/************************************************************************************
* This library provides primitives for interrupt protection,
* configuration, checking and acknowledgement. 
*
* Copyright (c) 2009, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _IRQ_INTERFACE_H_
#define _IRQ_INTERFACE_H_

#include "derivative.h"     /* include peripheral declarations */
#include "EmbeddedTypes.h"  // include special data types


/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/
//typedef void(*irqCallback_t)(void);  
#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near irqCallback_t)(void);
#else
  typedef void(* __near irqCallback_t)(void);
#endif

/************************************************************************************
*************************************************************************************
* Interface macro definitions
*************************************************************************************
************************************************************************************/

  
/************************************************************************************
* Primitives to enable/disable Irq interrupts.
* Performed locally on the processors external irq registers
* 
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
  
#define IRQ_EnableIrq()              {IRQSC |=  0x02;}      // Set IRQPE in HCS08 => IRQ pin from MC1319x enabled
#define IRQ_DisableIrq()             {IRQSC &= ~0x02;}      // Clr IRQPE in HCS08 => IRQ pin from MC1319x disabled




/************************************************************************************
* Primitives to protect/unprotect from Irq interrupts.
* Performed locally on the processors external irq registers.
* Utilizes a backup/restore approach
* 
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IRQ_ProtectFromIrq(var)      {var = IRQSC; IRQ_DisableIrq()}
#define IRQ_UnprotectFromIrq(var)    {IRQSC = var;}


/************************************************************************************
* Primitive to check if irq is (still) asserted.
* Checks irq pin state through processors external irq registers.
* 
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IRQ_IsIrqAsserted()          (IRQSC & 0x08)

/************************************************************************************
* Primitive to init/setup Irq interrupts in the processor.
* Enables external interrupt from Irq and configures the processor external irq
* to react to active low and edge-triggered irq.
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IRQ_InitIrq()                {IRQSC  = 0x16;} // Enable external interrupt from MC1319x, active low and edge-triggered

/************************************************************************************
* Primitive to acknowledge Irq interrupts in the processors external irq register.
* Clears latched irqs in the processor (if such mechanism on the processor).
* On HCS08 clears IRQF bit in IRQSC register
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IRQ_AckIrq()                 {IRQSC |= 0x04;}

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/

/************************************************************************************
* Initialize the IRQ module and hardware.
*
* Interface assumptions: None
*
* Return value:          None
*
************************************************************************************/
void IRQ_Init
(
irqCallback_t UserCallBack
);

/* Place it in NON_BANKED memory */
#pragma CODE_SEG ISR_SEGMENT

/************************************************************************************
* IRQ´s Interrupr service function
*
* Interface assumptions: TBD
*
* Return value:          TBD
*
************************************************************************************/
INTERRUPT_KEYWORD void IRQ_Isr(void);


#pragma CODE_SEG DEFAULT
/***********************************************************************************/

//-----------------------------------------------------------------------------------

#endif //_IRQ_INTERFACE_H_
