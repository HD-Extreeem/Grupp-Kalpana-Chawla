/*
 * Task_UnoComm.c
 *
 * Created: 2017-04-20 16:25:09
 *  Author: Desiree
 */ 

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>

void task_unoComm( void *pvParameters)
{
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	
	xLastWakeTime = xTaskGetTickCount(); //Initialise the xLastWakeTime variable with the current time.
	
		ioport_set_pin_dir (PIO_PA23_IDX, IOPORT_DIR_OUTPUT);						// Pin A2		LED YELLOW
	ioport_set_pin_dir (PIO_PA16_IDX, IOPORT_DIR_OUTPUT);						//Pin A0		LED RED
	ioport_set_pin_dir (PIO_PA24_IDX, IOPORT_DIR_OUTPUT);						// Pin A1		LED GREEN
	
	uint32_t out = 0;
	printf("TASK UNO INNE\r\n");
	while (1) {
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		
		//ioport_set_pin_level(PIO_PA16_IDX,HIGH);			//RED LED 
	//	ioport_set_pin_level(PIO_PA23_IDX,LOW);							
	//	ioport_set_pin_level(PIO_PA24_IDX,HIGH);										//GREEN LED OFF
		//delay_s(1);		
		
		//	vTaskDelay(400);
		
		ioport_set_pin_level(PIO_PA24_IDX,out);
		printf("TASK UNO INNE\r\n");
		if (out == 0)
			out = 1;
		else
			out = 0;
	}
	
}