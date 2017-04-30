/*
 * PID_move.c
 *
 * Created: 2017-04-26 14:38:37
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 
#include <asf.h>
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"
#include "PID_move.h"

extern uint16_t counter_1;
extern uint16_t counter_2;





void moveTo (int distance, int direktion){
    uint16_t totalPulses=distance/1.35;
	uint16_t speed = 1600;
	int error=0;
	int referenceValue=0;
	int measurementValue;
	int controlValue;
	int kp=20;
	reset_Counter();
	if ( direktion!=1 || direktion!=-1 )
	{
		direktion=1;
	}
	while (counter_1 <= totalPulses)
	{
			measurementValue = (counter_2-counter_1);
			error = (referenceValue - measurementValue);
			controlValue = (kp*error);
			rightWheel((speed+controlValue));
			leftWheel((speed-controlValue));
			error=error*-1;
	}
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
	// if its more then 180 turn to the left
 	if(degree>180){    
 	    degree=360-degree;
 		course=-1;
 	}
	// total number of pulses required for rotation
    totalPulses=((degree)/2);
	reset_Counter();
	while ((counter_1+counter_2) < totalPulses)  
	{
		if ((counter_1+counter_2) > checkValue)
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

