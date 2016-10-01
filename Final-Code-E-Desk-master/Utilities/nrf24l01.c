/**
  ******************************************************************************
  * @file       nrf24l01.c
  * @author     
  * @version    
  * @date       20/03/2016
  ******************************************************************************
**/

#include "nrf24l01.h"
#include "tm_stm32f4_disco.h"
uint8_t dataOut[32], dataIn[32];
extern uint8_t a[32];
//uint8_t count24=0;
//int count_Fuzzy;
//uint8_t mLCD_LcdMode = 0;
uint8_t TxAddress[] = {
	0xE7,
	0xE7,
	0xE7,
	0xE7,
	0xE7
};
/* My address */
uint8_t MyAddress[] = {
	0x7E,
	0x7E,
	0x7E,
	0x7E,
	0x7E
};
uint8_t MyAddress_RX_ADDR_P0[5] = {0x7E,0x7E,0x7E,0x7E,0x7E};
uint8_t MyAddress_RX_ADDR_P1[5] = {0xC2,0xC2,0xC2,0xC2,0xC2};
uint8_t MyAddress_RX_ADDR_P2[5] = {0xC2,0xC2,0xC2,0xC2,0xC3};
uint8_t MyAddress_RX_ADDR_P3[1] = {0xC4};
uint8_t MyAddress_RX_ADDR_P4[1] = {0xC5};
uint8_t MyAddress_RX_ADDR_P5[1] = {0xC6};





TM_NRF24L01_Transmit_Status_t transmissionStatus;
void mRF_initRf(){
/* NRF24L01 goes to RX mode by default */
	TM_NRF24L01_Init(100, 32);
	
	/* Set RF settings, Data rate to 2Mbps, Output power to -18dBm */
	TM_NRF24L01_SetRF(TM_NRF24L01_DataRate_2M, TM_NRF24L01_OutputPower_0dBm);
	
	/*buoofn my address, 5 bytes */
	TM_NRF24L01_SetMyAddress(MyAddress);
//    TM_NRF24L01_SetMyAddress_Multiciver(
//    MyAddress_RX_ADDR_P0,
//    MyAddress_RX_ADDR_P1,
//    MyAddress_RX_ADDR_P2,
//    MyAddress_RX_ADDR_P3,
//    MyAddress_RX_ADDR_P4,
//    MyAddress_RX_ADDR_P5
//    );
	
	/* Set TX address, 5 bytes */
	TM_NRF24L01_SetTxAddress(TxAddress);
}

void mRF_receiveRF(NRF_ThoiTiet* NRF)
{
			TM_NRF24L01_GetData(dataIn);
			NRF->ApSuat = dataIn[0]<<24|dataIn[1]<<16|dataIn[2]<<8|(dataIn[3]);
			NRF->NhietDo = dataIn[4];
			NRF->DoAm = dataIn[5];
			NRF->Mua = dataIn[6];			
//			if(NRF->ApSuat!=0 && NRF->NhietDo!=0 && NRF->DoAm!=0 && NRF->Mua!=0)
//			{
//				count24=count24+1;
//				count_Fuzzy=1;				
//			}
//			else 
//			{
//				mLCD_LcdMode=3;
//				TM_DISCO_LedOn(LED_ORANGE);
//			}
}

void readRfData(NRF_ThoiTiet* NRF)
{
			TM_NRF24L01_GetData(dataIn);
			NRF->ApSuat = dataIn[0]<<24|dataIn[1]<<16|dataIn[2]<<8|(dataIn[3]);
			NRF->NhietDo = dataIn[4];
			NRF->DoAm = dataIn[5];
			NRF->Mua = dataIn[6];
	
}

int getApsuat(long pressure[], NRF_ThoiTiet NRF, int count24_, float decrease[],
              float percentPr[])
{
  if (count24_ == 0)
  {
    pressure[count24_] = NRF.ApSuat;
    decrease[23] = (pressure[count24_] - pressure[23]) * 1000;
    percentPr[23] = (decrease[23] / pressure[23]) * 100;
  }
  else
  {
    pressure[count24_] = NRF.ApSuat;
    decrease[count24_ - 1] =
        (pressure[count24_] - pressure[count24_ - 1]) * 1000;
    percentPr[count24_ - 1] =
        (decrease[count24_ - 1] / pressure[count24_ - 1]) * 100;
  }
  return 1;
}
