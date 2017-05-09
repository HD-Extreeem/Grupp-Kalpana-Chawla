/*
 * Task_GetCordinates.c
 *
 * Created: 2017-04-20 14:12:03
 *  Author: D�sir�e J�nsson och Jonas Eiselt 2017-04-20
 */

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_GetCordinates.h"
objects[3][2] = {{100,100},{-100,200},{100,400}};
void task_getCordinates(void *pvParameters)
{

	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;

	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.


	while (1) {

		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.

	}
}
