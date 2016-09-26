#include "Forecast.h"


int rainForecast(float output,Forecast *forecast)
{
  forecast->rateRain = output *100;
  if (output < 0.5)
  {
    /* code */
    forecast->rainForecast = 2;
  }
  else if (output >= 50 && output <= 60)
  {
    /* code */
    forecast->rainForecast = 1;
  }
  else if (output > 60)
  {
    forecast->rainForecast = 0;
  }
  else
  {
    forecast->rainForecast = 2;
  }
  return 1;
}

int calculateTime(Forecast *forecast)
{
  int temp =0;
  temp = (200-(forecast->decreasePres))/200;
  forecast->timeForecast = temp;
	return 1;
}
