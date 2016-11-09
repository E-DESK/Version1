#include <stdio.h>
#include <iostream>
using namespace std;
#define MAX(a,b) a>b?a:b
/*====================================================================================================================*/
/*                                       INTERNAL FUNCTIONS                                                           */
/*====================================================================================================================*/
float triangle(float u, float w, float c)

/* Determine degree of membership for a TRIANGLE membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return MAX(0, (1 - (u - c) / w));
	else
		return MAX(0, (1 - (c - u) / w));
}
float leftall(float u, float w, float c)
/* Determine degree of membership for a leftall membership function.
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u < c)
		return 1.0;
	else
		return MAX(0, (1 - (u - c) / w));
}
float rightall(float u, float w, float c)
/* Determine degree of membership for a RIGHTALL membership function
NOTE:  u is input, c is mem. fun. center, and w is mem. fun. width. */

{
	if (u >= c)
		return 1.0;
	else
		return MAX(0, (1 - (c - u) / w));
}



float calTermPer(float temp)
{
	float percent = 0;
	percent = (float)(0.1647524*temp - 0.0058274*temp*temp + 0.0000623*temp*temp*temp - 0.4685328) * 100;
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
(0.016lux+62.5)*1.4		if lux≤500
(-0.012lux+52.5)*1.4	if lux>500)
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

float calDconPer(float cTime)
{
	float percent = 0;
	if (cTime < 0)
		cTime = 0;
	else if (cTime > 10000)
		cTime = 10000;
	if (cTime <= 30)
	{
		percent = rightall(cTime, 30, 30);
	}
	else
	{
		percent = leftall(cTime, 90, 30);
	}
	return percent;
}
/*====================================================================================================================*/
/*                                       EXTERNAL FUNCTIONS                                                           */
/*====================================================================================================================*/

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



float calWorkPer(float cTime, float dTime)
{
	return (calDconPer(cTime) + triangle(dTime / 60, 8, 8))*50;
}

float trungbinhcongthem(float tb_sanco, float gt_congthem)
{
	float temp = 0;
	static int dem;
	if (tb_sanco == 0)
		dem = 0;
	temp = ((dem*tb_sanco) + gt_congthem) / (dem + 1);
	dem++;
	return temp;
}

//int main()
//{
//	float a;
//	static int dem = 0;
//	float trungbinhcongdon = 0;
//	while (1)
//	{
//		cout << "Nhap vao gia tri :";
//		cin >> a;
//		trungbinhcongdon = trungbinhcongthem(trungbinhcongdon, a);

//		cout << "Trung binh cong don : " << trungbinhcongdon;
//			system("pause");
//	}
//	//float light = 600;
//	//float temp = 22;
//	//float CTime = 0;
//	//float DTime = 0;
//	//float realDconPer = 0;

//	//cout << "TempPercent" << calTermPer(temp) << endl;
//	//cout << "LightPercent" << calLightPer(light) << endl;
//	//cout << "Percent" << calEnviPer(temp, light) << endl;
//	//while (1)
//	//{
//	//	realDconPer = trungbinhcongthem(realDconPer, count);
//	//	CTime += 10;
//	//	DTime += 10;
//	//	cout << "Ctime :" << CTime << "=>Percent CTime :" << calDconPer(CTime) * 100 << endl;
//	//	cout << "Dtime :" << DTime << "=>Percent DTime :" << triangle(DTime / 60, 8, 8) * 100 << endl;
//	//	cout << "Percent WTime :" << calWorkPer(CTime, DTime) << endl<<endl;

//	//	system("pause");
//	//}
}



