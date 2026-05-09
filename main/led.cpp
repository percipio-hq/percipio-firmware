#include "led.h"
#include "config.h"
#include <Arduino.h>

static const unsigned long GREEN_MS   = 3000;
static const unsigned long RED_MS     = 5000;
static const unsigned long BLINK_HALF = 200; // ms per half-period

enum LedMode { LED_OFF, LED_GREEN_PULSE, LED_RED_BLINK };

static LedMode       mode      = LED_OFF;
static unsigned long modeStart = 0;
static unsigned long lastBlink = 0;
static bool          blinkHigh = false;

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
  mode      = LED_GREEN_PULSE;
  modeStart = millis();
  digitalWrite(PIN_LED_GREEN, HIGH);
  digitalWrite(PIN_LED_RED,   LOW);
}

void ledRedBlink() {
  mode      = LED_RED_BLINK;
  modeStart = millis();
  lastBlink = millis();
  blinkHigh = true;
  digitalWrite(PIN_LED_RED,   HIGH);
  digitalWrite(PIN_LED_GREEN, LOW);
}

void ledUpdate() {
  if (mode == LED_OFF) return;

  unsigned long now = millis();

  if (mode == LED_GREEN_PULSE) {
    if (now - modeStart >= GREEN_MS) {
      digitalWrite(PIN_LED_GREEN, LOW);
      mode = LED_OFF;
    }
  } else { // LED_RED_BLINK
    if (now - modeStart >= RED_MS) {
      digitalWrite(PIN_LED_RED, LOW);
      mode = LED_OFF;
    } else if (now - lastBlink >= BLINK_HALF) {
      lastBlink = now;
      blinkHigh = !blinkHigh;
      digitalWrite(PIN_LED_RED, blinkHigh ? HIGH : LOW);
    }
  }
}
