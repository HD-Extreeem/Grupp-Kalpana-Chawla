/**
 * Task_Move.c
 *
 * Created: 2017-04-20 14:11:27
 *  Author: Désirée Jönsson och Jonas Eiselt
 */

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_Move.h"
#include "PID_move.h"
#include "MathFunctions.h"
#include "NaviControl.h"
lastPosition [1][1]={0,-100};
presentPosition [1][1]={0,0};

void task_move(void *pvParameters)
{
	
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle
		
		//delay_s(2);
		moveTo(100,1);
		//delay_s(10);
		rotation(calcluteRotationAngle(0,-100,0,0,100,100),130);
		moveTo(calculateDistance(0,0,100,100),1);

		//delay_s(5);
		rotation(calcluteRotationAngle(0,0,100,100,-100,300),130);
		moveTo(calculateDistance(100,100,-100,300),1);

		//delay_s(5);

		rotation(calcluteRotationAngle(100,100,-100,300,100,600),130);
		moveTo(calculateDistance(-100,300,100,600),1);
		lastPosition[0][0]= presentPosition[0][0];
		lastPosition[0][1]= presentPosition[0][1];
		presentPosition[0][0] = objects[2][0];
		presentPosition[0][1] = objects[2][1];
		//delay_s(5);
	}
}
