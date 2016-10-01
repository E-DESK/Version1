//--------------------------------------------------------------
// File     : BH1750.h
//--------------------------------------------------------------
#ifndef __BH1750_H
#define __BH1750_H
//--------------------------------------------------------------
// Includes
//--------------------------------------------------------------
#include "I2C1.h"

#define Device_Address_L 	0x46 // Device address when address pin LOW
#define Device_Address_H 	0x5C // Device address when address pin LOW
//all command here taken from Data sheet OPECODE Table page 5
#define Power_Down			 	0x00
#define Power_On					0x01
#define reset 						0x07
#define Continuous_H_resolution_Mode  	0x10
#define Continuous_H_resolution_Mode2  	0x11
#define Continuous_L_resolution_Mode  	0x13
#define OneTime_H_resolution_Mode  			0x20
#define OneTime_H_resolution_Mode2  		0x21
#define OneTime_L_resolution_Mode  			0x23//As well as address value
//--------------------------------------------------------------
// Define
//--------------------------------------------------------------
#define AddrPin GPIO_Pin_0 // Address pin enable
//--------------------------------------------------------------
// Variables
//--------------------------------------------------------------
#define BH1750Address Device_Address_L
//--------------------------------------------------------------
// Global Function
//--------------------------------------------------------------
void BH1750_Init(void);
uint16_t BH1750_Read(void);
void delay_ms( uint32_t _time );
//--------------------------------------------------------------
#endif // __BH1750_H
