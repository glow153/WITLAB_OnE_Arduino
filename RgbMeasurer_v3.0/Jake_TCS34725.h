#pragma once

#include <Wire.h>
#include "Adafruit_TCS34725.h"

#define TCS34725_R_Coef 0.136 
#define TCS34725_G_Coef 1.000
#define TCS34725_B_Coef -0.444
#define TCS34725_GA 1.0
#define TCS34725_DF 310.0
#define TCS34725_CT_Coef 3810.0
#define TCS34725_CT_Offset 1391.0

class TCS34725 {
private:
  struct tcs_agc {
    tcs34725Gain_t ag;
    tcs34725IntegrationTime_t at;
    uint16_t mincnt;
    uint16_t maxcnt;
  };
  Adafruit_TCS34725 tcs;
  static const tcs_agc agc_lst[];
  uint16_t agc_cur;

  void setGainTime(void);  
  
public:
  TCS34725(void);
  boolean begin(void);
  void getData(void);  
  void printResult();

  boolean isAvailable, isSaturated;
  uint16_t againx, atime, atime_ms;
  uint16_t r, g, b, c;
  uint16_t ir; 
  uint16_t r_comp, g_comp, b_comp, c_comp;
  uint16_t saturation, saturation75;
  float cratio, cpl, cct, lux, maxlux;
};
