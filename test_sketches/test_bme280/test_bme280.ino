#include <Wire.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

void setup() {
  Serial.begin(115200);
  if (!bme.begin(0x76)) {
    Serial.println("BME280 not found — check wiring and I2C address");
    while (1) delay(1000);
  }
  Serial.println("BME280 OK");
}

void loop() {
  Serial.printf("T=%.1f  H=%.1f  P=%.1f\n",
    bme.readTemperature(),
    bme.readHumidity(),
    bme.readPressure() / 100.0f);
  delay(2000);
}
