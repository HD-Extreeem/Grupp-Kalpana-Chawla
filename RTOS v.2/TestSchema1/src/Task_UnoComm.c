/**
* Task_UnoComm.c
*
* Created: 2017-04-20 16:25:09
*  Author: Desiree
*/

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>

void task_unoComm(void *pvParameters)
{

	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 50;

	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.

	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle after have finished everything
		
	}
}
