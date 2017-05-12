
/*

  Désirée Jönsson och Namra Gill 170512

  StateMachine for controlling lifting process and
  for communication between Ardunio Uno "Påbyggnadsanläggning" and Ardunio Due "Plattform"


*/

#include <Servo.h>
//Adafruit PN532
#include <Wire.h>
#include <Adafruit_PN532.h>

//Define the different Switch-Cases
enum States {Start, GrabObject, LiftUp, ReleaseObject, LiftDown};

Servo servoArm;  // create servo object to control a servo
Servo servoBottom;

char state = 'readCommand';

//shield with an I2C connection:
#define PN532_IRQ   (2)
#define PN532_RESET (3)

//shield with an I2C connection:
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int value = 1;
int pos = 0;
int i = 0;
const int buttonPin = 7;     // the number of the pushbutton pin
int buttonState = LOW;         // variable for reading the pushbutton status


uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };//Buffer to store the returned UID
uint8_t uidLength; //Length of the UID (4 or 7 bytes)
#define CM 1      //Centimeter
#define INC 0     //Inch
#define TP 8      //Trig_pin
#define EP 10      //Echo_pin


void setup() {
  Serial.begin(115200);

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    while (1); // halt
  }
  Serial.print("Found chip PN5"); Serial.println((versiondata >> 24) & 0xFF, HEX); // Got ok data, print it out!
  Serial.print("Firmware ver. "); Serial.print((versiondata >> 16) & 0xFF, DEC);
  Serial.print('.'); Serial.println((versiondata >> 8) & 0xFF, DEC);
  nfc.SAMConfig(); // configure board to read RFID tags

  Serial.println("Waiting for an ISO14443A Card ...");
  pinMode(TP, OUTPUT);      // set TP output for trigger
  pinMode(EP, INPUT);       // set EP input for echo


  //For Claw
  servoArm.attach(5);

  //For Dc-Motor--------------------------------------
  const int buttonPin = 7;
  pinMode(buttonPin, INPUT);
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT); //Initiates Brake Channel A pin
  //---------------------------------------------------

  //  ServoMotor 0-100 grader
  for (pos = 0; pos <= 100; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    servoArm.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
    digitalWrite(9, HIGH); //stanna

  }
}

void loop() {

  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, uid, &uidLength);
  buttonState = digitalRead(buttonPin);

  States current_state = Start;
  States next_state;

  while (1) {
    switch (current_state) {
      case Start:

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


          Serial.print("Case Start\n");
          next_state = GrabObject;
        }
        break;

      case GrabObject:

        Serial.print("Case GrabObject\n");
        for (pos = 0; pos <= 30; pos += 1) {   // goes from 0 degrees to 30 degrees
          // in steps of 1 degree
          servoArm.write(pos);                    // tell servo to go to position in variable 'pos'
          delay(50);
          next_state = LiftUp;
          break;

        case LiftUp:

          // read the state of the pushbutton value:
          buttonState = digitalRead(buttonPin);

          // if button is pressed, the buttonState is HIGH:
          while (buttonState == HIGH) {
            digitalWrite(12, HIGH); //snurrar uppåt
            analogWrite(3, 100);
            delay(5000);
            Serial.println("Button is off");

          }

          if (buttonState != HIGH) {
            Serial.println("Button is on");
            digitalWrite(9, HIGH); //stanna
            Serial.print("Case LiftUp\n");
            next_state = ReleaseObject;
          }

          break;

        case ReleaseObject:
          Serial.print("Case ReleaseObject\n");
          next_state = LiftDown;
          break;

        case LiftDown:
          Serial.print("Case LiftDown\n");
          next_state = GrabObject;
          break;
        }

        current_state = next_state; //Change case
    }
  }
}

