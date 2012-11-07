/************************************************************************************
* EEPROM interface header. 
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* Freescale Semiconductor Confidential Proprietary
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
*
************************************************************************************/


#ifndef __EEPROM_INTERFACE_H__               
  #define __EEPROM_INTERFACE_H__ 

#include "derivative.h"     // include peripheral declarations 
#include "EmbeddedTypes.h"  // include special data types
#include "PortConfig.h"
#include "PLM_Config.h"
 

/************************************************************************************
*************************************************************************************
* Public Constants
*************************************************************************************
************************************************************************************/

#ifndef gEepromSupported_d   
  #define gEepromSupported_d   FALSE
#endif

typedef enum eepromErr_tag
{
  gEepromNoErr_c = 0,
  gEepromErrInvalidParam_c,
  gEepromErrSerialComNoInit_c,
  gEepromErrNoEepromInBus_c,
  gEepromErrBusBusy_c,
  gEepromErrAddrOverflow_c,
  gEepromErrNotSupported_c,  
  gEepromErrMax_c
} eepromErr_t;



/************************************************************************************
*************************************************************************************
* Type definitions
*************************************************************************************
************************************************************************************/

#ifdef MEMORY_MODEL_BANKED
  typedef void __near(* __near eepromCallback_t)(eepromErr_t);
#else
  typedef void(* __near eepromCallback_t)(eepromErr_t);
#endif


typedef struct eepromDataPackage_tag
{
  union
  {
     uint32_t  u32Address;
     uint8_t   u8Address[4];
  }EepromAddress;
  uint8_t* 		        pu8Data;
  uint8_t		          u8DataSize;	
  eepromCallback_t    pfEepromCallback;
} eepromDataPackage_t;


/************************************************************************************
*************************************************************************************
* Memory declarations
*************************************************************************************
************************************************************************************/



/************************************************************************************
*************************************************************************************
* Macro definitions
*************************************************************************************
************************************************************************************/

#if gTargetBoard_c == gMc1320xS08qe128Evb_c
 #define  Eeprom_EnableWriteProtect()       Gpio_PinSet(gEepromWriteProtectPin_c)
 #define  Eeprom_DisableWriteProtect()      Gpio_PinClear(gEepromWriteProtectPin_c)
#endif


#if (gTargetBoard_c == gMc1323xRcm_c || gTargetBoard_c == gMc1323xRem_c)
 /* Write Protection pin is connected to Vcc in 1323xMRB, and then no HW protection
    is used. If the user wants to protect the memory, He'll need to perform SW
    protection. Please refer to At45db021d datasheet.  */
 #define  Eeprom_EnableWriteProtect()       // Not implemented   
 #define  Eeprom_DisableWriteProtect()      // Not implemented  
#endif

/************************************************************************************
*************************************************************************************
* Interface functions
*************************************************************************************
************************************************************************************/


/************************************************************************************
*
* Eeprom_Init
*
* Description:  
* This entry point initiliazes the state machine of the Eeprom driver. It validates if             
* the Serial Communication (SPI or IIC) is previously initialized. Also, this API  
* performs a 5 byte reading of the memory, to check if the connection with the 
* external memory is correct.
*
* Interface assumptions: 
* The user MUST initialized the Serial Communication previous to call any of the 
* Eeprom APIs: IIC for 1320xQE128EVB board, and SPI for 1323xMRB board.
* This API should be called once prior to any other API from the Eeprom driver.
* 
* Return value:  EepromErr_t type
*         gEepromNoErr_c              - API Operation was completed succesfully. 
*         gEepromErrInvalidParam_c    - The user introduced a parameter not supported 
*                                       by the API.
*         gEepromErrNoEepromInBus_c   - Communication with external Eeprom presented
*                                       errors 
*         gEepromErrSerialComNoInit_c - The Serial Communication is not initialized
*         gEepromErrBusBusy           - The Bus communication is busy. 
*.
************************************************************************************/

extern eepromErr_t  Eeprom_Init 
(
  void
);

