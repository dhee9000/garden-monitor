#pragma once
#include <Adafruit_AHTX0.h>
#include <Wire.h>

class SensorManager {
public:
    SensorManager(TwoWire* wire);
    bool begin();
    bool read(float& temperature, float& humidity);
private:
    Adafruit_AHTX0 aht10;
    TwoWire* i2c;
}; 