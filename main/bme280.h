#pragma once
#include "firebase.h"

void          bme280Init();
bool          bme280IsOk();
SensorReading bme280Read(); // returns {0,0,0} if sensor not connected
