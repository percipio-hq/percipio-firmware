#pragma once
#include "firebase.h"

void      radarInit();
bool      radarIsOk();  // true after first valid frame received
RadarData radarRead();  // parses latest LD2450 UART frame
