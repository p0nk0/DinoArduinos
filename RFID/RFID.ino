// code is modified from a guide (I think it was on the ardruino website)

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 11
#define RST_PIN 7
MFRC522 myRFID(SS_PIN, RST_PIN);   // Create MFRC522 instance.

int pinLED=2;
 
void setup() 
{
  Serial.begin(115200);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  myRFID.PCD_Init();   // Initiate MFRC522
  while (!Serial);
  Serial.println("Please scan your RFID card...");
  Serial.println();
  // pinMode(pinLED, OUTPUT);

}
void loop() 
{
  // Wait for RFID cards to be scanned
  if ( ! myRFID.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // an RFID card has been scanned but no UID 
  if ( ! myRFID.PICC_ReadCardSerial()) 
  {
    Serial.println("something scanned, no UID");
    return;
  }
  //Show UID on serial monitor
  // digitalWrite(pinLED,HIGH);
  Serial.print("USER ID tag :");
  String content= "";
 
  for (byte i = 0; i < myRFID.uid.size; i++) 
  {
     Serial.print(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(myRFID.uid.uidByte[i], HEX);
     content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(myRFID.uid.uidByte[i], HEX));
  }
  delay(1000);
  // digitalWrite(pinLED,LOW);
  Serial.println();
} 