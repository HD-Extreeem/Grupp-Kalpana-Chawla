/**

   Author: Désirée Jönsson the LiftingProcess part witch the switch cases etc. Namra Gill have worked on and helped find the angels on the Servo Motor.
           The communication code by Jonas Eiselt and Hadi Deknache.

   OBS! Servomotorn skall implementeras.
*/

#include <Wire.h>
#include <Servo.h>
#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3

/* Crane-id */
#define CRANE 0x33
#define ARLO_COLLECT_STATUS 0


/* TWI states */
typedef enum
{
  TWI_CMD_ARM_INIT        = 0x20,
  TWI_CMD_DROP_OFF_START        = 0x21,
  TWI_CMD_PICK_UP_START        = 0x22,
  TWI_CMD_DROP_OFF_STATUS    = 0x23,
  TWI_CMD_PICK_UP_STATUS        = 0x24,
  TWI_CMD_ERROR        = 0x25
} TWI_CMD;

typedef enum
{
  TWI_CMD_ARM_REQ_BOX_INFO    = 2,
  TWI_CMD_ARM_REQ_OBJ_INFO    = 3,
  TWI_CMD_ARM_REQ_COLLECT_INFO    = 4
} TWI_CMD_Init_Req;

typedef enum
{
  PICK_UP_DONE                        = 2,
  PICK_UP_FORWARD                     = 3,
  PICK_UP_BACKWARD                    = 4,
  PICK_UP_RUNNING                 = 5,
  PICK_UP_FAILED                 = 6,
  PICK_UP_DONE_DRIVE                 = 7,
  PICK_UP_IDLE                        = 8
} Pick_Up_Status;

typedef enum
{
  DROP_OFF_DONE                       = 2,
  DROP_OFF_RUNNING                 = 3,
  DROP_OFF_FAILED                 = 4,
  DROP_OFF_IDLE                 = 5
} Drop_Off_Status;

typedef enum
{
  SOCK                             = 2,
  CUBE                             = 3,
  GLASS                             = 4,
  OBJECT                             = 5
} Object;
Object object_t;
TWI_CMD_Init_Req twi_cmd_init_req_t;
Drop_Off_Status drop_off_status_t;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;
//Find_Object_Status find_object_status_t = OBJECT_NOT_FOUND;
TWI_CMD twi_cmd_t;

/* Buffers for receiving and transmitting bytes */
uint8_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];


//////////////----------Kran setup!!!--------------////////////////
//Define the different Switch-Cases
enum States {Start, GrabObject, LiftUp, ReleaseObject, LiftDown, Stop, WaitForRelease, UpdateStatus};

States current_state = Start;
States next_state;
int value = 1;
int pos = 0;
int i = 0;

int buttonUp = 7;     // the number of the pushbutton pin
int buttonDown = 6;     // the number of the pushbutton pin
int buttonStateUp = 0;         // variable for reading the pushbutton status
int buttonStateDown = 0;
int brake = 9;
int motorA = 12;
int pwmSpeed = 3;

Servo servoArm;  // create servo object to control a servo

