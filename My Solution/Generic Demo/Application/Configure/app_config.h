/************************************************************************************
* Application Parameters Configuration 
*
* (c) Copyright 2010, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/

#ifndef __SMAC_APP_CONFIG_H__
#define __SMAC_APP_CONFIG_H__

#include "RTI_Interface.h"          /*Include all RTI functionality*/
#include "SPI_Interface.h"          /*Include all SPI functionality*/
#include "Timer_Interface.h"        /*Include all Timer functionality*/
#include "UART_Interface.h"         /*Include all UART functionality*/
#include "ASM_Interface.h"          /*Include all ASM functionality*/
#include "Flash_Interface.h"        /*Include all Flash functionality*/
#include "GPIO_interface.h"         /*Include all GPIO functionality*/
#include "IIC_Interface.h"          /*Include all IIC functionality*/
#include "IRQ_Interface.h"          /*Include all IRQ functionality*/
#include "KBI_Interface.h"          /*Include all KBI functionality*/
#include "LED_Interface.h"          /*Include all LED functionality*/
#include "LPM_Interface.h"          /*Include all LPM functionality*/
#include "LVD_interface.h"          /*Include all LVD functionality*/
#include "TouchPad_Interface.h"     /*Include all Touchpad functionality*/
#include "ADC_Interface.h"          /*Include all ADC functionality*/
#include "Keyboard_Interface.h"     /*Include all Keyboard functionality*/
#include "CMT_Interface.h"          /*Include all CMT functionality*/
#include "Accelerometer_Interface.h"/*Include all Accelerometer functionality*/
#include "Eeprom_Interface.h"       /*Include all Eeprom functionality*/
#include "LCD_Interface.h"          /*Include all LCD functionality*/

 
/* BEGIN BeeKit Configuration Parameters Definition */
#define gDefaultChannelNumber_c gChannel15_c

#define gDefaultUartPort_c   gUart2Index_c
#define gDefaultOutputPower_c   8
/* END BeeKit Configuration Parameters Definition */




#endif /* __SMAC_APP_CONFIG_H__ */

