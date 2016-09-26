#include "esp8266.h"
//char* buffer_Esp;
/**
 * Hàm này dùng để giao tiếp với esp
 * @param  USARTx   Bộ USART
 * @param  pinspack Chân USART
 * @param  baudrate Tốc độ bound
 * @return          1
 */

int setEsp(USART_TypeDef* USARTx, TM_USART_PinsPack_t pinspack,
           uint32_t baudrate)
{
  TM_USART_Init(USARTx, pinspack, baudrate);
  printf(" AT+RST\r\n");
	//vTaskDelay(1500 / portTICK_RATE_MS);
  //Delayms(1500);
  return 1;
}

int connectWifi(char* sidd, char* passWifi)
{
  printf("AT+CWMODE=1\r\n");
	vTaskDelay(500 / portTICK_RATE_MS);
  //Delayms(500);
  printf("AT+CWJAP=\"%s\",\"%s\"\r\n", sidd, passWifi);
	vTaskDelay(5000/ portTICK_RATE_MS);
  //Delayms(5000);
  printf("AT+CIPMUX=1\r\n");
	vTaskDelay(500 / portTICK_RATE_MS);
  //Delayms(500);
  return 1;
}
/**
 * Hàm dùng để gửi lệnh kết nối với các port, chân đã thiết lập
 * @param  ip     Địa chỉ IP của trang thingspeaks
 * @param  port   Port esp
 * @param  apiKey channel key trên thingspeak
 * @param  fieldx Trường cần gửi
 * @param  data   Dữ liệu cần gửi
 * @return        1
 */
int sendEsp(char* ip, uint32_t port, char apiKey[], char fieldx[],
            uint8_t data)
{
	//buffer_Esp = (char *) malloc(sizeof(char));
  char server[] = "GET /update?key=";
  strcat(server, apiKey);
  // strcat(server,"&");
  strcat(server, fieldx);
  // strcat(server,"=");
  printf("AT+CIPSTART=3,\"TCP\",\"%s\",%d\r\n", ip, port);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("AT+CIPSEND=3,%d\r\n", strlen(server) + 4);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("%s%d\r\n", server, data);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("AT+CIPSEND=3,%d\r\n", strlen(server) + 4);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
//	TM_USART_Gets(USART1, buffer_Esp, sizeof(buffer_Esp));
//	while(buffer_Esp!="ok")
//	{
//  printf("AT+CIPSTART=3,\"TCP\",\"%s\",%d\r\n", ip, port);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("AT+CIPSEND=3,%d\r\n", strlen(server) + 4);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("%s%d\r\n", server, data);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("AT+CIPSEND=3,%d\r\n", strlen(server) + 4);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//	}
//	free(buffer_Esp);
  printf("AT+CIPCLOSE\r\n");
  return 1;
}

int connect(void)
{
//	buffer_Esp = (char *) malloc(sizeof(char));
	connectWifi(SIDD, PASS);
//	TM_USART_Gets(USART1, buffer_Esp, sizeof(buffer_Esp));
//	while(buffer_Esp!="ok")
//	{
//  connectWifi(SIDD, PASS);
//	}
//	free(buffer_Esp);
  return 1;
}

/**
 * Gửi 4 dữ liệu đo được lên Thingspeak
 * @param  NRF Struct thời tiết
 * @return     1
 */
int senddata(NRF_ThoiTiet NRF)
{
  sendEsp(IP, PORT, APIKEY, "field1=", NRF.NhietDo);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  sendEsp(IP, PORT, APIKEY, "field2=", NRF.DoAm);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  sendEspPre(IP, PORT, APIKEY, "field3=", NRF.ApSuat);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  sendEsp(IP, PORT, APIKEY, "field4=", NRF.Mua);
//	vTaskDelay(3000 / portTICK_RATE_MS);
//	sendEspPre(IP, PORT, APIKEY, "field5=",WthFor.output*100);
	vTaskDelay(3000 / portTICK_RATE_MS);
  //Delayms(3000);
  return 1;
}


int sendEspPre(char* ip, uint32_t port, char apiKey[], char fieldx[],
               long data)
{
	//buffer_Esp = (char *) malloc(sizeof(char));
  char server[] = "GET /update?key=";
  strcat(server, apiKey);
  // strcat(server,"&");
  strcat(server, fieldx);
  // strcat(server,"=");
  printf("AT+CIPSTART=4,\"TCP\",\"%s\",%d\r\n", ip, port);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("AT+CIPSEND=4,%d\r\n", strlen(server) + 8);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("%s%ld\r\n", server, data);
	vTaskDelay(1000 / portTICK_RATE_MS);
  //Delayms(3000);
  printf("AT+CIPSEND=4,%d\r\n", strlen(server) + 8);
	vTaskDelay(1000 / portTICK_RATE_MS);
//	TM_USART_Gets(USART1, buffer_Esp, sizeof(buffer_Esp));
//	while(buffer_Esp!="ok")
//	{
//	printf("AT+CIPSTART=4,\"TCP\",\"%s\",%d\r\n", ip, port);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("AT+CIPSEND=4,%d\r\n", strlen(server) + 8);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("%s%ld\r\n", server, data);
//	vTaskDelay(1000 / portTICK_RATE_MS);
//  //Delayms(3000);
//  printf("AT+CIPSEND=4,%d\r\n", strlen(server) + 8);
//	vTaskDelay(1000 / portTICK_RATE_MS);		
//	}
//	free(buffer_Esp);
  //Delayms(3000);
  printf("AT+CIPCLOSE\r\n");
  return 1;
}

PUTCHAR_PROTOTYPE {
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART */
  USART_SendData(USART1, (uint8_t)ch);

  /* Loop until the end of transmission */
  while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {
  }

  return ch;
}

