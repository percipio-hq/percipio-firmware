#include "radar.h"
#include "config.h"
#include <HardwareSerial.h>

// LD2450 frame: AA FF 03 00 | target1(8B) target2(8B) target3(8B) | 55 CC
// Each target: X(2B) Y(2B) speed(2B) resolution(2B), little-endian
// Sign encoding: bit15=1 → negative, value = -(raw & 0x7FFF)
//                bit15=0 → positive, value = raw
// Speed unit: cm/s → divide by 100 for m/s

static HardwareSerial radarSerial(2);

static const int FRAME_LEN = 30;
static uint8_t   buf[FRAME_LEN];
static int       pos = 0;

static const uint8_t HDR[] = { 0xAA, 0xFF, 0x03, 0x00 };

void radarInit() {
  radarSerial.begin(256000, SERIAL_8N1, PIN_RADAR_RX, PIN_RADAR_TX);
  Serial.println("[radar] LD2450 UART started");
}

static int16_t parseSigned(uint8_t lo, uint8_t hi) {
  uint16_t raw = ((uint16_t)hi << 8) | lo;
  if (raw & 0x8000)
    return -(int16_t)(raw & 0x7FFF);
  return (int16_t)raw;
}

// Returns count >= 0 if a complete frame was parsed, -1 if no new frame yet.
RadarData radarRead() {
  while (radarSerial.available()) {
    uint8_t b = radarSerial.read();

    if (pos < 4) {
      if (b == HDR[pos]) {
        buf[pos++] = b;
      } else {
        pos = 0;
        if (b == HDR[0]) buf[pos++] = b;
      }
      continue;
    }

    buf[pos++] = b;

    if (pos == FRAME_LEN) {
      pos = 0;
      if (buf[28] != 0x55 || buf[29] != 0xCC) continue;

      RadarData data = { 0, {} };
      for (int i = 0; i < 3; i++) {
        int      off   = 4 + i * 8;
        int16_t  x     = parseSigned(buf[off + 0], buf[off + 1]);
        int16_t  y     = parseSigned(buf[off + 2], buf[off + 3]);
        int16_t  speed = parseSigned(buf[off + 4], buf[off + 5]);

        if (x == 0 && y == 0) continue; // empty slot

        data.targets[data.count++] = { x, y, speed / 100.0f };
      }
      return data;
    }
  }

  return { -1, {} }; // no complete frame yet
}
