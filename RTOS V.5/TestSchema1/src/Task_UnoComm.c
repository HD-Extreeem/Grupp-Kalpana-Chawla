/**
* Task_UnoComm.c
*
* Created: 2017-04-20 16:25:09
* Author: Desiree
*
* Modified by Hadi Deknache and Jonas Eiselt
*/

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>
#include "task.h"
#include "arlo/Arlo.h"

//extern Bool liftProcessFinished;
int angle=0;
int distance=0;
extern Bool pick_up_process_finished;
extern Bool drop_off_process_finished;
extern Bool process_running;

extern Pick_Up_Status pick_up_status_t;
extern Find_Object_Status find_object_status_t;
extern Drop_Off_Status drop_off_status_t;

uint8_t object_counter=1;

typedef enum
{
	START,
	SEARCH,
	PICK_UP,
	DROP_OFF
} states;

states current_state = START;
states next_state;

uint8_t already_lifted = 0;
uint8_t already_dropped = 0;
uint8_t objects_left = 3;

uint8_t pick_up_tries = 0;
uint8_t number_of_rotations = 0;	// Used for

void task_unoComm(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;

	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.

	while (1)
	{
		switch (current_state)
		{
			///////////////////////////Start-state//////////////////////////////////////
			case START:
			printf("START CASE\r\n");
			arlo_find_object(OBJECT);
			next_state = SEARCH;
			break;
			///////////////////////////Search-state////////////////////////////////////
			case SEARCH:
			printf("SEARCH CASE\r\n");
			if (pick_up_tries >= 3)
			{
				printf("pick_up_process_finished = true\r\n");
				pick_up_process_finished = true;
				pick_up_status_t = 0;
				find_object_status_t = 0;
				next_state = START;
				vTaskSuspend(NULL);
			}
			else
			{
				find_object_status_t = arlo_get_find_object_status();
				if (find_object_status_t == OBJECT_FOUND)
				{
					printf("Object found\r\n");
					number_of_rotations = 0;
					pick_up_tries = 0;
					next_state = PICK_UP;
				}
				else
				{
					if (find_object_status_t == ARLO_ROTATE_LEFT)
					{
						angle = -10;
					}
					else if (find_object_status_t == ARLO_ROTATE_RIGHT)
					{
						angle = 10;
					}
					else if (find_object_status_t == ARLO_FORWARD)
					{
						distance = 10;
					}
					else if (find_object_status_t == ARLO_BACKWARD)
					{
						distance = -10;
					}
					else if (find_object_status_t == OBJECT_NOT_FOUND)
					{
						number_of_rotations++;
						if (number_of_rotations == 36)
						{
							printf("Rotating");
							number_of_rotations = 0;
							pick_up_tries++;
						}
					}
					next_state = SEARCH;
				}
			}
			break;
			///////////////////////////Pick_up-state////////////////////////////////////
			case PICK_UP:
			printf("PICK UP CASE");
			if (already_lifted == 0)
			{
				arlo_pick_up_object(OBJECT);
				already_lifted = 1;
				next_state = PICK_UP;
			}
			else
			{
				if (pick_up_status_t != PICK_UP_DONE)
				{
					pick_up_status_t = arlo_get_pick_up_status();
				}
				else
				{
					printf("Pick up process finished\r\n");
					pick_up_process_finished = true;
					// liftProcessFinished = true;
					pick_up_status_t = 0;
					find_object_status_t=0;
					already_lifted = 0;
					
					// If Arlo can collect all items and there are no more objects to pick up or if it cannot collect them all
					if ((arlo_get_collect_status() == 1 && objects_left == 0) || (arlo_get_collect_status() == 0))
					{
						next_state = DROP_OFF;
					}
					else
					{
						next_state = START;
					}
					vTaskSuspend(NULL);
				}
			}
			break;
			///////////////////////////Drop_off-state////////////////////////////////////
			case DROP_OFF:
			printf("DROP OFF CASE\r\n");
			if (already_dropped == 0)
			{
				arlo_drop_object(OBJECT);
				already_dropped = 1;
				next_state = DROP_OFF;
			}
			else
			{
				drop_off_status_t = arlo_get_drop_off_status();
				if (drop_off_status_t == DROP_OFF_DONE)
				{
					printf("Drop off process finished\r\n");
					drop_off_process_finished = true;
					if (arlo_get_collect_status() == 1) 
					{
						objects_left = objects_left - 3;
					}
					else 
					{
						objects_left--;
					}
				
					// liftProcessFinished = true;
					drop_off_status_t = 0;
					already_dropped = 0;
					
					next_state = START;
					vTaskSuspend(NULL);
				}
				else
				{
					next_state = DROP_OFF;
				}
			}
			break;
			////////////////////////-----SLUT-----!!!!///////////////////////////////////
		}
		current_state = next_state;
		
		//		static uint8_t i = 1;
		/* Lifts object */
		// 		if (flag_i==1)
		// 		{
		// 			//arlo_lift_object(CUBE);
		// 			arlo_find_object(CUBE);
		// 			flag_i = 2;
		// 		}
		
		// 		if (find_object_status_t == OBJECT_NOT_FOUND)
		// 		{
		// 			//delay_ms(500);
		// 			find_object_status_t = arlo_get_find_object_status();
		// 		}
		// 		if (status != PICK_UP_DONE)
		// 		{
		// 			//delay_ms(500);
		// 			status = arlo_get_pick_up_status();
		// 		}
		// 		else
		// 		{
		// 			printf("liftProcessFinieshed = true\r\n");
		// 			liftProcessFinished = true;
		// 			flag_i = 1;
		// 			status = 0;
		// 			vTaskSuspend(NULL);
		// 		}
		
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle after have finished everything
		//vTaskSuspend(NULL);
	}
}