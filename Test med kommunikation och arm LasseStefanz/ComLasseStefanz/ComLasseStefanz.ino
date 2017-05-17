/**
   UnoReceiver.ino

   Author: Jonas Eiselt
   Created: 2015-05-05
*/

#include <Wire.h>
#include <Servo.h>
//Adafruit PN532
#include <Adafruit_PN532.h>

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33
//////////////////////////////////////////////////////////////////////
/******************Konfigurering av statemachine ********************/
//////////////////////////////////////////////////////////////////////

//Define the different Switch-Cases
enum States {Start, GrabObject, LiftUp, ReleaseObject, LiftDown, Stop};

Servo servoArm;  // create servo object to control a servo

char state = 'readCommand';

//shield with an I2C connection:
//#define PN532_IRQ   (2)
//#define PN532_RESET (3)

//shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int value = 1;
int pos = 0;
int i = 0;

//int buttonState = LOW;         // variable for reading the pushbutton status


const int buttonPin = 7;     // the number of the pushbutton pin
const int buttonDown = 4;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
int buttonStateDown = 0;

uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };//Buffer to store the returned UID
uint8_t uidLength; //Length of the UID (4 or 7 bytes)
#define CM 1      //Centimeter
#define INC 0     //Inch
#define TP 8      //Trig_pin
#define EP 10      //Echo_pin

States current_state = Stop;
States next_state = Stop;
//////////////////////////////////////////////////////////////////////
/****************Konfigurering av statemachine klar******************/
//////////////////////////////////////////////////////////////////////

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
  TWI_CMD_ERROR     = 0x25,
  TWI_CMD_NONE      = 0x26
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
  PICK_UP_IDLE    = 8,
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
TWI_CMD twi_cmd_t = TWI_CMD_NONE;

/* Buffers for receiving and transmitting bytes */
uint16_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];

void setup()
{
  Serial.begin(BAUD_RATE);

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  //////////////////////Test ifrån lasse stefanz/////////////////7///
  //nfc.begin();

  //uint32_t versiondata = nfc.getFirmwareVersion();
  //if (! versiondata) {
  //Serial.print("Didn't find PN53x board");
  //while (1); // halt
  //}
  //Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX); // Got ok data, print it out!
  //Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  //Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  //nfc.SAMConfig(); // configure board to read RFID tags

  Serial.println("Waiting for an ISO14443A Card ...");
  pinMode(TP, OUTPUT);      // set TP output for trigger
  pinMode(EP, INPUT);       // set EP input for echo


  //For Claw
  servoArm.attach(5);

  //   servoArm.write(180);
  //   delay(5000);
  //  servoArm.write(0);

  //For Dc-Motor--------------------------------------

  pinMode(buttonPin, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  digitalWrite(9, HIGH);
  //---------------------------------------------------

  //  ServoMotor 0-100 grader
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoArm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    digitalWrite(9, HIGH); //stanna
    //////////////////Test ifrån lasse stefanz slut!!!//////////////
  }
}

void loop()
{
  buttonStateDown = digitalRead(buttonDown);
  buttonState = digitalRead(buttonPin);

  switch (current_state) {
    case Start:

      //success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
      /*

        if (success)
        {
        // Display some basic information about the card
        Serial.println("Found an ISO14443A card");
        Serial.print("  UID Length: ");
        Serial.print(uidLength, DEC);
        Serial.println(" bytes");
        Serial.print("  UID Value: ");
        nfc.PrintHex(uid, uidLength);
        Serial.println("");
      */
      Serial.print("Case Start\n");
      next_state = GrabObject;
      break;

    // }

    //Enter this case when the object is detected through NFC-tag. In this case the claw grabs the objects.
    case GrabObject:

      Serial.print("Case GrabObject\n");
      //  for (pos = 0; pos <= 20; pos += 1) {   // goes from 0 degrees to 30 degrees
      // in steps of 1 degree
      pos = 20;

      servoArm.write(pos);                    // tell servo to go to position in variable 'pos'
      delay(50);
      // }

      next_state = LiftUp;
      break;

    //Enter this case when the object is grabbed. In this case the lift lifts up the object untill button is pressed.
    case LiftUp:
      Serial.print("Case Lift Up\n");
      digitalWrite(9, LOW);

      while (1) {
        // read the state of the pushbutton value:
        buttonState = digitalRead(buttonPin);

        // check if the pushbutton is pressed.
        // if it is, the buttonState is HIGH:
        if (buttonState == LOW) {
          // turn LED on:
          digitalWrite(12, HIGH); //snurrar uppåt
          Serial.print("Button är HIGH,dvs INTE nedtryckt");
          delay(15);
          analogWrite(3, 100);

          Serial.println("Button is off");

        } else {
          // turn LED off:
          Serial.println("Button is on BUTTON är NEDTRYCKT ");
          digitalWrite(9, HIGH); //stanna
          next_state = ReleaseObject;
          break;
        }
      }


    //Enter this case when the lifting process is done. In this case the Claw releases the object.
    case ReleaseObject:
      Serial.print("Case ReleaseObject\n");
      delay(5000);
      //  ServoMotor 0-100 grader
      for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
        // in steps of 1 degree
        servoArm.write(pos);              // tell servo to go to position in variable 'pos'
        delay(15);                       // waits 15ms for the servo to reach the position
        digitalWrite(9, HIGH); //stanna

      }

      next_state = LiftDown;
      break;

    //Enter this case when the release is done. The lift goes down.
    case LiftDown:
      Serial.print("Case LiftDown\n");


      digitalWrite(9, LOW);

      while (1) {
        // read the state of the pushbutton value:
        buttonStateDown = digitalRead(buttonDown);

        // check if the pushbutton is pressed.
        // if it is, the buttonState is HIGH:
        if (buttonStateDown == LOW) {
          // turn LED on:
          digitalWrite(12, LOW); //snurrar uppåt
          Serial.print("Button är HIGH,dvs INTE nedtryckt för DOWN knappen");
          delay(15);
          analogWrite(3, 100);

          Serial.println("Button is off");

        } else {
          // turn LED off:
          Serial.println("Button is on BUTTON är NEDTRYCKT för DOWN knappen ");
          digitalWrite(9, HIGH); //stanna
          Serial.print("KLAR");
          pick_up_status_t = PICK_UP_DONE;
          next_state = Stop;
          break;
        }
      }

      break;

    case Stop:

      //Serial.println("Stop");
      //next_state = Stop;
      if (twi_cmd_t == TWI_CMD_PICK_UP_START && pick_up_status_t != PICK_UP_DONE)
      {
        Serial.println("\nStart pick-up has been requested");

        liftCube();
      }
  else {
    next_state = Stop;
  }

  break;

}
current_state = next_state; //Change case
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
      pick_up_status_t = PICK_UP_RUNNING;
      current_state = Start;

      Serial.print("Pick_up started");
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

void liftCube()
{
  /* Program code goes here... */
  next_state = Start;
  Serial.println("Lifting...");
  pick_up_status_t = PICK_UP_RUNNING;
  //delay(500);
  //return true;
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
