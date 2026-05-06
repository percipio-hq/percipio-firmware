#include <TFT_eSPI.h>

// TTGO T-Display: 240x135px, rotation 1 (landscape)
// Cycles through all 3 screens with hardcoded data every 3s

TFT_eSPI tft;

static const uint16_t COL_MUTED  = 0x528A;
static const uint16_t COL_BORDER = 0x2124;
static const uint16_t COL_GREEN  = 0x0764;
static const uint16_t COL_RED    = 0xE8A4;

void drawDots(uint8_t active) {
  uint16_t xs[3] = { 108, 120, 132 };
  for (int i = 0; i < 3; i++)
    tft.fillCircle(xs[i], 129, 3, i == active ? TFT_WHITE : COL_MUTED);
}

void drawScreen0() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(1); tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("TEMP", 5, 5);
  tft.drawString("HUMIDITY", 130, 5);
  tft.drawString("PRESSURE", 5, 47);

  tft.setTextFont(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("22.4 C", 5, 15);
  tft.drawString("48%", 130, 15);
  tft.drawString("1013.2 hPa", 5, 57);

  tft.drawFastHLine(0, 90, 240, COL_BORDER);

  tft.fillCircle(10, 103, 4, COL_GREEN);
  tft.setTextFont(2); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("WiFi", 20, 96);
  tft.fillCircle(80, 103, 4, COL_GREEN);
  tft.drawString("Firebase", 92, 96);

  drawDots(0);
}

void drawScreen1() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(1); tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("RADAR", 5, 5);

  tft.setTextFont(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("2 PEOPLE", 5, 15);

  tft.drawFastHLine(0, 46, 240, COL_BORDER);

  tft.setTextFont(2);
  const char* labels[] = { "P1", "P2", "P3" };
  const char* vals[]   = { "X +1200  Y  +800 mm", "X  -500  Y +1500 mm", "---" };
  for (int i = 0; i < 3; i++) {
    uint16_t y = 52 + i * 22;
    tft.setTextColor(COL_MUTED, TFT_BLACK);
    tft.drawString(labels[i], 5, y);
    tft.setTextColor(i < 2 ? TFT_WHITE : COL_BORDER, TFT_BLACK);
    tft.drawString(vals[i], 28, y);
  }

  drawDots(1);
}

void drawScreen2() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(1); tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("LAST ENTRY", 5, 5);

  tft.setTextFont(4); tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("Jacek", 5, 15);

  tft.drawFastHLine(0, 46, 240, COL_BORDER);

  tft.setTextFont(2);
  tft.setTextColor(COL_MUTED, TFT_BLACK); tft.drawString("UID", 5, 52);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.drawString("A1B2C3D4", 40, 52);

  tft.setTextColor(COL_MUTED, TFT_BLACK); tft.drawString("TIME", 5, 72);
  tft.setTextColor(TFT_WHITE, TFT_BLACK); tft.drawString("14:32", 48, 72);

  tft.setTextColor(COL_GREEN, TFT_BLACK);
  tft.drawString("AUTHORIZED", 5, 95);

  drawDots(2);
}

int screen = 0;

void setup() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
}

void loop() {
  switch (screen) {
    case 0: drawScreen0(); break;
    case 1: drawScreen1(); break;
    case 2: drawScreen2(); break;
  }
  screen = (screen + 1) % 3;
  delay(3000);
}
