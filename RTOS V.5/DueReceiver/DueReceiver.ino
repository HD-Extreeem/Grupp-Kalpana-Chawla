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

  uint16_t cube_x = 100;
  uint16_t cube_y = 100;

  
  uint16_t sock_x = 300;
  uint16_t sock_y = 100;

  
  uint16_t glass_x = 120;
  uint16_t glass_y = 300;

  
  uint16_t box_x = 20;
  uint16_t box_y = 0;

   uint8_t msb_x;
  uint8_t lsb_x;

  uint8_t msb_y;
  uint8_t lsb_y;
  
typedef enum
{ 
  SOCK = 0x52,
  CUBE = 0x53,
  GLASS = 0x54,
  BOX = 0x55
  }object;
  
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
  
  switch (rx_buf[0]){
    case SOCK:
    msb_x=(uint8_t) (sock_x >> 8);
    lsb_x=(uint8_t) (sock_x);
    msb_y = (uint8_t) (sock_y >> 8);
    lsb_y = (uint8_t) (sock_y);
    
    break;


   case CUBE:
    msb_x=(uint8_t) (cube_x >> 8);
    lsb_x=(uint8_t) (cube_x);
    msb_y = (uint8_t) (cube_y >> 8);
    lsb_y = (uint8_t) (cube_y);
   break;

   case GLASS:
    msb_x=(uint8_t) (glass_x >> 8);
    lsb_x=(uint8_t) (glass_x);
    msb_y = (uint8_t) (glass_y >> 8);
    lsb_y = (uint8_t) (glass_y);
   break;

   case BOX:
    msb_x=(uint8_t) (box_x >> 8);
    lsb_x=(uint8_t) (box_x);
    msb_y = (uint8_t) (box_y >> 8);
    lsb_y = (uint8_t) (box_y);
   break;
  }
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
