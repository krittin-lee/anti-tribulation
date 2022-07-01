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
String V;

unsigned long sendDataPrevMillis1;
uint16_t count1;
String json;
String path = "";

/*int relay1 = 21;
  int relay2 = 25;*/
int relay3 = 33;
int relay4 = 32;
int relay5 = 23;
int relay6 = 16;
int relay7 = 25;
int relay8 = 4;
String hosefire;
String waterr;
String firee;
int FarTemp;
int FarHumi;
int fire;
int water;
String* str = NULL;

void setup() {

  Serial.begin(115200);
  Serial.println("Start Done!");
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed! Sadddddd");
    while (1);
  }

  LoRa.receive();
  Serial.println("init ok");
  /*  pinMode(relay1, OUTPUT);
    pinMode(relay2, OUTPUT);*/
  pinMode(relay3, OUTPUT);
  pinMode(relay4, OUTPUT);
  pinMode(relay5, OUTPUT);
  pinMode(relay6, OUTPUT);
  pinMode(relay7, OUTPUT);
  pinMode(relay8, OUTPUT);
  Serial.println();
  Serial.println();


}

void loop() {
  int packetSize = LoRa.parsePacket();

  int smoke = analogRead(37);

  if (packetSize)
  {
    // received a packet
    Serial.print("Received packet '");

    // read packet
    while (LoRa.available())
    {
      String LoRaData = LoRa.readString();
      Serial.print(LoRaData);
      LoRaData.td_split( "/", &str );
      Serial.println();
      Serial.println(str[0]);
      Serial.println(str[1]);
      Serial.println(str[2]);
      Serial.println(str[3]);
      Serial.println(str[4]);
      Serial.println("Hose smoke");
      Serial.println(smoke);
      /* int FarTemp = str[0].toInt();
        int FarHumi = str[1].toInt();
        int fire = str[2].toInt();
        int water = str[3].toInt();
      */
      FarTemp = str[0].toInt();
      FarHumi = str[1].toInt();
      fire = str[2].toInt();
      water = str[3].toInt();
      json = " { \"TempOut\":" + str[0] + "  ,\"Humidity\":" + str[1] + "  ,\"Farfire\":" + str[2] + "  ,\"water\":" + str[3] + "  ,\"CO\":" + smoke + " } ";


    }

    if (smoke >= 850)
    {
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      digitalWrite(relay7, HIGH);
      digitalWrite(relay8, HIGH);
      delay(500);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      // digitalWrite(relay7, LOW);
      // digitalWrite(relay8, LOW);
      delay(500);
      digitalWrite(relay3, HIGH);
      digitalWrite(relay4, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay6, HIGH);
      //digitalWrite(relay7, HIGH);
      // digitalWrite(relay8, HIGH);
      delay(500);
      digitalWrite(relay3, LOW);
      digitalWrite(relay4, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay6, LOW);
      //digitalWrite(relay7, LOW);
      //digitalWrite(relay8, LOW);
      delay(500);
    }
    else {
      digitalWrite(relay7, LOW);
      digitalWrite(relay8, LOW);
      delay(200);
    }



    if (fire == 0) {
      digitalWrite(relay6, LOW);
      digitalWrite(relay5, LOW);
      digitalWrite(relay7, LOW);


    }
    else if (fire == 1) {
      Serial.println("Alert fire lv.1 !");
      digitalWrite(relay6, HIGH);

      delay(1000);
      digitalWrite(relay6, LOW);

      delay(1000);
      digitalWrite(relay6, HIGH);

      delay(1000);
      digitalWrite(relay6, LOW);

      delay(1000);
      digitalWrite(relay6, HIGH);

      delay(1000);




    }

    else if (fire == 2) {
      Serial.println("Alert fire lv.2 !");

      digitalWrite(relay6, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay7, HIGH);
      delay(1000);
      digitalWrite(relay6, LOW);
      digitalWrite(relay5, LOW);
      delay(1000);
      digitalWrite(relay6, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay7, HIGH);
      delay(1000);
      digitalWrite(relay6, LOW);
      digitalWrite(relay5, LOW);
      delay(1000);
      digitalWrite(relay6, HIGH);
      digitalWrite(relay5, HIGH);
      digitalWrite(relay7, HIGH);
      delay(1000);


      //   LINE_Notify("There is a 'serious' wildfire outside (WILDFIRE LV.2)");

    }


  }
  if (water == 0) {
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    digitalWrite(relay7, LOW);
    // Firebase.setInt("water", 0);

  }

  else if (water == 2) {

    Serial.println("Alert water!");
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay7, HIGH);
    delay(500);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    delay(500);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);

    delay(500);
    digitalWrite(relay3, LOW);
    digitalWrite(relay4, LOW);
    delay(500);
    digitalWrite(relay3, HIGH);
    digitalWrite(relay4, HIGH);
    digitalWrite(relay7, HIGH);
    delay(500);
  }


// print RSSI of packet
Serial.print("' with RSSI ");
Serial.println(LoRa.packetRssi());


}
