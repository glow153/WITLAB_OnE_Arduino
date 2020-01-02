#define DEBUG 1

#include <SoftwareSerial.h>

#define TOCON_E2_PIN A0
#define DEV_ID      1
#define OLED_MOSI   8
#define OLED_CLK    9
#define OLED_DC    10
#define OLED_CS    11
#define OLED_RESET 12

#define BLE_TX 3
#define BLE_RX 2
SoftwareSerial hm10(BLE_TX, BLE_RX);

#define PACKET_TX_SYNC_LEN 16
#define PACKET_RX_LEN 20

unsigned long long lastMillis = 0;

const byte b_flag = (byte)0xff;
//const unsigned long ul_flag = (unsigned long)0xff;
String datetime = "2018-04-16 00:00"; //default value
bool bConnect = true;
int connectFailCount = 0;

byte txpktSync[] = {
  (byte)0x02, //stx
  (byte)0xff,
  (byte)DEV_ID, //device id
  (byte)0xff,
  (byte)0x00, (byte)0x00, (byte)0x00, (byte)0x00, //for uvi (idx : 4,5,6,7)
  (byte)0xff,
  (byte)0x00, //checksum (idx:9)
  (byte)0x03  //etx
};

void setup() {
  Serial.begin(9600);
  hm10.begin(9600);
}

void loop() {
  lastMillis = millis();

  byte packetRx[22];
  char badt[17]={0}; // MUST add null end!

  if (hm10.available()) { // read packet from app
    connectFailCount = 0;
    hm10.readBytes(packetRx, PACKET_RX_LEN);
    
#if DEBUG
    Serial.print(F("received packet : "));
    Serial.println(pktToString(packetRx, 0, PACKET_RX_LEN-1));
#endif

    for (int i = 0; i < 16; i++) //copy without null end
      badt[i] = (char)packetRx[4 + i];
    datetime = String(badt);
    
#if DEBUG
    Serial.print(F("received datetime : "));
    Serial.println(datetime);
#endif
  } else {
    connectFailCount += 1;
  }
  
  double uvi = measureUVI();
//  writeUvi2Packet(uvi);
  String msg = "UV:" + String(uvi, 2) + String('\0');
  byte tmpPacket[8] = {0};
  msg.getBytes(tmpPacket, 8);
  
#if DEBUG
  Serial.print(F("send packet> "));
//  Serial.println(pktToString(txpktSync, 0, PACKET_TX_SYNC_LEN-1));
  Serial.println(msg);
  Serial.print(F("connect failure count : "));
  Serial.println(connectFailCount);
#endif

  // send Packet
//  hm10.write(txpktSync, PACKET_TX_SYNC_LEN);
  hm10.write(tmpPacket, 7);
  
  if(connectFailCount <= 3) {
    bConnect = true;
  } else if (connectFailCount > 3) {
    bConnect = false;
    connectFailCount = 4;
  }
}

double measureUVI() {
  double sumUvi = 0.0;
  int count = 0;
  
  while (millis() - lastMillis < 1000) {
    double cuvi = 5000 * (analogRead(TOCON_E2_PIN) / 1024.0) / 170;
    sumUvi += cuvi;
    count++;
  }
  
  return sumUvi / count;
}

void writeUvi2Packet(double uvi) {
  byte *b = (byte *)(&uvi);

  for(int i=0;i<4;i++)
    txpktSync[7-i] = b[i];
  
#if DEBUG
  Serial.print(F("UVI) source: ")); Serial.print(uvi, 8);
  Serial.print(F(", byte array : ")); Serial.println(pktToString(txpktSync, 4, 7));
#endif
}

#if DEBUG
String pktToString(byte* packet, int startIdx, int endIdx) {
  String ret = "";
  for (int i = startIdx; i <= endIdx; i++) {
    String sb = String(packet[i], 16);
    if (sb.length() < 2)
      sb = "0" + sb;
    ret += (sb + " ");
  }
  return ret;
}
#endif