/************************************************************************************
*
* Eeprom_Write
*
* Description:
* This entry point allows the user to write data into the external Eeprom.  
*
* Interface assumptions: 
* The user MUST be aware that the Eeprom may be written by using pages (read Eeprom
* Datasheets). If the user stars to write at the end of the page, a number of bytes 
* is bigger of the bytes left in that page, the Eeprom will start to overwrite bytes
* from the beggining of the page. The driver will notify the user by returning 
* "gEepromErrAddrOverflow_c". 
*
* Return value:  EepromErr_t type
*         gEepromNoErr_c            - API Operation was completed succesfully. 
*         gEepromErrInvalidParam_c  - The user introduced a parameter not supported 
*                                     by the API.
*         gEepromErrAddrOverflow_c  - Bad manage of Page offset Address vs size of the
*                                     data that will be written
*         gEepromErrNoEepromInBus_c - Communication with external Eeprom presented
*                                     errors 
*         gEepromErrBusBusy         - The Bus communication is busy. 
*.
************************************************************************************/


extern eepromErr_t  Eeprom_Write 
(
  eepromDataPackage_t* pDataPackage   /* IN, This parameter contains the start Address, 
                                         a pointer to the Data, the Data size, and a  
                                         pointer to the callback that will be called
                                         After the Writting proccess is finished. The 
                                         Callback runs in interrupt context, and then,
                                         the user must not called any of the Eeprom 
                                         driver entry points from the callback */ 
);

/************************************************************************************
*
* Eeprom_Read
*
* Description: 
* This entry point allows the user to read data from the external Eeprom 
*
* Interface assumptions: 
* The user MUST be aware that the IIC Eeprom (only for 1320xQE128EVB) requires paging 
* reading. If the user stars to read at the end of the page, a number of bytes 
* is bigger of the bytes left in that page, the Eeprom will start to read bytes
* from the beggining of the page. The driver will notify the user by returning 
* "gEepromErrAddrOverflow_c".
* 
* Return value:  EepromErr_t type
*         gEepromNoErr_c            - API Operation was completed succesfully. 
*         gEepromErrInvalidParam_c  - The user introduced a parameter not supported 
*                                     by the API.
*         gEepromErrBusBusy         - The Bus communication is busy. 
*         gEepromErrAddrOverflow_c  - Bad manage of Page offset Address vs size of the
*                                     data that will be read
*         gEepromErrNoEepromInBus_c - Communication with external Eeprom presented
*                                     errors 
*      
************************************************************************************/


extern eepromErr_t  Eeprom_Read
(
  eepromDataPackage_t* pDataPackage  /* IN/OUT, This parameter contains the start  
                                        Address, a pointer to the Data, the Data size,  
                                        and a  pointer to the callback that will be 
                                        called after the Writting proccess is finished
                                        The Callback runs in interrupt context, and 
                                        then,the user must not called any of the Eeprom 
                                        driver entry points from the callback */ 
);

/************************************************************************************
*
* Eeprom_Erase
*
* Description: 
* This entry point erases all the Eeprom array. 
*
* Interface assumptions:
* This API is not implemented for 1320xQE128EVB Eeprom At24C1024bwI2c does not require 
* to erase before writting.Erase procedure may be done by writting 0xFF data in the 
* memory. The user may use "Eeprom_Write" API to achieve this.  
* For 1323xMRB: After sending the erase chip sequence, the Eeprom will be busy during
* 3.6 to 6 seconds. The user must have this in mind because any other Eeprom API will 
* return BusBusy error while the Eeprom finishes the erase procedure.
* 
* Return value:  EepromErr_t type
*         gEepromNoErr_c            - API Operation was completed succesfully. 
*         gEepromErrNotSupported_c  - This API is not supported for the current 
*                                     Platform
*         gEepromErrNoEepromInBus_c - Communication with external Eeprom presented
*                                     errors       
*.
************************************************************************************/

extern eepromErr_t  Eeprom_Erase
(
  void
);



#endif //__EEPROM_INTERFACE_H__

