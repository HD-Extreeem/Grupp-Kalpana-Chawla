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


void task_move(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 100; //Time given for the task to complete work...
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	while (1)
	{
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle after have finished everything

	}
}
