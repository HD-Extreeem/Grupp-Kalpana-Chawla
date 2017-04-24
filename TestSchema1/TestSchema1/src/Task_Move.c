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


void task_move(void *pvParameters)
{
	bool value;
	
	printf("Startar Move task\r\n");
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	uint32_t out = 0;
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle
		
		ioport_set_pin_level(PIO_PA24_IDX, HIGH);
		printf("TASK Move INNE\r\n");
		delay_ms(100);
		ioport_set_pin_level(PIO_PA24_IDX, LOW);
		
		
	}
}
