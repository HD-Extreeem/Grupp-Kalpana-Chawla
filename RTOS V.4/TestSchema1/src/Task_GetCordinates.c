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

uint16_t coord_arr[2] = {0};

void task_getCordinates(void *pvParameters)
{

	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 100;

	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.


	while (1) 
	{	
		arlo_get_position(coord_arr);
		printf("\n x : %d\r\n", coord_arr[0]);
		printf("y : %d\r\n", coord_arr[1]);
		vTaskSuspend(NULL);
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.

	}
}
