/* 
RFID Scanner
By: Taylor Roberts

scans anything it sees and sends it over serial
Some of this code comes from a guide, I forgot where

Taylor Roberts

*/


#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 11
#define RST_PIN 7
MFRC522 myRFID(SS_PIN, RST_PIN);   // Create MFRC522 instance.
 
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  myRFID.PCD_Init();   // Initiate MFRC522
  while (!Serial); // wait for Serial to initiate
  Serial.println("initialized");
}
void loop() 
{
  // code for reading from serial! Not needed here, but leaving it around for future reference
  
  // if (Serial.available()) {
  //   String input = Serial.readString();

  //   Serial.print("Unity said: ");
  //   Serial.println(input);
  // }
  // Wait for RFID cards to be scanned
  if ( ! myRFID.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // an RFID card has been scanned but no UID 
  if ( ! myRFID.PICC_ReadCardSerial()) 
  {
    // Serial.println("something scanned, no UID");
    return;
  }

  // send message over serial
  String content = ""; // "USER ID tag :";  // Start with identifier text
  for (byte i = 0; i < myRFID.uid.size; i++) 
  {
     content.concat(String(myRFID.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(myRFID.uid.uidByte[i], HEX));
  }
  Serial.println(content); // Send the full UID as a string, with the label and formatted nicely
  
  delay(1000);
} 