#include <Stdio.h>
//#include "nrf24l01.h"
typedef struct forecast
{
	float rateRain;
	float decreasePres;
	float timeForecast;
	// rain forecast: 2 Sunny, 1 Rain, 0 Heavy Rain
	int rainForecast;
	
	//------------
	int countPres;
	int statePresure;
	int currentState;
	int preState;
	int currentIndex;
}Forecast;
/**
 * This function use to calculate rateRain
 * @param  output   [description]
 * @param  forecast [description]
 * @return          [description]
 */
int rainForecast(float output,Forecast *forecast);

int calculateTime(Forecast *forecast);

