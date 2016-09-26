#include "TFT.h"
void write(int dem,char* chuoi[],int change,char* date[],char* Month[],TM_DS1307_Time_t time,char sbuff[])
{
	  LCD_Clear(BLACK);
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(BLUE);
    LCD_CharSize(16);
    LCD_StringLine(14,290,(unsigned char*)"UNIVERSITY OF INFOMATION TECHNOLOGY");
    LCD_StringLine(28,275,(unsigned char*)"FACULTY OF COMPUTER ENGINEERING");
    LCD_SetTextColor(RED);
    LCD_StringLine(42,240,(unsigned char*)"LOCAL WEATHER FORECAST");
    sprintf(sbuff, "%s,%d,%s,%d", date[time.day], time.date, Month[time.month],time.year + 2000);
    LCD_StringLine(86, 230, (uint8_t *)sbuff);
		LCD_SetTextColor(YELLOW);
	if(change==1)
	{
			switch (dem)
			{
				case 0:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					break;
				}
				case 1:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);

					break;
				}
				case 2:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);

					break;
				}
				case 3:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
					LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);		
					break;
				}
				case 4:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
					LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
					LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
					break;
				}
				case 5:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
					LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
					LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
					LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
					break;
				}
				case 6:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
					LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
					LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
					LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
					LCD_StringLine(200,310,(uint8_t *)chuoi[dem-6]);				
					break;
				}
					case 7:
				{
					LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
					LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
					LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
					LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
					LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
					LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
					LCD_StringLine(200,310,(uint8_t *)chuoi[dem-6]);
					LCD_StringLine(216,310,(uint8_t *)chuoi[dem-7]);
					break;
				}
					default :
					{
						break;
					}
			}	
	}else
	{		switch(dem)
		{
		case 0:
		{
			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem+7]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem+6]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem+5]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem+4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem+3]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
			case 1:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem+6]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem+5]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem+4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem+3]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
			case 2:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem+5]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem+4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem+3]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
				case 3:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem+4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem+3]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
				case 4:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem+3]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
				case 5:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem+2]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
				case 6:
		{

			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem-6]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem+1]);
			break;
		}
				case 7:
		{
			LCD_StringLine(104,310,(uint8_t *)chuoi[dem]);
			LCD_StringLine(120,310,(uint8_t *)chuoi[dem-1]);
			LCD_StringLine(136,310,(uint8_t *)chuoi[dem-2]);
			LCD_StringLine(152,310,(uint8_t *)chuoi[dem-3]);
			LCD_StringLine(168,310,(uint8_t *)chuoi[dem-4]);
			LCD_StringLine(184,310,(uint8_t *)chuoi[dem-5]);
			LCD_StringLine(200,310,(uint8_t *)chuoi[dem-6]);
			LCD_StringLine(216,310,(uint8_t *)chuoi[dem-7]);
			break;
		}
				default:
		{
			break;
		}
	}
}
    }
