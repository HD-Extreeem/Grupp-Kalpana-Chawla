/*
*  @file Task_GetCordinates.c
*
*  @brief This source file handle the positioning, updates positioning for Arlo platform
* 
*  @authors D�sir�e J�nsson and Jonas Eiselt(structure), Hadi Deknache & Jonas Eiselt(Code in structure)
*
*  @date: 2017-04-20 14:12:03
*/

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "arlo/Arlo.h"
#include "Task_GetCoordinates.h"
#include "task.h"
#include "Task_NaviControl.h"

//Bool variable to tell if new data needed
extern Bool newData;

/** Task request data from positioning system asking for new coordinates for Arlo robot platform
*	@param pvParameters parameter to be sent if needed
**/
void task_getCoordinates(void *pvParameters)
{
	//Variable which checks last run
	portTickType xLastWakeTime;
	//Period time set to 200 ms
	const portTickType xTimeIncrement = 200;

	xLastWakeTime = xTaskGetTickCount();//Initialize the xLastWakeTime variable with the current time.
	while (1)
	{
		//Array for storing positioning data
		int16_t coord_arr[2]={0};
		//Request data through I2C from positioning system
		arlo_get_position(coord_arr);
		//Store data for usage
		coord.presentX=coord_arr[0];
		coord.presentY=coord_arr[1];
		//Data got 
		newData=false;
		//Suspending task 
		vTaskSuspend(NULL);
		// Wait for the next cycle.
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); 
	}
}
