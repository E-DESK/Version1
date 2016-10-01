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
#include <string.h>
/*----------  FreeRTOS  ----------*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
/*----------  RTC  ----------*/
#include "tm_stm32f4_ds1307.h"
/*----------  RF  ----------*/
//#include "esp8266.h"
/*----------  LCD  ----------*/
#include "SSD1289.h"
/*----------  Fuzzy  ----------*/
#include "Fuzzy.h"
/*=====  End of Include module  ======*/
/****************************************************************************/

typedef struct struct_MyDateTime
{
	int day;
	int month;
	int year;
	int hour;
	int minute;

}MyDateTime;

typedef struct 
{
    uint8_t Temp; 
    uint8_t Humi; 
    long Pres;   
    uint8_t Rain;    
} WeatherRoot;

typedef struct
{
    MyDateTime currentDateTime;
    WeatherRoot currentWeather[5];
    float presDown;
    float rateInYear;
    float rateInDay;
    double output;
    double totalDecrease;
    char *strWeather;
    char *strForWeather;
    float ratio;
}WeatherForecast;
/****************************************************************************/
void write(int dem,char* chuoi[],int change,char* date[],char* Month[],TM_DS1307_Time_t time,char sbuff[]);
