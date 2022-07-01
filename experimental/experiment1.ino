#include <SPI.h>
#include <LoRa.h>

#include <WiFi.h>
#include <WiFiClientSecure.h>

#define LINE_TOKEN "bGtxcn58yTVD9O6gqFfRhOtazCflwQCtv7yT9JMcXXv"

#define WIFI_STA_NAME "!?!"
#define WIFI_STA_PASS "12345677"

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

bool LINE_Notify(String message) {
  WiFiClientSecure client;

  if (!client.connect("notify-api.line.me", 443)) {
    Serial.println("connection failed");
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

  // Serial.println("-------------");
  long timeOut = millis() + 30000;
  while(client.connected() && timeOut > millis()) {
    if (client.available()) {
      String str = client.readString();
      // Serial.print(str);
    }
    delay(10);
  }
  // Serial.println("-------------");

  return timeOut > millis();
}

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
    Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(WIFI_STA_NAME);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_STA_NAME, WIFI_STA_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }

  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

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
       
       LoRaData.td_split( "/", &str );
       Serial.println();
       Serial.println(str[0]);
       Serial.println(str[1]);
       Serial.println(str[2]);
       Serial.println(str[3]);
       int fire = str[2].toInt();
       int water = str[3].toInt();
       
       if (fire == 2){
        Serial.println("Alert fire !");
        LINE_Notify("%E0%B9%84%E0%B8%9F%E0%B9%84%E0%B8%AB%E0%B8%A1%E0%B9%89%20!!!");
        
       }
       if (water == 2){
        Serial.println("Alert water !");
        LINE_Notify("%E0%B8%99%E0%B9%89%E0%B8%B3%E0%B8%97%E0%B9%88%E0%B8%A7%E0%B8%A1%20!!!");
        
       }
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
  
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
  }
}
