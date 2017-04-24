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
	
	//ioport_set_pin_dir (PIO_PA23_IDX, IOPORT_DIR_OUTPUT);	// A1 (green LED)
	
	printf("Startar UnoComm task\r\n");
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle.
		ioport_set_pin_level(PIO_PA23_IDX, HIGH);
		printf("TASK UnoComm INNE\r\n");
		
		delay_ms(100);
		ioport_set_pin_level(PIO_PA23_IDX, LOW);
	}
}