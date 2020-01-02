#pragma once

#include "Jake_TCS34725.h"

//TCS34725::TCS34725() : isAvailable(0) {
//}
//
//boolean TCS34725::begin(void) {
//  tcs = Adafruit_TCS34725(); // default setting (int time = 2.4ms, gain = 1x)
//  isAvailable = tcs.begin(); 
//  return(isAvailable);
//}
//
//
//void TCS34725::getData(void) {
//  // read the sensor and autorange if necessary
//  tcs.getRawData(&r, &g, &b, &c);
//
//  // DN40 calculations for eliminate ir
//  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;
//  r_comp = r - ir;
//  g_comp = g - ir;
//  b_comp = b - ir;
//  c_comp = c - ir;   
//
//  // from here, followed the definition, not DN40
//  lux = float(tcs.calculateLux(r_comp, g_comp, b_comp));
//  cct = float(tcs.calculateColorTemperature(r_comp, g_comp, b_comp));
//}
//
//void TCS34725::printResult() {
//  Serial.print("raw R: "); Serial.print(r, DEC); Serial.print(" ");
//  Serial.print("raw G: "); Serial.print(g, DEC); Serial.print(" ");
//  Serial.print("raw B: "); Serial.print(b, DEC); Serial.print(" ");
//  Serial.print("raw C: "); Serial.print(c, DEC); Serial.println();
//  Serial.print("R: "); Serial.print(r_comp, DEC); Serial.print(" ");
//  Serial.print("G: "); Serial.print(g_comp, DEC); Serial.print(" ");
//  Serial.print("B: "); Serial.print(b_comp, DEC); Serial.print(" ");
//  Serial.print("C: "); Serial.print(c_comp, DEC); Serial.println();
//  Serial.print("Lux: "); Serial.print(lux, DEC); Serial.println(" - ");
//  Serial.print("Color Temp: "); Serial.print(cct, DEC); Serial.println(" K - ");
//}


const TCS34725::tcs_agc TCS34725::agc_lst[] = {
  { TCS34725_GAIN_60X, TCS34725_INTEGRATIONTIME_700MS,     0, 47566 },
  { TCS34725_GAIN_16X, TCS34725_INTEGRATIONTIME_154MS,  3171, 63422 },
  { TCS34725_GAIN_4X,  TCS34725_INTEGRATIONTIME_154MS, 15855, 63422 },
  { TCS34725_GAIN_1X,  TCS34725_INTEGRATIONTIME_2_4MS,   248,     0 }
};

TCS34725::TCS34725() : agc_cur(0), isAvailable(0), isSaturated(0) {
}

boolean TCS34725::begin(void) {
  tcs = Adafruit_TCS34725(agc_lst[agc_cur].at, agc_lst[agc_cur].ag);
  if ((isAvailable = tcs.begin())) 
    setGainTime();
  return(isAvailable);
}

void TCS34725::setGainTime(void) {
  tcs.setGain(agc_lst[agc_cur].ag);
  tcs.setIntegrationTime(agc_lst[agc_cur].at);
  atime = int(agc_lst[agc_cur].at);
  atime_ms = ((256 - atime) * 2.4);  
  switch(agc_lst[agc_cur].ag) {
  case TCS34725_GAIN_1X: 
    againx = 1; 
    break;
  case TCS34725_GAIN_4X: 
    againx = 4; 
    break;
  case TCS34725_GAIN_16X: 
    againx = 16; 
    break;
  case TCS34725_GAIN_60X: 
    againx = 60; 
    break;
  }
}

void TCS34725::getData(void) {
  // read the sensor and autorange if necessary
  tcs.getRawData(&r, &g, &b, &c);
  while(1) {
    if (agc_lst[agc_cur].maxcnt && c > agc_lst[agc_cur].maxcnt) 
      agc_cur++;
    else if (agc_lst[agc_cur].mincnt && c < agc_lst[agc_cur].mincnt)
      agc_cur--;
    else break;

    setGainTime(); 
    delay((256 - atime) * 2.4 * 2); // shock absorber
    tcs.getRawData(&r, &g, &b, &c);
    break;    
  }

  // DN40 calculations
  ir = (r + g + b > c) ? (r + g + b - c) / 2 : 0;
  r_comp = r - ir;
  g_comp = g - ir;
  b_comp = b - ir;
  c_comp = c - ir;   
  cratio = float(ir) / float(c);

  saturation = ((256 - atime) > 63) ? 65535 : 1024 * (256 - atime);
  saturation75 = (atime_ms < 150) ? (saturation - saturation / 4) : saturation;
  isSaturated = (atime_ms < 150 && c > saturation75) ? 1 : 0;
  cpl = (atime_ms * againx) / (TCS34725_GA * TCS34725_DF); 
  maxlux = 65535 / (cpl * 3);

  lux = (TCS34725_R_Coef * float(r_comp) + TCS34725_G_Coef * float(g_comp) + TCS34725_B_Coef * float(b_comp)) / cpl;
  cct = TCS34725_CT_Coef * float(b_comp) / float(r_comp) + TCS34725_CT_Offset;
}

void TCS34725::printResult() {
  Serial.print(F("Gain:")); 
  Serial.print(againx); 
  Serial.print(F("x "));
  Serial.print(F("Time:")); 
  Serial.print(atime_ms); 
  Serial.print(F("ms (0x")); 
  Serial.print(atime, HEX);   
  Serial.println(F(")"));
  
  Serial.print(F("Raw R:")); 
  Serial.print(r); 
  Serial.print(F(" G:")); 
  Serial.print(g); 
  Serial.print(F(" B:")); 
  Serial.print(b); 
  Serial.print(F(" C:")); 
  Serial.println(c); 

  Serial.print(F("IR:")); 
  Serial.print(ir); 
  Serial.print(F(" CRATIO:"));
  Serial.print(cratio); 
  Serial.print(F(" Sat:"));
  Serial.print(saturation); 
  Serial.print(F(" Sat75:"));
  Serial.print(saturation75); 
  Serial.print(F(" "));
  Serial.println(isSaturated ? "*SATURATED*" : "");

  Serial.print(F("CPL:")); 
  Serial.print(cpl);
  Serial.print(F(" Max lux:")); 
  Serial.println(maxlux);

  Serial.print(F("Compensated R:")); 
  Serial.print(r_comp); 
  Serial.print(F(" G:")); 
  Serial.print(g_comp); 
  Serial.print(F(" B:")); 
  Serial.print(b_comp); 
  Serial.print(F(" C:")); 
  Serial.println(c_comp); 
  
  Serial.print(F("Lux:")); 
  Serial.print(lux);
  Serial.print(F(" CCT:")); 
  Serial.print(cct);
  Serial.println(F("K"));
}
