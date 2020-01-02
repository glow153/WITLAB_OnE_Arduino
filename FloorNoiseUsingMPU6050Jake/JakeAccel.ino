int16_t ax, ay, az;

void initAccel(void) {
  // join I2C bus (I2Cdev library doesn't do this automatically)
  #if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
    Wire.begin();
  #elif I2CDEV_IMPLEMENTATION == I2CDEV_BUILTIN_FASTWIRE
    Fastwire::setup(400, true);
  #endif
  Serial.begin(38400);

  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();

  // verify connection
  Serial.println("Testing device connections...");
  while(!accelgyro.testConnection()) ;
  Serial.println("MPU6050 connection successful");

  accelgyro.setFullScaleAccelRange(MPU6050_ACCEL_FS_2);
  accelgyro.setFullScaleGyroRange(MPU6050_GYRO_FS_250);
  accelgyro.setDLPFMode(MPU6050_DLPF_BW_5);
}

double calibration(bool debug) {
  double sum = 0.0;
  double mean = 0.0;
  int count = 0;
  const int calibrationTime = 3000; //the longer the more accurate
  Serial.println(F("Initializing calibrating measurement..."));
  Serial.println(F("Do not exert any vibration on sensor during calibration!"));
  long start = millis();
  while(millis() - start < calibrationTime) {
    double norm = getAccelNorm(false);
    sum += norm;
    mean = sum / (++count);
    if(debug) {
      Serial.print(F("**Dbg/getMean()** "));
      Serial.print(F("norm = "));Serial.print(norm, 5);Serial.print(F("m/s^2"));
      Serial.print(F("\tmean = "));Serial.print(mean, 5);Serial.println(F("m/s^2"));
    }
  }
  return mean;
}

double getAccelNorm(bool debug) {
  accelgyro.getAcceleration(&ax, &ay, &az);
  if(debug) {
    Serial.print(F("value from accelerometer : "));
    Serial.print(F("x:"));Serial.print(ax);
    Serial.print(F(", y:"));Serial.print(ay);
    Serial.print(F(", z:"));Serial.println(az);
  }
  double sqsum = sq(((double) ax / ACCEL_SENSITIVITY) * 9.80665)
      + sq(((double) ay / ACCEL_SENSITIVITY) * 9.80665)
      + sq(((double) az / ACCEL_SENSITIVITY) * 9.80665);
  return sqrt(sqsum);
}

