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
extern bool c1Loop;
extern bool c2Loop;


void moveTo (int distance, int direction){
	float totalPulses = (distance/1.38);
	uint16_t speed = 1700;
	int proportionalError = 0;
    int derivativeError=0;
    int integralError=0;
	int referenceValue = 0;
	int measurementValue=0;
	int controlValue=0;
    int iPart=0;
    int dPart=0;
	int kp=10;
    int kd=2;
    int ki=2;
	float totMovement = 0;
	delay_us(300);
	reset_Counter();
	delay_us(300);
	if ( direction!=1 || direction!=-1 )
	{
		direction=1;
	}

	
    while (totMovement <= totalPulses) {
		delay_ms(1);
      if (c1Loop == true && c2Loop == true) {
		totMovement = totMovement + ((counter_1+counter_2)/2);
		delay_ms(1);
        measurementValue = (counter_2-counter_1);
		delay_us(500);
        reset_Counter();
		delay_us(500);
        dPart = (kd*(proportionalError-derivativeError));
		delay_us(500);
        iPart = (ki*integralError);
		delay_us(500);
		proportionalError = (referenceValue - measurementValue);
		delay_us(500);
		controlValue = ((kp*proportionalError)+iPart+dPart);
		delay_us(500);
// 		if ((totMovement/totalPulses)>= 0.8)
// 		{
// 			speed = 1650;
// 		}
// 		else if ((totMovement/totalPulses)>= 0.9)
// 		{
// 			speed = 1580;
// 		}
		
		rightWheel((speed+controlValue));
		leftWheel((speed-controlValue));
		delay_us(500);
	    //error=error*-1;
        derivativeError = proportionalError;
		delay_us(500);
        integralError = (integralError+proportionalError);
		delay_us(500);
        c1Loop = false;
        c2Loop = false;
		delay_us(500);
		//printf("%d\n",totMovement);
      }

	}
	rightWheel(1500);
	leftWheel(1500);
	delay_s(2);
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
