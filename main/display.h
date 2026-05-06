#pragma once
#include "firebase.h"

void displayInit();
void displayLoop();
void displayNextScreen();
void displayUpdateEnv(const SensorReading& r);
void displayUpdateRadar(const RadarData& d);
void displayUpdateRfid(const RfidEvent& e);
void displaySetStatus(bool wifiOk, bool dbOk);
