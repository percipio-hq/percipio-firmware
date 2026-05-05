#pragma once

void ledInit();
void ledSetWifi(bool connected);
void ledGreenPulse();  // known card — 3s solid
void ledRedBlink();    // unknown card — 5s blink
