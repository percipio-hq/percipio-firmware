#pragma once
#include "firebase.h"

void      radarInit();
RadarData radarRead(); // parses latest LD2450 UART frame
