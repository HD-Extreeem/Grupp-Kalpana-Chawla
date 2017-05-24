/*
 * Task_Navi.c
 *
 * Created: 2017-05-09 10:14:01
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 
#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "PwmFunctions.h"
#include "Task_Navi.h"
#include "NaviControl.h"
#include "Task_Move.h"
#include "StepCounter_ISR.h"
extern int distance;

void Task_Navi(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 50;

	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.

   int i=1;
	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		if (i==1)
		{   i=0;
			distance=70;
		}
	
	
	
		
// 	printf("distance%d\n",distance);
//      switch (i)
//      {
//      case 'a':
// 	 printf("case a");
// 	 if (distance==0)
// 	 {
// 		  distance=100;
// 	   i='b';
// 	 }
//      	break;
// 	 case 'b':
//         if (distance==0)
//         {
// 			printf("case b");
// 			rotation(90,130);
// 			distance=0;
// 			i='a';
//         }
// 		break;
// 	case 'c':
// 		printf("case b");
// 		i='c';
// 	 }
// 
 	}
}

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
	totalPulses=((degree)/1.4);
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