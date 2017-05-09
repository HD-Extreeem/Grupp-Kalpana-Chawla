/*
 * PID_move.c
 * Created: 2017-04-26 14:38:37
 * This source file handle the main regulation for wheels and rotation
 * for a certain degree.
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */
#include <asf.h>
#include "PwmFunctions.h"
#include "StepCounter_ISR.h"
#include "PID_move.h"


/**
This method moveTo handle position moving for a certain distance and direction
Method uses a PID controller for smoother movement of the robot
	@param distance the distance which the robot need to move in centimeters
	@param direction the direction robot need to move forward or backwards
**/
void moveTo (int distance, int direction){
	//printf("Run now!");
  int pulse =1.38;
	if (distance > 200)
	{
		pulse =1.36;
	}
	float totalPulses = (distance/pulse); //Calculate the total pulses needed to move to destination
	uint16_t speed = 200; // Set speed for moving the robot
	double proportionalError = 0; //P-controller error variable
	double referenceValue = 0;
	double measurementValue=0;
	double controlValue=0; //Variable to store total value for PID-controller
	//double iPart=0; //Variable for the I-controller to keep calculated error*gain
	//double dPart=0; //Variable for the D-controller to keep calculated error*gain
	double kp=2.5; //Gain for the P-controller
	//double kd=0; //Gain for the D-controller
	//double ki=0; //Gain for the I-controller
	float totMovement = 0; //Variable to store totalmovement during the transportation


	//--------------------------------------Updated D�sir�e och Ansam 170503
	double integral=0;
	double derivate=0;
	double prevD=0;
	double dT=0.5;
	double Td=0.265;//0.53;
	double Ti=1.075;//2.15;
	int32_t sum=0;
	//--------------------------------------Updated D�sir�e och Ansam 170503
	//delay_us(500);
	reset_Counter(); //Reset counter for encoders for wheel to ensure counter reseted for next movement
	//delay_us(500);

	// Controlls if moving forward or backwards
	if ( direction!=1 && direction!=-1 )
	{
		direction=1;
	}

		/**
		This while loop runs until robot reaches its destination
		checks if totalmovemnt pulses is the same or more than the total pulses needed
		**/
		delay_us(1);
    while (totMovement <= totalPulses) {
		delay_us(1);
			/**
			Checks if both encoder have counted up since last time
			if counted, then regulates the need values from encoder
			**/
      if (c1Loop == true && c2Loop == true) {
		totMovement = totMovement + ((counter_1+counter_2)/2);
		//delay_us(200);
        measurementValue = (counter_2-counter_1);// Calculates the error diffferce
		//delay_us(500);
        reset_Counter();//Reset counter for next regulation later
		//delay_us(500);
	    proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain

		//--------------------------------------Updated D�sir�e och Ansam 170503
		sum = (sum + prevD);
		//delay_us(500);
		integral= (sum * (dT/Ti));
		//delay_us(500);
		derivate = ((Td/dT) * (proportionalError-prevD));
		//delay_us(500);
	   controlValue =(kp*(proportionalError+integral+ derivate)); //PID
	   prevD=proportionalError;
	   //--------------------------------------Updated D�sir�e och Ansam 170503

	   //iPart = (Ti*integralError); //Calculates i-controller gain
	   // dPart = //(kd*(proportionalError-derivativeError)); //Calculates d-controller gain
	   //controlValue = (kp*(proportionalError+iPart+dPart)); //Total regulation for PID calculate new value for correcting the error
		//delay_us(500);

		 //	Check if almost reached the destination to slow down and make a smoother brake
		 if (((totMovement/totalPulses)>= 0.85) || ((totMovement/totalPulses)<= 0.1))
 		{
 			//speed = speed - ((totMovement/totalPulses)*150) ;
			 speed = 110;
 		}
		
		//Else same speed set
 		else
 		{
 			speed = 180;
 		}


		rightWheel(1500 + ((speed+controlValue)*direction));//New speed for rightWheel
		leftWheel( 1500 + ((speed-controlValue)*direction));//New speed for leftWheel
		//delay_us(5);
        c1Loop = false; //Loop finished to prevent from running loop again
        c2Loop = false;//Loop finished to prevent from running loop again
		//delay_us(5);
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
