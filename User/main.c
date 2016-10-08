#include "main.h"
#define __cplusplus
/*PID*/
#define PID_PARAM_KP        100         /* Proporcional */
#define PID_PARAM_KI        0.025       /* Integral */
#define PID_PARAM_KD        20          /* Derivative */
#define LIGHT_CURRENT       lights[1]
#define LIGHT_WANT          lights[0]
#define LIGHT_WANT_VALUE    150
float lights[2];
float lightWantValue = LIGHT_WANT_VALUE;
/*
RTOS TASK
*/
#define STACK_SIZE_MIN 128 /* usStackDepth	- the stack size DEFINED IN \
                              WORDS.*/
uint8_t light;
/*xTaskHandle*/
xTaskHandle ptr_readSensor;
xTaskHandle ptr_showLCD;
xTaskHandle ptr_response;
/**/
static void vDA2_ReadSensor(void *pvParameters);
static void vDA2_ShowLCD   (void *pvParameters);
static void vDA2_Response  (void *pvParemeters);

/*LCD functions*/
int mLCD_resetLcd(void);
int mLCD_showTitle(void);
int mLCD_showDateTime(void);
int mLCD_showSensor(void);
//char reminder[100]={0};
char *reminder;

void TM_TIMER_Init(void) {
	TIM_TimeBaseInitTypeDef TIM_BaseStruct;
	
	/* Enable clock for TIM4 */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
/*	
	TIM4 is connected to APB1 bus, which has on F407 device 42MHz clock 				
	But, timer has internal PLL, which double this frequency for timer, up to 84MHz 	
	Remember: Not each timer is connected to APB1, there are also timers connected 	
	on APB2, which works at 84MHz by default, and internal PLL increase 				
	this to up to 168MHz 															
	
	Set timer prescaller 
	Timer count frequency is set with 
	
	timer_tick_frequency = Timer_default_frequency / (prescaller_set + 1)		
	
	In our case, we want a max frequency for timer, so we set prescaller to 0 		
	And our timer will have tick frequency		
	
	timer_tick_frequency = 84000000 / (0 + 1) = 84000000 
*/	
	TIM_BaseStruct.TIM_Prescaler = 0;
	/* Count up */
    TIM_BaseStruct.TIM_CounterMode = TIM_CounterMode_Up;
/*
	Set timer period when it have reset
	First you have to know max value for timer
	In our case it is 16bit = 65535
	To get your frequency for PWM, equation is simple
	
	PWM_frequency = timer_tick_frequency / (TIM_Period + 1)
	
	If you know your PWM frequency you want to have timer period set correct
	
	TIM_Period = timer_tick_frequency / PWM_frequency - 1
	
	In our case, for 10Khz PWM_frequency, set Period to
	
	TIM_Period = 84000000 / 10000 - 1 = 8399
	
	If you get TIM_Period larger than max timer value (in our case 65535),
	you have to choose larger prescaler and slow down timer tick frequency
*/
    TIM_BaseStruct.TIM_Period = 8399; /* 10kHz PWM */
    TIM_BaseStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_BaseStruct.TIM_RepetitionCounter = 0;
	/* Initialize TIM4 */
    TIM_TimeBaseInit(TIM2, &TIM_BaseStruct);
	/* Start count on TIM4 */
    TIM_Cmd(TIM2, ENABLE);
}

void TM_PWM_Init(void) {
	TIM_OCInitTypeDef TIM_OCStruct;
	
	/* Common settings */
	
	/* PWM mode 2 = Clear on compare match */
	/* PWM mode 1 = Set on compare match */
	TIM_OCStruct.TIM_OCMode = TIM_OCMode_PWM2;
	TIM_OCStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCStruct.TIM_OCPolarity = TIM_OCPolarity_Low;
	
/*
	To get proper duty cycle, you have simple equation
	
	pulse_length = ((TIM_Period + 1) * DutyCycle) / 100 - 1
	
	where DutyCycle is in percent, between 0 and 100%
	
	25% duty cycle: 	pulse_length = ((8399 + 1) * 25) / 100 - 1 = 2099
	50% duty cycle: 	pulse_length = ((8399 + 1) * 50) / 100 - 1 = 4199
	75% duty cycle: 	pulse_length = ((8399 + 1) * 75) / 100 - 1 = 6299
	100% duty cycle:	pulse_length = ((8399 + 1) * 100) / 100 - 1 = 8399
	
	Remember: if pulse_length is larger than TIM_Period, you will have output HIGH all the time
*/
	TIM_OCStruct.TIM_Pulse = 2099; /* 25% duty cycle */
	TIM_OC1Init(TIM4, &TIM_OCStruct);
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 4199; /* 50% duty cycle */
	TIM_OC2Init(TIM4, &TIM_OCStruct);
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 6299; /* 75% duty cycle */
	TIM_OC3Init(TIM4, &TIM_OCStruct);
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);
	
	TIM_OCStruct.TIM_Pulse = 8399; /* 100% duty cycle */
	TIM_OC4Init(TIM4, &TIM_OCStruct);
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);
}

