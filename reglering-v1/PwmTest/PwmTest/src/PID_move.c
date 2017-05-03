/*
 * PID_move.c
 * Created: 2017-04-26 14:38:37
 * This source file handle the main regulation for wheels and rotation
 * for a certain degree.
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */
#include <asf.h>
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"
#include "PID_move.h"


extern uint16_t counter_1; //Encoder counter for rightWheel
extern uint16_t counter_2; //Encoder counter for leftWheel
extern bool c1Loop; //Boolean for checking if encoder counter 1 triggered interrupt
extern bool c2Loop; //Boolean for checking if encoder counter 2 triggered interrupt

/**
This method moveTo handle position moving for a certain distance and direction
Method uses a PID controller for smoother movment of the robot
	@param distance the distance which the robot need to move in centimeters
	@param direction the direction robot need to move forward or backwards
**/
void moveTo (int distance, int direction){
	float totalPulses = (distance/1.38); //Calculate the total pulses needed to move to destination
	uint16_t speed = 1700; // Set speed for moving the robot
	int proportionalError = 0; //P-controller error variable
  int derivativeError=0; //D-controller error variable
  int integralError=0; //I-controller error variable
	int referenceValue = 0; //
	int measurementValue=0;
	int controlValue=0; //Variable to store total value for PID-controller
  int iPart=0; //Variable for the I-controller to keep calculated error*gain
  int dPart=0; //Variable for the D-controller to keep calculated error*gain
	int kp=10; //Gain for the P-controller
  int kd=2; //Gain for the D-controller
  int ki=2; //Gain for the I-controller
	float totMovement = 0; //Variable to store totalmovement during the transportation
	delay_us(300);
	reset_Counter(); //Reset counter for encoders for wheel to ensure counter reseted for next movement
	delay_us(300);

	// Controlls if moving forward or backwards
	if ( direction!=1 || direction!=-1 )
	{
		direction=1;
	}

		/**
		This while loop runs until robot reaches its destination
		checks if totalmovemnt pulses is the same or more than the total pulses needed
		**/
    while (totMovement <= totalPulses) {
		delay_ms(1);
			/**
			Checks if both encoder have counted up since last time
			if counted, then regulates the need values from encoder
			**/
      if (c1Loop == true && c2Loop == true) {
		totMovement = totMovement + ((counter_1+counter_2)/2);
		delay_ms(1);
        measurementValue = (counter_2-counter_1);// Calculates the error diffferce
		delay_us(500);
        reset_Counter();//Reset counter for next regulation later
		delay_us(500);
        dPart = (kd*(proportionalError-derivativeError)); //Calculates d-controller gain
		delay_us(500);
        iPart = (ki*integralError); //Calculates i-controller gain
		delay_us(500);
		proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain
		delay_us(500);
		controlValue = ((kp*proportionalError)+iPart+dPart); //Total regulation for PID calculate new value for correcting the error
		delay_us(500);
		 /**
		 	Check if almost reached the destination to slow down and make a smoother brake
		 **/
 		if ((totMovement/totalPulses)>= 0.7)
 		{
 		//	speed = speed - ((totMovement/totalPulses)*150) ;
		 speed= 1600;
 		}
		//Else same speed set


		rightWheel((speed+controlValue));//New speed for rightWheel
		leftWheel((speed-controlValue));//New speed for leftWheel
		delay_us(500);
	    //error=error*-1;
        derivativeError = proportionalError;
		delay_us(500);
        integralError = (integralError+proportionalError);
		delay_us(500);
        c1Loop = false; //Loop finished to prevent from running loop again
        c2Loop = false;//Loop finished to prevent from running loop again
		delay_us(500);
		//printf("%d\n",totMovement);
      }

	}
	rightWheel(1500);//Stop rightWheel
	leftWheel(1500);//Stop leftWheel
}



/* This method rotates the robot around its own axis at the desired degree and speed
   @param degree rotation degree
   @param ratationSpeed rotation speed
*/
void rotation (int degree, int rotationSpeed){
	int controlValue=0;
	int measurementValue=0;
	int gain = 5;
	int checkValue=0;
	int totalPulses;
	int course=1;     // rotation course, 1 to right -1 to left
	rightWheel(1500);
	leftWheel(1500);
     if (degree<0)
     {
		 course=-1;
		 degree=degree*course;
     }
	// total number of pulses required for rotation
    totalPulses=((degree)/2);
	reset_Counter();
	while ((counter_1+counter_2) < totalPulses)
	{
		if ((counter_1+counter_2) >= checkValue)
		{
 		measurementValue = (counter_2-counter_1);
 		controlValue = (gain*measurementValue);
		// update wheels speed
 		leftWheel(1500 + ( (rotationSpeed*course) + (controlValue*course)) );
		rightWheel(1500 - ( (rotationSpeed*course) - (controlValue*course)) );
		checkValue=counter_1+counter_2;
		}
	}
	//  stop wheels
	rightWheel(1500);
	leftWheel(1500);
	reset_Counter();
}
