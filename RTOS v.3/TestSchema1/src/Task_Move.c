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
double K=2.5; //Gain for the PID-controller
double totMovement = 0; //Variable to store totalmovement during the transportation
double integral=0;
double derivate=0;
double prevD=0;
double dT=0.1;
double Td=0.053;//0.53;
double Ti=0.215;//2.15;
int32_t sum=0;
int course;
int rotationGain=5;
int rotationSpeed=90;
int wait =0;
int step =0;
int check =0;

Bool newData=false;
uint8_t object_counter = 1;
Bool liftProcessFinished = false;
Bool liftStart = false;
extern xTaskHandle xTaskMove;
extern xTaskHandle xTaskCom;
extern xTaskHandle xTaskCoordinate;
typedef enum {STARTGL,STARTLASSE,BEFORE_ROTATE,ROTATE,MOVE,LIFT,NAVI,CLOSE} states;
states currentState = STARTGL;
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


			case STARTGL:
			updateNextPosGL();
			nextState = BEFORE_ROTATE;
			if (object_counter>=5)
			{
				nextState=CLOSE;
			}

			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case STARTLASSE:
			updateNextPosLasse();
			nextState = BEFORE_ROTATE;

			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case NAVI:

			//Kollar efter ny data efter 20 pulser
			if ((totMovement/20)>=1)
			{
				// vTaskResume(xTaskCoordinate);
			}

			//Kollar ifall ny data finns efter varje 40ms*20pulser=800ms
			if (newData)
			{
				vTaskSuspend(xTaskCoordinate);
				updateLastPresent();
				calculateAngleDistance();
				nextState = MOVE;
			}
			if (liftStart)
			{
				vTaskResume(xTaskCom);
				liftStart=false;
				nextState = LIFT; //Får ändra senare för att möjligöra rörelse under körning
			}

			else{
				nextState = MOVE;
			}

			break;

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case MOVE:
			printf("MOVE");
			// 			check++;
			// 			if (check==40)
			// 			{
			// 				angle =90;
			// 				referenceValue = referenceValue - angle/2;
			// 			}
			//
			// 			if (check==90)
			// 			{
			// 				angle =-90;
			// 				referenceValue = referenceValue - angle/2;
			// 			}

			totalPulses = (distance*direction/1.45);
			if (totMovement+2>=totalPulses)
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

				liftStart=true;
				nextState = NAVI;
				//wait=0;
			}
			else
			{
				move();
				nextState = NAVI;

			}

			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case BEFORE_ROTATE:
			printf("BEFORE ROTATE");
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
			printf("ROTATE");
			if ((counter_1+counter_2+1) >= totalPulses)
			{
				//  stop wheels
				rightWheel(1500);
				leftWheel(1500);
				reset_Counter();
				angle=0;
				totalPulses=0;
				totMovement=0;
				nextState = MOVE;

			}
			else
			{
				leftWheel(1500 + ( rotationSpeed*course) );
				rightWheel(1500 - ( rotationSpeed*course) );
				nextState = ROTATE;
			}


			break;

			case CLOSE:
			while(1);
			printf("CLOSE");
			nextState=CLOSE;
			break;

			/************************************************************************/
			/*                                                                      */
			/************************************************************************/
			case LIFT:

			if (liftProcessFinished)
			{
				printf("lift finished\r\n");
				liftProcessFinished=false;
				vTaskSuspend(xTaskCom);
				nextState = STARTGL;
			}
			else
			{
				// vTaskResume(xTaskCom);
				nextState = LIFT;
			}

			break;
			/************************************************************************/
			/*                                                                      */
			/************************************************************************/

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

	totMovement = ((counter_1+counter_2)/2);
	measurementValue = (counter_2-counter_1);// Calculates the error differences
	proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain
	if ((referenceValue > 0) && (proportionalError < 0))
	{
		referenceValue = 0;
		proportionalError=0;
		sum=0;
		prevD=0;
		counter_1 = (counter_1+counter_2)/2;
		counter_2=counter_1;
	}
	if ((referenceValue < 0) && (proportionalError > 0))
	{
		referenceValue = 0;
		proportionalError=0;
		sum=0;
		prevD=0;
		counter_1 = (counter_1+counter_2)/2;
		counter_2=counter_1;
	}
	sum = (sum + prevD);
	integral= (sum * (dT/Ti));
	derivate = ((Td/dT) * (proportionalError-prevD));
	controlValue =(K*(proportionalError+integral+ derivate)); //PID
	prevD=proportionalError;
	//	Check if almost reached the destination to slow down and make a smoother brake
	if (((totMovement/totalPulses)>= 0.90) || ((totMovement/totalPulses)<= 0.05))
	{
		speed = 130;
	}
	//Else same speed set
	else
	{
		speed = 200;
	}
	if (controlValue>70)
	{
		controlValue=70;
	}
	else if (controlValue<-70)
	{
		controlValue=-70;
	}
	rightWheel(1500 + ((speed+controlValue)*direction));//New speed for rightWheel
	leftWheel( 1500 + ((speed-controlValue)*direction));//New speed for leftWheel

}



