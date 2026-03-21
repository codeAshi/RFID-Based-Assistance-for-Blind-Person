#include <SPI.h>
#include <MFRC522.h>
#include <AudioFileSourceSPIFFS.h>
#include <AudioGeneratorWAV.h>
#include <AudioOutputI2S.h>
#include "SPIFFS.h"

#define SS_PIN 5
#define RST_PIN 22

MFRC522 mfrc522(SS_PIN, RST_PIN);

AudioGeneratorWAV *wav;
AudioFileSourceSPIFFS *file;
AudioOutputI2S *out;

// 🔥 Timer control
unsigned long lastScanTime = 0;
const int scanDelay = 3000; // 3 seconds (change to 5000 for 5 sec)

void setup() {
  Serial.begin(115200);

  SPI.begin();
  mfrc522.PCD_Init();

  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS failed");
    return;
  }

  // I2S setup
  out = new AudioOutputI2S();
  out->SetPinout(26, 25, 27); // BCLK, LRC, DIN
  out->SetGain(0.8);

  Serial.println("Scan RFID...");
}

void loop() {

  // ⏱️ Allow scan only after delay
  if (millis() - lastScanTime < scanDelay) {
    return;
  }

  if (!mfrc522.PICC_IsNewCardPresent()) return;
  if (!mfrc522.PICC_ReadCardSerial()) return;

  lastScanTime = millis();  // 🔥 update scan time

  byte block = 4;
  byte buffer[18];
  byte size = sizeof(buffer);

  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;

  if (mfrc522.PCD_Authenticate(
        MFRC522::PICC_CMD_MF_AUTH_KEY_A,
        block, &key, &(mfrc522.uid)) != MFRC522::STATUS_OK) {
    Serial.println("Auth failed");
    return;
  }

  if (mfrc522.MIFARE_Read(block, buffer, &size) != MFRC522::STATUS_OK) {
    Serial.println("Read failed");
    return;
  }

  String text = "";
  for (int i = 0; i < 16; i++) {
    if (buffer[i] != 0) text += (char)buffer[i];
  }

  text.trim();
  Serial.println("Speaking: " + text);

  // 🔊 Play audio
  if (text == "Class 201") {

    file = new AudioFileSourceSPIFFS("/class201.wav");
    wav = new AudioGeneratorWAV();

    wav->begin(file, out);

    while (wav->isRunning()) {
      wav->loop();
    }

    wav->stop();
    delete file;
    delete wav;
  }
  else if (text == "Entrance") {

    file = new AudioFileSourceSPIFFS("/entrance.wav");
    wav = new AudioGeneratorWAV();

    wav->begin(file, out);

    while (wav->isRunning()) {
      wav->loop();
    }

    wav->stop();
    delete file;
    delete wav;
  }
  else {
    Serial.println("No audio mapped");
  }

  // 🔥 VERY IMPORTANT: Reset RFID
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}
