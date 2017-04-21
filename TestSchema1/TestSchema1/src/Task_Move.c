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
	
	ioport_set_pin_dir (PIO_PA6_IDX, IOPORT_DIR_INPUT);	// A4 (button)
	
	uint32_t out = 0;
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		value = ioport_get_pin_level(PIO_PA6_IDX);
		ioport_set_pin_level(PIO_PA24_IDX, HIGH);						// Pin A1		LED GREEN
		
		ioport_set_pin_level(PIO_PA22_IDX, out);
		printf("TASK Move INNE\r\n");
		if (value)
		{
			printf("Knapp nedtryckt\r\n");
			out = 1;
		}
		else
		{
			out = 0;
			printf("Knapp ej nedtryckt\r\n");
		}
		delay_ms(100);
		ioport_set_pin_level(PIO_PA24_IDX, LOW);						// Pin A1		LED GREEN
		
	}
}