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
double dT=0.5;
double Td=0.265;//0.53;
double Ti=1.075;//2.15;
int32_t sum=0;
int checkAngle=0;
Bool c1_bool;
Bool c2_bool;
void task_move(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 50;
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle

	  // Controlls if moving forward or backwards
	  if (distance < 0)
	  {
		 direction =-1;
	  }
	  else
	  {
		  direction =1;
	  }
		/**
		This while loop runs until robot reaches its destination
		checks if total movement pulses is the same or more than the total pulses needed
		**/
		totalPulses = (distance);
		delay_us(1);
     while (totMovement < totalPulses) {
		delay_us(1);
		 totalPulses = (distance);
		 		//printf("\n c1= %d",(int)totalPulses);
			/**
			Checks if both encoder have counted up since last time
			if counted, then regulates the need values from encoder
			**/
			if (c1_bool && c2_bool)
			{
			
				totMovement = totMovement + ((counter_1+counter_2)/2);
				measurementValue = (counter_2-counter_1);// Calculates the error differences
			    reset_Counter();//Reset counter for next regulation later
				proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain
			    sum = (sum + prevD);
				integral= (sum * (dT/Ti));
				derivate = ((Td/dT) * (proportionalError-prevD));
				controlValue =(K*(proportionalError+integral+ derivate)); //PID
				prevD=proportionalError;
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
				c1_bool=false;
				c2_bool=false;
			}
	 }
	    distance=0;
		totMovement=0;
		rightWheel(1500);//Stop rightWheel
		leftWheel(1500);//Stop leftWheel
	}
}
