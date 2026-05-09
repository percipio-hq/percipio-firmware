#pragma once

void ledInit();
void ledSetWifi(bool connected);
void ledGreenPulse();  // known card  — starts 3s solid green (non-blocking)
void ledRedBlink();    // unknown card — starts 5s blinking red (non-blocking)
void ledUpdate();      // call every loop tick
