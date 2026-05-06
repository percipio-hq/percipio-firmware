#include <WiFi.h>
#include "config.h"
#include "led.h"
#include "firebase.h"
#include "display.h"
#include "gesture.h"
#include "bme280.h"
#include "rfid.h"
#include "radar.h"
#include "telegram.h"

static unsigned long lastSend = 0;
static const unsigned long SEND_INTERVAL = 10000; // ms

void setup() {
  Serial.begin(115200);
  Serial.println("\n[Percipio] booting...");

  ledInit();

  // WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("[WiFi] connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" connected");
  Serial.print("[WiFi] IP: ");
  Serial.println(WiFi.localIP());
  ledSetWifi(true);

  firebaseInit();

  bme280Init();
  displayInit();
  displaySetStatus(true, false);
  gestureInit();

  rfidInit();

  // Phase 3 — uncomment when radar is implemented
  // radarInit();

  Serial.println("[Percipio] ready");
}

void loop() {
  if (millis() - lastSend >= SEND_INTERVAL) {
    lastSend = millis();

    SensorReading reading = bme280Read();
    firebaseSendReading(reading);
    displayUpdateEnv(reading);
    displaySetStatus(WiFi.status() == WL_CONNECTED, true);
  }

  gestureLoop();
  displayLoop();

  RfidEvent evt = rfidLoop();
  if (evt.uid[0] != '\0') {
    firebaseSendRfid(evt);
    displayUpdateRfid(evt);
  }
}