void coordinatesInit (void){

	coord.presentX = 0;
	coord.presentY = 0;
	coord.lastX = 0;
	coord.lastY = -100;

	coord.objectA[0] = 0;
	coord.objectA[1] = 100;

	coord.objectB[0] = 100;
	coord.objectB[1] = 200;

	coord.objectC[0] = 0;
	coord.objectC[1] = 300;

	coord.objectD[0] = 0;
	coord.objectD[1] = 0;
}


void updateNextPosGL(void){
	//static uint8_t i = 1;
	if (object_counter==1)
	{
		coord.targetX=coord.objectA[0];
		coord.targetY=coord.objectA[1];
		calculateAngleDistance();
	}

	else if(object_counter==2){
		updateLastPresent();
		coord.presentX=coord.objectA[0];
		coord.presentY=coord.objectA[1];
		coord.targetX=coord.objectB[0];
		coord.targetY=coord.objectB[1];
		calculateAngleDistance();
	}

	else if(object_counter==3){
		updateLastPresent();
		coord.presentX=coord.objectB[0];
		coord.presentY=coord.objectB[1];
		coord.targetX=coord.objectC[0];
		coord.targetY=coord.objectC[1];
		calculateAngleDistance();
	}

	else if(object_counter==4){
		updateLastPresent();
		coord.presentX=coord.objectC[0];
		coord.presentY=coord.objectC[1];
		coord.targetX=coord.objectD[0];
		coord.targetY=coord.objectD[1];
		calculateAngleDistance();
	}

	printf("I counter %d \r\n",object_counter);
	object_counter++;
}
void updateNextPosLasse(void){
	//static uint8_t i = 1;
	if (object_counter==1)
	{
		coord.targetX=coord.objectA[0];
		coord.targetY=coord.objectA[1];
		calculateAngleDistance();
	}

	else if(object_counter==2){
		updateLastPresent();
		coord.presentX=coord.objectA[0];
		coord.presentY=coord.objectA[1];
		coord.targetX=coord.objectD[0];
		coord.targetY=coord.objectD[1];
		calculateAngleDistance();
	}

	else if(object_counter==3){
		updateLastPresent();
		coord.presentX=coord.objectD[0];
		coord.presentY=coord.objectD[1];
		coord.targetX=coord.objectB[0];
		coord.targetY=coord.objectB[1];
		calculateAngleDistance();
	}

	else if(object_counter==4){
		updateLastPresent();
		coord.presentX=coord.objectB[0];
		coord.presentY=coord.objectB[1];
		coord.targetX=coord.objectD[0];
		coord.targetY=coord.objectD[1];
		calculateAngleDistance();
	}
	else if(object_counter==5){
		updateLastPresent();
		coord.presentX=coord.objectD[0];
		coord.presentY=coord.objectD[1];
		coord.targetX=coord.objectC[0];
		coord.targetY=coord.objectC[1];
		calculateAngleDistance();
	}
	else if(object_counter==6){
		updateLastPresent();
		coord.presentX=coord.objectC[0];
		coord.presentY=coord.objectC[1];
		coord.targetX=coord.objectD[0];
		coord.targetY=coord.objectD[1];
		calculateAngleDistance();
	}
	else{
		printf("\n close now!\r\n");
		nextState=CLOSE;
	}
	printf("\nObject counter: %d\r\n",object_counter);
	object_counter++;
}

void updateLastPresent(void){
	coord.lastX=coord.presentX;
	coord.lastY=coord.presentY;
}

void calculateAngleDistance(void){
	printf("NAVI");
	angle = calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.targetX,coord.targetY);
	printf("%d",angle);
	distance = calculateDistance(coord.presentX,coord.presentY,coord.targetX,coord.targetY);
	printf("%d",distance);
}
