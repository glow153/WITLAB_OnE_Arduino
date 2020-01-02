#include <SoftwareSerial.h>

SoftwareSerial hm10(5, 6); // RX, TX

void setup() {
  Serial.begin(115200);  //initial the Serial
  hm10.begin(115200);
}

void loop() {
  int serial_len, hm10_len;
  if ((serial_len = Serial.available())) {
    Serial.print("received from bluno(" + String(serial_len) + "): ");
    for (int i=0;i<serial_len;i++)
      Serial.print(String((int)Serial.read()) + " ");
    Serial.println();
  }
  if ((hm10_len = hm10.available())) {
    Serial.print("received from hm10(" + String(hm10_len) + "): ");
    for (int i=0;i<hm10_len;i++)
      Serial.print(String((int)hm10.read()) + " ");
    Serial.println();
  }
}
