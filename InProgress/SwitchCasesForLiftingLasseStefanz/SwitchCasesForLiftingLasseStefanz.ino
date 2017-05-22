
/*

  Désirée Jönsson 170512 FUNGERAR Den här fungerar med liftUp

  StateMachine for controlling lifting process and
  for communication between Ardunio Uno "Påbyggnadsanläggning" and Ardunio Due "Plattform"
  
*/


#include <Wire.h>
#include <Servo.h>
//Adafruit PN532
#include <Adafruit_PN532.h>
#include <SPI.h>

#define DEVICE_ADDRESS 2
#define BAUD_RATE 115200

#define RX_DATA_LENGTH 3
#define TX_DATA_LENGTH 3


//Define the different Switch-Cases
enum States {Start, GrabObject, LiftUp, ReleaseObject, LiftDown,Stop};

Servo servoArm;  // create servo object to control a servo


  States current_state = Start;
  States next_state= Start;

char state = 'readCommand';

//shield with an I2C connection:
#define PN532_IRQ   (2)
#define PN532_RESET (3)

//shield with an I2C connection:
//Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET);

int value = 1;
int pos = 0;
int i = 0;

//int buttonState = LOW;         // variable for reading the pushbutton status


const int buttonPin = 7;     // the number of the pushbutton pin
const int buttonDown = 4;     // the number of the pushbutton pin
int buttonState = 0;         // variable for reading the pushbutton status
int buttonStateDown=0;

uint8_t success;
uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };//Buffer to store the returned UID
uint8_t uidLength; //Length of the UID (4 or 7 bytes)
#define CM 1      //Centimeter
#define INC 0     //Inch
#define TP 8      //Trig_pin
#define EP 10      //Echo_pin


void setup() {
  Serial.begin(115200);

/*
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

*/
  //For Claw
 servoArm.attach(10);

         //   servoArm.write(180);  
         //   delay(5000);
          //  servoArm.write(0);
  
  //For Dc-Motor--------------------------------------

  pinMode(buttonPin, INPUT);
  pinMode(buttonDown, INPUT);
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


  buttonState = digitalRead(buttonPin);
  buttonStateDown=digitalRead(buttonDown);



 // while (1) {
    buttonState = digitalRead(buttonPin);
    switch (current_state) {
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


