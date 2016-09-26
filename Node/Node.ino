#include <Wire.h>
#include <SPI.h>
#include "RF24.h"
#include <Adafruit_BMP085.h>
#include "DHT.h"
#include <Sleep_n0m1.h>
//Sleep
int count = 0;
Sleep sleep;
unsigned long sleepTime=300000; //how long you want the arduino to sleep
//DHT Pin
#define DHTPIN 2  
#define DHTTYPE DHT11   
DHT dht(DHTPIN, DHTTYPE);
//RF address 
const uint8_t pipe[5] ={0x7E,0x7E,0x7E,0x7E,0x7E};// địa chỉ để phát
//const uint8_t pipe[5]={ 0xB3,0xB4,0xB5,0xB6,0xF1};
RF24 radio(9,10); //thay 10 thành 53 với mega
uint8_t msg[32];
//float data[4]={1,2,3,4};float *ptr = data;

int value = 0;
int sensorReading=0;
int range=0;
float h = 0;
float t = 0;
long p = 0;
// lowest and highest sensor readings:
const int sensorMin = 0;     // sensor minimum
const int sensorMax = 1024;  // sensor maximum

Adafruit_BMP085 bmp;
void setup() {

  // put your setup code here, to run once:
   //============================================================Module NRF24
  radio.begin();                     
   for(int i = 0; i<6;i++){
    radio.setAutoAck(i,1);
  }
  //radio.setAutoAck(1);
  radio.setRetries(1,1);             
  radio.setDataRate(RF24_2MBPS);    // Tốc độ truyền
  radio.setPALevel(RF24_PA_MAX);      // Dung lượng tối đa
  radio.setChannel(100);               // Đặt kênh
  radio.openWritingPipe(pipe);        // mở kênh

  
  Serial.begin(9600);
  Serial.println("Hello");

  dht.begin();

  if (!bmp.begin())
  Serial.println("Could not find a valid BMP085 sensor, check wiring!");

  rf24_crclength_e a =  radio.getCRCLength();
  Serial.println(a);
  delay(50);
}

void loop() {
    Serial.print("Wake");
    delay(1000);
    sensorReading = analogRead(A0);
    range = map(sensorReading, sensorMin, sensorMax, 0, 3);
    h = dht.readHumidity();
    t = bmp.readTemperature();
    p = bmp.readPressure();
    delay(1000);
    msg[0]=(uint8_t)(p>>24);
    msg[1]=(uint8_t)(p>>16);
    msg[2]=(uint8_t)(p>>8);
    msg[3]=(uint8_t)p;
    msg[4]=(uint8_t)t;
    msg[5]=(uint8_t)h;
    msg[6]=(uint8_t)range;
    for(int i = 0; i<10; i++)
    {
    radio.write(&msg, sizeof(msg));
    delay(50);
    }
    Serial.println("Sleep");
    delay(50);
    sleep.pwrDownMode(); //set sleep mode
    sleep.sleepDelay(sleepTime); //sleep for: sleepTime
}
