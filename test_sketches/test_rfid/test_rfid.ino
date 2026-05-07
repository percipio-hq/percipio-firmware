#include <SPI.h>
#include <MFRC522.h>

#define PIN_RFID_CS  27
#define PIN_RFID_RST 26

MFRC522 mfrc(PIN_RFID_CS, PIN_RFID_RST);

void setup() {
  Serial.begin(115200);
  SPI.begin(18, 2, 19, PIN_RFID_CS); // SCK=18, MISO=2, MOSI=19
  mfrc.PCD_Init();
  Serial.println("MFRC522 OK — tap a card");
}

void loop() {
  if (!mfrc.PICC_IsNewCardPresent() || !mfrc.PICC_ReadCardSerial()) return;

  Serial.print("Card UID: ");
  for (byte i = 0; i < mfrc.uid.size; i++) {
    if (mfrc.uid.uidByte[i] < 0x10) Serial.print("0");
    Serial.print(mfrc.uid.uidByte[i], HEX);
    if (i < mfrc.uid.size - 1) Serial.print(" ");
  }
  Serial.println();

  mfrc.PICC_HaltA();
  delay(1000);
}
