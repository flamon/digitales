/*
 * uMac.c
 *
 *  Created on: Nov 1, 2012
 */


//Librerias
#include "McuInit.h"                /*CPU and System Clock related functions*/
#include "EmbeddedTypes.h"          /*Include special data types*/       
#include "SMAC_Interface.h"         /*Include all OTA functionality*/
#include "uMac.h"        			 /*Include all OTA functionality*/
#include "app_config.h"


//Definiciones
typedef enum{
	uMac_NoInit = 0,
	uMac_Init = 1,
	uMac_WaitRx = 2,
	uMac_Rx = 3,
	uMac_Tx = 4
}uMac_Engine_State;

channels_t       bestChannel;
bool_t           bScanDone;

static uMac_Engine_State uMac_Current_State;
static bool_t uMac_On = FALSE;
static uint8_t uMac_Best_Channel;

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

//Prototipos
//void MLMEScanComfirm(channels_t ClearestChann); //Poner tambien
void InitSmac(void); //Poner en memoria bankeada******

void Init_uMac(uMac_nodeType NodeType,uMac_txCallBack TxCallBack,uMac_rxCallBack RxCallBack){
	uMac_On = TRUE;
	uMac_RxPacket = (uMac_Packet *)AppRxPacket->smacPdu.u8Data;
	uMac_TxPacket = (uMac_Packet *)AppTxPacket->smacPdu.u8Data;
	//guardar tipo de nodo y callbacks
}

void uMac_Engine(){
	uint16_t ChannelsToScan = 0xFFF;
	uint8_t TheBestChannel[16];
	rxPacket_t ReceivedPacket;
	uMac_Packet HelloResponse;
	switch (uMac_Current_State){
		case uMac_NoInit:
			//No Init
			if(uMac_On==TRUE){
				uMac_On = FALSE;
				InitSmac();
				(void) MLMEScanRequest(ChannelsToScan, gScanModeED_c, TheBestChannel);
				uMac_Current_State = uMac_Init;
			}
		break;
		case uMac_Init:
			//Init
			if(bScanDone==TRUE){
				bScanDone = FALSE;
				(void) MLMESetChannelRequest(bestChannel);
				(void) MLMERXEnableRequest(AppRxPacket,0); 
				uMac_Current_State = uMac_WaitRx;
			}
		break;
		case uMac_WaitRx:
			//Wait Rx
			if(bRxDone==TRUE){
				bRxDone = FALSE;
				//Analizar el paquete
				if(AppRxPacket->rxStatus==rxSuccessStatus_c){
					//El paquete esta chido
					uMac_RxPacket = (uMac_Packet *)&AppRxPacket->smacPdu.u8Data;
					if(uMac_RxPacket->Packet_Type==uMac_Hello){
						//Si el paquete es de control, responder con otro hello
						uMac_TxPacket->Dest_Add = uMac_RxPacket->Source_Add;
						uMac_TxPacket->Packet_Type = uMac_Hello;
						uMac_TxPacket->Pan_ID = 44;
						uMac_TxPacket->Source_Add = 0;
						uMac_TxPacket->uMac_PDU[0] = 'H';
						// AppTxPacket->smacPdu.u8Data = HelloResponse;
					}
				}
			}
			
		break;
		case uMac_Rx:
			//Rx
		break;
		case uMac_Tx:
			if(bRxDone==TRUE){
				uMac_Current_State = uMac_WaitRx;
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
    AppTxPacket->smacPdu.u8Data[0] = 'T';
    
    
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