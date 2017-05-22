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

extern Bool requestedByUno;

extern Pick_Up_Status pick_up_status_t;
extern Drop_Off_Status drop_off_status_t;

uint8_t object_counter=1;

typedef enum
{
	PICK_UP_START,
	PICK_UP_DO,
	DROP_OFF
} states;

states current_state = PICK_UP_START;
states next_state;
extern Bool arloNeedsToDrive;
extern Bool arloIsDone;

uint8_t already_lifted = 0;
uint8_t already_dropped = 0;
uint8_t objects_left = 3;

uint8_t pick_up_tries = 0;
uint8_t number_of_rotations = 0;	// Used for

void task_unoComm(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 400;

	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.

	while (1)
	{
		switch (current_state)
		{
			///////////////////////////Start-state//////////////////////////////////////
			case PICK_UP_START:
			printf("PICK_UP_START\r\n");
			arlo_pick_up_object(OBJECT);
			next_state = PICK_UP_DO;
			break;
			///////////////////////////Search-state////////////////////////////////////
			case PICK_UP_DO:
			printf("PICK_UP_DO\r\n");
			pick_up_status_t = arlo_get_pick_up_status();
			
			switch (pick_up_status_t)
			{
				case PICK_UP_FAILED:
				printf("PICK_UP_FAILED\r\n");
				if (!arloNeedsToDrive)
				{
					arloNeedsToDrive = true;
					angle = 10;
				}
				if (arloIsDone)
				{
					arloNeedsToDrive = false;
					arloIsDone=false;
					arlo_done_drive(PICK_UP_DONE_DRIVE);
					next_state = PICK_UP_START;
				}
				break;
				case PICK_UP_FORWARD:
				printf("PICK_UP_FORWARD\r\n");
				if (!arloNeedsToDrive)
				{
					distance = 10;
					arloNeedsToDrive = true;
				}
				if (arloIsDone)
				{
					arloNeedsToDrive = false;
					arlo_done_drive(PICK_UP_DONE_DRIVE);
					next_state = PICK_UP_START;
				}
				break;
				case PICK_UP_BACKWARD:
				printf("PICK_UP_BACKWARD\r\n");
				if (!arloNeedsToDrive)
				{
					distance = -10;
					arloNeedsToDrive = true;
				}
				if (arloIsDone)
				{
					arloNeedsToDrive = false;
					arlo_done_drive(PICK_UP_DONE_DRIVE);
					next_state = PICK_UP_START;
				}
				break;
				case PICK_UP_DONE:
				printf("PICK_UP_DONE\r\n");
				objects_left--;
				// If Arlo can collect all items and there are no more objects to pick up or if it cannot collect them all
				if ((arlo_get_collect_status() == 1 && objects_left == 0) || (arlo_get_collect_status() == 0))
				{
					printf("Skall lämna allt nu!!!!!!!!!!!!!!\r\n");
					next_state = DROP_OFF;
				}
				else
				{
					printf("NÄÄÄÄÄÄÄÄÄSTTTTTTTTTTTTAAAAAAAA!!!!!!!!!!!!!!\r\n");
					printf("\n pick_up_process_finished=true\r\n");
					pick_up_process_finished=true;
					next_state = PICK_UP_START;
					vTaskSuspend(NULL);
				}
				break;
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
					pick_up_process_finished=true;
					drop_off_status_t = 0;
					already_dropped = 0;
					next_state = PICK_UP_START;
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
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle after have finished everything
	}
}