/************************************************************************************
* Private header file for the MC1323x Packet processor driver.
*
*
*
* (c) Copyright 2010, Freescale Semiconductor, Inc. All rights reserved.
*
*
* No part of this document must be reproduced in any form - including copied,
* transcribed, printed or by any electronic means - without specific written
* permission from Freescale Semiconductor.
************************************************************************************/


#ifndef _IOCONFIG_MC1323x_H_
#define _IOCONFIG_MC1323x_H_


/***********  DIRECT ACCESS REGISTERS  ***********/
/* direct access registers addresses */



#define     PP_PHY_TXP         (*((volatile         void **)(0x0040)))	/* 802.15.4 Packet Processor PHY TXD_ADR_PNTR (entire) */
#define     PP_PHY_TXPH        (*((volatile unsigned char *)(0x0040)))	/* 802.15.4 Packet Processor PHY TXD_ADR_PNTR (high) */
#define     PP_PHY_TXPL        (*((volatile unsigned char *)(0x0041)))	/* 802.15.4 Packet Processor PHY TXD_ADR_PNTR (low) */
#define     PP_PHY_RXP         (*((volatile         void **)(0x0042)))	/* 802.15.4 Packet Processor PHY RXD_ADR_PNTR (entire) */
#define     PP_PHY_RXPH        (*((volatile unsigned char *)(0x0042)))	/* 802.15.4 Packet Processor PHY RXD_ADR_PNTR (high) */
#define     PP_PHY_RXPL        (*((volatile unsigned char *)(0x0043)))	/* 802.15.4 Packet Processor PHY RXD_ADR_PNTR (low) */
#define     PP_PHY_CTL1        (*((volatile unsigned char *)(0x0044)))	/* 802.15.4 Packet Processor PHY CNTRL1 */
#define     PP_PHY_CTL2        (*((volatile unsigned char *)(0x0045)))	/* 802.15.4 Packet Processor PHY CNTRL2 */
#define     PP_PHY_CTL3        (*((volatile unsigned char *)(0x0046)))	/* 802.15.4 Packet Processor PHY CNTRL3 */
#define     PP_PHY_CTL4        (*((volatile unsigned char *)(0x0047)))	/* 802.15.4 Packet Processor PHY CNTRL4 */
#define     PP_PHY_SCTL        (*((volatile unsigned char *)(0x0048)))	/* 802.15.4 Packet Processor PHY SRC_CNTRL */
#define     PP_PHY_SADRSUMD    (*((volatile unsigned short*)(0x0049)))	/* 802.15.4 Packet Processor PHY SRC_ADDRS_SUM_DATA (entire) */
#define     PP_PHY_SADRSUMDH   (*((volatile unsigned char *)(0x0049)))	/* 802.15.4 Packet Processor PHY SRC_ADDRS_SUM_DATA (high) */
#define     PP_PHY_SADRSUMDL   (*((volatile unsigned char *)(0x004A)))	/* 802.15.4 Packet Processor PHY SRC_ADDRS_SUM_DATA (low) */
#define     PP_PHY_RXWTRMARK   (*((volatile unsigned char *)(0x004B)))	/* 802.15.4 Packet Processor PHY RX_WTR_MARK */
#define     PP_PHY_RXBYTECNT   (*((volatile unsigned char *)(0x004C)))	/* 802.15.4 Packet Processor PHY RX_BYTE_COUNT */
#define     PP_PHY_STATUS1     (*((volatile unsigned char *)(0x004D)))	/* 802.15.4 Packet Processor PHY STATUS1 */
#define     PP_PHY_STATUS2     (*((volatile unsigned char *)(0x004E)))	/* 802.15.4 Packet Processor PHY STATUS2 */
#define     PP_PHY_CCAINST     (*((volatile unsigned char *)(0x004F)))	/* 802.15.4 Packet Processor PHY CCA_INST */
#define     PP_PHY_CCAFNL      (*((volatile unsigned char *)(0x0050)))	/* 802.15.4 Packet Processor PHY CCAFNL */
#define     PP_PHY_EVENTTMR16  (*((volatile unsigned char *)(0x0051)))	/* 802.15.4 Packet Processor PHY EVENT_TMR [23:16] */
#define     PP_PHY_EVENTTMR8   (*((volatile unsigned char *)(0x0052)))	/* 802.15.4 Packet Processor PHY EVENT_TMR [15:8]  */
#define     PP_PHY_EVENTTMR0   (*((volatile unsigned char *)(0x0053)))	/* 802.15.4 Packet Processor PHY EVENT_TMR [7:0]   */
#define     PP_PHY_TIMESTMP16  (*((volatile unsigned char *)(0x0054)))	/* 802.15.4 Packet Processor PHY TIME_STAMP [23:16] */
#define     PP_PHY_TIMESTMP8   (*((volatile unsigned char *)(0x0055)))	/* 802.15.4 Packet Processor PHY TIME_STAMP [15:8]  */
#define     PP_PHY_TIMESTMP0   (*((volatile unsigned char *)(0x0056)))	/* 802.15.4 Packet Processor PHY TIME_STAMP [7:0]   */
#define     PP_PHY_T3CMP16     (*((volatile unsigned char *)(0x0057)))	/* 802.15.4 Packet Processor PHY T3CMP [23:16] */
#define     PP_PHY_T3CMP8      (*((volatile unsigned char *)(0x0058)))	/* 802.15.4 Packet Processor PHY T3CMP [15:8]  */
#define     PP_PHY_T3CMP0      (*((volatile unsigned char *)(0x0059)))	/* 802.15.4 Packet Processor PHY T3CMP [7:0]   */
#define     PP_PHY_TC2PRIME    (*((volatile unsigned short*)(0x005A)))	/* 802.15.4 Packet Processor PHY TC2PRIME [15:0]  */
#define     PP_PHY_TC2PRIMEH   (*((volatile unsigned char *)(0x005A)))	/* 802.15.4 Packet Processor PHY TC2PRIME [15:8]  */
#define     PP_PHY_TC2PRIMEL   (*((volatile unsigned char *)(0x005B)))	/* 802.15.4 Packet Processor PHY TC2PRIME [7:0]   */
#define     PP_PHY_SHRTADDR    (*((volatile unsigned short*)(0x005C)))	/* 802.15.4 Packet Processor PHY macShortAddrs [15:0]  */
#define     PP_PHY_SHRTADDRH   (*((volatile unsigned char *)(0x005C)))	/* 802.15.4 Packet Processor PHY macShortAddrs [15:8]  */
#define     PP_PHY_SHRTADDRL   (*((volatile unsigned char *)(0x005D)))	/* 802.15.4 Packet Processor PHY macShortAddrs [7:0]   */
#define     PP_PHY_PANID       (*((volatile unsigned short*)(0x005E)))	/* 802.15.4 Packet Processor PHY macPanId [15:0]  */
#define     PP_PHY_PANIDH      (*((volatile unsigned char *)(0x005E)))	/* 802.15.4 Packet Processor PHY macPanId [15:8]  */
#define     PP_PHY_PANIDL      (*((volatile unsigned char *)(0x005F)))	/* 802.15.4 Packet Processor PHY macPanId [7:0]   */

