/**
 * \file
 *
 * \brief Empty user application template
 *
 * Author: Désirée Jönsson och Jonas Eiselt 2017-04-20
 *
 */


#include <asf.h>
#include "Task_Move.h"
#include "Task_GetCordinates.h"
#include "Task_UnoComm.h"


static unsigned char pvParameters;
xTaskHandle xHandle=NULL;

int main (void)
{
	sysclk_init();
	board_init();
	
	
	/* Create the task with the highest priority the task_GetCordinates*/
	if (xTaskCreate(task_move, (const signed char * const) "Move", TASK_MOVE_STACK_SIZE, &pvParameters, TASK_MOVE_STACK_PRIORITY, &xHandle) != pdPASS) {
		printf("Failed to test regulator task\r\n");
	}
	
	
	/* Create the task with the second priority the task_Move*/
	if (xTaskCreate(task_getCordinates, (const signed char * const) "Move", TASK_GET_STACK_SIZE, &pvParameters, TASK_GET_STACK_PRIORITY, &xHandle) != pdPASS) {
		printf("Failed to test regulator task\r\n");
	}
	
	
	/* Create the task with the least priority the task task_UnoComm */
	if (xTaskCreate(task_unoComm, (const signed char * const) "Move", TASK_UNO_STACK_SIZE, &pvParameters, TASK_UNO_STACK_PRIORITY, &xHandle) != pdPASS) {
		printf("Failed to test regulator task\r\n");
	}
	
	
	
	/* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();

	// Insert application code here, after the board has been initialized.
}
