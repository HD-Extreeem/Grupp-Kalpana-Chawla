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
	uint32_t out = 0;
	printf("task_unoComm\r\n");
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	
	ioport_set_pin_dir (PIO_PA24_IDX, IOPORT_DIR_OUTPUT);	// A1 (green LED)
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle.
		
		out++;
		ioport_set_pin_level(PIO_PA24_IDX, out);
		
		if (out == 0)
		{
			out = 1;
		}
		else 
		{
			out = 0;
		}
	}
}