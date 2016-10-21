#include "main.h"
#define __cplusplus
/*Define scope*/
#define LCD_FUNCs 1
/*PID*/
#define PID_PARAM_KP        1         /* Proporcional */
#define PID_PARAM_KI        10       /* Integral */
#define PID_PARAM_KD        0          /* Derivative */
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

#define TIMER_X             280
#define TIMER_Y             25
#define TIMER_RANGE         100
#define TIMER_SIZE          24
#define TIMER_COLOR         GREEN

#define DATE_X              300
#define DATE_Y              50
#define DATE_RANGE          100
#define DATE_SIZE           16
#define DATE_COLOR          GREEN

#define TITLE_UIT_X         300
#define TITLE_UIT_Y         5
#define TITLE_UIT_RANGE     100
#define TITLE_UIT_SIZE      16
#define TITLE_UIT_COLOR     BLUE

#define TITLE_CE_X          290
#define TITLE_CE_Y          45
#define TITLE_CE_RANGE      100
#define TITLE_CE_SIZE       100
#define TITLE_CE_COLOR      BLUE

#define INFO_X              97
#define INFO_Y              05
#define INFO_RANGE          100
#define INFO_SIZE           100
#define INFO_COLOR          RED
#define INFO_BACKGOURND     BLACK

#define LATED_X              317-8
#define LATED_Y              137
#define LATED_RANGE          100
#define LATED_SIZE           100
#define LATED_COLOR          RED
#define LATED_BACKGOURND     BLACK

#define NOTE_X              LATED_X+1
#define NOTE_Y              LATED_Y+12
#define NOTE_RANGE          10240
#define NOTE_SIZE           16
#define NOTE_COLOR          BLUE
#define NOTE_BACKGOURND     BLACK

#define ADV_X               300
#define ADV_Y               90
#define ADV_RANGE          10240
#define ADV_SIZE           16
#define ADV_COLOR          BLUE
#define ADV_BACKGOURND     BLACK

#define ADV_Hour_Con        2      /*Thời gian thích hợp ngồi liên tục*/
#define ADV_Hour_Day        480       /*Thời gian làm việc 1 ngày*/
#define ADV_DISTANCE        20
#define ADV_TAKE_REST       "TAKE A REST, PLS" 
#define ADV_HAPPY           "WHAT A HAPPY DAY" 
#define ADV_TOO_HARD        "STOP AND GO OUT." 


int mLCD_resetLcd(void);
int mLCD_showTitle(void);
int mLCD_showDateTime(void);
int mLCD_showDate(void);
int mLCD_showSensor(void);
int mLCD_ShowListRemind(void);
int mLCD_RS(void);
/*REMINDER*/
#define MAX_STRING_REMINDER 100
char reminder[MAX_STRING_REMINDER]={0};
char * p_reminder = reminder;
int newRemindFlag=0;
struct node * tmpNext;

int tempMinutes = 0;
int countMinutes =0;

int i=0;

int clearReminder(char rmd[])
{
    for(int i=0; i < MAX_STRING_REMINDER ;i++)
    {
        rmd[i] = NULL;
    }
    return 1;
}

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