void setup()
{
  Serial.begin(BAUD_RATE);

  //  ServoMotor 0-100 grader
  servoArm.attach(10);
  pos = 0;
  servoArm.write(pos);              // tell servo to go to position in variable 'pos'




  //For Dc-Motor--------------------------------------
  pinMode(motorA, OUTPUT); //Initiates Motor Channel A pin
  pinMode(brake, OUTPUT); //Initiates Brake Channel A pin
  pinMode(buttonUp, INPUT);
  pinMode(pwmSpeed, OUTPUT);

  pinMode(buttonDown, INPUT);
  digitalWrite(brake, HIGH);
  digitalWrite(buttonUp, HIGH);
 
  digitalWrite(buttonDown, HIGH);

  //---------------------------------------------------
  
  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
}
void loop()
{
  buttonStateDown = digitalRead(buttonDown);
  buttonStateUp = digitalRead(buttonUp);

  /*
    digitalWrite(12, HIGH); //snurrar uppåt
    analogWrite(3, 100);
    digitalWrite(brake, LOW);

    while (1) {
      buttonStateDown = digitalRead(buttonDown);
      buttonStateUp = digitalRead(buttonUp);
      if (buttonStateDown == LOW) {
        Serial.println("Down!!!");
      }
      else if (buttonStateUp == LOW) {
        Serial.println("UP!!!");
        digitalWrite(brake, HIGH);
      }
      else{
        Serial.println("Nothing!!!");
      }
      }*/

  switch (current_state) {

    ////////////////////////START CASE!!!!!/////////////////////////
    case Start:

      if (twi_cmd_t == TWI_CMD_PICK_UP_START && pick_up_status_t != PICK_UP_DONE)
      {
        Serial.println("Börja hitta objektet!");
        pick_up_status_t = PICK_UP_RUNNING;
        digitalWrite(brake, LOW);
        digitalWrite(motorA, LOW); //snurrar uppåt
        analogWrite(pwmSpeed, 100);
        next_state = LiftDown;
      }
      else {
        next_state = Start;
      }

      break;

    ////////////////////////LIFTDOWN CASE!!!!!/////////////////////////

    case LiftDown:
      digitalWrite(brake, LOW);
      digitalWrite(motorA, LOW);
      analogWrite(pwmSpeed, 250);
      Serial.print("Case LiftDown\n");
      // find_object_status_t = OBJECT_NOT_FOUND;
      // read the state of the pushbutton value:
      buttonStateDown = digitalRead(buttonDown);
      // check if the pushbutton is pressed.
      // if it is, the buttonState is HIGH:
      if (buttonStateDown == LOW) {
        // turn LED off:
        Serial.println("Ner knappen tryckt!!!!!");
        digitalWrite(brake, HIGH); //stanna
        Serial.print("KLAR");
        next_state = GrabObject;
      }
      else {
        next_state = LiftDown;
      }

      break;
    ////////////////////////GrabObject CASE!!!!!/////////////////////////

    case GrabObject:
      Serial.println("grabobject");
      Serial.println("Found object!!!");

      pos = 100;                               // The position the claw shall go to and grab the object.
      servoArm.write(pos);                    // tell servo to go to position in variable 'pos'

      delay(3000);
      Serial.print("Case GrabObject\n");
      digitalWrite(brake, LOW);
      digitalWrite(motorA, HIGH); //snurrar uppåt
      //Serial.print("Button är HIGH,dvs INTE nedtryckt");
      analogWrite(pwmSpeed, 100);
      next_state = LiftUp;

      break;

    ////////////////////////LIFTUP CASE!!!!!/////////////////////////

    //Enter this case when the object is grabbed. In this case the lift lifts up the object untill button is pressed.
    case LiftUp:
      // check if the pushbutton is pressed.
      // if it is, the buttonState is HIGH:

      if (buttonStateUp == LOW) {
        // turn LED off:
        Serial.println("Stannar motorn nu uppe!");
        digitalWrite(brake, HIGH); //stanna
        pick_up_status_t = PICK_UP_DONE;
        // drop_off_status_t = DROP_OFF_RUNNING;
        next_state = WaitForRelease;
      }
      else {
        next_state = LiftUp;
      }
      break;
    ////////////////////////WAITFORELEASE CASE!!!!!/////////////////////////

    //Enter this case and waiting for the Robot to go to the box.
    case WaitForRelease:
      Serial.println("VÄNTAR PÅ ATT SLÄNGA OBJEKTET");
      if (twi_cmd_t == TWI_CMD_DROP_OFF_START && drop_off_status_t != DROP_OFF_DONE)
      {
        //delay(3000);
        Serial.println("drop of starting!");
        // drop_off_status_t = DROP_OFF_RUNNING;
        //next_state = ReleaseObject;
      }

      else if (twi_cmd_t == TWI_CMD_DROP_OFF_STATUS && drop_off_status_t != DROP_OFF_DONE) {
        Serial.println("Releasing now!");
        //  drop_off_status_t = DROP_OFF_RUNNING;
        next_state = ReleaseObject;
      }

      else {

        next_state = WaitForRelease;
      }
      break;
    ////////////////////////RELEASEOBJECT CASE!!!!!/////////////////////////

    //Enter this case when the WaitForRelese case is done. This is when the Robot is at the Box cordinates.
    //In this case the Claw releases the object.
    case ReleaseObject:
      // drop_off_status_t = DROP_OFF_RUNNING;
      Serial.print("Case ReleaseObject\n");
      pos = 0;                                // The position the claw shall go to and grab the object.
      servoArm.write(pos);                    // tell servo to go to position in variable 'pos'
      delay(3000);
      digitalWrite(brake, HIGH); //stanna

      //   delay(2000);
      //   drop_off_status_t = DROP_OFF_IDLE;
      //   pick_up_status_t = PICK_UP_IDLE;
      //  find_object_status_t = OBJECT_NOT_FOUND;
      next_state = UpdateStatus;
      drop_off_status_t = DROP_OFF_DONE;
      break;

    ////////////////////////Update Status!!!!!/////////////////////////

    //Enter this case when the Release case is done. In this state the satus of drop_off and pick up changes.
    //In this case the Claw releases the object.
    case UpdateStatus:
      // drop_off_status_t = DROP_OFF_RUNNING;
      Serial.print("Case new Satus\n");
      //   delay(3000);
      //  digitalWrite(brake, HIGH); //stanna
      drop_off_status_t = DROP_OFF_IDLE;
      pick_up_status_t = PICK_UP_IDLE;
      next_state = Start;
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

  twi_cmd_t = rx_buf[0];
  switch (twi_cmd_t)
  {

    /*
      case TWI_CMD_FIND_OBJECT:
      /* Pick up object */
    //  object_t = rx_buf[1];
    //  find_object_status_t = OBJECT_NOT_FOUND;
    //   break;


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
      tx_buf[0] = TWI_CMD_DROP_OFF_STATUS;
      tx_buf[1] = drop_off_status_t;  // Fill index with current drop off status
      break;
    /*

        case TWI_CMD_FIND_OBJECT_STATUS:
          Serial.println("\nRobot requesting find object status!");
          tx_buf[0] = TWI_CMD_FIND_OBJECT_STATUS;
          tx_buf[1] = find_object_status_t;  // Fill index with current drop off status
          Serial.print(find_object_status_t, HEX);
          Serial.println();
          break;

    */
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



//void findObject()
//{
/* Program code goes here... */
// Serial.println("\n~~~~~~~~\nFinding cube");
//  find_object_status_t = OBJECT_NOT_FOUND;

//  Serial.println("Finding... Finding...");
//  for (int i = 0; i < 10000; i++)
// {
//   Serial.println(i);
// Serial.println(i);
// Serial.print(' ');
// delay(400000);
// }
// Serial.println();

// find_object_status_t = OBJECT_FOUND;
// Serial.println("Cube was found - NOT\n~~~~~~~~\n");
//}


