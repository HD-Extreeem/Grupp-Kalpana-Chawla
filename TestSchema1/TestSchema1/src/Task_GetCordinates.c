/*
 * Task_GetCordinates.c
 *
 * Created: 2017-04-20 14:12:03
 *  Author: Désirée Jönsson och Jonas Eiselt 2017-04-20
 */ 

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_GetCordinates.h"

void task_getCordinates(void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 100;
	
	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.
	
	printf("TASK GetCord INNE\r\n");
	uint32_t out = 0;
	
	while (1) {	
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
	
		ioport_set_pin_level(PIO_PA23_IDX,out);
		printf("TASK GetCord INNE\r\n");
		if (out == 0)
		out = 1;
		else
		out = 0;
		
	}
}