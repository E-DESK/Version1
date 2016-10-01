/**
******************************************************************************
*@Connect: 
DATA	-	A10
VCC		-	5V

******************************************************************************
*/



/*
*************************************************************************************************************************************
*															INCLUDED FILES															*
*************************************************************************************************************************************
*/
#include "DHT11.h"
#include "tm_stm32f4_delay.h"
/*
*************************************************************************************************************************************
*															PRIVATE DEFINE															*
*************************************************************************************************************************************
*/
/*
*************************************************************************************************************************************
*														 	DATA TYPE DEFINE															*
*************************************************************************************************************************************
*/
/*
*************************************************************************************************************************************
*													   		PRIVATE VARIABLES														*
*************************************************************************************************************************************
*/ 
GPIO_InitTypeDef  GPIO_InitStructure;

uint8_t dh_nhiet_do,dh_do_am;
/*
*************************************************************************************************************************************
*							  								LOCAL FUNCTIONS															*
*************************************************************************************************************************************
*/
//static	void DHT_Delay(uint32_t nCount)
//{
//	__IO uint32_t index = 0;
//	for(index = (33 * nCount); index != 0; index--);
//}
/*
*************************************************************************************************************************************
*															GLOBAL FUNCTIONS														*
*************************************************************************************************************************************
*/
uint8_t DHT_GetTemHumi(void)
{	
	//SystemCoreClockUpdate();	
	uint8_t buffer[5]={0,0,0,0,0};
	uint8_t ii,i,checksum; 
	//-------------------------------------------------------------
	TM_DELAY_Init();
	TM_DELAY_INT_InitTIM();
	//-------------------------------------------------------------
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DHT11Clock, ENABLE);
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
	//---------------------------------------------------------------
	GPIO_SetBits(GPIO_DHT11,GPIO_Pin_DHT11);
	Delay(40);
	GPIO_ResetBits(GPIO_DHT11,GPIO_Pin_DHT11);
	Delayms(25);
	GPIO_SetBits(GPIO_DHT11,GPIO_Pin_DHT11);
	//--------------------------------------------------------------
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_DHT11;
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_NOPULL;
	GPIO_Init(GPIO_DHT11,&GPIO_InitStructure);
	Delay(40);
	if(GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11)==1)
	return DHT_ER ;
	else
		{
			Delay(80);
			if(GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11)==0)
			return DHT_ER ; 
			else 
			{
				Delay(80);
				if(GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11)==1)
				return DHT_ER;
				else 
				{
						for(i=0;i<5;i++) 
						{ 
							for(ii=0;ii<8;ii++) 
								{    
									while(!(GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11)));//Doi A10 len 1 
									Delay(50); 
									if((GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11))) 
										{ 
											buffer[i]|=(1<<(7-ii)); 
											while((GPIO_ReadInputDataBit(GPIO_DHT11,GPIO_Pin_DHT11)));//Doi A10 xuong 0 
										} 
								} 
							} 
						 checksum=buffer[0]+buffer[1]+buffer[2]+buffer[3]; 
							//Kiem tra check sum 
						if((checksum)!=buffer[4]) return DHT_ER ;
						//Lay du lieu 
						dh_nhiet_do 	=   buffer[2];
						dh_do_am		=   buffer[0];
						return DHT_OK;				
			}		
		}	
	}				
}

uint8_t DHT_nhietdo()
{
	return dh_nhiet_do;
}

uint8_t DHT_doam()
{
return dh_do_am;
}

//============================================================================
