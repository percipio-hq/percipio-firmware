#pragma once
#include "firebase.h"

void      rfidInit();
RfidEvent rfidLoop(); // returns event if card scanned, empty uid otherwise
