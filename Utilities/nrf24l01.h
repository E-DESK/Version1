/**
  ******************************************************************************
  * @file       nrf24l01.h
  * @author     LinhKien69
  * @version    V1.0.0
  * @date       28/02/2015
  ******************************************************************************
**/

#ifndef NRF024L01_
#define NRF024L01_
#include "stm32f4xx.h"
/* Include my libraries here */
#include "defines.h"
#include "tm_stm32f4_nrf24l01.h"
#include "tm_stm32f4_delay.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//extern uint8_t count24;
//extern uint8_t mLCD_LcdMode;
typedef struct 
{
    uint8_t NhietDo; 
    uint8_t DoAm; 
    long ApSuat;   
    uint8_t Mua;    
} NRF_ThoiTiet;

void mRF_initRf(void);

void mRF_receiveRF(NRF_ThoiTiet* NRF);
int getApsuat(long pressure[],NRF_ThoiTiet NRF,int i,float deregre[],float percentPr[]);
#endif
