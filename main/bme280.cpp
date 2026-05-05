#include "bme280.h"
#include <Wire.h>
#include <Adafruit_BME280.h>

static Adafruit_BME280 bme;

void bme280Init() {
  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found at 0x76");
  }
}

SensorReading bme280Read() {
  return {
    bme.readTemperature(),
    bme.readHumidity(),
    bme.readPressure() / 100.0f
  };
}
