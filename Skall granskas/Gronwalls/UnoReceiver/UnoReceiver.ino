/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05
   Gronwalls
*/

#include <Wire.h>
/*inlcuding servo and stepper libraries in order to use associated methods for crane*/
#include <Servo.h>
#include <Stepper.h>
/*including library in order to use associated methods for rfid sensor */
//#include <Adafruit_PN532.h>

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33
#define ARLO_COLLECT_STATUS 1
/*If using the breakout or shield with I2C, define just the pins connected
  to the IRQ and reset lines.
*/
#define PN532_IRQ   (2)
#define PN532_RESET (3)

/*shield with an I2C connection for rfid use:*/
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);
/*variables for rfid use */
//uint8_t success;
// Buffer to store the returned UID
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
uint8_t uidLength;

/*Defining the different Switch-Cases for the lifting process*/
enum States {Start, ArmDown, GrabObject,
             ArmUp, ReleaseObject, EmptyBox, Wait
            };

/*Defining start state for lifting process*/
States current_state = Wait;
/*Equivalent to 1.8 degrees per step*/
const int stepsPerRevolution = 200;

/*Creating servo object to control a servo*/
Servo servoRightArm;
Servo servoLeftArm;
Servo servoBox;
/*initializing the stepper library on pins 7 through 10:*/
Stepper myStepper(stepsPerRevolution, 7, 8, 9, 10);

/*Defining laser pin*/
const byte Laser = 3;
boolean laserstate = false;

/*Defining pulse-values to control a servo*/
int openRight = 80;
int openLeft = 120;
int openBox = 180;
int closeRight = 120;
int closeLeft = 80;
int lift = 3700;

/* TWI states */
typedef enum
{
  TWI_CMD_ARM_INIT    = 0x20,
  TWI_CMD_DROP_OFF_START    = 0x21,
  TWI_CMD_PICK_UP_START   = 0x22,
  TWI_CMD_DROP_OFF_STATUS = 0x23,
  TWI_CMD_PICK_UP_STATUS    = 0x24,
  TWI_CMD_ERROR   = 0x25
} TWI_CMD;

typedef enum
{
  TWI_CMD_ARM_REQ_BOX_INFO  = 2,
  TWI_CMD_ARM_REQ_OBJ_INFO  = 3,
  TWI_CMD_ARM_REQ_COLLECT_INFO  = 4
} TWI_CMD_Init_Req;

typedef enum
{
  PICK_UP_DONE   = 2,
  PICK_UP_FORWARD = 3,
  PICK_UP_BACKWARD  = 4,
  PICK_UP_RUNNING  = 5,
  PICK_UP_FAILED   = 6,
  PICK_UP_DONE_DRIVE = 7,
  PICK_UP_IDLE  = 8
} Pick_Up_Status;

typedef enum
{
  DROP_OFF_DONE = 2,
  DROP_OFF_RUNNING = 3,
  DROP_OFF_FAILED = 4,
  DROP_OFF_IDLE = 5
} Drop_Off_Status;

typedef enum
{
  SOCK   = 2,
  CUBE   = 3,
  GLASS  = 4,
  OBJECT = 5
} Object;


Object object_t;
TWI_CMD_Init_Req twi_cmd_init_req_t;
Drop_Off_Status drop_off_status_t;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;
TWI_CMD twi_cmd_t;

/* Buffers for receiving and transmitting bytes */
uint16_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];

void setup()
{
  Serial.begin(BAUD_RATE);
  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  /*configurating analog and digital ports*/
  pinMode(Laser, INPUT);
  servoRightArm.attach(6);
  servoLeftArm.attach(5);
  servoBox.attach(11); //the box on Arlo Robot
  /*Arm on Arlo robot opened */
  servoRightArm.write(openRight);
  servoLeftArm.write(openLeft);
  /*setting the speed of stepper motor at 80 rpm*/
  myStepper.setSpeed(80);

}

void loop()
{
  if (twi_cmd_t == TWI_CMD_PICK_UP_START && pick_up_status_t != PICK_UP_DONE)
  {
    laserstate = false;
    attachInterrupt(digitalPinToInterrupt(Laser), laserState, LOW);
    pick_up_status_t = PICK_UP_RUNNING;
    current_state = Start;
  }

  else if (twi_cmd_t == TWI_CMD_DROP_OFF_START && drop_off_status_t != DROP_OFF_DONE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Dropping off objects has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    current_state = EmptyBox;
    drop_off_status_t = DROP_OFF_RUNNING;
  }

  switch (current_state) {
    /*
       Start position where arms are being opened.
       If laser is not activated, the next state is entered.
    */
    case Start:

      Serial.print("Activate laser to begin\n");
      delay(100);
      servoRightArm.write(openRight);
      servoLeftArm.write(openLeft);
      current_state = ArmDown;
      break;

    /*
      Second case where arms are lifted down
      by defining correct step values.
      When arm down, grabbing process is to be entered.
    */
    case ArmDown:
      Serial.print("Case ArmDown\n");
      servoRightArm.write(openRight);
      servoLeftArm.write(openLeft);
      Serial.println("Arm going down");
      myStepper.step(lift);
      //Om laser bryts
      //Skicka command till due att backa X cm
      //ANNARS gå vidare till nästa case
      current_state = GrabObject;
      break;

    /*
      Here objects are being grabbed. To make sure an object is grabbed,
      a tag is put on every object. When the rfid sensor detects the tag,
      an object has been grabbed. Next state to be entered.
    */
    case GrabObject:
      Serial.println("Grabbing object");
      servoRightArm.write(closeRight);
      servoLeftArm.write(closeLeft);
      current_state = ArmUp;
      break;

    /*
      When objects are grabbed, its time to
      lift up the arms with the specified object. The arm
      will stop when its close enough to the Arlo box. When done,
      next state to be entered.
    */
    case ArmUp:
      Serial.println("Arm going up");
      myStepper.step(-lift-100);
      current_state = ReleaseObject;
      break;

    /*
      When arms are close enough to the Arlo box, the
      arms will open, and release the specified object in the
      Arlo box. When object is released, arms to be shut.
      Next state is entered only when all three objects are grabbed.
    */
    case ReleaseObject:
      Serial.println("Release object in box");
      servoRightArm.write(openRight);
      servoLeftArm.write(openLeft);  
      delay(300);
      current_state = Wait;
      delay(300);
      pick_up_status_t = PICK_UP_DONE;
      break;
    case EmptyBox:
      Serial.println("Empty box in container");
      delay(7000);
      servoBox.write(180);
      current_state = Wait;
      drop_off_status_t = DROP_OFF_DONE;
      break;
    case Wait:
      // Just wait for Arlo to respond
      break;
  }
}

void laserState()
{

  laserstate = true;
  detachInterrupt(digitalPinToInterrupt(Laser));

}






/********************************************************/
/* Behöver inte röra dessa funktioner                   */
/********************************************************/
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
  }
}

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
          tx_buf[2] = 0x50; // Random value
          break;
        case TWI_CMD_ARM_REQ_OBJ_INFO:
          tx_buf[0] = TWI_CMD_ARM_INIT;
          tx_buf[1] = TWI_CMD_ARM_REQ_BOX_INFO;
          tx_buf[2] = 0x50; // Random value
          break;
        case TWI_CMD_ARM_REQ_COLLECT_INFO:
          tx_buf[0] = TWI_CMD_ARM_INIT;
          tx_buf[1] = TWI_CMD_ARM_REQ_COLLECT_INFO;
          tx_buf[2] = ARLO_COLLECT_STATUS;  // Collect all info
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

