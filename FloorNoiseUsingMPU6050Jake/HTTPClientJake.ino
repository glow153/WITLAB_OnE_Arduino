PhpocClient client;

void initShield(bool debug) {
  if (debug) {
    Phpoc.begin(PF_LOG_SPI | PF_LOG_NET);
  } else {
    Phpoc.begin();
  }
}

bool connectToServer(void) {
  Serial.println(F("<ARD MSG> Trying to connect to AWS"));
  isConnected = true;
  return isConnected;
}

void upload(int id, double value, bool debug) {
  String httpBody = "sensor_id=" + String(id) + "&value=" + String(value, 4);
  int contentLen = httpBody.length();

   Serial.println(httpBody);
  client.print(F("POST ")); client.print(dirName); client.print(phpName); client.println(" HTTP/1.1");
  client.print(F("Host: ")); client.println(server_name);
  client.println(F("Content-Type: application/x-www-form-urlencoded"));
  client.print(F("Content-Length: ")); client.println(contentLen);
  client.println();
  client.println(httpBody);
  if(debug) {
    Serial.print(F("<ARD MSG> http send : "));Serial.println(httpBody);
  }
}

