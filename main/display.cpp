#include "display.h"
#include <TFT_eSPI.h>
#include <time.h>

// TTGO T-Display: 240x135px, landscape rotation 1
// Font heights: font1=8px, font2=16px, font4=26px

static TFT_eSPI tft;

static uint8_t       currentScreen = 0;
static unsigned long lastRotate    = 0;
static unsigned long lastRedraw    = 0;
static const unsigned long ROTATE_MS = 5000;
static const unsigned long REDRAW_MS = 1000;

static SensorReading envData   = { 0.0f, 0.0f, 0.0f };
static RadarData     radarData = { 0, {} };
static char rfidName[32] = "---";
static char rfidUid[20]  = "---";
static bool rfidAuth     = false;
static char rfidTime[6]  = "--:--";
static bool statusWifi   = false;
static bool statusDb     = false;

static const uint16_t COL_MUTED  = 0x528A; // dark gray
static const uint16_t COL_BORDER = 0x2124; // dimmer separator
static const uint16_t COL_GREEN  = 0x0764; // ~#1D9E75 brand green
static const uint16_t COL_RED    = 0xE8A4; // #E24B4A danger

static void drawDots(uint8_t active) {
  uint16_t xs[3] = { 108, 120, 132 };
  for (int i = 0; i < 3; i++) {
    if (i == (int)active)
      tft.fillCircle(xs[i], 129, 3, TFT_WHITE);
    else
      tft.fillCircle(xs[i], 129, 3, COL_MUTED);
  }
}

static void drawScreen0() {
  tft.fillScreen(TFT_BLACK);

  // --- labels ---
  tft.setTextFont(1);
  tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("TEMP", 5, 5);
  tft.drawString("HUMIDITY", 130, 5);
  tft.drawString("PRESSURE", 5, 47);

  // --- values ---
  char buf[20];
  tft.setTextFont(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);

  snprintf(buf, sizeof(buf), "%.1f C", envData.temperature);
  tft.drawString(buf, 5, 15);

  snprintf(buf, sizeof(buf), "%.0f%%", envData.humidity);
  tft.drawString(buf, 130, 15);

  snprintf(buf, sizeof(buf), "%.1f hPa", envData.pressure);
  tft.drawString(buf, 5, 57);

  // --- separator ---
  tft.drawFastHLine(0, 90, 240, COL_BORDER);

  // --- status row ---
  tft.fillCircle(10, 103, 4, statusWifi ? COL_GREEN : COL_RED);
  tft.setTextFont(2);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString("WiFi", 20, 96);

  tft.fillCircle(80, 103, 4, statusDb ? COL_GREEN : COL_RED);
  tft.drawString("Firebase", 92, 96);

  drawDots(0);
}

static void drawScreen1() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(1);
  tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("RADAR", 5, 5);

  tft.setTextFont(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  char buf[40];
  snprintf(buf, sizeof(buf), "%d PEOPLE", radarData.count);
  tft.drawString(buf, 5, 15);

  tft.drawFastHLine(0, 46, 240, COL_BORDER);

  tft.setTextFont(2);
  for (int i = 0; i < 3; i++) {
    uint16_t y = 52 + i * 22;
    snprintf(buf, sizeof(buf), "P%d", i + 1);
    tft.setTextColor(COL_MUTED, TFT_BLACK);
    tft.drawString(buf, 5, y);

    if (i < radarData.count) {
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      snprintf(buf, sizeof(buf), "X%+5d  Y%+5d mm", radarData.targets[i].x, radarData.targets[i].y);
      tft.drawString(buf, 28, y);
    } else {
      tft.setTextColor(COL_BORDER, TFT_BLACK);
      tft.drawString("---", 28, y);
    }
  }

  drawDots(1);
}

static void drawScreen2() {
  tft.fillScreen(TFT_BLACK);

  tft.setTextFont(1);
  tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("LAST ENTRY", 5, 5);

  tft.setTextFont(4);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(rfidName, 5, 15);

  tft.drawFastHLine(0, 46, 240, COL_BORDER);

  tft.setTextFont(2);
  tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("UID", 5, 52);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(rfidUid, 40, 52);

  tft.setTextColor(COL_MUTED, TFT_BLACK);
  tft.drawString("TIME", 5, 72);
  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.drawString(rfidTime, 48, 72);

  tft.setTextColor(rfidAuth ? COL_GREEN : COL_RED, TFT_BLACK);
  tft.drawString(rfidAuth ? "AUTHORIZED" : "UNKNOWN", 5, 95);

  drawDots(2);
}

void displayInit() {
  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextDatum(TL_DATUM);
  lastRedraw = millis() - REDRAW_MS; // draw immediately on first loop
}

void displayLoop() {
  unsigned long now = millis();

  if (now - lastRotate >= ROTATE_MS) {
    lastRotate = now;
    currentScreen = (currentScreen + 1) % 3;
  }

  if (now - lastRedraw >= REDRAW_MS) {
    lastRedraw = now;
    switch (currentScreen) {
      case 0: drawScreen0(); break;
      case 1: drawScreen1(); break;
      case 2: drawScreen2(); break;
    }
  }
}

void displayNextScreen() {
  currentScreen = (currentScreen + 1) % 3;
  lastRotate = millis();
  lastRedraw = millis() - REDRAW_MS;
}

void displayPrevScreen() {
  currentScreen = (currentScreen + 2) % 3;
  lastRotate = millis();
  lastRedraw = millis() - REDRAW_MS;
}

void displayUpdateEnv(const SensorReading& r) {
  envData = r;
}

void displayUpdateRadar(const RadarData& d) {
  radarData = d;
}

void displayUpdateRfid(const RfidEvent& e) {
  strncpy(rfidName, e.name, sizeof(rfidName) - 1);
  rfidName[sizeof(rfidName) - 1] = '\0';
  strncpy(rfidUid, e.uid, sizeof(rfidUid) - 1);
  rfidUid[sizeof(rfidUid) - 1] = '\0';
  rfidAuth = e.authorized;

  time_t now = time(nullptr);
  struct tm* t = localtime(&now);
  snprintf(rfidTime, sizeof(rfidTime), "%02d:%02d", t->tm_hour, t->tm_min);
}

void displaySetStatus(bool wifiOk, bool dbOk) {
  statusWifi = wifiOk;
  statusDb   = dbOk;
}
