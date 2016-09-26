/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    04-September-2015
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
/*======================================
=            Include module            =
======================================*/
/*----------  ST Library  ----------*/
#include "stm32f4xx.h"
#include "integer.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "tm_stm32f4_disco.h"
#include "tm_stm32f4_usart.h"
/*----------  FreeRTOS  ----------*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
/*----------  RTC  ----------*/
#include "tm_stm32f4_ds1307.h"
/*----------  LCD  ----------*/
#include "SSD1289.h"
/*----------  TFT  ----------*/
#include "TFT.h"
/*----------  BH1750  ----------*/
#include "I2C1.h"
#include "BH1750.h"
/*----------  DHT11  ----------*/
#include "DHT11.h"
/*----------  ADC  ----------*/
#include "tm_stm32f4_adc.h"
/*=====  End of Include module  ======*/
#include <math.h>
#define __cplusplus

TM_DS1307_Time_t time;
uint8_t Time_Start = 0;
int Time_Write = 0;
uint8_t count24=0;
int temp;
char* date[8] = {"","Monday", "Tuesday",  "Wednesday","Thursday", "Friday", "Saturday", "Sunday"};
char* Month[13] = {"","January","February","March","April","May","June","July","August", "September", "October", "November", "December"};
char* Mua[3] = {"Heavy","Rain","Sunny"};
char s01[300];
char s02[300];
char s03[300];
char s04[300];
char s05[300];
char s06[300];
char s07[300];
char s08[300];
char* chuoi[8]={s01,s02,s03,s04,s05,s06,s07,s08};
int change=1;
int dem =-1;
uint8_t mLCD_LcdMode = 0;

struct Environment
{
	uint8_t NhietDo;
	uint32_t AnhSang;
	uint8_t DoAmKhi;
};
typedef struct Environment Env;

struct UserData_Struct
{
    float distance;
    uint16_t time;
};
typedef struct UserData_Struct USERDATA_TYPE;

int readRf(void);
int showTime(void);

int mLCD_resetLcd(void);
int mLCD_showTitle(void);

int sendThSp(void);
/*declarece*/
int initMain(int flag);
int initTask(void);
char sbuff[300];
int ResetWeather(WeatherForecast weatherForecast);
float pushPresDownArray(float pushValue);
uint8_t LCD_showDateTime(uint8_t mode);
int LCD_showUpdate(int mode);
int mLCD_storeChuoi(void);

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
