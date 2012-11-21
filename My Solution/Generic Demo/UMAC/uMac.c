/*
 * uMac.c
 *
 *  Created on: Nov 1, 2012
 */

#include "McuInit.h"                /*CPU and System Clock related functions*/
#include "EmbeddedTypes.h"          /*Include special data types*/       
#include "SMAC_Interface.h"         /*Include all OTA functionality*/
#include "uMac.h"                   /*Include all OTA functionality*/
#include "app_config.h"
#include "OTAP_Interface.h"
#include "PLM_config.h"


#define MyID 1

typedef enum {
    uMac_NoInit = 0,
    uMac_Init = 1,
    uMac_WaitRx = 2,
    uMac_Rx = 3,
    uMac_Tx = 4
} uMac_Engine_State;

static uMac_Engine_State uMac_Current_State;
static bool_t uMac_On = FALSE;

static uint8_t gau8RxDataBuffer[130]; /* 123 bytes is the SDU max size in non
                                         promiscuous mode. 
                                         125 bytes is the SDU max size in 
                                         promiscuous mode. 
                                         You have to consider the SDU plus 5 more 
                                         bytes for SMAC header and that the buffer 
                                         can not be bigger than 130 */
static uint8_t gau8TxDataBuffer[128]; /* This buffer can be as big as the biggest
                                         packet to transmit in the app plus 3 
                                         bytes reserved for SMAC packet header.
                                         The buffer can not be bigger than 128 */
        
static txPacket_t *AppTxPacket;
static rxPacket_t *AppRxPacket;

static uMac_Packet *uMac_RxPacket;
static uMac_Packet *uMac_TxPacket;

bool_t bTxDone, bRxDone, bScanDone, bDoTx;
uint8_t uMacbroad = 254, ui = 0, ChannelsEnergy[16];
uint16_t ChannelsToScan = 0xFFF;
channels_t bestChannel, Channels[] = {gChannel11_c, gChannel12_c, gChannel13_c, gChannel14_c, gChannel15_c,
            gChannel16_c, gChannel17_c, gChannel18_c, gChannel19_c, gChannel20_c, gChannel21_c,
            gChannel22_c, gChannel23_c, gChannel24_c, gChannel25_c, gChannel26_c};


void InitSmac (void);  // Poner en memoria bankeada
void uMac_Txf (void);

void Init_uMac (void)
{
    InitSmac();

    uMac_On = TRUE;
    uMac_RxPacket = (uMac_Packet *)AppRxPacket->smacPdu.u8Data;
    uMac_TxPacket = (uMac_Packet *)AppTxPacket->smacPdu.u8Data;
    uMac_Current_State = uMac_NoInit;
}

void uMac_Txf (void)
{
    bDoTx = TRUE;
}

void uMac_Engine (void)
{   
    switch (uMac_Current_State) {
    case uMac_NoInit:
        if (uMac_On == TRUE) {
            uMac_On = FALSE;
            
            if (ui == 16) {
                ui = 0;
            }
            
            (void) MLMESetChannelRequest(Channels[0]);
            //(void) MLMESetChannelRequest(gChannel11_c);
            uMac_TxPacket->Dest_Add = uMacbroad;
            uMac_TxPacket->Packet_Type = 0;
            uMac_TxPacket->Pan_ID = 10;
            uMac_TxPacket->Source_Add = MyID;
            (void) MCPSDataRequest(AppTxPacket);
            uMac_Current_State = uMac_Init;
        }
        break;
    case uMac_Init:
        if (bTxDone == TRUE) {
            bTxDone = FALSE;
            (void) MLMERXEnableRequest(AppRxPacket, 0x00000001); 
            uMac_Current_State = uMac_WaitRx;
            // uMac_On = TRUE;
            // uMac_Current_State = uMac_NoInit;
        }
        break;
    case uMac_WaitRx:
        if (bRxDone == TRUE) {
            bRxDone = FALSE;
            if (AppRxPacket->rxStatus == rxSuccessStatus_c) {
                if (uMac_RxPacket->Pan_ID == 10) {
                    if (uMac_RxPacket->Dest_Add == MyID || uMac_RxPacket->Dest_Add == uMacbroad) {
                        switch (uMac_RxPacket->Source_Add) {
                        case 1:
                            Led_PrintValue(0x08);
                            break;
                        case 2:
                            Led_PrintValue(0x06);
                            break;
                        case 3:
                            Led_PrintValue(0x03);
                            break;
                        }
                        
                        if (uMac_RxPacket->Packet_Type != 0) {
                            // Llamar callback
                        }
                        
                        (void) MLMERXEnableRequest(AppRxPacket, 0);
                    }
                } else {
                    uMac_Current_State = uMac_NoInit;
                    uMac_On = TRUE;
                }
            } else if (AppRxPacket->rxStatus == rxTimeOutStatus_c) {
                uMac_Current_State = uMac_NoInit;
                uMac_On = TRUE;
            }
        }

        if (bDoTx == TRUE) {
            bDoTx = FALSE;
            (void) MLMERXDisableRequest();  // Deshabilitar la recepcion antes de transmitir (necesario por SMAC)
            (void) MCPSDataRequest(AppTxPacket);
            uMac_Current_State = uMac_Tx;
        }
        
        break;
    case uMac_Tx:
        if (bTxDone == TRUE) {
            bTxDone = FALSE;
            uMac_Current_State = uMac_Init;
            (void) MLMERXEnableRequest(AppRxPacket, 0);
        }
        break;
    }
}

void InitSmac (void)
{
    AppTxPacket = (txPacket_t *) gau8TxDataBuffer;
    AppTxPacket->u8DataLength = 10;

    AppRxPacket = (rxPacket_t *) gau8RxDataBuffer; 
    AppRxPacket->u8MaxDataLength = gMaxSmacSDULenght_c;
    
    
    (void)MLMERadioInit();
    (void)MLMESetClockRate(gClko16MHz_c);
    (void)MCU_UseExternalClock();
    
    (void)MLMESetTmrPrescale(gTimeBase250kHz_c);
    while (gErrorNoError_c != MLMESetChannelRequest(gDefaultChannelNumber_c));
    (void)MLMEPAOutputAdjust(gDefaultOutputPower_c);
    (void)MLMEFEGainAdjust(gGainOffset_c);
 }

// Place it in NON_BANKED memory
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

void MLMEScanComfirm (channels_t ClearestChann)
{
    bestChannel = ClearestChann; 
    bScanDone = TRUE;
}

void MCPSDataIndication (rxPacket_t *gsRxPacket)
{  
    bRxDone = TRUE;
}

void MCPSDataComfirm (txStatus_t TransmissionResult)
{  
    bTxDone = TRUE;
}

void MLMEResetIndication(void) { }

void MLMEWakeComfirm(void) { }

#pragma CODE_SEG DEFAULT
