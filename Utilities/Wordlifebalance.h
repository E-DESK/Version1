#include <stdio.h>
float calTermPer(float temp)
{
	float percent = 0;
	percent = (float)((float)0.1647524*temp - (float)0.0058274*temp*temp + (float)0.0000623*temp*temp*temp - (float)0.4685328) * (float)100;
	if (percent > 100)
	{
		percent = 100;
	}
	else if (percent < 0)
	{
		percent = 0;
	}
	return percent;
	return percent;
}
/*L=
0 						if lux<250
(0.016lux+62.5)*1.4    if lux≤500
(-0.012lux+52.5)*1.4   if lux>500)
*/
float calLightPer(int light)
{
	float percent = 0;
	if (light < 250)
	{
		percent = 0;
	}
	else if (light <= 500)
	{
		percent = (float)(((float)0.016*light + (float)62.5)*(float)1.4);
		if (percent > 100)
		{
			percent = 100;
		}
		else if (percent < 0)
		{
			percent = 0;
		}
	}
	else if (light > 500)
	{
		percent = (float)(((float)-0.012 *light + (float)52.5)*(float)1.4);
		if (percent > 100)
		{
			percent = 100;
		}
		else if (percent < 0)
		{
			percent = 0;
		}
	}
	else
	{
		percent = 0;
	}
	return percent;
}

float calEnviPer(float temp, float light)
	{
		float percent = 0;
		float lightP = (float)calLightPer(light);
		float  tempP = (float)calTermPer(temp);
		percent = (float)((float)0.2*tempP + (float)0.8*lightP);
		if (percent > 100)
		{
			percent = 100;
		}
		else if (percent < 0)
		{
			percent = 0;
		}
		return percent;
	}



