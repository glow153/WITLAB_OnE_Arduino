#include <Wire.h>
/* IMU Data */
int16_t accX, accY, accZ;
uint8_t i2cData[14]; // Buffer for I2C data

void setup() {
  Serial.begin(9600);
  Wire.begin();
  i2cData[0] = 7; // Set the sample rate to 1000Hz - 8kHz/(7+1) = 1000Hz
  i2cData[1] = 0x00; // Disable FSYNC and set 260 Hz Acc filtering, 256 Hz Gyro filtering, 8 KHz sampling
  i2cData[2] = 0x00; // Set Gyro Full Scale Range to ±250deg/s
  i2cData[3] = 0x00; // Set Accelerometer Full Scale Range to ±2g
  while(i2cWrite(0x19,i2cData,4,false)); // Write to all four registers at once
  while(i2cWrite(0x6B,0x01,true)); // PLL with X axis gyroscope reference and disable sleep mode
  while(i2cRead(0x75,i2cData,1));
  if(i2cData[0] != 0x68) { // Read "WHO_AM_I" register
    Serial.print(F("Error reading sensor"));
    while(1);
  }
  
  delay(100); // Wait for sensor to stabilize
}

void loop() {
  /* Update all the values */
  while(i2cRead(0x3B,i2cData,14));
  accX = ((i2cData[0] << 8) | i2cData[1]);
  accY = ((i2cData[2] << 8) | i2cData[3]);
  accZ = ((i2cData[4] << 8) | i2cData[5]);
  /* Print Data */
  display_formatted_float(accX, 5, 0, 3, false);
  display_formatted_float(accY, 5, 0, 3, false);
  display_formatted_float(accZ, 5, 0, 3, false);

  double sqsum = sq(((double) accX / 16384) * 9.80665)
      + sq(((double) accY / 16384) * 9.80665)
      + sq(((double) accZ / 16384) * 9.80665);
  double norm = sqrt(sqsum);

  Serial.print(F("\tnorm = "));Serial.print(norm, 5);Serial.println(F("m/s^2"));
  delay(100);
}

void display_formatted_float(double val, int characteristic, int mantissa, int blank, boolean linefeed) {
  char outString[16];
  int len;

  dtostrf(val, characteristic, mantissa, outString);
  len = strlen(outString);
  for(int i = 0; i < ((characteristic+mantissa+blank)-len); i++)
    Serial.print(F(" "));
  Serial.print(outString);
  if(linefeed)
    Serial.print(F("\n"));
}

