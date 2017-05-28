/**
   LarzKristerzKran.ino
   State machine for Demonstrationday
   Author: Namra Gill (crane code) and Jonas Eiselt (TWI-code)
   Created: 2017-05-21
*/

#include <Servo.h>
#include <Wire.h>

Servo servoArm;  // create servo object to control a servo
Servo servoBottom;

//digital pins for microswitches
const int buttonDown = 2;
const int buttonTop = 6;

int pos = 0;//angle of servo motor
int counter = 3;//counter to count objects
int buttonDownState = 0; //state of buttonDown
int buttonTopState = 0; //state of buttonUp

/*flag to know when to drive forward*/
int flagDrive;
int objectsLeft = 3;
int detectionTries = 0;

/*States*/
enum States {Wait, GrabObject, DropObject, LiftUp};
States state = Wait;

boolean doThisOnce = true;

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33
#define ARLO_COLLECT_STATUS 1

/* TWI states */
typedef enum
{
  TWI_CMD_ARM_INIT      = 0x20,
  TWI_CMD_DROP_OFF_START    = 0x21,
  TWI_CMD_PICK_UP_START   = 0x22,
  TWI_CMD_DROP_OFF_STATUS   = 0x23,
  TWI_CMD_PICK_UP_STATUS    = 0x24,
  TWI_CMD_ERROR       = 0x25,
  TWI_CMD_NONE = 0x28
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
  GLASS = 4,
  OBJECT  = 5
} Object;

Object object_t;
TWI_CMD_Init_Req twi_cmd_init_req_t;
Drop_Off_Status drop_off_status_t;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;
TWI_CMD twi_cmd_t = TWI_CMD_NONE;

/* Buffers for receiving and transmitting bytes */
uint8_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];


/*This method will execute once*/
void setup()
{
  Serial.begin(BAUD_RATE);

  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT);  //Initiates Brake Channel A pin

  //set switches as input
  pinMode(buttonDown, INPUT);
  pinMode(buttonTop, INPUT);

  //signal pins for servomotors
  servoArm.attach(4);
  servoBottom.attach(5);
  //start angle of servomotors
  servoArm.write(70);
  servoBottom.write(150);

  //lift down the crane
  liftDown();

  //open the arm
  openArm();

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  /*Start state*/
  state = Wait;
}

void loop()
{
  /* If a pick-up-object-req have been requested and if object is not grabbed */
  if (twi_cmd_t == TWI_CMD_PICK_UP_START && pick_up_status_t != PICK_UP_DONE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Pick-up of an object has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    state = GrabObject;
  }
  /* If a pick-up-obj-status-req have been requested and if object hasnt been grabbed */
  else if (twi_cmd_t ==  TWI_CMD_PICK_UP_STATUS && pick_up_status_t != PICK_UP_DONE)
  {
    pick_up_status_t = PICK_UP_DONE;
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Pick-up status has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("PICK UP DONE.....................");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    state = Wait;
  }

  /*If drop off object has been requested*/
  else if (twi_cmd_t == TWI_CMD_DROP_OFF_START && drop_off_status_t != DROP_OFF_DONE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Dropping off objects has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
  }
  /*If drop off status has been requested and the object isnt dropped*/
  else if (twi_cmd_t == TWI_CMD_DROP_OFF_STATUS && drop_off_status_t != DROP_OFF_DONE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~");
    Serial.println("Dropping off object");
    Serial.println("~~~~~~~~~~~~~~~~~~~~\n");
    state = DropObject;
  }

  /**********************************/
  /*          Switch case           */
  /**********************************/
  switch (state)
  {
    case GrabObject:
      /* Grab object*/
      closeArm();

      objectsLeft--;
      Serial.print(objectsLeft);
      Serial.println(" left to pick up");

      /* If all the objects have been grabbed */
      if (objectsLeft == 0)
      {
        /*Lift up the crane */
        state = LiftUp;
      }
      else
      {
        pick_up_status_t = PICK_UP_DONE;
        state = Wait;
      }
      break;
    case LiftUp:
      if (doThisOnce)
        if (doThisOnce)
        {
          servoArm.write(50);
          digitalWrite(9, LOW); //brakes off
          digitalWrite(12, HIGH); //lifting up
          analogWrite(3, 255);//pwm

          Serial.println("Lift up elevator");

          doThisOnce = false;
        }

      buttonTopState = digitalRead(buttonTop);
      Serial.println(buttonTop);

      /* If tbe switched isnt pushed*/
      if (buttonTopState == HIGH)
      {
        state = LiftUp;
      }
      else
      {
        Serial.println("Upper button is on");
        digitalWrite(9, HIGH);    //brakes on dc motor

        state = Wait;
        pick_up_status_t = PICK_UP_DONE;
      }
      break;
    case DropObject:
      /* drop the object*/
      dropObject();
      drop_off_status_t = DROP_OFF_DONE;
      break;
    case Wait:
      //Do nothing
      break;
  }
}

