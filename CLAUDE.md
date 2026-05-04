# Percipio Firmware

ESP32 C++/Arduino firmware for the Percipio room awareness system.

## Board
TTGO T-Display (ESP32 + built-in 240×135px TFT screen).
Select **NodeMCU-32s** in Arduino IDE (under ESP32 Arduino).

## Libraries required
- TFT_eSPI — configure `User_Setup_Select.h` → `Setup25_TTGO_T_Display.h`
- SparkFun APDS9960
- MFRC522
- Adafruit BME280
- Firebase Arduino Client Library for ESP32 (mobizt)
- ArduinoJson
- UniversalTelegramBot

## File structure
```
main/
  main.ino            — entry point: setup() + loop()
  config.h            — WiFi + API keys (gitignored — copy from config.h.example)
  config.h.example    — template to copy
  display.h/.cpp      — TTGO TFT: 3 rotating screens
  gesture.h/.cpp      — APDS-9960: swipe navigation
  bme280.h/.cpp       — temperature, humidity, pressure
  rfid.h/.cpp         — MFRC522 card reader
  radar.h/.cpp        — LD2450 UART frame parser
  firebase.h/.cpp     — Firestore writes
  telegram.h/.cpp     — bot notifications
  led.h/.cpp          — red / green / blue LED feedback
test_sketches/
  test_bme280.ino
  test_rfid.ino
  test_radar.ino
  test_gesture.ino
  test_display.ino
  test_led.ino
```

## Config setup
```
cp main/config.h.example main/config.h
```
Fill in WiFi credentials, Firebase API key, and Telegram tokens.

## Flash order (at lab session)
1. `test_bme280.ino` → verify `T=22.4 H=48.0 P=1013.2` in Serial Monitor
2. `test_rfid.ino` → verify card UID printed
3. `test_display.ino` → verify screen lights up
4. `test_gesture.ino` → verify LEFT/RIGHT/UP/DOWN detected
5. `test_led.ino` → verify green / red blink / blue
6. `test_radar.ino` → verify target X/Y/speed printed
7. `main.ino` → full system

## Key pin rules (ESP32)
- GPIO 6–11: never use (connected to flash)
- GPIO 34–39: input only, no PWM
- ADC2 pins: avoid when WiFi is active
