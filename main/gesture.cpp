#include "gesture.h"
#include "display.h"
#include <SparkFun_APDS9960.h>

static SparkFun_APDS9960 apds;
static bool ready = false;

void gestureInit() {
  if (!apds.init()) {
    Serial.println("[gesture] APDS-9960 init failed — check wiring");
    return;
  }
  if (!apds.enableGestureSensor(false)) { // false = polling, no interrupt pin needed
    Serial.println("[gesture] enableGestureSensor failed");
    return;
  }
  ready = true;
  Serial.println("[gesture] APDS-9960 OK");
}

void gestureLoop() {
  if (!ready) return;
  if (!apds.isGestureAvailable()) return;

  switch (apds.readGesture()) {
    case DIR_RIGHT: displayNextScreen(); break;
    case DIR_LEFT:  displayPrevScreen(); break;
    default: break; // UP/DOWN ignored
  }
}
