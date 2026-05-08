#include "bme280.h"
#include <Wire.h>
#include <Adafruit_BME280.h>

static Adafruit_BME280 bme;
static bool bme_ok = false;

void bme280Init() {
  bme_ok = bme.begin(0x76);
  if (!bme_ok) Serial.println("[bme280] not found at 0x76");
  else Serial.println("[bme280] OK");
}

SensorReading bme280Read() {
  if (!bme_ok) return { 0.0f, 0.0f, 0.0f };
  return {
    bme.readTemperature(),
    bme.readHumidity(),
    bme.readPressure() / 100.0f
  };
}
