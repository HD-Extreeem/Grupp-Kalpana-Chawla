/* Statemachine for Lars-Kristerz crane
  @author Namra Gill
  @Commented by Namra Gill
*/
#include <Servo.h>

// create servo object to control a servo motor
Servo servoArm;
Servo servoBottom;

//digital pins for microswitches
const int buttonDown = 2;
const int buttonTop = 6;
int pos = 0;//angle of servo motor
int counter = 3;//counter to count objects
int buttonDownState = 0; //state of buttonDown
int buttonTopState = 0; //state of buttonUp

//Ultrasonic sensor
#define CM 1      //Centimeter
#define TP 8      //Trig_pin
#define EP 10     //Echo_pin
long distance_cm; //distance to object in cm

//All states in Statemachine
enum States {ReadCommand, Start, SensorDetection,
             GrabObject, LiftUpToTop, DropObject,
             LiftUp, Done
            };
//State from the beginning
States state = ReadCommand;

/*This method will only execute once*/
void setup() {
  Serial.begin(9600);
  pinMode(12, OUTPUT); //Initiates Motor Channel A pin
  pinMode(9, OUTPUT);  //Initiates Brake Channel A pin
  pinMode(TP, OUTPUT); // set TP output for trigger
  pinMode(EP, INPUT);  // set EP input for echo
  //set switches as input
  pinMode(buttonDown, INPUT);
  pinMode(buttonTop, INPUT);

  //signal pins for servomotors
  servoArm.attach(4);
  servoBottom.attach(5);
  //start angle of servomotors
  servoArm.write(70);
  servoBottom.write(150);
}

/*This method will loop*/
void loop() {
  /*State machine*/
  switch (state) {

    /*The crane is supposed to wait for Arlo the
      plattform to start its functions */
    case ReadCommand :
      Serial.println("reading command from Arlo");
      state = Start;
      break;

    /*The crane will be elevated down with dc motor until the
      microswitch goes LOW(gets pushed by crane)*/
    case Start :
      digitalWrite(9, LOW);
      digitalWrite(12, LOW); //downwards direction
      Serial.println("Goimg down");
      analogWrite(3, 255);
      buttonDownState = digitalRead(buttonDown);
      Serial.println(digitalRead(buttonDown));

      if (buttonDownState == LOW) {
        Serial.println("Button is on");
        digitalWrite(9, HIGH);
        state = SensorDetection;
      }
      else {
        state = Start;
      }
      break;

    /*The ultrasonuc sensor will be activated,
      if it finds object in the range of 30cm-40cm
      the arm on the crane will open*/
    case SensorDetection :
      delay(1000);
      sensor(true);
      if (distance_cm <= 40 && distance_cm >= 30 ) {

        //opens the Arm
        for (pos = 70; pos <= 180; pos += 1) {
          servoArm.write(pos);
          Serial.println(pos);// tell servo to go to position/angle in variable 'pos'
          delay(15);          // waits 15ms for the servo to reach the position
        }
        state = GrabObject;
      }
      break;

    /*The arm grabs the object by pushing
      in object in the container of the crane*/
    case GrabObject:
      delay(5000);

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

      //after every object that is grabbed minimizez counter with 1
      counter = counter - 1;

      //if all objects are grabbed the crane is elevated upwars
      if (counter == 0) {
        state = LiftUpToTop;
      }
      else {
        delay(3000);
        state = ReadCommand;
      }
      break;

    /*The crane is elevated upwards until buttontop is LOW*/
    case LiftUpToTop :
      // the arm pushed the objects inside the container
      //when the crane is elevating upwards
      servoArm.write(50);
      delay(1000);
      digitalWrite(9, LOW);
      digitalWrite(12, HIGH); //upwards direction
      analogWrite(3, 255); //pwm

      buttonTopState = digitalRead(buttonTop);
      Serial.println(buttonTop);

      if (buttonTopState == LOW) {
        Serial.println("Button is on");
        digitalWrite(9, HIGH);
        delay(2000);
        servoArm.write(70);
        state = DropObject;
      }

      else {
        state = LiftUpToTop ;
      }
      break;

    /*when arlo is with the box,
      the bottom of the container will
      open so that the objects can fall into he box*/
    case DropObject :
      delay(2000);
      sensor(true);
      //opening the bottom of the crane
      if (distance_cm <= 5) {
        for (pos = 0; pos <= 50; pos += 1) {
          Serial.println(pos);
          servoBottom.write(pos);
          delay(40);
        }
        state = Done;
      }
      break;

    /*The crane has done all its functions*/
    case Done :
      Serial.println("done");
      break;
  }
}
/*This method calculates the distance to the object*/
long Distance(long time, int flag)
{
  long distance;
  //Distance_CM  = ((Duration of high level)*(Sonic :340m/s))/2
  distance = time / 29 / 2  ;
  return distance;
}

/*Initiation of pins*/
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

/*This method calculates and
  returns the information of the object
  detected by Ultrasonic sensor*/
void sensor(boolean s) {
  long microseconds = TP_init();
  Serial.print("ret=");      //
  Serial.println(microseconds);
  distance_cm = Distance(microseconds, CM);
  Serial.print("Distacne_CM = ");
  Serial.println(distance_cm);

}

