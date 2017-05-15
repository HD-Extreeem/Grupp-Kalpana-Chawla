/**
   DueReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05
*/

#include <Wire.h>

#define DEVICE_ADDRESS 3
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 1
#define TX_DATA_LENGTH 5

/* Buffers for receiving and transmitting bytes */
uint8_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];

void setup()
{
  Serial.begin(BAUD_RATE);

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}

void loop()
{
  delay(100);
}

/*
    Use receiveEvent to receive. Receiving from loop doesn't work.
*/
void receiveEvent(int howMany)
{
  int i = 0;
  Serial.print("Got: ");
  while (Wire.available())
  {
    rx_buf[i] = Wire.read();

    Serial.print(rx_buf[i], HEX);
    Serial.print(' ');
    
    i++;
  }
  Serial.println();
}

/*
   Use requestEvent to send. Sending from loop doesn't work.
*/
void requestEvent()
{
  /* Send back current coordinate data */
  tx_buf[TX_DATA_LENGTH] = {0};

  uint16_t x_coord = 355;
  uint16_t y_coord = 500;

  uint8_t msb_x = (uint8_t) ((x_coord & 0xFF00) >> 8);
  uint8_t lsb_x = (uint8_t) (x_coord & 0x00FF);

  uint8_t msb_y = (uint8_t) ((y_coord & 0xFF00) >> 8);
  uint8_t lsb_y = (uint8_t) (y_coord & 0x00FF);
  
  tx_buf[0] = 0x50;
  tx_buf[1] = msb_x;
  tx_buf[2] = lsb_x;
  tx_buf[3] = msb_y;
  tx_buf[4] = lsb_y;

  Wire.write(tx_buf, sizeof(tx_buf));

  Serial.print("Sent ");
  for (int i = 0; i < sizeof(tx_buf); i++)
  {
    Serial.print(tx_buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println("\n");
}
