/**
   UnoReceiver.ino
   Author: Namra Gill (crane code) and Jonas Eiselt (TWI-code)
   Created: 2017-05-21
   
   Modified by ??
*/

#include <Servo.h>
#include <Wire.h>

Servo servoArm;  // create servo object to control a servo
Servo servoBottom;

const int buttonPin = 2;     // the number of the pushbutton pin
const int buttonPin2 = 6;

int pos = 0;
int buttonState = 0;
int buttonState2 = 0;
int next;

#define CM 1      //Centimeter
#define INC 0     //Inch
#define TP 8      //Trig_pin
#define EP 10     //Echo_pin

int flagDrive = 0;
int objectsLeft = 3;

enum States {Wait, SensorDetection, GrabObject, DropObject, LiftUp};
States state = Wait;

long distance_cm;
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
uint16_t rx_buf[RX_DATA_LENGTH];
uint8_t tx_buf[TX_DATA_LENGTH];

void setup()
{
  Serial.begin(BAUD_RATE);

  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  pinMode(TP, OUTPUT);      // set TP output for trigger
  pinMode(EP, INPUT);       // set EP input for echo

  pinMode(buttonPin, INPUT);
  pinMode(buttonPin2, INPUT);

  servoArm.attach(4);
  servoBottom.attach(5);
  servoArm.write(70);
  servoBottom.write(150);

  // liftDown();

  // liftUp();

  Wire.begin(DEVICE_ADDRESS);
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);

  state = Wait;
}

void loop()
{
  /* Om en pick-up-object-req har kommit in OCH Om objekt inte har plockats upp */
  if (twi_cmd_t == TWI_CMD_PICK_UP_START && pick_up_status_t == PICK_UP_IDLE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Pick-up of an object has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    state = SensorDetection;
  }
  /* Om en pick-up-obj-status-req har kommit in OCH Om objekt inte har plockats upp */
  else if (twi_cmd_t == TWI_CMD_PICK_UP_STATUS && pick_up_status_t == PICK_UP_DONE_DRIVE && flagDrive == 0)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Pick-up status has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");

    /* Öppna arm */
    openArm();

    /* Vi vill att Arlo ska köra framåt OCH vänta tills Arlo kört färdigt */
    pick_up_status_t = PICK_UP_FORWARD;
    state = Wait;

    flagDrive = 1;
  }
  else if (twi_cmd_t == TWI_CMD_PICK_UP_STATUS && pick_up_status_t == PICK_UP_DONE_DRIVE && flagDrive == 1)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Grabbing objects has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    state = GrabObject;
  }
  else if (twi_cmd_t == TWI_CMD_DROP_OFF_START && drop_off_status_t == DROP_OFF_IDLE)
  {
    Serial.println("\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~");
    Serial.println("Dropping off objects has been requested");
    Serial.println("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n");
    
    state = DropObject;
  }

  /**********************************/
  /*          Switch case           */
  /**********************************/
  switch (state)
  {
    case SensorDetection:
      sensor(true);
      if (distance_cm <= 30)
      {
        Serial.println("Object was detected!");

        /* Vi vill att Arlo ska köra bakåt OCH vänta tills Arlo kört färdigt */
        pick_up_status_t = PICK_UP_BACKWARD;
        state = Wait;
      }
      else
      {
        state = SensorDetection;
      }
      break;
    case GrabObject:
      /* Greppa objekt */
      closeArm();

      objectsLeft--;
      Serial.print(objectsLeft);
      Serial.println(" left to pick up");

      /* Om det inte finns några objekt kvar att plocka upp */
      if (objectsLeft == 0)
      {
        /* Höj arm */
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
      {
        servoArm.write(50);
        delay(100); // Nödvändigt att ha en delay?
        digitalWrite(9, LOW);
        digitalWrite(12, HIGH); //uppåt
        analogWrite(3, 255);

        Serial.println("Lift up elevator");

        doThisOnce = false;
      }

      buttonState2 = digitalRead(buttonPin2);
      Serial.println(buttonState2);

      /* Om den övre knappen inte är nedtryckt */
      if (buttonState2 == HIGH)
      {
        state = LiftUp;
      }
      else
      {
        Serial.println("Upper button is on");
        digitalWrite(9, HIGH);    // Stänger av hiss

        state = Wait;
        pick_up_status_t = PICK_UP_DONE;
      }
      break;
    case DropObject:
      /* Släpp objekt i låda */
      dropObject();
      drop_off_status_t = DROP_OFF_DONE;
      break;
    case Wait:
      // Gör ingenting
      break;
  }
}