void TM_LEDS_Init(void) {
	GPIO_InitTypeDef GPIO_InitStruct;
	
	/* Clock for GPIOD */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Alternating functions for pins */
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_TIM2);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_TIM4);
//	GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_TIM4);
	
	/* Set pins */
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOA, &GPIO_InitStruct);
}

long getCompare(TM_DS1307_Time_t time)
{
    long temp =0;
    temp = (time.seconds)&(time.minutes<<2)&(time.hours<<4)&(time.date<<6)
    &(time.month<<8)&(time.year<<10);
    return temp;
}

int compareTime(TM_DS1307_Time_t time1, TM_DS1307_Time_t time2)
{
	long arr1[2] = { 0 };
	long arr2[2] = { 0 };
	arr1[0] = (time1.date) | (time1.month << 8) | (time1.year << 16);
	arr2[0] = (time2.date) | (time2.month << 8) | (time2.year << 16);
	if (arr1[0] > arr2[0])
		return 1;
	else if (arr1[0] < arr2[0])
		return -1;
	else
	{
		arr1[1] = (time1.minutes) | (time1.hours << 8);
		arr2[1] = (time2.minutes) | (time2.hours << 8);
		if (arr1[1] > arr2[1])
			return 1;
		else if (arr1[1] < arr2[1])
			return -1;
		else
			return 0;
	}
}
/*=============================DEFINE_REMINDER_TYPE===========================*/
typedef struct STRUCT_OF_REMINDER
{
    TM_DS1307_Time_t reminderTime;
    char *reminderText;
}REMINDER;

REMINDER newREMINDER(int minute, int hours, int date, int month, int year, char *text)
{
    REMINDER temp;
    temp.reminderTime.seconds = 0;
    temp.reminderTime.minutes = minute;
    temp.reminderTime.hours = hours;
    temp.reminderTime.date = date;
    temp.reminderTime.month = month;
    temp.reminderTime.year = year;
    temp.reminderText = text;
}
/*===============================LINK_LIST_REFERENCES=========================*/
/*CREATE_LINK_LIST_NODE_AND_DEFINE*/
struct node
{
    REMINDER reminder;
    struct node *next;
};
struct node* head = NULL;
struct node* current = NULL;

/*INSERT_NEW_NODE*/
void insertFirst(REMINDER _reminder)
{
    //create a link
    struct node *link = (struct node*) malloc(sizeof(struct node));

    link->reminder = _reminder;

    //point it to old first node
    link->next = head;

    //point first to new first node
    head = link;
}
/*DELETE_NODE*/
struct node* deleteFirst()
{
    //save reference to first link
    struct node *tempLink = head;

    //mark next to first link as first 
    head = head->next;

    tempLink->next = NULL;

    //return the deleted link
    return tempLink;
}
/*CHECK_NUMBER_OF_NODE*/
int length()
{
    int lenght =0;
    struct node * _current;
    for(_current = head; _current != NULL; _current = current->next)
    {
        lenght++;
    }
    return lenght;
}