#define     PP_PHY_T1CMP16     (*((volatile unsigned char *)(0x1840)))	/* 802.15.4 Packet Processor PHY T1CMP [23:16] */
#define     PP_PHY_T1CMP8      (*((volatile unsigned char *)(0x1841)))	/* 802.15.4 Packet Processor PHY T1CMP [15:8] */
#define     PP_PHY_T1CMP0      (*((volatile unsigned char *)(0x1842)))	/* 802.15.4 Packet Processor PHY T1CMP [7:0] */
#define     PP_PHY_T2CMP16     (*((volatile unsigned char *)(0x1843)))	/* 802.15.4 Packet Processor PHY T2CMP [23:16] */
#define     PP_PHY_T2CMP8      (*((volatile unsigned char *)(0x1844)))	/* 802.15.4 Packet Processor PHY T2CMP [15:8] */
#define     PP_PHY_T2CMP0      (*((volatile unsigned char *)(0x1845)))	/* 802.15.4 Packet Processor PHY T2CMP [7:0] */
#define     PP_PHY_T4CMP16     (*((volatile unsigned char *)(0x1846)))	/* 802.15.4 Packet Processor PHY T4CMP [23:16] */
#define     PP_PHY_T4CMP8      (*((volatile unsigned char *)(0x1847)))	/* 802.15.4 Packet Processor PHY T4CMP [15:8] */
#define     PP_PHY_T4CMP0      (*((volatile unsigned char *)(0x1848)))	/* 802.15.4 Packet Processor PHY T4CMP [7:0] */
#define     PP_PHY_LO1FRAC     (*((volatile unsigned short*)(0x1849)))	/* 802.15.4 Packet Processor PHY LO1_FRAC [15:0] */
#define     PP_PHY_LO1FRACH    (*((volatile unsigned char *)(0x1849)))	/* 802.15.4 Packet Processor PHY LO1_FRAC [15:8] */
#define     PP_PHY_LO1FRACL    (*((volatile unsigned char *)(0x184A)))	/* 802.15.4 Packet Processor PHY LO1_FRAC [7:0] */
#define     PP_PHY_LO1INT      (*((volatile unsigned char *)(0x184B)))	/* 802.15.4 Packet Processor PHY LO1_INT */
#define     PP_PHY_PAPWRCTRL   (*((volatile unsigned char *)(0x184C)))	/* 802.15.4 Packet Processor PHY PA PWR CNTL */
#define     PP_PHY_LONGADR56   (*((volatile unsigned char *)(0x184D)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [63:56] */
#define     PP_PHY_LONGADR48   (*((volatile unsigned char *)(0x184E)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [55:48] */
#define     PP_PHY_LONGADR40   (*((volatile unsigned char *)(0x184F)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [47:40] */
#define     PP_PHY_LONGADR32   (*((volatile unsigned char *)(0x1850)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [39:32] */
#define     PP_PHY_LONGADR24   (*((volatile unsigned char *)(0x1851)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [31:24] */
#define     PP_PHY_LONGADR16   (*((volatile unsigned char *)(0x1852)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [23:16] */
#define     PP_PHY_LONGADR8    (*((volatile unsigned char *)(0x1853)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [15:8] */
#define     PP_PHY_LONGADR0    (*((volatile unsigned char *)(0x1854)))	/* 802.15.4 Packet Processor PHY MAC LONG ADDRS [7:0] */
#define     PP_PHY_MAXFRMLEN   (*((volatile unsigned char *)(0x1855)))	/* 802.15.4 Packet Processor PHY MAX FRAME LENGTH */
#define     PP_PHY_RXFRMFLT    (*((volatile unsigned char *)(0x1856)))	/* 802.15.4 Packet Processor PHY RX FRAME FILTER */
#define     PP_PHY_FRMREVTMR   (*((volatile unsigned char *)(0x1857)))	/* 802.15.4 Packet Processor PHY FRAME VER & TMR */
#define     PP_PHY_CCATHRSHD   (*((volatile unsigned char *)(0x1858)))	/* 802.15.4 Packet Processor PHY CCA THRESHOLD */
#define     PP_PHY_CCAOFSCMP   (*((volatile unsigned char *)(0x1859)))	/* 802.15.4 Packet Processor PHY CCA OFFSET CMP */
#define     PP_PHY_FSM         (*((volatile unsigned char *)(0x185A)))	/* 802.15.4 Packet Processor PHY FSM */
#define     PP_PHY_INDACCIDX   (*((volatile unsigned char *)(0x185B)))	/* 802.15.4 Packet Processor PHY INDIRECT ACCESS INDEX */
#define     PP_PHY_INDACCREG   (*((volatile unsigned char *)(0x185C)))	/* 802.15.4 Packet Processor PHY INDIRECT ACCESS REG */

