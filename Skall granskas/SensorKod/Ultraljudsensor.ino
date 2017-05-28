/* Ultrasonic Ranging
  Library for HC-SR04 Ultrasonic Ranging Module.librar
  created 2011
  by Robi.Wang
  Modified by Namra Gill
  Commented by Namra Gill
  www.Elecfreak.com
*/

#define CM 1      //Centimeter
#define TP 8      //Trig_pin
#define EP 10     //Echo_pin

/*Metoden exekveras endast en gång*/
void setup() {
  // set TP output for trigger pulses
  pinMode(TP, OUTPUT);
  // set EP input for echo
  pinMode(EP, INPUT);
  Serial.begin(9600);
  Serial.println("-------------------Starting--detection---------------------");
}

/*Metoden kommer att loopas hela tiden*/
void loop() {
  long microseconds = TP_init();
  Serial.print("ret=");
  Serial.println(microseconds);
  long distance_cm = Distance(microseconds, CM);
  Serial.print("Distacne_CM = ");
  Serial.println(distance_cm);
}

/*Metoden beräknar avståndet till objektet i cm*/
long Distance(long time, int flag)
{
  long distance;
  distance = time / 29 / 2; // distance_cm : (Duration of high level)/(Sonic :29.4 cm/us))/2
  return distance;
}

/*Initiering av pinnar*/
long TP_init()
{
  digitalWrite(TP, LOW);
  delayMicroseconds(2);
  digitalWrite(TP, HIGH);// pull the Trig pin to high level for more than 10us impulse
  delayMicroseconds(10);
  digitalWrite(TP, LOW);
  long microseconds = pulseIn(EP, HIGH);// waits for the pin to go HIGH, and returns the length of the pulse in microseconds
  return microseconds;// return microseconds
}