/****************************************************************/
/*                Metoder för själva påbyggnaden                */
/****************************************************************/

/*
 * Metod för att höja kran till sin högsta nivå. Anropas från setup när 
 * kommunikation ej har initierats -> OK att ha en while-loop då!
 */
void liftUp()
{
  buttonState2 = 1;

  servoArm.write(50);
  delay(1000);
  digitalWrite(9, LOW);
  digitalWrite(12, HIGH); //uppåt
  analogWrite(3, 255);
  Serial.println("Lift up elevator");

  /* Vänta tills kranen är helt uppe */
  while (buttonState2 != LOW)
  {
    buttonState2 = digitalRead(buttonPin2);
    Serial.println(buttonState2);
  }

  Serial.println("Upper button is on");
  digitalWrite(9, HIGH);    // Stänger av hiss
}

/* Metod för att sänka kranen till botten */
void liftDown()
{
  buttonState = 1;

  Serial.println("Starting detection");
  digitalWrite(9, LOW);
  digitalWrite(12, LOW); //hissar neråt
  Serial.println("Going down");
  analogWrite(3, 255);

  /* Vänta tills kranen är helt nere */
  while (buttonState != LOW)
  {
    buttonState = digitalRead(buttonPin);
    Serial.println(digitalRead(buttonPin));
  }
  Serial.println("Bottom button is on");
  digitalWrite(9, HIGH);    // Stänger av hiss
}

/* Metod för att öppna arm */
void openArm()
{
  for (pos = 70; pos <= 180; pos += 1)
  {
    servoArm.write(pos);    // tell servo to go to position in variable 'pos'
    Serial.println(pos);
    delay(15);              // waits 15 ms for the servo to reach the position
  }
}

/* Metod för att fösa in föremål i lådan */
void closeArm()
{
  Serial.println("Closing arm to push in object");
  for (pos = 0; pos <= 180; pos += 1)
  {
    servoArm.write(pos);
    Serial.println(pos);// tell servo to go to position in variable 'pos'
    delay(15);   // waits 15ms for the servo to reach the position
  }

  for (pos = 0; pos <= 50; pos += 1)
  { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoArm.write(pos);
    Serial.println(pos);// tell servo to go to position in variable 'pos'
    delay(15);   // waits 15ms for the servo to reach the position
  }
  servoArm.write(70);
}

/* Metod som släpper objekten */
void dropObject()
{
  Serial.println("Dropping objects");
  for (pos = 0; pos <= 50; pos += 1)
  {
    Serial.println(pos);
    servoBottom.write(pos);              // tell servo to go to position in variable 'pos'
    delay(40);                       // waits 15ms for the servo to reach the position
  }
}

/****************************************************************/
/*               Metoder för ultraljudssensor                   */
/****************************************************************/

/* Metod för att bestämma ifall ett objekt har detekterats */
boolean isObjectDetected()
{
  sensor(true);
  if (distance_cm <= 30)
  {
    return true;
  }
  return false;
}

/* Metod som returnerar ?? */
long Distance(long time, int flag)
{
  long distance;
  if (flag)
    distance = time / 29 / 2  ; //Distance_CM  = ((Duration of high level)*(Sonic :340m/s))/2
  else
    distance = time / 74 / 2; //INC
  return distance;
}

/* Metod som ?? */
long TP_init()
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH); //pull the Trig pin to high level for more than 10us impulse
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  long  microseconds = pulseIn(EP, HIGH); //waits for the pin to go HIGH, and returns the length of the pulse in microseconds
  return microseconds;  //return microseconds
}

/* Metod som ?? */
void sensor(boolean s)
{
  long microseconds = TP_init();
  Serial.print("ret=");      //
  Serial.println(microseconds);
  distance_cm = Distance(microseconds, CM);
  Serial.print("Distance_CM = ");
  Serial.println(distance_cm);
}

/****************************************************************/
/*               Metoder för I2C-kommunikationen                */
/****************************************************************/

/* ISR-metod som tar emot data från Arduino Due */
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

/* ISR-metod som skickar data till Arduino Due */
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
