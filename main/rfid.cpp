#include "rfid.h"
#include "config.h"
#include "led.h"
#include "display.h"
#include <SPI.h>
#include <MFRC522.h>

static MFRC522 mfrc(PIN_RFID_CS, PIN_RFID_RST);

void rfidInit() {
  SPI.begin();
  mfrc.PCD_Init();
  Serial.println("[rfid] MFRC522 OK");
}

static const char* lookupName(const char* uid) {
  for (int i = 0; i < CARD_COUNT; i++) {
    if (strcmp(CARD_MAP[i][0], uid) == 0)
      return CARD_MAP[i][1];
  }
  return nullptr;
}

// Returns event with uid set if a card was scanned, uid="" otherwise.
RfidEvent rfidLoop() {
  if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial())
    return { "", "Unknown", false };

  // Build hex UID string e.g. "A1B2C3D4"
  static char uid[16];
  uid[0] = '\0';
  for (byte i = 0; i < mfrc.uid.size; i++) {
    char hex[3];
    snprintf(hex, sizeof(hex), "%02X", mfrc.uid.uidByte[i]);
    strncat(uid, hex, sizeof(uid) - strlen(uid) - 1);
  }

  mfrc.PICC_HaltA();

  const char* name = lookupName(uid);
  bool authorized = (name != nullptr);
  if (!authorized) name = "Unknown";

  Serial.printf("[rfid] Card UID: %s  Name: %s\n", uid, name);

  if (authorized)
    ledGreenPulse();
  else
    ledRedBlink();

  return { uid, name, authorized };
}
