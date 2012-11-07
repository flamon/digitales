/************************************************************************************
* Public header file used to set the MC13233 registers definitions.
*
*
* (c) Copyright 2009, Freescale, Inc.  All rights reserved.
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale.
*
************************************************************************************/

#ifndef __MC1323XREGS_H__
#define __MC1323XREGS_H__


#include "mc13233c.h"
#include "PhypacketProcessor.h"

#define SCI1BDH   SCIBDH
#define SCI1BDL   SCIBDL
#define SCI1C1    SCIC1
#define SCI1C2    SCIC2
#define SCI1S1    SCIS1
#define SCI1S2    SCIS2
#define SCI1C3    SCIC3
#define SCI1D     SCID

#define SPI1C1       SPIC1 
#define SPI1C2       SPIC2
#define SPI1BR       SPIBR
#define SPI1S        SPIS
#define SPI1D        SPID
#define SPI1C1_SPIE  SPIC1_SPIE
#define SPI1C1_SPTIE SPIC1_SPTIE
#define SPI1S_SPRF   SPIS_SPRF
#define SPI1S_SPTEF  SPIS_SPTEF

#endif /* __MC1323XREGS_H__ */