int senddata(ENVIRONMENT_TYPE _env)
{
  sendEsp(IP, PORT, APIKEY, "field1=", _env.AnhSang);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  sendEsp(IP, PORT, APIKEY, "field2=", _env.DoAmKhi);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  sendEspPre(IP, PORT, APIKEY, "field3=", _env.NhietDo);
	vTaskDelay(3000 / portTICK_RATE_MS);
  return 1;
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
#if 1 
typedef struct STRUCT_OF_REMINDER
{
    TM_DS1307_Time_t reminderTime;
    char * reminderText;
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
    return temp;
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
    for(_current = head; _current != NULL; _current = _current->next)
    {
        lenght++;
    }
    return lenght;
}

uint8_t Alarm()
{
    if((head->reminder.reminderTime.date == CrTime.date)
    &&(head->reminder.reminderTime.month == CrTime.month)
    &&(head->reminder.reminderTime.year == CrTime.year)
    &&(head->reminder.reminderTime.hours == CrTime.hours)
    &&(head->reminder.reminderTime.minutes == CrTime.minutes + 30))
    {
        return 1;
    }
    return 0;
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
        //long currentCompare;
        //long nextCompare;
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
#endif /*LINK LIST*/

float getDistance(uint16_t voltaValue);
REMINDER parsingLine(char* inputString);

ENVIRONMENT_TYPE InRoomEvn;
USERDATA_TYPE userData;



REMINDER userNotes[10];
int reminderIndex=0;
/*Global PWM PID*/
/* Timer data for PWM */
TM_PWM_TIM_t TIM_Data;
char buf[150];
uint8_t devices, count;
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
        NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );

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
        sortList();
        
        LCD_Init();

        I2C1_Init();
        /*I2C1_PINS PACK 2
        SCL=PB8_SDA=PB9  */
        //BH1750_Init();

        DHT_GetTemHumi();
        
        /*Sensor khoang cach*/
        /*ADC: Pin_A0 (ADC1 channel 0)*/
        TM_ADC_Init(ADC1,ADC_Channel_0);

        TM_USART_Init(USART6, TM_USART_PinsPack_1, 100);
        //TM_USART_Init(USART2, TM_USART_PinsPack_2, 9600);
        TM_USART_SetCustomStringEndCharacter(USART6,'.');
        
        /*I2C2_PINS PACK 1
        SCL=PB10_SDA=PB11  */
        if (TM_DS1307_Init() != TM_DS1307_Result_Ok)
        {
            /*Warning that RTC error*/
            TM_DISCO_LedOn(LED_ALL);
        }
            
#if 1 /*Setting datetime*/
        {
            /* Set date and time */
            /* Day 7, 26th May 2014, 02:05:00 */
            CrTime.hours = 16;
            CrTime.minutes = 15;
            CrTime.seconds = 00;
            CrTime.date = 15;
            CrTime.day = 7;
            CrTime.month = 10;
            CrTime.year = 16;
            TM_DS1307_SetDateTime(&CrTime);

            /* Disable output first */
            TM_DS1307_DisableOutputPin();

            /* Set output pin to 4096 Hz */
            TM_DS1307_EnableOutputPin(TM_DS1307_OutputFrequency_4096Hz);
        }
        
        
#endif /*END DATETIME SETTING*/
        TM_DS1307_GetDateTime(&CrTime);
        CrTime.year += 2000;
        //mLCD_resetLcd();
        mLCD_showDate();
        
        LCD_Clear(BLACK);
        mLCD_showTitle();
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
        //TM_LEDS_Init();
        /* Init timer */
        //TM_TIMER_Init();
        /* Init PWM */
        //TM_PWM_Init();
        TM_PWM_InitTimer(TIM2, &TIM_Data, 8399);

        /* Initialize TIM2, Channel 1, PinsPack 2 = PA5 */
        TM_PWM_InitChannel(TIM2, TM_PWM_Channel_1, TM_PWM_PinsPack_2);

        /* Set default duty cycle */
        TM_PWM_SetChannelPercent(TIM1, &TIM_Data, TM_PWM_Channel_1, duty);
        //TM_PWM_SetChannelMicros(TIM2, &TIM_Data, TM_PWM_Channel_1, duty);
        }
        
        userData.uD_distance = 0;
        userData.uD_hisTime_Con =0;
        userData.uD_hisTime_Day =0;
        userData.uD_isSitting =0;
        
        LCD_SetDisplayWindow(0, 0, 238, 318);
        
        //setEsp(ESP_USART,ESP_PINPACK,ESP_BAUDRATE);
    }
    return 1;
}

int initTask()
{
    xTaskCreate(vDA2_ReadSensor,(const signed char*)"vDA2_ReadSensor",STACK_SIZE_MIN,NULL,tskIDLE_PRIORITY+3,&ptr_readSensor);
    xTaskCreate(vDA2_ShowLCD, (const signed char *)"vDA2_ShowLCD", STACK_SIZE_MIN+384,NULL, tskIDLE_PRIORITY+1,&ptr_showLCD);
    //xTaskCreate(vDA2_Response, (const signed char *)"vDA2_Response", STACK_SIZE_MIN,NULL, tskIDLE_PRIORITY+2,&ptr_response);
    return 1;
}

