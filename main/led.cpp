#include "led.h"
#include "config.h"
#include <Arduino.h>

void ledInit() {
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED,   OUTPUT);
  pinMode(PIN_LED_BLUE,  OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED,   LOW);
  digitalWrite(PIN_LED_BLUE,  LOW);
}

void ledSetWifi(bool connected) {
  digitalWrite(PIN_LED_BLUE, connected ? HIGH : LOW);
}

void ledGreenPulse() {
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(3000);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void ledRedBlink() {
  unsigned long end = millis() + 5000;
  while (millis() < end) {
    digitalWrite(PIN_LED_RED, HIGH); delay(200);
    digitalWrite(PIN_LED_RED, LOW);  delay(200);
  }
}
