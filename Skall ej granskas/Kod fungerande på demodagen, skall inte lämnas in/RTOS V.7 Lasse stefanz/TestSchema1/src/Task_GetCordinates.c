/*
* Task_GetCordinates.c
*
* Created: 2017-04-20 14:12:03
*  Author: D�sir�e J�nsson och Jonas Eiselt 2017-04-20
*/

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "arlo/Arlo.h"
#include "Task_GetCordinates.h"
#include "task.h"
#include "Task_Move.h"

extern Bool newData;
uint8_t get_count = 0;

void task_getCordinates(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 300;

	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.
	while (1)
	{
		
		int16_t coord_arr[2]={0};
		arlo_get_position(coord_arr);
		coord.presentX=coord_arr[0];
		coord.presentY=coord_arr[1];
		printf("\n x : %d\r\n", coord.presentX);
		printf("y : %d\r\n", coord.presentY);
		newData=false;
		vTaskSuspend(NULL);
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
	}
}