static void vDA2_Response(void *pvParameters)
{
    int tempSecond = CrTime.seconds;
    for(;;)
    {
/*====================================================================================================================*/
/*                                                             PID                                                    */
/*====================================================================================================================*/
        /* Calculate error */
//        LIGHT_CURRENT = InRoomEvn.AnhSang;
//        LIGHT_WANT = lightWantValue;
//        pid_error = LIGHT_WANT - LIGHT_CURRENT;
////        int pastDuty = duty;
////        if(pid_error >=0)
////        {
//            /* Calculate PID here, argument is error */
//            /* Output data will be returned, we will use it as duty cycle parameter */
//            //currentDuty = duty;
//            duty = arm_pid_f32(&PID, pid_error)/1000;
//            /* Check overflow, duty cycle in percent */
//            if (duty > 100) {
//            duty = 100;
//            } else if (duty < 0) {
//            duty = 0;
//            }
//            pastDuty = duty;
//        }
//        else
//        {
//            pid_error = pid_error * (-1);
//             duty = arm_pid_f32(&PID, pid_error);
//            /* Check overflow, duty cycle in percent */
//            /**/
//            duty = duty - pastDuty;
//            if (duty > 100) {
//            duty = 100;
//            } else if (duty < 0) {
//            duty = 0;
//            }
//            pastDuty = duty;
//        }
        i++;
            if(i==100)
                {
                    i=0;
                } 
 //vTaskDelay(1000 / portTICK_RATE_MS );                
        TM_PWM_SetChannelPercent(TIM2, &TIM_Data, TM_PWM_Channel_1, i);

/*====================================================================================================================*/
/*                                               XỬ lÝ ẢNH HƯỞNG SỨC KHỎE                                             */
/*====================================================================================================================*/
        if((userData.uD_distance < ADV_DISTANCE) && (userData.uD_distance >10))
        /*Nếu khoảng cách nhỏ hơn ADV_DISTANCE*/
        {
            /*=====YES=====*/
            if(userData.uD_isSitting)
            /*Nếu mà đang ngồi*/
            {
                if(tempSecond!=CrTime.seconds)
                {
                /*Thời gian ngồi liên tục  = CrTime - Thời gian bắt đầu*/
                    userData.uD_hisTime_Con++;
                    userData.uD_hisTime_Day++;
                    tempSecond = CrTime.seconds;
                //userData.uD_hisTime_Con = (CrTime.minutes - userData.uD_StartTime.minutes)*60+(CrTime.seconds - userData.uD_StartTime.seconds);
                //userData.uD_hisTime_Day = tempTDay + userData.uD_hisTime_Con;
                }
            }
            else
            /*Không phải là đang ngồi*/
            {
                /*isSitting =1*/
                userData.uD_isSitting = 1;
                /*Thời gian bắt đầu = CrTime*/
                userData.uD_StartTime.hours = CrTime.hours;
                userData.uD_StartTime.minutes = CrTime.minutes;
                userData.uD_StartTime.seconds = CrTime.seconds;
            }
        }
        else
        {
            /*=====NO=====*/
            /*Ngồi liên tục = 0*/
            //userData.uD_hisTime_Day += userData.uD_hisTime_Con;
            userData.uD_hisTime_Con = 0;
            /*Đang ngồi = 0*/
            userData.uD_isSitting =0;
        }
///*====================================================================================================================*/
///*                                               REMINDER                                                             */
///*====================================================================================================================*/
//        
//        if(compareTime(head->reminder.reminderTime,CrTime) == 0)
//        {
//            TM_USART_Puts(USART6,head->reminder.reminderText);
//            deleteFirst();
//        }
//        vTaskDelay(100 / portTICK_RATE_MS );
    }

}

