/**
* @file main.c
*
* @brief This source file is the main source file, which configures needed settings for PWM, 
*  interrupts, pins and communication with Arlo robot platform
*  initialize task for communication and navigation
*		
* @Author Désirée Jönsson, Jonas Eiselt, Hadi Deknache & Yurdaer Dalkic
*
* @date 2017-04-20
*
*/

#include <asf.h>
#include <inttypes.h>
#include "Task_NaviControl.h"
#include "Task_GetCoordinates.h"
#include "Task_UnoComm.h"
#include "consoleFunctions.h"
#include "StepCounter_ISR.h"
#include "PwmFunctions.h"
#include "conf_board.h"
#include "arlo/Arlo.h"

xTaskHandle xTaskNaviControl=NULL;
xTaskHandle xTaskCom = NULL;
xTaskHandle xTaskCoordinate=NULL;

int main (void)
{
	sysclk_init();
	/* Board initialization, for communication and printouts */
	board_init();
	delay_s(3);
	/* Serial communication initialization, for printouts */
	configureConsole();
	/* Interrupt initialization */
	attach_interrupt();
	/* PWM initialize */
	PWM_init();
	delay_s(3);
	/* Arlo robot initialization */
	arlo_init();
	coordinatesInit();
	
	/* Print info in terminal Window */
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	
	/* Create the task with the highest priority the task_Move*/
	if (xTaskCreate(task_NaviControl, (const signed char * const) "NaviControl", TASK_NaviControl_STACK_SIZE, NULL, TASK_NaviControl_STACK_PRIORITY, &xTaskNaviControl) != pdPASS) 
	{
		printf("Failed to test task_Move task\r\n");
	}
	
	/* Create the task with the second highest priority the task_GetCordinates*/
	if (xTaskCreate(task_getCoordinates, (const signed char * const) "Get", TASK_GET_STACK_SIZE, NULL, TASK_GET_STACK_PRIORITY, &xTaskCoordinate) != pdPASS) 
	{
		printf("Failed to test GetCordinates task\r\n");
	}
	
	/* Create the task with the least priority the task task_UnoComm */
	if (xTaskCreate(task_unoComm, (const signed char * const) "UNO", TASK_UNO_STACK_SIZE, NULL, TASK_UNO_STACK_PRIORITY, &xTaskCom) != pdPASS) 
	{
		printf("Failed to test UnoComm task\r\n");
	}
	
	//* Suspending communication since no needed during driving */
	vTaskSuspend(xTaskCom);
	vTaskSuspend(xTaskCoordinate);
	
	/* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();
}
