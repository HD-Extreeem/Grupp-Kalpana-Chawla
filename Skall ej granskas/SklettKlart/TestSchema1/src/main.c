/**
 * \file
 *
 *  TaskTest
 *
 * Author: D�sir�e J�nsson och Jonas Eiselt 2017-04-20
 *
 */

#include <asf.h>
#include "Task_Move.h"
#include "Task_GetCordinates.h"
#include "Task_UnoComm.h"
#include "consoleFunctions.h"
#include "StepCounter_ISR.h"

int main (void)
{
	sysclk_init();
	board_init();
	sysclk_enable_peripheral_clock(ID_TWI0);
	configureConsole();
	attach_interupt();
	
		
	/* Print info in terminal Window*/
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	
	
	/* Create the task with the highest priority the task task_StepCounter */
	//if (xTaskCreate(task_StepCounter, (const signed char * const) "Step", TASK_STEP_STACK_SIZE, NULL, TASK_STEP_STACK_PRIORITY, NULL) != pdPASS) {
	//	printf("Failed to test _StepCounter task\r\n");
	//}
		
	/* Create the task with the second priority the task_GetCordinates*/
	//if (xTaskCreate(task_getCordinates, (const signed char * const) "�Get", TASK_GET_STACK_SIZE, NULL, TASK_GET_STACK_PRIORITY, NULL) != pdPASS) {
	//	printf("Failed to test GetCordinates task\r\n");
	//}
	
	/* Create the task with the third priority the task_Move*/
	//if (xTaskCreate(task_move, (const signed char * const) "Move", TASK_MOVE_STACK_SIZE, NULL, TASK_MOVE_STACK_PRIORITY, NULL) != pdPASS) {
	//	printf("Failed to test task_Move task\r\n");
		
	//}
		/* Create the task with the least priority the task task_UnoComm */
	if (xTaskCreate(task_unoComm, (const signed char * const) "UNO", TASK_UNO_STACK_SIZE, NULL, TASK_UNO_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to test UnoComm task\r\n");
	}
	
	/* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();
}
