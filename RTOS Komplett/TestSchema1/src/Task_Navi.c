/*
 * Task_Navi.c
 *
 * Created: 2017-05-09 10:14:01
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 
#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "Task_Navi.h"
#include "NaviControl.h"

void Task_Navi(void *pvParameters){
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;

	xLastWakeTime = xTaskGetTickCount();//Initialise the xLastWakeTime variable with the current time.


	while (1) {
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		
// 		switch 
// 			case 

	}
}