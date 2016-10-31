#include "BH1750.h"

uint8_t buff[2];

void BH1750_Init(){
    TM_I2C_Start(I2C1,BH1750Address,I2C_Direction_Transmitter,1);
    TM_I2C_WriteData(I2C1,16);
    TM_I2C_Stop(I2C1);
}

uint16_t BH1750_Read(){
	uint16_t val=0;
    TM_I2C_Start(I2C1,BH1750Address,I2C_Direction_Receiver,1);    
    buff[0] = TM_I2C_ReadAck(I2C1);
    buff[1] = TM_I2C_ReadNack(I2C1);
    TM_I2C_Stop(I2C1);
	val=((buff[0]<<8)|buff[1])/1.2;
	return val;
}

void delay_ms( uint32_t _time ){
	_time = _time * 420;
	while( _time-- ){
	}
}
