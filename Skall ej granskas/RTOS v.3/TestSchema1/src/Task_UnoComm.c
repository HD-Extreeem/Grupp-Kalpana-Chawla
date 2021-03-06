/**
* Task_UnoComm.c
*
* Created: 2017-04-20 16:25:09
*  Author: Desiree
*/
#include "task.h"
#include "arlo/Arlo.h"

extern Bool liftProcessFinished;
Pick_Up_Status status = 0;

void task_unoComm(void *pvParameters)
{

	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;
 while(1){
		static uint8_t i = 1;
		/* Lifts object */
		if (i==1)
		{
			arlo_lift_object(CUBE);
			i = 2;
		}

		if (status != PICK_UP_DONE)
		{
			//delay_ms(500);
			status = arlo_get_pick_up_status();
		}
		else
		{
			printf("liftProcessFinished = true\r\n");
			liftProcessFinished = true;
			i = 1;
			vTaskSuspend(NULL);
		}

		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle after have finished everything
		//vTaskSuspend(NULL);
		}
	}
}