/* direct access registers bit definitions */
//  PP_PHY_CTL1
#define     PP_PHY_CTL1_TMRTRIGEN_BIT     (1<<7)
#define     PP_PHY_CTL1_SLOTTED_BIT       (1<<6)
#define     PP_PHY_CTL1_CCABFRTX_BIT      (1<<5)
#define     PP_PHY_CTL1_RXACKRQD_BIT      (1<<4)
#define     PP_PHY_CTL1_AUTOACK_BIT       (1<<3)
#define     PP_PHY_CTL1_XCVSEQ_MASK       (7<<0)

//  PP_PHY_CTL2
#define     PP_PHY_CTL2_CCA_TYPE_POS      3
#define     PP_PHY_CTL2_TRCV_MSK_BIT      (1<<7)
#define     PP_PHY_CTL2_TC3TMOUT_BIT      (1<<6)
#define     PP_PHY_CTL2_PANCORDNTR_BIT    (1<<5)
#define     PP_PHY_CTL2_CCA_TYPE_MASK     (3<<PP_PHY_CTL2_CCA_TYPE_POS)
#define     PP_PHY_CTL2_TMRLOAD_BIT       (1<<2)
#define     PP_PHY_CTL2_PROMISCUOUS_BIT   (1<<1)
#define     PP_PHY_CTL2_TC2PRIME_EN_BIT   (1<<0)