/*SORT_LIST_OF_NODE*/
void sortList()
{
    int i, j, k;
    REMINDER tempData;
    struct node *current;
    struct node *next;

    int size = length();
    k = size;

    for (i = 0; i < size - 1; i++, k--)
    {
        current = head;
        long currentCompare;
        long nextCompare;
        next = head->next;
        for (j = 1; j < k; j++)
        {
            if (compareTime(current->reminder.reminderTime,next->reminder.reminderTime)==1)
            {
                tempData.reminderText = current->reminder.reminderText;
                current->reminder.reminderText = next->reminder.reminderText;
                next->reminder.reminderText = tempData.reminderText;

                tempData.reminderTime.minutes = current->reminder.reminderTime.minutes;
                current->reminder.reminderTime.minutes = next->reminder.reminderTime.minutes;
                next->reminder.reminderTime.minutes = tempData.reminderTime.minutes;

                tempData.reminderTime.hours = current->reminder.reminderTime.hours;
                current->reminder.reminderTime.hours = next->reminder.reminderTime.hours;
                next->reminder.reminderTime.hours = tempData.reminderTime.hours;

                tempData.reminderTime.date = current->reminder.reminderTime.date;
                current->reminder.reminderTime.date = next->reminder.reminderTime.date;
                next->reminder.reminderTime.date = tempData.reminderTime.date;

                tempData.reminderTime.month = current->reminder.reminderTime.month;
                current->reminder.reminderTime.month = next->reminder.reminderTime.month;
                next->reminder.reminderTime.month = tempData.reminderTime.month;

                tempData.reminderTime.year = current->reminder.reminderTime.year;
                current->reminder.reminderTime.year = next->reminder.reminderTime.year;
                next->reminder.reminderTime.year = tempData.reminderTime.year;
            }
            current = current->next;
            next = next->next;
        }
    }
}
/*FIND_NEW_NODE_POSITION*/
/*PRINT_NODE*/
/*CHECK_LIST_FUNCTIONS*/
bool isListRemindEmpty()
{
   return head == NULL;
}

/*==============================END_LINK_LIST=================================*/
/**/
float getDistance(uint16_t voltaValue);
REMINDER parsingLine(char* inputString);

Env InRoomEvn;
USERDATA_TYPE userData;



REMINDER userNotes[10];
int reminderIndex=0;
/*Global PWM PID*/
/* Timer data for PWM */
TM_PWM_TIM_t TIM_Data;
char buf[150];
uint8_t devices, i, count;
/* PID error */
float pid_error;
/* Duty cycle for PWM */
float duty = 100;
float currentDuty =0;
/* ARM PID Instance, float_32 format */
arm_pid_instance_f32 PID;

int main() 
{
    initMain(1);
    if(initTask())
    {
        vTaskStartScheduler();
    }
    for(;;);
}

