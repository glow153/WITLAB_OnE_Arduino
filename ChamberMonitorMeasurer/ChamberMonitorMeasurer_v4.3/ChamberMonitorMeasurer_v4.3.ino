#include <SPI.h>
#include <Phpoc.h>
#include "DHT.h"
PhpocClient client;
 
#define DHTPIN 2        // SDA 핀의 설정
#define DHTTYPE DHT22   // DHT22 (AM2302) 센서종류 설정

DHT dht(DHTPIN, DHTTYPE);
float temp, humi;
const char* server = "210.102.142.14";
bool debug = true;

void setup() {
  // put your setup code here, to run once:
  dht.begin();
  if(debug) {
    Serial.begin(9600);
    Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);
  } else {
    Phpoc.begin();
  }
}

void loop() {
  measureTempHumi();
  sendTempHumi(temp, humi);
  delay(2000);
}

void measureTempHumi() {
  temp = dht.readTemperature();
  humi = dht.readHumidity();
  if (isnan(temp) || isnan(humi)){
    temp = -999.0;
    humi = -999.0;
  }
}

void sendTempHumi(float temp, float humi) {
  if(client.connect(server, 80)) {
    bool received = false;
    String body = "temp=" + String(temp, 1) + "&humi=" + String(humi, 1);
    if(debug)
      Serial.println("send> " + body);
    client.println(F("POST /insert/chamber_temphumi/ HTTP/1.0"));
    client.print(F("Host: ")); client.println(server);
    client.println(F("Content-Type: application/x-www-form-urlencoded"));
    client.print(F("Content-Length: ")); client.println(body.length());
    client.println();
    client.println(body);

    if(debug) {
      while(1) {
        if(client.available()) {
          received = true;
          Serial.print((char)client.read());  
        }
        if(!client.available() && received)
          break;
      }
      Serial.println();
    }
    client.stop();
  }
}