static void vDA2_ReadSensor(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay( 250 / portTICK_RATE_MS );
        DHT_GetTemHumi();
        InRoomEvn.AnhSang = BH1750_Read();
        InRoomEvn.DoAmKhi = DHT_doam();
        InRoomEvn.NhietDo = DHT_nhietdo();
        
        userData.uD_distance = getDistance(TM_ADC_Read(ADC1,ADC_Channel_0));
        
        TM_DS1307_GetDateTime(&CrTime);
        CrTime.year+=2000;
        if(tempMinutes != CrTime.minutes)
        {
            countMinutes++;
            tempMinutes = CrTime.minutes;
        }
        
        if(!TM_USART_BufferEmpty(USART6))
        {
            newRemindFlag = 1;
        }
        //if(CrTime.minutes%2==0)
        //{
        //    connect();
        //    senddata(InRoomEvn);
        //}
    }
}

static void vDA2_ShowLCD(void *pvParameters)
{
    for(;;)
    {
        vTaskDelay( 200 / portTICK_RATE_MS );
        TM_DISCO_LedToggle(LED_ORANGE);
        mLCD_showSensor();
        mLCD_showDateTime();
    }
}


float getDistance(uint16_t voltaValue)
{
    float volts = voltaValue*0.0048828125;
    // value from sensor * (5/1024) - if running 3.3.volts then change 5 to 3.3
    float distance = 320*pow(volts, -1.10);
    return distance;
}

void copy_string(char *target, char *source)
{
   while (*source)
   {
      *target = *source;
      source++;
      target++;
   }
   *target = '\0';
}

uint8_t getSizeOfString(char *string)
{
    uint8_t size=0;
    while(*string)
    {
        size++;
        string++;
    }
    return size;
}

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
        int a = getSizeOfString(pch);
        tempNoteType.reminderText = (char*)malloc(a);
        //copy_string(tempNoteType.reminderText,pch);
        
        strcpy(tempNoteType.reminderText,pch);
        
        //free(tempNoteType.reminderText);
    }
    return tempNoteType;
}
#if LCD_FUNCs
/*LCD FUNCTIONS*/
int mLCD_RS()
{
    LCD_SetBackColor(BLACK);
    LCD_SetTextColor(WHITE);
    LCD_CharSize(8);
    return 1;
}

int mLCD_writeLine(uint16_t posY, uint16_t posX, int range, uint16_t color, uint16_t backgournd,uint8_t size,char* string)
{
    //LCD_Clear_P(BLACK,14,320,8960);
    LCD_SetBackColor(backgournd);
    LCD_SetTextColor(color);
    LCD_CharSize(size);
    LCD_StringLine(posY,posX,(unsigned char*)string);
    return 1;
}


