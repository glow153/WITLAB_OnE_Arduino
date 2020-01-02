#include <SoftwareSerial.h>

const int blueTx = 11, blueRx = 10;
SoftwareSerial bluetooth(blueTx, blueRx);
String sInput="";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); 
  bluetooth.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(bluetooth.available()){
    while(bluetooth.available()){
      char ch = (char)bluetooth.read();
      sInput += ch;
      delay(5); //수신 문자열 끊김 방지
    }
  } else if (Serial.available()) {
    char c = (char)Serial.read();
    bluetooth.write(c);
  }

  if(!sInput.equals("")){  //sInput 값이 있다면
    Serial.println("input value: "+sInput); //시리얼모니터에 sInput값 출력
    if(sInput=="on"){  //sInput 값이 'on' 이라면
      digitalWrite(13, HIGH); //LED ON
    } else {
      digitalWrite(13, LOW);  //LED OFF
    }
    sInput = "";
  }
}
