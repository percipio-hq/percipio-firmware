#include <Arduino.h>

#define PIN_LED_GREEN 25
#define PIN_LED_RED   32
#define PIN_LED_BLUE  33

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED_GREEN, OUTPUT);
  pinMode(PIN_LED_RED,   OUTPUT);
  pinMode(PIN_LED_BLUE,  OUTPUT);
  digitalWrite(PIN_LED_GREEN, LOW);
  digitalWrite(PIN_LED_RED,   LOW);
  digitalWrite(PIN_LED_BLUE,  LOW);
  Serial.println("LED test start");
}

void loop() {
  // GREEN — solid 3s (known card)
  Serial.println("GREEN on 3s");
  digitalWrite(PIN_LED_GREEN, HIGH);
  delay(3000);
  digitalWrite(PIN_LED_GREEN, LOW);
  delay(500);

  // RED — blink 5s (unknown card)
  Serial.println("RED blink 5s");
  unsigned long end = millis() + 5000;
  while (millis() < end) {
    digitalWrite(PIN_LED_RED, HIGH); delay(200);
    digitalWrite(PIN_LED_RED, LOW);  delay(200);
  }
  delay(500);

  // BLUE — solid 3s (WiFi connected)
  Serial.println("BLUE solid 3s");
  digitalWrite(PIN_LED_BLUE, HIGH);
  delay(3000);
  digitalWrite(PIN_LED_BLUE, LOW);
  delay(500);
}
