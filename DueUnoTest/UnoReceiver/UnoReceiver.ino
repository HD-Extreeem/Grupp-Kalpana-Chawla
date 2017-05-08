/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05

   Note: Modified Arduino-example
*/
#include <Wire.h>

#define DEVICE_ADDRESS 0x50
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 1
#define TX_DATA_LENGTH 3

#define CRANE 0x33

boolean received = false;

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
  /*tx_buf[DATA_LENGTH];
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
    Serial.println();*/

  /* Doesn't make any difference with beginTransmission */
  /*Wire.beginTransmission(DEVICE_ADDRESS);
    Wire.write(tx_buf, sizeof(tx_buf));
    Wire.endTransmission();*/

  // received = false;
  // }
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

/*
   Use requestEvent to send.. sending from loop doesn't seem to work..
*/
void requestEvent()
{
  tx_buf[TX_DATA_LENGTH] = {0};
  
  Serial.print("Got 0x");
  Serial.print(rx_buf[0], HEX);
  Serial.println();

  switch (rx_buf[0])
  {
    case 0x20:
      // Identification
      tx_buf[0] = 0x10;
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    case 0x21:
      // Lift cube
      if (!liftCube())
      {
        // Failed to lift cube
        tx_buf[0] = 0x15;
      }
      else
      {
        tx_buf[0] = 0x14;
      }
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    case 0x22:
      // Lift glass
      if (!liftGlass())
      {
        tx_buf[0] = 0x15;
      }
      else
      {
        tx_buf[0] = 0x14;
      }
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    case 0x23:
      // Lift sock
      if (!liftSock())
      {
        tx_buf[0] = 0x15;
      }
      else
      {
        tx_buf[0] = 0x14;
      }
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    case 0x24:
      // Cancel lift
      cancelLift();
      break;
    case 0x25:
      // Return object
      if (!returnObject())
      {
        // Failed to return object
        tx_buf[0] = 0x17;
      }
      else
      {
        tx_buf[0] = 0x16;
      }
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    default:
      // Do nothing...
      break;
  }
  Wire.write(tx_buf, sizeof(tx_buf));
  Serial.print("Sent ");
  for (int i = 0; i < sizeof(tx_buf); i++)
  {
    Serial.print(tx_buf[i], HEX);
    Serial.print(' ');
  }
  Serial.println("\n");
}

boolean liftCube()
{
  /* Program code goes here... */
  Serial.println("Lifting cube...");
  return true;
}

boolean liftGlass()
{
  /* Program code goes here... */
  Serial.println("Lifting glass...");
  return true;
}

boolean liftSock()
{
  /* Program code goes here... */
  Serial.println("Lifting sock...");
  return true;
}

void cancelLift()
{
  /* Program code goes here... */
  Serial.println("Canceling lift...");
  return true;
}

boolean returnObject()
{
  /* Program code goes here... */
  Serial.println("Returning object...");
  return true;
}