int mLCD_showSensor()
{
    mLCD_RS();
    LCD_SetBackColor(INFO_BACKGOURND);
    LCD_SetTextColor(INFO_COLOR);
    LCD_CharSize(16);
    //LCD_Clear_P(WHITE, 110, 240, 5120);
    sprintf(sbuff, "Temp:%5.1d C",InRoomEvn.NhietDo);
    LCD_StringLine(INFO_Y+16, INFO_X, (uint8_t *)sbuff);

    sprintf(sbuff, "Lght:%5.1d L",InRoomEvn.AnhSang);
    LCD_StringLine(INFO_Y+32, INFO_X, (uint8_t *)sbuff);
    
    sprintf(sbuff, "Humi:%5.1d %%",InRoomEvn.DoAmKhi);
    LCD_StringLine(INFO_Y+48, INFO_X, (uint8_t *)sbuff);
    
    sprintf(sbuff, "Dist:%5.1f C",userData.uD_distance);
    LCD_StringLine(INFO_Y+64, INFO_X, (uint8_t *)sbuff);
    
    sprintf(sbuff, "TCon:%5.1d S",userData.uD_hisTime_Con);
    LCD_StringLine(INFO_Y+80, INFO_X, (uint8_t *)sbuff);
    
    sprintf(sbuff, "TDay:%5.1d S",userData.uD_hisTime_Day);
    LCD_StringLine(INFO_Y+96, INFO_X, (uint8_t *)sbuff);
    
    mLCD_RS();
    if(newRemindFlag)
    {
        /*New remind*/
        clearReminder(reminder);
        TM_USART_Gets(USART6,reminder,TM_USART6_BUFFER_SIZE);
        TM_USART_ClearBuffer(USART6);

        /*Parsing*/
        insertFirst(parsingLine(reminder));
        
        if(compareTime(head->reminder.reminderTime,CrTime) == 1)/*If new OK ?*/
        {
            /*Send ok*/
            TM_USART_Puts(USART6, "OK!\r\n");
            /*Show*/
            LCD_SetBackColor(INFO_BACKGOURND);
            LCD_SetTextColor(NOTE_COLOR);
            /*LATED*/
            LCD_SetColors(DGRAY,BLACK);
            LCD_CharSize(12);
            LCD_Clear_P(BLACK,LATED_Y,LATED_X - 60,3040);
            sprintf(sbuff,"<%d:%d %d-%d-%d>%s",
            head->reminder.reminderTime.hours,
            head->reminder.reminderTime.minutes,
            head->reminder.reminderTime.date, 
            head->reminder.reminderTime.month,
            head->reminder.reminderTime.year,
            head->reminder.reminderText);
            LCD_StringLineNotEndLine(LATED_Y,LATED_X -48,(uint8_t *)sbuff);
            /*Sort*/
            sortList();
            /*Show list*/
            mLCD_ShowListRemind();
        }
        else
        {
            TM_DISCO_LedToggle(LED_GREEN);
            /*Send Not OK,show*/
            LCD_Clear_P(BLACK,LATED_Y,LATED_X - 60,3040);
            LCD_SetColors(DGRAY,BLACK);
            LCD_CharSize(12);
            sprintf(sbuff, "%s","Not valid!!!!");
            LCD_StringLineNotEndLine(LATED_Y,LATED_X-48, (uint8_t *)sbuff);
            
            TM_USART_Puts(USART6, "NOT OK, NOTE TIME < TIME!\r\n");
            /*Delete*/
            deleteFirst();
        }
        newRemindFlag =0;
    }
    if(compareTime(CrTime,head->reminder.reminderTime)==1)
    /*So sánh nếu thời gian hệ thống bằng với thời gian của head*/
    {
        /*Thông báo*/
        sprintf(sbuff, "Time To: %s\r\n",head->reminder.reminderText);
        TM_USART_Puts(USART6, sbuff);
        /*Xóa first*/
        deleteFirst();
        /*Cập nhật lại LCD*/
        mLCD_ShowListRemind();
    }
    
    /*Hiển thị LCD*/
    LCD_SetColors(ADV_COLOR,ADV_BACKGOURND);
    LCD_CharSize(ADV_SIZE);
    //LCD_Clear_P(BLACK,ADV_Y,ADV_X+12,ADV_RANGE);
    if((userData.uD_hisTime_Con == ADV_Hour_Con)
     ||(userData.uD_hisTime_Con == ADV_Hour_Con-1))
    /*Nếu mà ngồi liên tục > 2h*/
    {
        LCD_StringLine(ADV_Y,ADV_X,(uint8_t*)ADV_TAKE_REST);
        TM_USART_Puts(USART6,"ADV_TAKE_REST \r\n");
    }
    else if(userData.uD_hisTime_Day == ADV_Hour_Day)
    {
        LCD_StringLine(ADV_Y,ADV_X,(uint8_t*)ADV_TOO_HARD);
        TM_USART_Puts(USART6,"ADV_TOO_HARD \r\n");
    }
    else
    {
        LCD_StringLine(ADV_Y,ADV_X,(uint8_t*)ADV_HAPPY);
        //TM_USART_Puts(USART6,"ADV_HAPPY \r\n");
    }
    mLCD_RS();
    return 1;
}

