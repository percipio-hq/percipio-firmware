#include <HardwareSerial.h>

#define PIN_RADAR_RX 13
#define PIN_RADAR_TX 17

HardwareSerial radarSerial(2);

static const int FRAME_LEN = 30;
static uint8_t   buf[FRAME_LEN];
static int       pos = 0;
static const uint8_t HDR[] = { 0xAA, 0xFF, 0x03, 0x00 };

static int16_t parseSigned(uint8_t lo, uint8_t hi) {
  uint16_t raw = ((uint16_t)hi << 8) | lo;
  if (raw & 0x8000) return -(int16_t)(raw & 0x7FFF);
  return (int16_t)raw;
}

void setup() {
  Serial.begin(115200);
  radarSerial.begin(256000, SERIAL_8N1, PIN_RADAR_RX, PIN_RADAR_TX);
  Serial.println("LD2450 test — waiting for frames");
}

void loop() {
  while (radarSerial.available()) {
    uint8_t b = radarSerial.read();

    if (pos < 4) {
      if (b == HDR[pos]) { buf[pos++] = b; }
      else { pos = 0; if (b == HDR[0]) buf[pos++] = b; }
      continue;
    }

    buf[pos++] = b;

    if (pos == FRAME_LEN) {
      pos = 0;
      if (buf[28] != 0x55 || buf[29] != 0xCC) continue;

      int count = 0;
      for (int i = 0; i < 3; i++) {
        int     off = 4 + i * 8;
        int16_t x   = parseSigned(buf[off + 0], buf[off + 1]);
        int16_t y   = parseSigned(buf[off + 2], buf[off + 3]);
        int16_t spd = parseSigned(buf[off + 4], buf[off + 5]);
        if (x == 0 && y == 0) continue;
        Serial.printf("  Target%d  X=%+dmm  Y=%+dmm  Speed=%.2fm/s\n",
                      i + 1, x, y, spd / 100.0f);
        count++;
      }
      if (count == 0) Serial.println("No targets");
      else Serial.printf("--- %d target(s) ---\n", count);
    }
  }
}
