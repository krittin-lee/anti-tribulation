#include "FirebaseESP32.h"

//#include <Firebase.h>
//#include <FirebaseArduino.h>


#include <SPI.h>
#include <LoRa.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>

#include "DHT.h"
#define DHTPIN 2
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);
//#define LINE_TOKEN "bGtxcn58yTVD9O6gqFfRhOtazCflwQCtv7yT9JMcXXv"

#define FIREBASE_HOST "swk-project.firebaseio.com"
#define FIREBASE_AUTH "nf3f9zumxvl0f32AuqGm9OSs2Yvof0pqaqEWzFj3"

#define WIFI_STA_NAME "#fe000"
#define WIFI_STA_PASS "00000000"
FirebaseData firebaseData1;
FirebaseData firebaseData2;

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
String* str = NULL;

/*bool LINE_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("LINE connection failed");
    return false;
  }


  String payload = "message=" + message;
  String req = "";
  req += "POST /api/notify HTTP/1.1\r\n";
  req += "Host: notify-api.line.me\r\n";
  req += "Authorization: Bearer " + String(LINE_TOKEN) + "\r\n";
  req += "User-Agent: ESP32\r\n";
  req += "Content-Type: application/x-www-form-urlencoded\r\n";
  req += "Content-Length: " + String(payload.length()) + "\r\n";
  req += "\r\n";
  req += payload;
  // Serial.println(req);
  client.print(req);

  delay(20);


  long timeOut = millis() + 30000;

  while (client.connected() && timeOut > millis()) {
  if (client.available()) {
    String str = client.readString();
    // Serial.print(str);
  }
  delay(10);
  }
  return timeOut > millis();
  }
  /* if (millis() - sendDataPrevMillis1 > 15000)
  {
  sendDataPrevMillis1 = millis();
  count1++;


  }*/

void streamCallback(StreamData data)
{
  Serial.println("Stream Data 1 available...");
  Serial.println("STREAM PATH: " + data.streamPath());
  Serial.println("PATH: " + data.dataPath());
  Serial.println("TYPE: " + data.dataType());
  Serial.print("VALUE: ");
  if (data.dataType() == "int")
    Serial.println(data.intData());
  else if (data.dataType() == "float")
    Serial.println(data.floatData());
  else if (data.dataType() == "boolean")
    Serial.println(data.boolData());
  else if (data.dataType() == "string")
    Serial.println(data.stringData());
  else if (data.dataType() == "json")
    Serial.println(data.jsonData());
  else if (data.dataType() == "blob")
  {
    //See blob examples
  }
  Serial.println();
}



void setup() {

  Serial.begin(115200);
  dht.begin();
  Serial.println("Start Done");

  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver Callback");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(433E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }

  //LoRa.onReceive(cbk);
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
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);


  Firebase.setReadTimeout(firebaseData1, 1000 * 60);

  Firebase.setwriteSizeLimit(firebaseData1, "unlimited");



}

void loop() {
  int packetSize = LoRa.parsePacket();
  float t = dht.readTemperature();
  float h = dht.readHumidity();

  if (isnan(h) || isnan(t))
  {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
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
      int FarTemp = str[0].toInt();
      int FarHumi = str[1].toInt();
      int fire = str[2].toInt();
      int water = str[3].toInt();

      json = " { \"TempOut\":" + str[0] + "  ,\"Humidity\":" + str[1] + "  ,\"Farfire\":" + str[2] + "  ,\"water\":" + str[3] + "  ,\"CO\":" + smoke + " } ";

      Serial.println("------------------------------------");
      Serial.println("Update Data 1...");
      if (Firebase.updateNode(firebaseData1, path + "/Stream/data1", json))
      {
        Serial.println("PASSED");
        Serial.println("PATH: " + firebaseData1.dataPath());
        Serial.println("TYPE: " + firebaseData1.dataType());
        Serial.print("VALUE: ");
        if (firebaseData1.dataType() == "int")
          Serial.println(firebaseData1.intData());
        else if (firebaseData1.dataType() == "float")
          Serial.println(firebaseData1.floatData());
        else if (firebaseData1.dataType() == "boolean")
          Serial.println(firebaseData1.boolData());
        else if (firebaseData1.dataType() == "string")
          Serial.println(firebaseData1.stringData());
        else if (firebaseData1.dataType() == "json")
          Serial.println(firebaseData1.jsonData());
        Serial.println("------------------------------------");
        Serial.println();
      }
      else
      {
        Serial.println("FAILED");
        Serial.println("REASON: " + firebaseData1.errorReason());
        Serial.println("------------------------------------");
        Serial.println();
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
        //  LINE_Notify("There are some smoke incresing unnaturally (HOUSEFIRE)");



      }
      else {
        digitalWrite(relay7, LOW);
        digitalWrite(relay8, LOW);
        delay(200);

      }

      if (t >= 30)
      {
        digitalWrite(relay8, HIGH);

      }
      else {
        digitalWrite(relay8, LOW);

      }


      if (fire == 0) {
        digitalWrite(relay6, LOW);
        digitalWrite(relay5, LOW);
        digitalWrite(relay7, LOW);


      }
      if (fire == 1) {
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

        // LINE_Notify("Look like there is a wildfire outside (WILDFIRE LV.1)");


      }

      if (fire == 2) {
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
      /* if (water == 1) {
          Serial.println("Alert water lv.1!");
          digitalWrite(relay3, HIGH);
          digitalWrite(relay4, HIGH);
          delay(1000);
          digitalWrite(relay3, LOW);
          digitalWrite(relay4, LOW);
          delay(1000);
          digitalWrite(relay3, HIGH);
          digitalWrite(relay4, HIGH);
          delay(1000);
          digitalWrite(relay3, LOW);
          digitalWrite(relay4, LOW);
          delay(1000);
          digitalWrite(relay3, HIGH);
          digitalWrite(relay4, HIGH);
          delay(1000);
          digitalWrite(relay3, LOW);
          digitalWrite(relay4, LOW);
          delay(1000);
          LINE_Notify("%E0%B8%99%E0%B9%89%E0%B8%B3%E0%B8%97%E0%B9%88%E0%B8%A7%E0%B8%A1%20!!!");

        }*/
      if (water == 0) {
        digitalWrite(relay3, LOW);
        digitalWrite(relay4, LOW);
        digitalWrite(relay7, LOW);
        // Firebase.setInt("water", 0);

      }

      if (water == 2) {

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

        // Firebase.setInt("water", 2);
        // LINE_Notify("Water is rising!");


      }

      //
      /*  if (str[0].toInt() > 100)
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
      */
    }
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C "));


    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());

  }
}
