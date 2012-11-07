/******************************************************************************
* Public header file for the OTAP module.
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
*******************************************************************************/
#ifndef _OTAP_INTERFACE_H_
#define _OTAP_INTERFACE_H_

/* Include files section */
#include <hidef.h>
#include "EmbeddedTypes.h"         /* Include special data types */
#include "derivative.h"            /* Include peripheral declarations */
#include "SMAC_Interface.h"        /* Include all OTA functionality */
#include "Utilities_Interface.h"   /* Include string copy and Blocking delays*/
#include "Timer_Interface.h"       /* Include timer functionality*/
#include "Eeprom_Interface.h"      /* Include Eeprom functionality*/
#include "Flash_Interface.h"       /* Include Flash functionality*/
#include "Eeprom4OTAP_Interface.h"
#include "Flash4Otap_Interface.h"
#include "SMAC_config.h"


/******************************************************************************
*******************************************************************************
* Public definitions
*******************************************************************************
*******************************************************************************/

#ifndef gOtapSupported_d
 #define gOtapSupported_d FALSE
#endif 

/*Otap module configurations*/
#define gOtapId_c                0x0001
#define gOtapRequestCode_c       0xAABBCCDD
#define gOtapDeviceDescription_c "AnyDeviceABCDEF1"

/******************************************************************************
*******************************************************************************
* Public prototypes
*******************************************************************************
*******************************************************************************/
#if(TRUE == gOtapSupported_d)
/*Otap Firlware Loader (OFLC) Routines they are placed in reserved Flash ******/
#pragma CODE_SEG OFLC_ROUTINES

/******************************************************************************
* Otap_Startup
*
* Stack pointer initialization for the OFLC the reset vector shall always point
* to this function. 
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void __near Otap_Startup(void);

/******************************************************************************
* Otap_OflcMain
*
* Main function for the OFLC,It looks for a mark on the EEPROM in order to 
* start the moving the new Firmware Code from EEPROM to Flash. 
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void __near Otap_OflcMain(void);

#pragma CODE_SEG DEFAULT
#endif /*(TRUE == gOtapSupported_d)*/


/*Otap Protocol Component (OPC) Routines they are placed in Application Flash */

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */
/*The functions used in interrupt context shall be placed in non-banked Flash */
/******************************************************************************
* Otap_OpcMCPSDataComfirm
*
* Packet data comfirm handler for the OPC, this function shall be called in the 
* SMAC MCPSDataComfirm callback.
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void Otap_OpcMCPSDataComfirm
(
 txStatus_t *pTxResult //IN: SMAC packet Tx results.
);

/******************************************************************************
* Otap_OpcMCPSDataIndication
*
* Packet data indication handler for the OPC, this function shall be called in
* the SMAC MCPSDataIndication callback.
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void Otap_OpcMCPSDataIndication
(
 rxPacket_t *pRxPacket //IN: SMAC received packet.
);

#pragma CODE_SEG DEFAULT

/******************************************************************************
* Otap_OpcMain
*
* Main function of the OPC, It shares the processor and radio functionality 
* with the SMAC based application when needed, it mainly exist in main(), 
* MCPSDataIndication(), and MCPSDataComfirm().
* This component manages the OTAP Protocol where basically the OTA Code is 
* received and stored on the EEPROM.
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void Otap_OpcMain(void);

/******************************************************************************
* Otap_OpcInit
*
* Initialization function for the OPC. It initializes local data and pointers.
*
* Interface assumptions: 
*
* Return value:  void
*
*******************************************************************************/
extern void Otap_OpcInit(void);


#endif  /* _OTAP_INTERFACE_H_ */