//  PP_PHY_CTL3
#define     PP_PHY_CTL3_CRC_MSK_BIT       (1<<7)
#define     PP_PHY_CTL3_LO1UNLOCK_MSK_BIT (1<<6)
#define     PP_PHY_CTL3_FLTR_FAIL_MSK_BIT (1<<5)
#define     PP_PHY_CTL3_RX_WMRK_MSK_BIT   (1<<4)
#define     PP_PHY_CTL3_CCA_MSK_BIT       (1<<3)
#define     PP_PHY_CTL3_RX_MSK_BIT        (1<<2)
#define     PP_PHY_CTL3_TX_MSK_BIT        (1<<1)
#define     PP_PHY_CTL3_SEQ_MSK_BIT       (1<<0)

//  PP_PHY_CTL4
#define     PP_PHY_CTL4_TMR4MSK_BIT       (1<<7)
#define     PP_PHY_CTL4_TMR3MSK_BIT       (1<<6)
#define     PP_PHY_CTL4_TMR2MSK_BIT       (1<<5)
#define     PP_PHY_CTL4_TMR1MSK_BIT       (1<<4)
#define     PP_PHY_CTL4_TMR4CMP_EN_BIT    (1<<3)
#define     PP_PHY_CTL4_TMR3CMP_EN_BIT    (1<<2)
#define     PP_PHY_CTL4_TMR2CMP_EN_BIT    (1<<1)
#define     PP_PHY_CTL4_TMR1CMP_EN_BIT    (1<<0)

//  PP_PHY_STATUS1
#define     PP_PHY_STATUS1_CRCVALID_BIT   (1<<7)
#define     PP_PHY_STATUS1_CCA_BIT        (1<<6)
#define     PP_PHY_STATUS1_SRCADDR_BIT    (1<<5)
#define     PP_PHY_STATUS1_PI_BIT         (1<<4)
#define     PP_PHY_STATUS1_TMR4_IRQ       (1<<3)
#define     PP_PHY_STATUS1_TMR3_IRQ       (1<<2)
#define     PP_PHY_STATUS1_TMR2_IRQ       (1<<1)
#define     PP_PHY_STATUS1_TMR1_IRQ       (1<<0)

//  PP_PHY_STATUS2
#define     PP_PHY_STATUS2_FRPEND_BIT     (1<<7)
#define     PP_PHY_STATUS2_LO1UNLOCK_IRQ  (1<<6)
#define     PP_PHY_STATUS2_FLTR_FAIL_IRQ  (1<<5)
#define     PP_PHY_STATUS2_RX_WMRK_IRQ    (1<<4)
#define     PP_PHY_STATUS2_CCA_IRQ        (1<<3)
#define     PP_PHY_STATUS2_RX_IRQ         (1<<2)
#define     PP_PHY_STATUS2_TX_IRQ         (1<<1)
#define     PP_PHY_STATUS2_SEQ_IRQ        (1<<0)

//  PP_PHY_FRMREVTMR
#define     PP_PHY_FRMREVTMR_TMPRSC_MASK  (7<<5)
#define     PP_PHY_FRMREVTMR_FRMVER_MASK  (3<<0)

