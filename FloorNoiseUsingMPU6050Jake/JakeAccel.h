#include "I2Cdev.h"
#include "MPU6050.h"
#if I2CDEV_IMPLEMENTATION == I2CDEV_ARDUINO_WIRE
  #include "Wire.h"
#endif
#define ACCEL_SENSITIVITY 16384

MPU6050 accelgyro;

void initAccel(void);
double calibration(bool debug);
double getAccelNorm(bool debug);

