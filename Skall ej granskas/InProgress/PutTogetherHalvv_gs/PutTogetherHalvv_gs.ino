

#include <Wire.h>
#include <Servo.h>
//Adafruit PN532
#include <Adafruit_PN532.h>
#include <SPI.h>

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33

//**********************SPI***********************

// If using the breakout with SPI, define the pins for SPI communication.
//These pins can be changed
#define PN532_SCK (2) //(2)
#define PN532_MOSI (A0)// (8)
#define PN532_SS  (4)// (6)
#define PN532_MISO (5)//(10)
//#define PN532_RESET (13)

// Use this line for a breakout with a software SPI connection (recommended):
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);


uint8_t success;
// Buffer to store the returned UID
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };
// Length of the UID (4 or 7 bytes depending on ISO14443A card type)
uint8_t uidLength;

//*******************SPI-END**********************



//***************SAMMA I BÅDA***********************


//Define the different Switch-Cases
enum States {Start, GrabObject, LiftUp, ReleaseObject, LiftDown, Stop, WaitForRelease, Find};

Servo servoArm;  // create servo object to control a servo

States current_state = Start;
States next_state = Start;

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
const int buttonDown = 6;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
int buttonStateDown = 0;



//***************SAMMA I BÅDA SLUT***********************


//**********************KOMMUNIKATIONEN************



/* TWI states */
typedef enum
{
  TWI_CMD_ARM_INIT      = 0x20,
  TWI_CMD_DROP_OFF_START    = 0x21,
  TWI_CMD_PICK_UP_START   = 0x22,
  TWI_CMD_DROP_OFF_STATUS   = 0x23,
  TWI_CMD_PICK_UP_STATUS    = 0x24,
  TWI_CMD_ERROR       = 0x25,
  TWI_CMD_FIND_OBJECT     = 0x26,
  TWI_CMD_FIND_OBJECT_STATUS  = 0x27,
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
  OBJECT_NOT_FOUND  = 2,
  OBJECT_FOUND    = 3,
  ARLO_ROTATE_LEFT  = 4,
  ARLO_ROTATE_RIGHT = 5,
  ARLO_FORWARD    = 6,
  ARLO_BACKWARD   = 7
} Find_Object_Status;

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
Drop_Off_Status drop_off_status_t = DROP_OFF_IDLE;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;
Find_Object_Status find_object_status_t = OBJECT_NOT_FOUND;
TWI_CMD twi_cmd_t = TWI_CMD_NONE;

/* Buffers for receiving and transmitting bytes */
uint16_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];

//**********************KOMMUNIKATIONEN SLUT************

void setup() {

  //For Dc-Motor--------------------------------------
  pinMode(buttonPin, INPUT);
  pinMode(buttonDown, INPUT);
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  //---------------------------------------------------


  servoArm.attach(10);

  //  ServoMotor 0-100 grader
  for (pos = 0; pos <= 50; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoArm.write(pos);              // tell servo to go to position in variable 'pos'
   // delay(15);                       // waits 15ms for the servo to reach the position
    digitalWrite(9, HIGH); //stanna

  }


  Serial.begin(BAUD_RATE);

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);


  //******************FOR NFC TAG***************


  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX);
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);

  // configure board to read RFID tags
  nfc.SAMConfig();

  Serial.println("Waiting for a rfid/nfc tag");

  //******************FOR NFC TAG END***************



}

void loop() {


  buttonState = digitalRead(buttonPin);
  buttonStateDown = digitalRead(buttonDown);



  // while (1) {
  buttonState = digitalRead(buttonPin);

  // while (1) {
  buttonState = digitalRead(buttonPin);
  switch (current_state)

  {
     case Start:

       // success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
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
         pos=150;
          
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
        for (pos = 0; pos <= 60; pos += 1) { // goes from 0 degrees to 180 degrees
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
           // next_state = ReleaseObject;
            break;
          }
        }


         next_state = Start;
        break;

        case Stop:
        Serial.print("KLAR");
        break;
    }

    current_state = next_state; //Change case
  //}
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

  twi_cmd_t = rx_buf[0];
  switch (twi_cmd_t)
  {
    case TWI_CMD_FIND_OBJECT:
      /* Pick up object */
      object_t = rx_buf[1];
      find_object_status_t = OBJECT_NOT_FOUND;
      break;
    case TWI_CMD_PICK_UP_START:
      /* Pick up object */
      object_t = rx_buf[1];
      pick_up_status_t = PICK_UP_IDLE;
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
          tx_buf[2] = 0x50; // Collect all info
          break;
        default:
          // Do nothing
          break;
      }
      break;
    case TWI_CMD_DROP_OFF_STATUS:
      Serial.println("\nRobot requesting drop off status!");
      tx_buf[0] = TWI_CMD_DROP_OFF_STATUS;
      tx_buf[1] = drop_off_status_t;  // Fill index with current drop off status
      break;
    case TWI_CMD_FIND_OBJECT_STATUS:
      Serial.println("\nRobot requesting find object status!");
      tx_buf[0] = TWI_CMD_FIND_OBJECT_STATUS;
      tx_buf[1] = find_object_status_t;  // Fill index with current drop off status
      Serial.print(find_object_status_t, HEX);
      Serial.println();
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

void liftCube()
{
  // noInterrupts();

  /* Program code goes here... */
  Serial.println("\n~~~~~~~~\nLifting cube");
  pick_up_status_t = PICK_UP_RUNNING;

  Serial.println("Lifting... lifting...");
  for (int i = 0; i < 10000; i++)
  {
    Serial.println(i);
    // Serial.println(i);
    // Serial.print(' ');
    // delay(400000);
  }
  // Serial.println();

  pick_up_status_t = PICK_UP_DONE;
  Serial.println("Cube was lifted\n~~~~~~~~\n");

  // interrupts();
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

void findObject()
{
  /* Program code goes here... */
  Serial.println("\n~~~~~~~~\nFinding cube");
  find_object_status_t = OBJECT_NOT_FOUND;

  Serial.println("Finding... Finding...");
  for (int i = 0; i < 10000; i++)
  {
    Serial.println(i);
    // Serial.println(i);
    // Serial.print(' ');
    // delay(400000);
  }
  // Serial.println();

  find_object_status_t = OBJECT_FOUND;
  Serial.println("Cube was found - NOT\n~~~~~~~~\n");
}