//  PP_PHY_RXFRMFLT
#define     PP_PHY_RXFRMFLT_NS_FT_BIT     (1<<4)
#define     PP_PHY_RXFRMFLT_CMD_FT_BIT    (1<<3)
#define     PP_PHY_RXFRMFLT_ACK_FT_BIT    (1<<2)
#define     PP_PHY_RXFRMFLT_DATA_FT_BIT   (1<<1)
#define     PP_PHY_RXFRMFLT_BEACON_FT_BIT (1<<0)

//  PP_PHY_PAPWRCTRL
#define     PP_PHY_PAPWRCTRL_PALCRS_MASK  (3<<6)
#define     PP_PHY_PAPWRCTRL_PALFIN_MASK  (3<<4)
#define     PP_PHY_PAPWRCTRL_PADCRS_MASK  (3<<2)
#define     PP_PHY_PAPWRCTRL_PADFIN_MASK  (3<<0)

//  PP_PHY_SCTL
#define     PP_PHY_SCTL_INDEX_POS          4
#define     PP_PHY_SCTL_INDEX_MASK       (15<<PP_PHY_SCTL_INDEX_POS)
#define     PP_PHY_SCTL_ACK_FP_BIT        (1<<3)
#define     PP_PHY_SCTL_SRC_ADDR_EN_BIT   (1<<2)
#define     PP_PHY_SCTL_INDEX_EN_BIT      (1<<1)
#define     PP_PHY_SCTL_INDEX_DIS_BIT     (1<<0)

/* end of direct access registers bit definitions */

/***********  INDIRECT ACCESS REGISTERS  ***********/

// indirect access registers macros
#define     IoIndirectRead(addr)                    (PP_PHY_INDACCIDX = (addr), PP_PHY_INDACCREG)
#define     IoIndirectWrite(addr, data)              PP_PHY_INDACCIDX = (addr); PP_PHY_INDACCREG = (data)
#define     IoIndirectReadMultiple(addr,  data, n)   PP_PHY_INDACCIDX =((addr) | 0x80);{ uint8_t __macro_indx; for(__macro_indx = 0; __macro_indx<n; __macro_indx++) (data)[__macro_indx] = PP_PHY_INDACCREG;}
#define     IoIndirectWriteMultiple(addr, data, n)   PP_PHY_INDACCIDX = (addr) | 0x80; { uint8_t __macro_indx; for(__macro_indx = 0; __macro_indx<n; __macro_indx++) PP_PHY_INDACCREG = (data)[__macro_indx];}

/* indirect access registers addresses */
// ...
#define     PP_PHY_IAR_RESET              0x24
#define     PP_PHY_IAR_DIG_TESTMODE       0x59
#define     PP_PHY_IAR_FRACN_ACC          0x0A
// ...

/* indirect access registers bit definitions */
//  PP_PHY_IAR_RESET - 0x24
#define     PP_PHY_IAR_BIT_TRCV_RST       (1<<7)
#define     PP_PHY_IAR_BIT_R2RAM_RST      (1<<4)
#define     PP_PHY_IAR_BIT_EVTMR_RST      (1<<3)
#define     PP_PHY_IAR_BIT_TXMOD_RST      (1<<2)
#define     PP_PHY_IAR_BIT_RXMOD_RST      (1<<1)
#define     PP_PHY_IAR_BIT_SEQMGR_RST     (1<<0)

//  PP_PHY_IAR_DIG_TMUX - 0x58
#define     PP_PHY_IAR_BIT_TST_HF_EN      (1<<7)
#define     PP_PHY_IAR_BIT_TMUX_EN        (1<<6)
#define     PP_PHY_IAR_MASK_TMUX_PAGE     (0x3F)

//  PP_PHY_IAR_DIG_TESTMODE - 0x59
#define     PP_PHY_IAR_BIT_BER_EN         (1<<3)
#define     PP_PHY_IAR_BIT_FPGA_EN        (1<<2)
#define     PP_PHY_IAR_BIT_IDEAL_PSM_EN   (1<<1)
#define     PP_PHY_IAR_BIT_MODE_TMUX_EN   (1<<0)

/* end of indirect access registers bit definitions */



#endif /* _PHY_PACKET_PROCESSOR_H_ */
