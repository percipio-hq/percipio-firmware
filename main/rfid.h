#pragma once
#include "firebase.h"

void      rfidInit();
bool      rfidIsOk();  // true if MFRC522 firmware version is valid (0x91 or 0x92)
RfidEvent rfidLoop();  // returns event if card scanned, empty uid otherwise
