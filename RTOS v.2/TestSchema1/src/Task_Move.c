/**
* Task_Move.c
*
* Created: 2017-04-20 14:11:27
*  Author: Hadi Deknache && Yurdaer Dalkic
Desiree J�nsson och Jonas Eiselt
*/

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_Move.h"
#include "MathFunctions.h"
#include "PwmFunctions.h"
#include "StepCounter_ISR.h"

int distance=0;
int angle=0;
int direction=1;
double totalPulses; //Calculate the total pulses needed to move to destination
uint16_t speed = 200; // Set speed for moving the robot
double proportionalError = 0; //P-controller error variable
double referenceValue = 0;
double measurementValue=0;
double controlValue=0; //Variable to store total value for PID-controller
double K=1; //Gain for the PID-controller
double totMovement = 0; //Variable to store totalmovement during the transportation
double integral=0;
double derivate=0;
double prevD=0;
double dT=0.1;
double Td=0.265;//0.53;
double Ti=1.075;//2.15;
int32_t sum=0;
int course;
int rotationGain=5;
int rotationSpeed=90;
int wait =0;
int step =0;
typedef enum {START, BEFORE_ROTATE,ROTATE,MOVE,CLOSE,NAVI} states;
states currentState = START;
states nextState ;
void task_move(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 100; //Time given for the task to complete work...
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle after have finished everything
		
		switch (currentState)
		{
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case START:
			step++;
			if (step==1)
			{
				coord.targetX=coord.objectA[0];
				coord.targetY=coord.objectA[1];
			}
			else if (step==2)
			{
				coord.lastX=coord.presentX;
				coord.lastY=coord.presentY;
				coord.presentX=coord.objectA[0];
				coord.presentY=coord.objectA[1];
				coord.targetX=coord.objectB[0];
				coord.targetY=coord.objectB[1];
			}
			else if (step==3)
			{
				coord.lastX=coord.presentX;
				coord.lastY=coord.presentY;
				coord.presentX=coord.objectB[0];
				coord.presentY=coord.objectB[1];
				coord.targetX=coord.objectC[0];
				coord.targetY=coord.objectC[1];
			}
			else if (step==4)
			{
				coord.lastX=coord.presentX;
				coord.lastY=coord.presentY;
				coord.presentX=coord.objectC[0];
				coord.presentY=coord.objectC[1];
				coord.targetX=coord.objectD[0];
				coord.targetY=coord.objectD[1];
			}
			nextState=NAVI;
			
			if(step>4)
			{
				nextState = CLOSE;
			}

			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case NAVI:
			angle = calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.targetX,coord.targetY);
			distance = calculateDistance(coord.presentX,coord.presentY,coord.targetX,coord.targetY);
			nextState = BEFORE_ROTATE;
			break;

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case MOVE:
			if (totMovement>=totalPulses)
			{
				rightWheel(1500);//Stop rightWheel
				leftWheel(1500);//Stop leftWheel
				controlValue=0;
				measurementValue=0;
				totMovement=0;
				proportionalError=0;
				angle=0;
				sum=0;
				speed = 200;
				distance=0;
				reset_Counter();
				nextState = START;
				wait=0;
			}
			else
			{
				move();
				nextState = MOVE;

			}
			
			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case BEFORE_ROTATE:
			controlValue=0;
			measurementValue=0;
			course=1;     // rotation course, 1 to right -1 to left
			if (angle<0)
			{
				course=-1;
			}
			// total number of pulses required for rotation
			totalPulses=((angle*course)/2);
			rightWheel(1500);
			leftWheel(1500);
			reset_Counter();
			nextState = ROTATE;
			
			break;
			
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case ROTATE:
			
			if ((counter_1+counter_2+1) >= totalPulses)
			{
				//  stop wheels
				rightWheel(1500);
				leftWheel(1500);
				controlValue=0;
				measurementValue=0;
				angle=0;
				reset_Counter();
				nextState = MOVE;
			}
			else
			{
				leftWheel(1500 + ( rotationSpeed*course) );
				rightWheel(1500 - ( rotationSpeed*course) );
			}
			
			case CLOSE:
			
			break;
			
		}
		currentState = nextState;

	}
}



void move (void){
	if (distance < 0)
	{
		direction =-1;
	}
	else
	{
		direction =1;
	}
	totalPulses = (distance*direction/1.35)-totMovement;
	referenceValue = referenceValue + angle;
	totMovement = totMovement + ((counter_1+counter_2)/2);
	measurementValue = (counter_2-counter_1);// Calculates the error differences
	reset_Counter();
	proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain
	sum = (sum + prevD);
	integral= (sum * (dT/Ti));
	derivate = ((Td/dT) * (proportionalError-prevD));
	controlValue =(K*(proportionalError+integral+ derivate)); //PID
	prevD=proportionalError;
	//	Check if almost reached the destination to slow down and make a smoother brake
	if (((totMovement/totalPulses)>= 0.95) || ((totMovement/totalPulses)<= 0.05))
	{
		speed = 130;
	}
	//Else same speed set
	else
	{
		speed = 200;
	}
	rightWheel(1500 + ((speed+controlValue)*direction));//New speed for rightWheel
	leftWheel( 1500 + ((speed-controlValue)*direction));//New speed for leftWheel
	
}


coordinates coordinatesInit (void){
	coord.presentX = 0;
	coord.presentY = 0;
	coord.lastX = 0;
	coord.lastY = -100;

	coord.objectA[0] = 0;
	coord.objectA[1] = 100;

	coord.objectB[0] = 100;
	coord.objectB[1] = 100;

	coord.objectC[0] = 100;
	coord.objectC[1] = 0;

	coord.objectD[0] = 0;
	coord.objectD[1] = 0;
}
