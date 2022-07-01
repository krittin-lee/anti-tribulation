#include <SPI.h>
#include <LoRa.h>


#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND    868E6

String rssi = "RSSI --";
String packSize = "--";
String packet ;
int relay1 = 21;
int relay2 = 25;
int relay3 = 33;
int relay4 = 32;
int relay5 = 0;
int relay6 = 22;
int relay7 = 2;
int relay8 = 17;
String* str= NULL;
void setup() {
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK,MISO,MOSI,SS);
  LoRa.setPins(SS,RST,DI0);  
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  //LoRa.onReceive(cbk);
  LoRa.receive();
  Serial.println("init ok");
  pinMode(relay1, OUTPUT);
   pinMode(relay2, OUTPUT);
   pinMode(relay3, OUTPUT);
   pinMode(relay4, OUTPUT);
   pinMode(relay5, OUTPUT);
   pinMode(relay6, OUTPUT);
   pinMode(relay7, OUTPUT);
   pinMode(relay8, OUTPUT);

  }

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available()) 
    {
       String LoRaData = LoRa.readString();
       Serial.print(LoRaData);
       
       LoRaData.td_split( ".", &str );
       Serial.println();
       Serial.println(str[0]);
       Serial.println(str[1]);
       Serial.println(str[2]);
       
       if (str[0].toInt() > 100)
       {
    digitalWrite(relay8,1);
    digitalWrite(relay7,1);
    
    for(int i=0 ; i<16 ; i++)
            {
            digitalWrite(relay6,1);
            delay(500);
            digitalWrite(relay6,0);
            delay(500);
            }
    
    digitalWrite(relay7,0);
    digitalWrite(relay8,0);
      
      }
     delay(1000);

       
       if (str[1].toInt() > 60)
       {
    digitalWrite(relay8,1);
    digitalWrite(relay7,1);
    
    for(int i=0 ; i<16 ; i++)
            {
            digitalWrite(relay5,1);
            delay(500);
            digitalWrite(relay5,0);
            delay(500);
            }
    
    digitalWrite(relay7,0);
    digitalWrite(relay8,0);
      
      }
      
    }

    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