int mLCD_ShowListRemind()
{
    /*Show first*/
    mLCD_RS();
    struct node * _current;
    _current = head;
    LCD_SetTextColor(NOTE_COLOR);
    LCD_CharSize(NOTE_SIZE);
    LCD_Clear_P(BLACK,NOTE_Y,NOTE_X- 40, 3768);
    LCD_Clear_P(BLACK,NOTE_Y+12,NOTE_X+8, 3840);
    sprintf(sbuff,"<%d:%d %d-%d-%d> %s",
    _current->reminder.reminderTime.hours,
    _current->reminder.reminderTime.minutes,
    _current->reminder.reminderTime.date, 
    _current->reminder.reminderTime.month,
    _current->reminder.reminderTime.year,
    _current->reminder.reminderText);
    
    LCD_StringLine(NOTE_Y,NOTE_X-48, (uint8_t *)sbuff);
    
    //LCD_Clear_P(BLACK,NOTE_Y+32,NOTE_X-8, 15360);
    _current=_current->next;
    for(int i = 2; (i < 5)&&(_current!=NULL);i++,_current=_current->next)
    {
        LCD_SetTextColor(MAGENTA);
        LCD_CharSize(12);
        
        sprintf(sbuff,"<%02d:%02d %02d-%02d-%02d> %s",
        _current->reminder.reminderTime.hours,
        _current->reminder.reminderTime.minutes,
        _current->reminder.reminderTime.date, 
        _current->reminder.reminderTime.month,
        _current->reminder.reminderTime.year,
        _current->reminder.reminderText);
        LCD_Clear_P(BLACK,NOTE_Y+(i*16),NOTE_X, 3840);
        LCD_StringLineNotEndLine(NOTE_Y+(i*16),NOTE_X, (uint8_t *)sbuff);
    }
    sprintf(sbuff,"%02d",length());
    LCD_StringLine(239/2+1,60,(uint8_t *)sbuff);
    mLCD_RS();
    return 1;
}

int mLCD_showDate()
{
    //LCD_Clear_P(BLACK, 86, 320, 5120);
    LCD_SetTextColor(DATE_COLOR);
    LCD_CharSize(DATE_SIZE);
    sprintf(sbuff, "%s,%d,%s,%d", date[CrTime.day], CrTime.date, Month[CrTime.month],CrTime.year);
    LCD_StringLine(DATE_Y,DATE_X, (uint8_t *)sbuff);
    mLCD_RS();
    return 1;
}

int mLCD_showDateTime()
{
    LCD_SetTextColor(GREEN);
    LCD_CharSize(24);
    sprintf(sbuff, "%02d:%02d:%02d", CrTime.hours, CrTime.minutes, CrTime.seconds);
    LCD_StringLine(TIMER_Y, TIMER_X, (uint8_t *)sbuff);
    if (CrTime.hours == 0 && CrTime.minutes == 00 && CrTime.seconds == 0)
    {
         mLCD_showDate();
         userData.uD_hisTime_Day =0;
    }
    return 1;
}

int mLCD_showTitle()
{
    mLCD_RS();
    mLCD_writeLine(TITLE_UIT_Y,TITLE_UIT_X,TITLE_UIT_RANGE,TITLE_UIT_COLOR,BLACK,TITLE_UIT_SIZE,"UIT-COMPUTER ENGINEERING");
    mLCD_RS();
    /*LCD_SetTextColor(RED);
    LCD_StringLine(42,180,(unsigned char*)"E_DESK");*/
    mLCD_showDate();
    LCD_SetTextColor(RED);
    LCD_CharSize(16);
    LCD_DrawRect(0,0,239,319);
    
    //LCD_StringLine(INFO_Y, INFO_X, (uint8_t *)sbuff);
    LCD_SetTextColor(RED);
    LCD_DrawRect(0+1,0+1,239/2-2,319/3);
    LCD_DrawRect(0+1,107,117,211);
    LCD_DrawRect(0+1,107,70,211);
    LCD_DrawLine(18,0+1,319/3,LCD_DIR_HORIZONTAL);
    LCD_StringLine(0+2,319/3-22,(uint8_t *)"INFO DATA");
    

    LCD_DrawRect(118,1,120,317);
    LCD_DrawRect(118,1,18,317);
    LCD_DrawRect(118,1,18,150);
    //LCD_DrawLine(239/2+18,0+1,319-1,LCD_DIR_HORIZONTAL);
    LCD_StringLine(239/2+1,250,(uint8_t *)"NOTE");
    LCD_StringLine(239/2+1,140,(uint8_t *)"Number:");
    
    LCD_CharSize(12);
    LCD_SetColors(GREEN,BLACK);
    LCD_StringLine(LATED_Y, LATED_X, (uint8_t *)"LATED:");
    LCD_StringLine(LATED_Y+12, LATED_X, (uint8_t *)"NOTES:");
    mLCD_RS();
    return 1;
}
#endif

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
