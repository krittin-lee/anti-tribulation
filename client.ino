#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
#include <HCSR04.h>
//#include <SimpleDHT.h>

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND  868E6

int echoPin = 33;
int trigPin = 32;
/*int echoPin = 32;
int trigPin = 33;*/
//int pinDHT22 = 2;
long duration, cm, inches;
//SimpleDHT22 dht22(pinDHT22);
unsigned int counter = 0;
String rssi = "RSSI --";
String packSize; //variable used to receive the size of package converted in string
String packet = "OK";   //part of packet, that informs the status of sensor. That variable will be concatenate with the string "values"
String Value = ".-.-";

void setup() {
  pinMode(0, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Sender Test");

  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  
  Serial.println("init ok");

  delay(1500);
}

void loop() {
  int FS = analogRead(A0);
  Serial.println("Sending packet: ");
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  cm = (duration/2) / 29.1;     // Divide by 29.1 or multiply by 0.0343
  inches = (duration/2) / 74; 





  /*  Serial.print("Them = ");
    c = (temprature_sens_read() - 32) / 1.8;
    Serial.print(c);
    Serial.println("*c");
  */
  float temperature = 31;
  float humidity = 42;
  int tol;
  int wa;
  /*int err = SimpleDHTErrSuccess;
  if ((err = dht22.read2(&temperature, &humidity, NULL)) != SimpleDHTErrSuccess)
  {
    Serial.print("Read DHT22 faile, err="); Serial.println(err); delay(2000);
    return;
  }*/
  //Serial.print(FS);
  // Serial.println((int)temperature); Serial.print(" *C, ");
  if (FS < 3120) {
    tol = 2;
  }
  else if (FS < 3510) {
    tol = 1;
  }
  else {
    tol = 0;
  }

  if (cm <= 32)
  {
    wa = 2;
  }
  
  else {
    wa = 0;
  }
  if (temperature >= 45)
  {
    digitalWrite(0, HIGH);

  }
  else {
    digitalWrite(0, LOW);

  }


   //Value = String(temperature) + "/" + String(humidity) + "/" + String(tol) + "/" + String(wa);
   Value = String(temperature) + "/" + String(humidity) + "/" + String(tol) + "/" + String(wa) + "//" + String(cm) + "//" + String(FS);
 // Value = String(temperature) + "/" + String(humidity) + "/" + String(tol) + "/" + String(wa) + "/////" + String(FS);
  Serial.print(Value);

  // send packet
  LoRa.beginPacket();
  LoRa.print(Value);
  LoRa.endPacket();

  delay(1000);
}
