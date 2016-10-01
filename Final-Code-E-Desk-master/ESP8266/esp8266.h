#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "stm32f4xx.h"
#include "diskio.h"
#include "tm_stm32f4_ds1307.h"
#include "ff.h"
#include "integer.h"
#include "tm_stm32f4_disco.h"
#include "nrf24l01.h"
#include "tm_stm32f4_usart.h"
//#include "TFT.h"
/*----------  FreeRTOS  ----------*/
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "croutine.h"
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
  /* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
     set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

#define IP        "184.106.153.149"
#define PORT      80
#define USART     USART1
#define PINPACK   TM_USART_PinsPack_2
#define BAUDRATE 	9600
#define SIDD			"HQD"
#define PASS			"emkhongbiet"
#define APIKEY		"ZFWTJEXVLTYQKI6J&"
int sendEsp(char* ip, uint32_t port,char* apiKey,char* fieldx,uint8_t data);
int setEsp(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack, uint32_t baudrate);
int senddata(NRF_ThoiTiet NRF);
int connectWifi(char*sidd ,char* passWifi);
int connect(void);
int sendEspPre(char* ip, uint32_t port,char apiKey[],char fieldx[],long data);
//extern char* buffer_Esp;
