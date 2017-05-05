#include <Wire.h>

#define DEVICE_ADDRESS 0x50

void setup() 
{
  Wire.begin(DEVICE_ADDRESS);                // join i2c bus with address #8
  Wire.onReceive(receiveEvent); // register event
  Serial.begin(115200);           // start serial for output
}

void loop() 
{
  delay(100);
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany) 
{
  while (Wire.available()) 
  { // loop through all but the last
    char c = Wire.read(); // receive byte as a character
    Serial.print(c);         // print the character
    Serial.println();
  }
  int x = Wire.read();    // receive byte as an integer
  Serial.println(x);         // print the integer
}
