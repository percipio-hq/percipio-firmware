# Percipio — Wiring Guide

GPIO assignments for TTGO T-Display. Verify against this file before flashing.

---

## TTGO T-Display — built-in pins (do not use)

| GPIO | Used by |
|---|---|
| 4 | TFT backlight |
| 5 | TFT CS |
| 14 | ADC power control |
| 16 | TFT DC |
| 18 | TFT SCLK |
| 19 | TFT MOSI |
| 21 | I2C SDA |
| 22 | I2C SCL |
| 23 | TFT RST |
| 34 | ADC input (input only) |
| 35 | Button 1 (input only) |
| 0 | Button 2 |

---

## BME280 — environment sensor

I2C, 3.3V. Shares bus with APDS-9960.

| BME280 pin | Connects to |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |

I2C address: `0x76` (SDO → GND)

---

## APDS-9960 — gesture sensor

I2C, **must be 3.3V** (5V will damage it). Shares bus with BME280.

| APDS-9960 pin | Connects to |
|---|---|
| VCC | 3.3V |
| GND | GND |
| SDA | GPIO 21 |
| SCL | GPIO 22 |
| INT | not connected (polling used) |
| VL | 3.3V (IR LED power) |

I2C address: `0x39`

---

## MFRC522 — RFID reader

SPI, 3.3V. Shares VSPI bus with TFT (safe — TFT is write-only, no MISO line).

| MFRC522 pin | Connects to |
|---|---|
| VCC (3.3V) | 3.3V |
| GND | GND |
| SCK | GPIO 18 (shared VSPI) |
| MOSI | GPIO 19 (shared VSPI) |
| MISO | GPIO 2 |
| CS (SDA) | GPIO 27 |
| RST | GPIO 26 |
| IRQ | not connected |

---

## LD2450 — 24GHz radar

UART, 5V power, 3.3V logic (direct connect to ESP32, no level shifter needed).

| LD2450 pin | Connects to |
|---|---|
| VCC | 5V (USB pin) |
| GND | GND |
| TX | GPIO 13 (ESP32 receives) |
| RX | GPIO 17 (ESP32 transmits) |

UART: 256000 baud, 8N1

---

## LEDs (red + green + blue)

Each LED: anode → GPIO through 220Ω resistor → cathode → GND.

| Color | GPIO | Trigger |
|---|---|---|
| GREEN | 25 | Known RFID card — 3s solid |
| RED | 32 | Unknown card — 5s blink |
| BLUE | 33 | WiFi connected = solid / lost = fast blink |

---

## Power summary

| Component | Voltage |
|---|---|
| BME280 | 3.3V |
| APDS-9960 | 3.3V |
| MFRC522 | 3.3V |
| LD2450 | 5V |
| LEDs | GPIO (3.3V via 220Ω) |
