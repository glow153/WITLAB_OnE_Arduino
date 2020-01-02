#include <SoftwareSerial.h>

const int tx = 2;
const int rx = 3;
SoftwareSerial hm10(tx, rx);

String pktToString(byte* packet, int len) {
  String ret = "";
  for (int i = 0; i < len; i++) {
//    String sb = String(packet[i], 16);
    String sb = String((int)packet[i]);
    if (sb.length() < 2)
      sb = "0" + sb;
    ret += (sb + " ");
  }
  return ret;
}

void setup() {
  Serial.begin(9600);
  hm10.begin(9600);
}

void loop() {
  if (hm10.available() == 6) {
    byte packet[6] = {0};
    for (int i = 0; i < 6; i++)
      packet[i] = hm10.read();
    Serial.println(pktToString(packet, 6));
  }
//  while (Serial.available()) {
//    byte data = Serial.read();
//    hm10.write(data);
//  }
}