int initMain(int flag)
{
    if(flag)
    {
        SystemInit();

        TM_DISCO_LedInit();
        TM_DISCO_ButtonInit();
        TM_DELAY_Init();
        /*TEST SORT LINK LIST*/
//        insertFirst(newREMINDER(12,30,12,10,2016,"an com"));
//        insertFirst(newREMINDER(12,45,12,10,2016,"di tam"));
//        insertFirst(newREMINDER(10,30,12,10,2016,"mua com"));
//        insertFirst(newREMINDER(6,30,12,10,2016,"thuc day"));
//        insertFirst(newREMINDER(7,30,12,10,2016,"di hoc"));
//        insertFirst(newREMINDER(11,30,14,10,2016,"di test"));
//        sortList();
        
        LCD_Init();

        I2C1_Init();
        /*I2C1_PINS PACK 2
        SCL=PB8_SDA=PB9  */
        BH1750_Init();

        DHT_GetTemHumi();
        
        /*Sensor khoang cach*/
        /*ADC: Pin_A0 (ADC1 channel 0)*/
        TM_ADC_Init(ADC1,ADC_Channel_0);

        mLCD_resetLcd();
        LCD_Clear(BLACK);
        mLCD_showTitle();
        
        TM_USART_Init(USART6, TM_USART_PinsPack_1, 9600);
        TM_USART_SetCustomStringEndCharacter(USART6,'.');
        
        /*I2C2_PINS PACK 1
        SCL=PB10_SDA=PB11  */
        if (TM_DS1307_Init() != TM_DS1307_Result_Ok)
        {
            /*Warning that RTC error*/
            TM_DISCO_LedOn(LED_ALL);
        }
            
        if(1)
        {
            /* Set date and time */
            /* Day 7, 26th May 2014, 02:05:00 */
            time.hours = 15;
            time.minutes = 33;
            time.seconds = 20;
            time.date = 2;
            time.day = 8;
            time.month = 10;
            time.year = 16;
            TM_DS1307_SetDateTime(&time);

            /* Disable output first */
            TM_DS1307_DisableOutputPin();

            /* Set output pin to 4096 Hz */
            TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_4096Hz);
        }
        
        TM_DS1307_GetDateTime(&time);
            
        mLCD_resetLcd();
        
        InRoomEvn.AnhSang = 0;
        InRoomEvn.DoAmKhi = 0;
        InRoomEvn.DoAmKhi = 0;
        
        /*PWM and PID*/
        if(1)
        {

        /* Set PID parameters */
        /* Set this for your needs */
        PID.Kp = PID_PARAM_KP;		/* Proporcional */
        PID.Ki = PID_PARAM_KI;		/* Integral */
        PID.Kd = PID_PARAM_KD;		/* Derivative */

        /* Initialize PID system, float32_t format */
        arm_pid_init_f32(&PID, 1);
            
        /* Initialize TIM2, 1kHz frequency */
                     /* Init leds */
        TM_LEDS_Init();
        /* Init timer */
        TM_TIMER_Init();
        /* Init PWM */
        TM_PWM_Init();
        TM_PWM_InitTimer(TIM2, &TIM_Data, 10000);

        /* Initialize TIM2, Channel 1, PinsPack 2 = PA5 */
        TM_PWM_InitChannel(TIM2, TM_PWM_Channel_1, TM_PWM_PinsPack_2);

        /* Set default duty cycle */
        TM_PWM_SetChannelPercent(TIM2, &TIM_Data, TM_PWM_Channel_1, duty);
        //TM_PWM_SetChannelMicros(TIM2, &TIM_Data, TM_PWM_Channel_1, duty);
        }
    }
    return 1;
}

int initTask()
{
    xTaskCreate(vDA2_ReadSensor,(const signed char*)"vDA2_ReadSensor",STACK_SIZE_MIN,NULL,tskIDLE_PRIORITY+3,&ptr_readSensor);
    xTaskCreate(vDA2_ShowLCD, (const signed char *)"vDA2_ShowLCD", STACK_SIZE_MIN,NULL, tskIDLE_PRIORITY+1,&ptr_showLCD);
    xTaskCreate(vDA2_Response, (const signed char *)"vDA2_Response", STACK_SIZE_MIN,NULL, tskIDLE_PRIORITY+1,&ptr_response);
    return 1;
}

static void vDA2_Response(void *pvParameters)
{
    for(;;)
    {
        /* Calculate error */
        LIGHT_CURRENT = InRoomEvn.AnhSang;
        LIGHT_WANT = lightWantValue;
        pid_error = LIGHT_WANT - LIGHT_CURRENT;
//        if(LIGHT_CURRENT >= LIGHT_WANT)
//        {
//            pid_error = LIGHT_CURRENT - LIGHT_WANT;
//        }
//        else if(LIGHT_CURRENT >= LIGHT_WANT)
//        {
//            pid_error = LIGHT_WANT - LIGHT_CURRENT;
//        }
        /* Calculate PID here, argument is error */
        /* Output data will be returned, we will use it as duty cycle parameter */
        //currentDuty = duty;
        duty = arm_pid_f32(&PID, pid_error);

        /* Check overflow, duty cycle in percent */
        if (duty > 100) {
        duty = 100;
        } else if (duty < 0) {
        duty = 0;
        }
        TM_PWM_SetChannelPercent(TIM2, &TIM_Data, TM_PWM_Channel_1, duty);
        /*
        if(pid_error >= 0)
        {
            TM_PWM_SetChannelPercent(TIM2, &TIM_Data, TM_PWM_Channel_1, currentDuty - duty);
        }
        else if(pid_error < 0)
        {
            //Set PWM duty cycle for LED
            TM_PWM_SetChannelPercent(TIM2, &TIM_Data, TM_PWM_Channel_1, currentDuty + (duty*(-1)));
        }*/
        TM_DISCO_LedToggle(LED_GREEN);
        
        vTaskDelay( 250 / portTICK_RATE_MS );
    }
}

