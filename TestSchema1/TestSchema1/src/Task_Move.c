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
const portTickType xTimeIncrement = 100;
xLastWakeTime = xTaskGetTickCount(); //Initialise the xLastWakeTime variable with the current time.

while (1) {
	
	vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
	
}

}