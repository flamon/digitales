/************************************************************************************
* This library provides the MAC/PHY with primitives for interrupt protection,
* configuration, checking and acknowledgement. Protection primitives are provided
* both for the general MCU interrupts as well as for the specific MC1319x interrupts,
* while configuration, checking and acknowledgement primitives exist only for the
* MC1319x interrupts. 
*
* Copyright (c) 2009, Freescale, Inc.  All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
*****************************************************************************/

#ifndef _INTCONTROL_H_
#define _INTCONTROL_H_

#include "derivative.h"


/************************************************************************************/
/************************************************************************************/
// Micro Processor Irq Control
/************************************************************************************/
/************************************************************************************/

/************************************************************************************
* Primitives to enable/disable all processor interrupts.
* Through immediate access and no backup handles the state of the interrupt mask bit 
* in the status register.
* 
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IntControl_EnableAllInts()       {__asm CLI;}    
#define IntControl_DisableAllInts()      {__asm SEI;} 

#define IrqControlLib_EnableAllIrqs()  IntControl_EnableAllInts()
#define IrqControlLib_DisableAllIrqs() IntControl_DisableAllInts()

/************************************************************************************
* Primitives to protect/unprotect from processor interrupts.
*   - a safer/slower way of enabling/disabling processor interrupts. 
* Using backup backup of irq status register handles the state
* of the interrupt mask bit in the status register.
* 
* The backup/restore primitives use a local variable for backup.
* The push/pull primitives use the stack. 
*
* NOTE: 
*   While push/pull is slightly faster than backup/restore, it is potentially more risky. 
*   Care must be taken to not corrupt the stack inside the push/pull protection! 
*   
* Interface assumptions:
*   None
*   
* Return value:
*   None
* 
************************************************************************************/
#define IntControl_BackupIrqStatus(var)      {__asm TPA; __asm STA var;}
#define IntControl_RestoreIrqStatus(var)     {__asm LDA var; __asm TAP;}

#define IrqControlLib_BackupIrqStatus(var)   IntControl_BackupIrqStatus(var)
#define IrqControlLib_RestoreIrqStatus(var)  IntControl_RestoreIrqStatus(var)


// Alternate way of storing/restoring status register on stack.
// Push/Pull is used, and care must be taken not to mess up the
// stack when using these macros. Compared to IrqControlLib_BackupIrqStatus/
// IrqControlLib_RestoreIrqStatus the push/pull method results in a small
// decrease in code size, and a small increase in execution
// speed (9 bytes/11 cycles compared to 5 bytes/8 cycles) on HCS08.

#define IntControl_PushIrqStatus()       {__asm TPA;  __asm PSHA;}
#define IntControl_PullIrqStatus()       {__asm PULA; __asm TAP;}

#define IrqControlLib_PushIrqStatus()    IntControl_PushIrqStatus()
#define IrqControlLib_PullIrqStatus()    IntControl_PullIrqStatus()



/***********************************************************************************/

//-----------------------------------------------------------------------------------

#endif //_INTCONTROL_H_
