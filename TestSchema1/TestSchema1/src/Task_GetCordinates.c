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
	const portTickType xTimeIncrement = 500;
	
	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.
	
	printf("Startar GetCord task\r\n");
	
	while (1) {
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		
		ioport_set_pin_level(PIO_PA16_IDX,HIGH);				// A1 (red LED)
		printf("TASK GetCord INNE\r\n");

		delay_ms(100);
		ioport_set_pin_level(PIO_PA16_IDX,LOW);				// A2 (red LED)
	}
}