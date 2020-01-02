#include "JakeAccel.h"
#include "HTTPClientJake.h"

const char server_name[] = "13.124.129.42";
const char dirName[] = "/floornoise/";
const char phpName[] = "vibupdate.php";
const int sensor_id = 101;

double g_mean;
extern PhpocClient client;
bool isConnected = false;

void setup() {
  initAccel();
  initShield(true);
  g_mean = calibration(false);
  Serial.print(F("mean = ")); Serial.println(g_mean, 5);
}

void loop() {
  int srlAvl = 0;

  if ((srlAvl = Serial.available()) > 0) {
    int idx;
    char cmd[32];
    for (idx = 0; idx < (srlAvl < 31 ? srlAvl : 31); idx++) //input user command
      cmd[idx] = (char)Serial.read();
    cmd[idx] = (char)0x00;
    Serial.print(F("<USER> ")); Serial.println(cmd);
    translateUserCommand(cmd);
  }

  if (isConnected) {
    while (!client.connect(server_name, 80)) { //connect to server
      Serial.println(F("<ARD MSG> reconnection failed. retrying..."));
      delay(1000);
    }
    double vibration = g_mean - getAccelNorm(false);
    upload(sensor_id, vibration, true); //upload value
    client.stop(); //disconnect from server
  } else {
    delay(100);
  }
}

void translateUserCommand(String cmd) {
  if (cmd == "stop") {
    client.stop();
    isConnected = false;
    Serial.println(F("<ARD MSG> connection has been cancelled."));
  } else if (cmd == "start") {
    if (!isConnected)
      connectToServer();
    else
      Serial.println(F("<ARD MSG> already connected."));
  }
}