/****************************************************************/
/*                Methods for the crane              */
/****************************************************************/

/* Method to lift down the crane from the start */
void liftDown()
{
  digitalWrite(9, LOW);
  digitalWrite(12, LOW); //downwards direction
  Serial.println("Goimg down");
  analogWrite(3, 255);
  buttonDownState = digitalRead(buttonDown);
  Serial.println(digitalRead(buttonDown));
  /* If button is pushed */
  if (buttonDownState == LOW)
  {
    Serial.println("Bottom button is on");
    digitalWrite(9, HIGH);
  }

}

/* Method to open the arm */
void openArm()
{
  //opens the Arm
  for (pos = 70; pos <= 180; pos += 1) {
    servoArm.write(pos);
    Serial.println(pos);// tell servo to go to position/angle in variable 'pos'
    delay(15);          // waits 15ms for the servo to reach the position
  }
}

/* Method to push in the object*/
void closeArm()
{
  Serial.println("Closing arm to push in object");
  //shutting the arm and by that pushing in object
  for (pos = 0; pos <= 180; pos += 1) {
    servoArm.write(pos);
    Serial.println(pos);
    delay(15);
  }
  //opening and shutting the arm again to secure that the object nis inside
  for (pos = 0; pos <= 50; pos += 1) {
    servoArm.write(pos);
    Serial.println(pos);
    delay(15);
  }

  servoArm.write(70);// arm goes back to its strat position
}

/* Metod to drop the object*/
void dropObject()
{
  Serial.println("Dropping objects");
  for (pos = 0; pos <= 50; pos += 1)
  {
    Serial.println(pos);
    servoBottom.write(pos);
    delay(40);
  }
}

/****************************************************************/
/*               Methods for I2C-communication              */
/****************************************************************/

/* ISR-metod that reads data from Arduino Due */
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

  twi_cmd_t = rx_buf[0];
  switch (twi_cmd_t)
  {
    case TWI_CMD_PICK_UP_START:
      /* Pick up object */
      object_t = rx_buf[1];
      pick_up_status_t = PICK_UP_IDLE;
      break;
    case TWI_CMD_DROP_OFF_START:
      /* Drop off object */
      object_t = rx_buf[1];
      drop_off_status_t = DROP_OFF_IDLE;
      break;
    default:
      break;
  }
}

/* ISR-metod that sends data to Arduino Due */
void requestEvent()
{
  tx_buf[TX_DATA_LENGTH] = {0};
  twi_cmd_t = rx_buf[0];
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
          tx_buf[2] = ARLO_COLLECT_STATUS;  // Collect all info
          break;
        default:
          Serial.println("Waiting for Arlo");
          break;
      }
      break;
    case TWI_CMD_DROP_OFF_STATUS:
      Serial.println("\nRobot requesting drop off status!");
      tx_buf[0] = TWI_CMD_DROP_OFF_STATUS;
      tx_buf[1] = drop_off_status_t;  // Fill index with current drop off status
      break;
    case TWI_CMD_PICK_UP_STATUS:
      Serial.println("\nRobot requesting pick-up status!");
      tx_buf[0] = TWI_CMD_PICK_UP_STATUS;
      tx_buf[1] = pick_up_status_t;  // Fill index with current drop off status
      Serial.print(pick_up_status_t, HEX);
      Serial.println();
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

