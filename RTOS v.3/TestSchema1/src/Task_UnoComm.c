/**
* Task_UnoComm.c
*
* Created: 2017-04-20 16:25:09
*  Author: Desiree
*/

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>
#include "task.h"
#include "arlo/Arlo.h"
extern Bool liftProcessFinished;

void task_unoComm(void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;

	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.

	while (1)
	{
		static uint8_t i = 1;
		/* Lifts object */
		if (i)
		{
			arlo_lift_object(CUBE);
			i = 2;
		}
		
		Pick_Up_Status status = 0;
		if (status != PICK_UP_DONE)
		{
			delay_ms(500);
			status = arlo_get_pick_up_status();
		}
		else
		{
			liftProcessFinished = true;
			i = 1;
		}
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle after have finished everything
		vTaskSuspend(NULL);
	}
}
