
void setup() {
  Serial.begin(115200);  //initial the Serial
}

void loop() {
  if (Serial.available()) {
    for (int i=0;i<Serial.available();i++){
      Serial.print(String((int)Serial.read()) + " ");
    }
    Serial.println();
  }
}
