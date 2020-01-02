#pragma once
#include "Jake_ADXL345_init.h"

/* Assign a unique ID to this sensor at the same time */
Adafruit_ADXL345_Unified accel;

void setup(void) {
    Serial.begin(115200);
    initADXL345();
}

void loop(void) {
    /* Get a new sensor event */
    sensors_event_t event;
    accel.getEvent(&event);
    Serial.print("norm:");
    Serial.println(getNorm(event));
}

double getNorm(const sensors_event_t& e) {
    double x = e.acceleration.x;
    double y = e.acceleration.y;
    double z = e.acceleration.z;
    return sqrt(sq(x) + sq(y) + sq(z));
}

void calib(const sensors_event_t& e) {
    double initValMin = 157.0;
    double initValMax = -157.0;
    double avg = 0.0;
    int count = 0;
    long startMs = millis();
    while(millis() - startMs < 2000) {
        double sample = getNorm(e);
        if(sample < initValMin)
            initValMin = sample;
        else if(sample > initValMax)
            initValMax = sample;
        avg += sample;
        count++;
    }

    avg /= count;
}

