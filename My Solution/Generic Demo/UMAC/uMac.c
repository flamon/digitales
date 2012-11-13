/*
 * uMac.c
 *
 *  Created on: Nov 1, 2012
 */


//Librerias
#include "McuInit.h"                /*CPU and System Clock related functions*/
#include "EmbeddedTypes.h"          /*Include special data types*/       
#include "SMAC_Interface.h"         /*Include all OTA functionality*/
#include "uMac.h"        			/*Include all OTA functionality*/
#include "app_config.h"
#include "OTAP_Interface.h"
#include "PLM_config.h"


#define MyID 0

//Definiciones
typedef enum{
	uMac_NoInit = 0,
	uMac_Init = 1,
	uMac_WaitRx = 2,
	uMac_Rx = 3,
	uMac_Tx = 4
} uMac_Engine_State;

channels_t       bestChannel;
bool_t           bScanDone;

static uMac_Engine_State uMac_Current_State;
static bool_t uMac_On = FALSE;
//static uint8_t uMac_Best_Channel;

//Variables
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
        
static txPacket_t * AppTxPacket;
static rxPacket_t * AppRxPacket;

static uMac_Packet * uMac_RxPacket;
static uMac_Packet * uMac_TxPacket;

bool_t			bTxDone;
bool_t			bRxDone;
bool_t			bDoTx;
bool_t			bToggle = TRUE;

uMac_nodeType uMactype;
uint8_t uMacdest = 1, i = 0;

uint16_t ChannelsToScan = 0xFFF;
uint8_t ChannelsEnergy[16];
channels_t Channels[] = {gChannel11_c, gChannel12_c, gChannel13_c, gChannel14_c, gChannel15_c,
			gChannel16_c, gChannel17_c, gChannel18_c, gChannel19_c, gChannel20_c, gChannel21_c,
			gChannel22_c, gChannel23_c, gChannel24_c, gChannel25_c, gChannel26_c};

//Prototipos
//void MLMEScanComfirm(channels_t ClearestChann); //Poner tambien
void InitSmac(void); //Poner en memoria bankeada******
void uMac_Txf(void);

void Init_uMac(/*uMac_nodeType type, uint8_t dest, uMac_txCallBack TxCallBack, uMac_rxCallBack RxCallBack*/) {
	uMac_On = TRUE;
	//uMactype = type;
	//uMacdest = dest;
	InitSmac();
	uMac_RxPacket = (uMac_Packet *)AppRxPacket->smacPdu.u8Data;
	uMac_TxPacket = (uMac_Packet *)AppTxPacket->smacPdu.u8Data;
	uMac_Current_State = uMac_NoInit;
	//guardar tipo de nodo y callbacks
}

void uMac_Txf() {
	bDoTx = TRUE;
}

void uMac_Engine(){	
	switch (uMac_Current_State) {
		case uMac_NoInit:
			if(uMac_On == TRUE) {
				uMac_On = FALSE;
				(void) MLMESetChannelRequest(Channels[i++]);
				uMac_TxPacket->Dest_Add = uMacdest;
				uMac_TxPacket->Packet_Type = 0;
				uMac_TxPacket->Pan_ID = 10;
				uMac_TxPacket->Source_Add = 0;
				//for (;;) 
				(void) MCPSDataRequest(AppTxPacket);
				//while (bTxDone != TRUE) {
				//	bTxDone = FALSE;
					uMac_Current_State = uMac_Init;
				//}
			}
			break;
		case uMac_Init:
			if (bTxDone == TRUE) {
				(void) MLMERXEnableRequest(AppRxPacket, 100); 
				uMac_Current_State = uMac_WaitRx;
			}
			break;
		case uMac_WaitRx:
				if(bRxDone == TRUE) {
					bRxDone = FALSE;
					//Analizar el paquete
					if (AppRxPacket->rxStatus == rxSuccessStatus_c) {
						if (uMac_RxPacket->Pan_ID == 10) {
							if (uMac_RxPacket->Dest_Add == MyID) {
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
																
								/*if (bToggle == TRUE) {
									Led_PrintValue(0x08);
									bToggle = FALSE;
								} else {
									Led_PrintValue(0x00);
									bToggle = TRUE;
								}*/
								if (uMac_RxPacket->Packet_Type != 0) {
									// Llamar a la callback
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
					// Deshabilitar la recepcion antes de transmitir (necesario por SMAC)
					(void) MLMERXDisableRequest();
					(void) MCPSDataRequest(AppTxPacket);
					uMac_Current_State = uMac_Tx;
				}
				
				break;
		case uMac_Tx:
			if(bTxDone == TRUE){
				bTxDone = FALSE;
				uMac_Current_State = uMac_Init;
				(void) MLMERXEnableRequest(AppRxPacket, 0);
			}
			break;
	}
}

/************************************************************************************
*
* InitSmac
*
************************************************************************************/
void InitSmac(void)
{
    AppTxPacket = (txPacket_t*)gau8TxDataBuffer;
    AppRxPacket = (rxPacket_t*)gau8RxDataBuffer; 
    AppRxPacket->u8MaxDataLength = gMaxSmacSDULenght_c;
    // AppTxPacket->smacPdu.u8Data[0] = 'T';
   /* */
    AppTxPacket->u8DataLength = 10;
    
    
    (void)MLMERadioInit();
    (void)MLMESetClockRate(gClko16MHz_c);
    (void)MCU_UseExternalClock();
    
    (void)MLMESetTmrPrescale(gTimeBase250kHz_c);
    while (gErrorNoError_c != MLMESetChannelRequest(gDefaultChannelNumber_c));
    (void)MLMEPAOutputAdjust(gDefaultOutputPower_c);
    (void)MLMEFEGainAdjust(gGainOffset_c);
 }

/* Place it in NON_BANKED memory */
#ifdef MEMORY_MODEL_BANKED
#pragma CODE_SEG __NEAR_SEG NON_BANKED
#else
#pragma CODE_SEG DEFAULT
#endif /* MEMORY_MODEL_BANKED */

/************************************************************************************
* MLMEScanComfirm
* 
*
*
************************************************************************************/
void MLMEScanComfirm(channels_t ClearestChann)
{
  bestChannel = ClearestChann; 
  bScanDone = TRUE;
}

/************************************************************************************
* MCPSDataIndication
* 
*
*
************************************************************************************/
void MCPSDataIndication(rxPacket_t *gsRxPacket)
{  
  //Otap_OpcMCPSDataIndication(gsRxPacket);
  bRxDone = TRUE;
}

/************************************************************************************
* MCPSDataComfirm
* 
*
*
************************************************************************************/
void MCPSDataComfirm(txStatus_t TransmissionResult)
{  
    //Otap_OpcMCPSDataComfirm(&TransmissionResult);
    bTxDone = TRUE;
}

void MLMEResetIndication(void)
{
  
}


void MLMEWakeComfirm(void)
{
  
}


#pragma CODE_SEG DEFAULT