#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 5
#define RST_PIN 22

MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;

byte block = 4;

byte dataBlock[16] = {
'C','l','a','s','s',' ','2','0','1',' ',' ',' ',' ',' ',' ',' '
};

void setup() {
  Serial.begin(115200);
  SPI.begin(18,19,23,5);
  rfid.PCD_Init();

  for (byte i = 0; i < 6; i++)
    key.keyByte[i] = 0xFF;

  Serial.println("Place tag to write data");
}

void loop() {

  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  MFRC522::StatusCode status;

  status = rfid.PCD_Authenticate(
           MFRC522::PICC_CMD_MF_AUTH_KEY_A,
           block, &key, &(rfid.uid));

  if (status != MFRC522::STATUS_OK) {
    Serial.println("Authentication failed");
    return;
  }

  status = rfid.MIFARE_Write(block, dataBlock, 16);

  if (status == MFRC522::STATUS_OK)
    Serial.println("Data written successfully");
  else
    Serial.println("Write failed");

  delay(3000);
}