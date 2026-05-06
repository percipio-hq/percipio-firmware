#include <SparkFun_APDS9960.h>

SparkFun_APDS9960 apds;

void setup() {
  Serial.begin(115200);
  if (!apds.init()) {
    Serial.println("APDS-9960 not found — check wiring (SDA=21, SCL=22, VCC=3.3V)");
    while (1) delay(1000);
  }
  apds.enableGestureSensor(false);
  Serial.println("APDS-9960 OK — swipe over sensor");
}

void loop() {
  if (apds.isGestureAvailable()) {
    switch (apds.readGesture()) {
      case DIR_UP:    Serial.println("Gesture: UP");    break;
      case DIR_DOWN:  Serial.println("Gesture: DOWN");  break;
      case DIR_LEFT:  Serial.println("Gesture: LEFT");  break;
      case DIR_RIGHT: Serial.println("Gesture: RIGHT"); break;
      case DIR_NEAR:  Serial.println("Gesture: NEAR");  break;
      case DIR_FAR:   Serial.println("Gesture: FAR");   break;
      default:        Serial.println("Gesture: NONE");  break;
    }
  }
}
