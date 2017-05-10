/**
* \file
*
*  TaskTest
*
* Author: Désirée Jönsson och Jonas Eiselt 2017-04-20
*
*/

#include <asf.h>
#include "Task_Move.h"
#include "Task_GetCordinates.h"
#include "Task_UnoComm.h"
#include "consoleFunctions.h"
#include "StepCounter_ISR.h"
#include "PwmFunctions.h"


int main (void)
{
	sysclk_init();
	board_init();
	configureConsole();
	attach_interupt();
	PWM_init();
	
	typedef struct{
		double presentX,presentY,lastX,lastY;
		double objectA[2],objectB[2],objectC[2],objectD[2];
	}coordinates;
	
	coordinates coord;
	
	coord.presentX = 0;
	coord.presentY = 0;
	coord.lastX = 0;
	coord.lastY = 0;
	
	coord.objectA[0] = 137;
	coord.objectA[1] = 200;

	coord.objectB[0] = 56;
	coord.objectB[1] = 363;

	coord.objectC[0] = 452;
	coord.objectC[1] = 350;

	coord.objectD[0] = 434;
	coord.objectD[1] = 36;
	
	/* Print info in terminal Window*/
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	
	
	/* Create the task with the third priority the task_Move*/
	if (xTaskCreate(task_move, (const signed char * const) "Move", TASK_MOVE_STACK_SIZE, NULL, TASK_MOVE_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to test task_Move task\r\n");
	}
	
	// 	/* Create the task with the second priority the task_GetCordinates*/
	// 	if (xTaskCreate(task_getCordinates, (const signed char * const) "¨Get", TASK_GET_STACK_SIZE, NULL, TASK_GET_STACK_PRIORITY, NULL) != pdPASS) {
	// 		printf("Failed to test GetCordinates task\r\n");
	// 	}
	
	// 		/* Create the task with the least priority the task task_UnoComm */
	// 	if (xTaskCreate(task_unoComm, (const signed char * const) "UNO", TASK_UNO_STACK_SIZE, NULL, TASK_UNO_STACK_PRIORITY, NULL) != pdPASS) {
	// 		printf("Failed to test UnoComm task\r\n");
	// 	}
	
	/* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();
}
