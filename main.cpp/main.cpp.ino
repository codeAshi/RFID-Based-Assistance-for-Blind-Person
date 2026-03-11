#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte block = 4;
byte buffer[18];
byte size = sizeof(buffer);

void setup() {

  Serial.begin(115200);
  SPI.begin(18,19,23,5);
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  Serial.println("Scan tag...");
}

void loop() {

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
    Serial.println("Auth failed");
    return;
  }

  status = rfid.MIFARE_Read(block, buffer, &size);

  if(status != MFRC522::STATUS_OK){
    Serial.println("Read failed");
    return;
  }

  String text = "";

  for(byte i=0;i<16;i++){
    text += (char)buffer[i];
  }

  Serial.println(text);

  // IMPORTANT: stop communication so next tag can be read
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();

  delay(2000);
}