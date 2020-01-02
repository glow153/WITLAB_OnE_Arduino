#pragma once
#include "Jake_TCS34725.h"
#include "Jake_HttpPostman.h"

TCS34725 rgb_sensor;
HttpPostman postman;
long lastMs, currentMs;

void setup(void) {
  Serial.begin(115200);
  Serial.println("welcome to Jake's TCS34725 Sensor Measurer! :)");
  rgb_sensor.begin();
  Serial.println("rgb sensor init complete");
  postman.begin(true);
  Serial.println("WiFi init complete");  
  
//  pinMode(4, OUTPUT);
//  digitalWrite(4, LOW); // @gremlins Bright light, bright light!

  currentMs = lastMs = millis();
}


bool firsttime = true;
void loop(void) {
  if (firsttime) {
    rgb_sensor.getData();
    postman.sendLightChar(rgb_sensor);
    firsttime = false;
  }
  
  currentMs = millis();
  
  if(currentMs - lastMs > 60000){
    lastMs = millis();
    rgb_sensor.getData();
    postman.sendLightChar(rgb_sensor);
  }
}