static void vDA2_ReadSensor(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay( 250 / portTICK_RATE_MS );
        
        int index =0;
        DHT_GetTemHumi();
        InRoomEvn.AnhSang = BH1750_Read();
        InRoomEvn.DoAmKhi = DHT_doam();
        InRoomEvn.NhietDo = DHT_nhietdo();
        
        userData.distance = getDistance(TM_ADC_Read(ADC1,ADC_Channel_0));
        
        TM_DS1307_GetDateTime(&time);
        TM_DISCO_LedToggle(LED_ORANGE);
        
        if(!TM_USART_BufferEmpty(USART6))
        {
            TM_USART_Gets(USART6,reminder,TM_USART6_BUFFER_SIZE);
            TM_USART_ClearBuffer(USART6);
            TM_DISCO_LedToggle(LED_GREEN);
            /*In ra LCD truoc roi tinh gi thi tinh*/
            LCD_SetTextColor(YELLOW);
            sprintf(sbuff, "Note:%s",reminder);
            LCD_StringLine(180, 300, (uint8_t *)sbuff);
//            char tempInParse[50];
//            strcpy(tempInParse,reminder);
//            for(int i=0;i<50;i++)
//            {
//                tempInParse[i] = reminder[i];
//            }
            //insertFirst(parsingLine(reminder));
            
            //LCD_StringLine(180, 300, (uint8_t *)reminder);
            
//            userNotes[index] = parsingLine(reminder);
//            index++;
//            if(index>10)
//                index=0;
        }
    }
}

static void vDA2_ShowLCD(void *pvParameters)
{
    for(;;)
    {
        //vTaskDelay( 300 / portTICK_RATE_MS );
        mLCD_showSensor();
        mLCD_showDateTime();
    }
}
    
int mLCD_showSensor()
{
    LCD_CharSize(16);
    LCD_SetTextColor(RED);
    //LCD_Clear_P(WHITE, 110, 240, 5120);
    sprintf(sbuff, "%d*C , %02d lux , %02d humi   ",InRoomEvn.NhietDo,InRoomEvn.AnhSang,InRoomEvn.DoAmKhi);
    LCD_StringLine(110, 250, (uint8_t *)sbuff);
    
    sprintf(sbuff, "Distance : %0.2f",userData.distance);
    LCD_StringLine(150, 250, (uint8_t *)sbuff);
    
    //sprintf(sbuff, "Note:%s",reminder);
    //sprintf(sbuff,"%d:%d %d-%d-%d %s",userNotes[0].reminderTime.hours, userNotes[0].reminderTime.minutes ,userNotes[0].reminderTime.date, userNotes[0].reminderTime.month, userNotes[0].reminderTime.year, userNotes[0].reminder);
    //LCD_StringLine(180, 250, (uint8_t *)sbuff);
    return 1;
}

