/**
   DueReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05
*/

#include <Wire.h>

#define DEVICE_ADDRESS 3
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

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
  while (Wire.available())
  {
    rx_buf[i] = Wire.read();
    i++;

    Serial.print("Got: ");
    Serial.print(rx_buf[i], HEX);
    Serial.println();
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
  getRobotPos(tx_buf);

  Wire.write(tx_buf, sizeof(tx_buf));

  Serial.print("Sent ");
  for (int i = 0; i < sizeof(tx_buf); i++)
  {
    Serial.print(tx_buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println("\n");
}

void getRobotPos(uint8_t *rx_buf)
{
  /* Dummy code */
  for (int i = 0; i < TX_DATA_LENGTH; i++)
  {
    tx_buf[i] = i;
  }
}
