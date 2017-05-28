/**************************************************************************/
/*!
    @author   Adafruit Industries
    @Modified by Namra Gill
    @license  BSD (see license.txt)
    This example will wait for any ISO14443A card or tag, and
    depending on the size of the UID will attempt to read from it.

*/
/**************************************************************************/
#include <SPI.h>
#include <Adafruit_PN532.h>

// If using the breakout with SPI, define the pins for SPI communication.
//These pins can be changed
#define PN532_SCK  (2)
#define PN532_MOSI (3)
#define PN532_SS   (6)
#define PN532_MISO (5)

// Use this line for a breakout with a software SPI connection (recommended):
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);

uint8_t success;
// Buffer to store the returned UID
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
uint8_t uidLength;

/*This method will be executed only once*/
void setup(void) {

  Serial.begin(115200);
  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1);
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for a rfid/nfc tag");
}

/*This method will loop*/
void loop(void) {
  // detect an rfid/nfc tag
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);

  if (success) {
    //Display that the tag has been found
    Serial.println("Found an rfid/nfc tag");
    Serial.println("object is detected");
  }
}


