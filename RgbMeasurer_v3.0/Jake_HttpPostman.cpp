#pragma once

#include "Jake_HttpPostman.h"

HttpPostman::HttpPostman(void) : debug(true) {
	
}

void HttpPostman::begin(bool _debug) {
	debug = _debug;
	
	if(debug) {
      Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);
    } else {
      Phpoc.begin();
    }
}

void HttpPostman::sendLightChar(TCS34725 &inst) {
  if(client.connect(server, 80)) {
    bool received = false;
    String body = "r=" + String((unsigned long)inst.r) +
                  "&g=" + String((unsigned long)inst.g) +
                  "&b=" + String((unsigned long)inst.b) +
                  "&r_comp=" + String((unsigned long)inst.r_comp) +
                  "&g_comp=" + String((unsigned long)inst.g_comp) +
                  "&b_comp=" + String((unsigned long)inst.b_comp) +
                  "&lux=" + String(inst.lux, 2) +
                  "&cct=" + String(inst.cct, 2);

    Serial.println("send> " + body);
    
    // send HTTP POST request packet to server
    client.println(F("POST /insert/rgb/ HTTP/1.0"));
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
