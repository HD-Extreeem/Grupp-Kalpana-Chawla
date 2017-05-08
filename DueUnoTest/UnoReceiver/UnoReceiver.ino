/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05

   Note: Modified Arduino-example
*/
#include <Wire.h>

#define DEVICE_ADDRESS 0x50
#define BAUD_RATE 115200
#define DATA_LENGTH 2

boolean received = false;

uint8_t rx_buf[2];
uint8_t tx_buf[2];

void setup()
{
  Serial.begin(BAUD_RATE);

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
  tx_buf[DATA_LENGTH];
  if (received)
  {
    for (int i = 0; i < DATA_LENGTH; i++)
    {
      tx_buf[i] = rx_buf[i];
    }

    Serial.print("Rx: ");
    for (int i = 0; i < DATA_LENGTH; i++)
    {
      Serial.print(rx_buf[i], HEX);
      Serial.print(' ');
    }
    Serial.println();

    /* Doesn't make any difference with beginTransmission */
    /*Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(tx_buf, sizeof(tx_buf));
    Wire.endTransmission();*/

    /*
       Wire.write((tx_buf[1] >> 8) & 0xff);
       Wire.write(tx_buf[0] & 0xff);
    */

    /* MSB byte */
    // Wire.write((buf[1] >> 24) & 0xff);
    // Wire.write((buf[1] >> 16) & 0xff);

    /* LSB byte */
    // Wire.write((buf[0] >> 8) & 0xff);
    // Wire.write(buf[0] & 0xff);

    received = false;
  }
}

void receiveEvent(int howMany)
{
  int i = 0;
  while (Wire.available())
  {
    rx_buf[i] = Wire.read();
    i++;
  }
  received = true;
}

/* Use requestEvent to send.. sending from loop doesn't seem to work.. */
void requestEvent()
{
  Serial.println("Hello?");
  Wire.write(0x55);
  Wire.write(0x45);
}
