/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05
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
  CUBE  = 3,
  GLASS = 4
} Object;

Object object_t;
TWI_CMD_Init_Req twi_cmd_init_req_t;
Drop_Off_Status drop_off_status_t;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;

/* Buffers for receiving and transmitting bytes */
uint16_t rx_buf[RX_DATA_LENGTH];
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
  Serial.print("Got: ");
  while (Wire.available())
  {
    rx_buf[i] = Wire.read();

    Serial.print(rx_buf[i], HEX);
    Serial.print(' ');

    i++;
  }
  Serial.println();

  TWI_CMD twi_cmd_t = rx_buf[0];
  switch (twi_cmd_t)
  {
    case TWI_CMD_PICK_UP_START:
      /* Pick up object */
      object_t = rx_buf[1];
      switch (object_t)
      {
        case SOCK:
          /* Lift sock */
          liftSock();
          break;
        case CUBE:
          /* Lift cube */
          liftCube();
          break;
        case GLASS:
          /* Lift glass */
          liftGlass();
          break;
        default:
          break;
      }
      break;
    case TWI_CMD_DROP_OFF_START:
      /* Drop off object */
      returnObject();
      break;
    default:
      break;
  }
}

/*
   Use requestEvent to send. Sending from loop doesn't work.
*/
void requestEvent()
{
  tx_buf[TX_DATA_LENGTH] = {0};
  TWI_CMD twi_cmd_t = rx_buf[0];
  switch (twi_cmd_t)
  {
    case TWI_CMD_ARM_INIT:
      twi_cmd_init_req_t = rx_buf[1];
      switch (twi_cmd_init_req_t)
      {
        case TWI_CMD_ARM_REQ_BOX_INFO:
          tx_buf[0] = TWI_CMD_ARM_INIT;
          tx_buf[1] = TWI_CMD_ARM_REQ_BOX_INFO;
          tx_buf[2] = 0x50; // Box info
          break;
        case TWI_CMD_ARM_REQ_OBJ_INFO:
          tx_buf[0] = TWI_CMD_ARM_INIT;
          tx_buf[1] = TWI_CMD_ARM_REQ_BOX_INFO;
          tx_buf[2] = 0x50; // Object info
          break;
        case TWI_CMD_ARM_REQ_COLLECT_INFO:
          tx_buf[0] = TWI_CMD_ARM_INIT;
          tx_buf[1] = TWI_CMD_ARM_REQ_COLLECT_INFO;
          tx_buf[2] = 0x50; // Collect all info
          break;
        default:
          // Do nothing
          break;
      }
      break;
    case TWI_CMD_DROP_OFF_STATUS:
      drop_off_status_t = getDropOffStatus();
      tx_buf[0] = TWI_CMD_DROP_OFF_STATUS;
      tx_buf[1] = drop_off_status_t;  // Fill index with current drop off status
      break;
    case TWI_CMD_PICK_UP_STATUS:
      pick_up_status_t = getPickUpStatus();
      tx_buf[0] = TWI_CMD_PICK_UP_STATUS;
      tx_buf[1] = pick_up_status_t;  // Fill index with current drop off status
      break;
    default:
      // Do nothing
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
  pick_up_status_t = PICK_UP_RUNNING;
  delay(500);
  pick_up_status_t = PICK_UP_DONE;
  Serial.println("Cube was lifted...");
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

int getDropOffStatus()
{
  return 0;
}

Pick_Up_Status getPickUpStatus()
{
  return pick_up_status_t;
}
