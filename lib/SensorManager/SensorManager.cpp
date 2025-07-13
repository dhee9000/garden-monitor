#include "SensorManager.h"

SensorManager::SensorManager(TwoWire* wire) : i2c(wire) {}

bool SensorManager::begin() {
    return aht10.begin(i2c);
}

bool SensorManager::read(float& temperature, float& humidity) {
    sensors_event_t humidity_event, temp_event;
    aht10.getEvent(&humidity_event, &temp_event);
    temperature = temp_event.temperature;
    humidity = humidity_event.relative_humidity;
    return true;
} 