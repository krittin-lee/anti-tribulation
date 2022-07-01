#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>  
#include <HCSR04.h>
#include <SimpleDHT.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  868E6

int Echo_Pin = 32;
int Trig_Pin = 33;
int pinDHT22 = 2;
SimpleDHT22 dht22(pinDHT22);
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize; //variable used to receive the size of package converted in string
String packet = "OK";   //part of packet, that informs the status of sensor. That variable will be concatenate with the string "values"
String Value = ".-.-"; 


#ifdef __cplusplus
  extern "C" {
 #endif
 
  uint8_t temprature_sens_read();
 
#ifdef __cplusplus
}
#endif
 
uint8_t temprature_sens_read();
int c;

UltraSonicDistanceSensor distanceSensor(Trig_Pin, Echo_Pin);
void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");
  
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
//  LoRa.receive();
  Serial.println("init ok");
   
  delay(1500);
}

void loop() {
  Serial.println("Sending packet: ");
  Serial.print("Rage = ");
  Serial.print(distanceSensor.measureDistanceCm());
  Serial.println("cm");  
  int ultra = distanceSensor.measureDistanceCm();
  
  Serial.print("Them = "); 
  c = (temprature_sens_read() - 32) / 1.8;
  Serial.print(c);
  Serial.println("*c");
  
  float temperature = 0;
  float humidity = 0;
  int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess) 
  {
    Serial.print("Read DHT22 failed, err="); Serial.println(err);delay(2000);
    return;
  }
 
  Serial.println((int)temperature); Serial.print(" *C, ");

  Value = String(ultra)+"."+String(c)+"."+String(temperature)+"."+String(humidity);
  Serial.print(Value);
  
  // send packet
  LoRa.beginPacket();
  LoRa.print(Value);
  LoRa.endPacket();

  delay(1000);
}
