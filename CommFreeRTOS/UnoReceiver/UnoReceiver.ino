/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05

   Note: Modified Arduino-example
*/
#include <Wire.h>

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33

typedef struct 
{
  /* Distances are in cm and angles are in degrees */
  uint8_t box_distance;
  uint8_t box_angle;
  uint8_t object_distance;
  uint8_t object_angle;
  uint8_t collect_all;            // False if robot can only carry one object at a time
  uint8_t has_data;             // Just to check if the struct is set
} Arm_Info;

/* TWI states */
typedef enum  
{
  TWI_CMD_ARM_INIT    = 0x20,
  TWI_CMD_DROP_OFF_START  = 0x21,
  TWI_CMD_PICK_UP_START = 0x22,
  TWI_CMD_DROP_OFF_STATUS = 0x23,
  TWI_CMD_PICK_UP_STATUS  = 0x24,
  TWI_CMD_ERROR     = 0x25
} TWI_CMD;

typedef enum  
{
  TWI_CMD_ARM_REQ_BOX_INFO    = 2,
  TWI_CMD_ARM_REQ_OBJ_INFO    = 3,
  TWI_CMD_ARM_REQ_COLLECT_INFO  = 4
} TWI_CMD_Init_Req;

typedef enum 
{
  PICK_UP_DONE    = 2,
  PICK_UP_FORWARD   = 3,
  PICK_UP_BACKWARD  = 4,
  PICK_UP_RUNNING   = 5,
  PICK_UP_FAILED    = 6,
  PICK_UP_DONE_DRIVE  = 7,
  PICK_UP_IDLE    = 8
} Pick_Up_Status;

typedef enum 
{
  DROP_OFF_DONE   = 2,
  DROP_OFF_RUNNING  = 3,
  DROP_OFF_FAILED   = 4,
  DROP_OFF_IDLE   = 5
} Drop_Off_Status;

typedef enum 
{
  SOCK  = 2,
  SQUARE  = 3,
  GLASS = 4
} Object;

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
  tx_buf[TX_DATA_LENGTH] = {0};
  
  Serial.print("Got 0x");
  Serial.print(rx_buf[0], HEX);
  Serial.println();

  switch (rx_buf[0])
  {
    case 0x20:
      /* Identification */
      tx_buf[0] = 0x10;
      tx_buf[1] = 0x10;
      tx_buf[2] = 0x10;
      break;
    case 0x21:
      /* Lift cube */
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
      /* Lift glass */
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
      /* Lift sock */
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
      /* Cancel lift */
      cancelLift();
      break;
    case 0x25:
      /* Return object */
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
}

boolean returnObject()
{
  /* Program code goes here... */
  Serial.println("Returning object...");
  return true;
}
