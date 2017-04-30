/**
   Wire Slave Receiver
   by Nicholas Zambetti <http://www.zambetti.com>

   Demonstrates use of the Wire library
   Receives data as an I2C/TWI slave device
   Refer to the "Wire Master Writer" example for use with this

   Created 29 March 2006

   This example code is in the public domain.

   Editor: Jonas Eiselt
*/

#include <Wire.h>
#define SLAVE_ADDRESS  8

byte val = 0;

void setup()
{
  Serial.begin(9600);           // start serial for output
  Wire.begin();  

  Serial.println("---------");
  Serial.println("TWI Slave Example");
  Serial.println("2017-04-30");
  Serial.println("---------");
}

int tic = 0;

void loop()
{
  for (int i = 0; i < 7; i++)
  {
    Serial.print(tic++);
    Serial.println();
    
    Wire.beginTransmission(SLAVE_ADDRESS);
    Wire.write(0);  
    Wire.endTransmission();

    while (Wire.available())
    {
      char c = Wire.read(); // receive byte as a character
      Serial.print(c, HEX);
      Serial.println();
    }
    delay(250);
  }
  ESP.wdtFeed();
}
