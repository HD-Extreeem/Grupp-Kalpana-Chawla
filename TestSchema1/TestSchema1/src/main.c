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
#include "consoleFunctions.h"

//static unsigned char pvParameters;
//xTaskHandle xHandle=NULL;

//xSemaphoreHandle signal_semafor = 0;


int main (void)
{
	//uint32_t baud_rate = 115200;
	sysclk_init();
	board_init();
	configureConsole();
	
		
		ioport_set_pin_dir (PIO_PA16_IDX, IOPORT_DIR_OUTPUT);						//Pin A0		LED RED
	//	ioport_set_pin_level(PIO_PA16_IDX,HIGH);
		
		ioport_set_pin_dir (PIO_PA24_IDX, IOPORT_DIR_OUTPUT);						// Pin A1		LED GREEN
	//	ioport_set_pin_level(PIO_PA24_IDX,HIGH);
		
		ioport_set_pin_dir (PIO_PA23_IDX, IOPORT_DIR_OUTPUT);						// Pin A2		LED YELLOW
	//	ioport_set_pin_level(PIO_PA23_IDX,HIGH);
		
		ioport_set_pin_dir (PIO_PA22_IDX, IOPORT_DIR_OUTPUT);						// Pin A3		LED GREEN pull-down when Pin A4 enable
	//	ioport_set_pin_level(PIO_PA22_IDX,HIGH);
		
		ioport_set_pin_dir (PIO_PA6_IDX, IOPORT_DIR_INPUT);						// Pin A4 Knapp		
	//	ioport_set_pin_level(PIO_PA22_IDX,HIGH);
	
	
	
/* Print demo information on the terminal*/
printf("aaa");
printf("-- %s\n\r", BOARD_NAME);
printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
		
	/* Create the task with the highest priority the task_GetCordinates*/
	if (xTaskCreate(task_getCordinates, (const signed char * const) "¨Get", TASK_GET_STACK_SIZE, NULL, TASK_GET_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to test regulator task\r\n");
	}
	
	/* Create the task with the second priority the task_Move*/
	if (xTaskCreate(task_move, (const signed char * const) "Move", TASK_MOVE_STACK_SIZE, NULL, TASK_MOVE_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to test regulator task\r\n");
		
	}
		/* Create the task with the least priority the task task_UnoComm */
	if (xTaskCreate(task_unoComm, (const signed char * const) "UNO", TASK_UNO_STACK_SIZE, NULL, TASK_UNO_STACK_PRIORITY, NULL) != pdPASS) {
		printf("Failed to test regulator task\r\n");
	}
	
	/* Start the FreeRTOS scheduler running all tasks indefinitely*/
	vTaskStartScheduler();

	// Insert application code here, after the board has been initialized.
}