int mLCD_showDateTime()
{
    LCD_SetTextColor(GREEN);
    LCD_CharSize(24);
    sprintf(sbuff, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    LCD_StringLine(62, 235, (uint8_t *)sbuff);
    if (time.hours == 0 && time.minutes == 00 && time.seconds == 0)
    {
        LCD_Clear_P(BLACK, 86, 320, 5120);
        LCD_CharSize(16);
        sprintf(sbuff, "%s,%d %s %d", date[time.date], time.date, Month[time.month],time.year + 2000);
        LCD_StringLine(86, 230, (uint8_t *)sbuff);
    }
    return 1;
}

int mLCD_resetLcd()
{

    TM_DS1307_GetDateTime(&time);
    
    LCD_SetTextColor(GREEN);
    LCD_CharSize(24);
    sprintf(sbuff, "%02d:%02d:%02d", time.hours, time.minutes, time.seconds);
    LCD_StringLine(62, 235, (uint8_t *)sbuff);
    
    LCD_Clear_P(BLACK, 86, 320, 5120);
    LCD_CharSize(16);
    sprintf(sbuff, "%s,%d,%s,%d", date[time.date], time.date, Month[time.month],time.year + 2000);
    LCD_StringLine(86, 250, (uint8_t *)sbuff);

    
    LCD_CharSize(16);
    LCD_SetTextColor(RED);
    sprintf(sbuff, "%d*C , %02d lux , %02d humi",InRoomEvn.NhietDo,InRoomEvn.AnhSang,InRoomEvn.DoAmKhi);
    LCD_StringLine(110, 250, (uint8_t *)sbuff);
    /*Reset color*/
    LCD_SetTextColor(WHITE);
    return 1;
}

int mLCD_showTitle()
{
    LCD_Clear_P(BLACK,14,320,8960);
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(BLUE);
    LCD_CharSize(16);
    LCD_StringLine(14,290,(unsigned char*)"UNIVERSITY OF INFOMATION TECHNOLOGY");
    LCD_StringLine(28,275,(unsigned char*)"FACULTY OF COMPUTER ENGINEERING");
    /*LCD_SetTextColor(RED);
    LCD_StringLine(42,180,(unsigned char*)"E_DESK");*/
    return 1;
}

float getDistance(uint16_t voltaValue)
{
    float volts = voltaValue*0.0048828125;
    // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
    float distance = 65*pow(volts, -1.10);
    return distance;
}

//REMINDER parsingLine(char* inputString)
//{
//    REMINDER tempNoteType;
//    char * pch;
//    char tempNote[3][32] = {0};
//    char tempTime1[2][2] = {0};
//    char tempTime2[3][4] = {0};
//    pch = strtok (inputString," ");
//    if(pch!=NULL)
//    {
//        strcpy(tempNote[0],pch);
//        pch = strtok (NULL," ");
//        strcpy(tempNote[1],pch);
//        pch = strtok (NULL,".");
//        strcpy(tempNote[2],pch);
//    }
//    /*Parsing Time hour in tempNote[0]*/
//    /*  tempTime1[0] = (string)hour;
//    *   tempTime1[1] = (string)minutes;
//    */
//    pch = strtok(tempNote[0],":-'");
//    if(pch!=NULL)
//    {
//        strcpy(tempTime1[0],pch);
//        pch = strtok(NULL,":-'");
//        strcpy(tempTime1[1],pch);
//    }
//    tempNoteType.reminderTime.hours = atoi(tempTime1[0]);
//    tempNoteType.reminderTime.minutes = atoi(tempTime1[1]);
//    /*Parsing Day in tempNote[1]*/
//    /*  tempTime2[0] = (string)date;
//    *   tempTime2[1] = (string)month;
//    *   tempTime2[2] = (string)year;
//    */
//    pch = strtok(tempNote[1],":-'");
//    if(pch!=NULL)
//    {
//        strcpy(tempTime2[0],pch);
//        pch = strtok(NULL,":-'");
//        strcpy(tempTime2[1],pch);
//        pch = strtok(NULL,":-'");
//        strcpy(tempTime2[2],pch);
//    }
//    tempNoteType.reminderTime.date = atoi(tempTime2[0]);
//    tempNoteType.reminderTime.month = atoi(tempTime2[1]);
//    tempNoteType.reminderTime.year = atoi(tempTime2[2]);
//    
//    /*Parsing text reminder in tempNote[2]*/
//    strcpy(tempNoteType.reminder,tempNote[2]);
//    
//    /*Return to REMINDER*/
//    pch =NULL;
//    return tempNoteType;
//}

REMINDER parsingLine(char* inputString)
{
    REMINDER tempNoteType;
    char *pch;
    pch = strtok (inputString,":");
    if(pch!=NULL)
    {
        tempNoteType.reminderTime.hours = atoi(pch);
        
        pch = strtok (NULL," ");
        tempNoteType.reminderTime.minutes = atoi(pch);
        
        pch = strtok (NULL,"-");
        tempNoteType.reminderTime.date = atoi(pch);
        
        pch = strtok (NULL,"-");
        tempNoteType.reminderTime.month = atoi(pch);
        
        pch = strtok (NULL," ");
        tempNoteType.reminderTime.year = atoi(pch);
        
        pch = strtok (NULL,".");
        strcpy(tempNoteType.reminderText,pch);
        
        free(pch);
    }
    return tempNoteType;
}


#ifdef USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line) {
  /* User can add his own implementation to report the file name and line
     number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1) {
  }
}
#endif
