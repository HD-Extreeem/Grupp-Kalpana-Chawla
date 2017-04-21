/*
 * Task_Move.c
 *
 * Created: 2017-04-20 14:11:27
 *  Author: Désirée Jönsson och Jonas Eiselt
 */ 

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_Move.h"


void task_move( void *pvParameters)
{
	
 portTickType xLastWakeTime;
 const portTickType xTimeIncrement = 500;
 
 xLastWakeTime = xTaskGetTickCount(); //Initialise the xLastWakeTime variable with the current time.
 
 
 ioport_set_pin_dir (PIO_PA23_IDX, IOPORT_DIR_OUTPUT);						// Pin A2		LED YELLOW
 ioport_set_pin_dir (PIO_PA16_IDX, IOPORT_DIR_OUTPUT);						//Pin A0		LED RED
 ioport_set_pin_dir (PIO_PA24_IDX, IOPORT_DIR_OUTPUT);						// Pin A1		LED GREEN
 
 ioport_set_pin_dir (PIO_PA6_IDX, IOPORT_DIR_INPUT);						// Pin A4 Knapp	
 
 
uint32_t out = 0;
bool value;
printf("TASK MOVE INNE\r\n");
while (1) {
	
	vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
	ioport_set_pin_level(PIO_PA16_IDX,out);	
	out=1;
	value =ioport_get_pin_level(PIO_PA6_IDX);
	if(value){
	printf("NEDTRYCKT\r\n");
	}
	else{
		printf("ICKEEEEEEE\r\n");
	}
	/*
	if (out == 0)
		out = 1;
	else 
		out = 0;
	*/	
		
		
	
//	ioport_set_pin_level(PIO_PA16_IDX,LOW);	
//	delay_s(1);								//RED LED ON
//	ioport_set_pin_level(PIO_PA16_IDX,HIGH);	
//	ioport_set_pin_level(PIO_PA24_IDX,LOW);
	
	
//	ioport_set_pin_level(PIO_PA24_IDX,LOW);										//GREEN LED OFF
	
	//ioport_set_pin_dir (PIO_PA23_IDX, IOPORT_DIR_OUTPUT);						// Pin A2		LED YELLOW
	//ioport_set_pin_level(PIO_PA23_IDX,LOW);										//YELLOW LED OFF
	//	delay_s(1);
	
	//vTaskDelay(400);
		
}



}