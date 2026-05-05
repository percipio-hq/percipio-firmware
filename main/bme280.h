#pragma once
#include "firebase.h"

void          bme280Init();
SensorReading bme280Read(); // returns temperature, humidity, pressure
