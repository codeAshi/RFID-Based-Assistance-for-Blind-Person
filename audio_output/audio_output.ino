#include <SPI.h>
#include <MFRC522.h>
#include <Audio.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

Audio audio;

byte block = 4;
byte buffer[18];
byte size = sizeof(buffer);

void setup() {

  Serial.begin(115200);
  SPI.begin(18,19,23,5);
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  // MAX98357A I2S pins
  audio.setPinout(26, 25, 27);
  audio.setVolume(15);

  Serial.println("Scan tag...");
}

void loop() {

  audio.loop();

  if (!rfid.PICC_IsNewCardPresent())
    return;

  if (!rfid.PICC_ReadCardSerial())
    return;

  MFRC522::StatusCode status;

  status = rfid.PCD_Authenticate(
          MFRC522::PICC_CMD_MF_AUTH_KEY_A,
          block,
          &key,
          &(rfid.uid));

  if(status != MFRC522::STATUS_OK){
    Serial.println("Tag Record not found");
    return;
  }

  status = rfid.MIFARE_Read(block, buffer, &size);

  String text = "";

  for(byte i=0;i<16;i++){
    if(buffer[i] != 0)
      text += (char)buffer[i];
  }

  if(status == MFRC522::STATUS_OK){

    Serial.println(text);

    // speak text
    audio.connecttospeech(text.c_str(), "en");

  }

  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(2000);
}