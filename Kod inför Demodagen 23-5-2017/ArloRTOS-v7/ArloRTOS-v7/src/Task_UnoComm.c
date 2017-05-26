/**
* @file Task_UnoComm.c
*
* @brief This source file handle the whole process of communication between robotic platform and mounted lift platform
*		 Status is being requested from mounted lift and checks if finished or still running pick up/drop off
*
*
* @author Desiree(structure), Hadi Deknache and Jonas Eiselt (Code in structure)
*
* @date 2017-04-20 16:25:09
*/

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>
#include "task.h"
#include "arlo/Arlo.h"

//Variable for calculated angle
int angle=0;
//Variable for calculated distance
int distance=0;

//Variable which hold status of pick up
extern Bool com_process_finished;

//Status to be read from the mounted lift platform
Pick_Up_Status pick_up_status_t;
Drop_Off_Status drop_off_status_t;

//Counting objects got
uint8_t object_counter=1;

//Enum used for state machine to switch between different states
typedef enum
{
	PICK_UP_START,PICK_UP_DO,DROP_OFF
} states;

//State variable which holds current state
states current_state = PICK_UP_START;

//State variable to hold next state
states next_state;

//Variable to control if Arlo need to drive/rotate
extern Bool arloNeedsToDrive;

//Variable for checking if Arlo is done driving/rotating
extern Bool arloIsDone;

//Variable checking if dropped or not
uint8_t already_dropped = 0;

//Variable holding objects left to get
uint8_t objects_left = 3;


/** Task to be run when communication starts
*	Check pick up status from mounted lift on Arlo and checks Drop off status if finished
*
*	@param pvParameters parameter to be used if passed through initializing task
**/
void task_unoComm(void *pvParameters)
{
	//Variable which checks last run
	portTickType xLastWakeTime;
	//Period used for task to run 200 ms
	const portTickType xTimeIncrement = 200;

	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.

	while (1)
	{
		/** State machine used for controlling different states for mounted lift communication**/
		switch (current_state)
		{

			/************************************************************************/
			/*                      PICK_UP_START									*/
			/*	Case PICK_UP_START, starts communication with mounted lift			*/
			/*	by sending a request to lift										*/
			/************************************************************************/

			case PICK_UP_START:
			arlo_pick_up_object(OBJECT);
			next_state = PICK_UP_DO;
			break;
			/************************************************************************/
			/*								PICK_UP DO								*/
			/*	This case controls which state the mounted lift on Arlo is running	*/
			/*	Request status from robot platform									*/
			/************************************************************************/
			case PICK_UP_DO:
			pick_up_status_t = arlo_get_pick_up_status();

			switch (pick_up_status_t)
			{
				/************************************************************************/
				/*							PICK_UP_FAILED								*/
				/*	Never sent from our mounted lift 									*/
				/************************************************************************/
				case PICK_UP_FAILED:
// 				if (!arloNeedsToDrive)
// 				{
// 					arloNeedsToDrive = true;
// 					angle = 10;
// 				}
// 				if (arloIsDone)
// 				{
// 					arloNeedsToDrive = false;
// 					arloIsDone=false;
// 					arlo_done_drive(PICK_UP_DONE_DRIVE);
// 					next_state = PICK_UP_START;
// 				}
				break;
				/************************************************************************/
				/*							PICK_UP_FORWARD								*/
				/*	Never sent from our mounted lift 									*/
				/************************************************************************/
				case PICK_UP_FORWARD:
				//Method was used for driving forward
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
				/************************************************************************/
				/*							PICK_UP_BACKWARD							*/
				/*	Never sent from our mounted lift 									*/
				/************************************************************************/
				case PICK_UP_BACKWARD:
				//Method for drivning backwards
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
				/************************************************************************/
				/*							PICK_UP_RUNNING								*/
				/*	While running roto platform should just wait until					*/
				/*	mounted lift state have picked up the object						*/
				/************************************************************************/
				case PICK_UP_RUNNING:

				break;
				/************************************************************************/
				/*							PICK_UP_DONE_DRIVE							*/
				/*	Never sent from our mounted lift 									*/
				/************************************************************************/
				case PICK_UP_DONE_DRIVE:

				break;
				/************************************************************************/
				/*							PICK_UP_IDLE								*/
				/*	Lift automatic goes to idle mode when finished picking up object 	*/
				/*	Decrease the object counter and moves to next state for drop off	*/
				/*	or move to next object and pick up rest								*/
				/************************************************************************/
				case PICK_UP_IDLE:
				//Decreases the objects left
				objects_left--;
				// If Arlo can collect all items and there are no more objects to pick up or if it cannot collect them all
				if ((arlo_get_collect_status() == 1 && objects_left == 0) || (arlo_get_collect_status() == 0))
				{
					next_state = DROP_OFF;
				}
				else
				{
					next_state = PICK_UP_START;

				}
				//Pick up finished and should countinue for next destination
				com_process_finished=true;
				//Suspending task since no needed during moving robot
				vTaskSuspend(NULL);
				break;
				/************************************************************************/
				/*							PICK_UP_DONE								*/
				/*	Decrease the object counter and moves to next state for drop off	*/
				/*	or move to next object and pick up rest								*/
				/************************************************************************/
				case PICK_UP_DONE:
				//Decreases the objects left
				objects_left--;
				// If Arlo can collect all items and there are no more objects to pick up or if it cannot collect them all
				if ((arlo_get_collect_status() == 1 && objects_left == 0) || (arlo_get_collect_status() == 0))
				{
					next_state = DROP_OFF;
				}
				else
				{
					next_state = PICK_UP_START;

				}
				//Pick up finished and should continue for next destination
				com_process_finished=true;
				//Suspending task since no needed during moving robot
				vTaskSuspend(NULL);
				break;
				/************************************************************************/
				/*                             INNER SWITCH CASE				        */
				/************************************************************************/
			}
			break;
			/************************************************************************/
			/*                              DROP_OFF CASE							*/
			/*	This case handle the drop off process for robot platform			*/
			/*	Communication between robotic platform and mounted lift				*/
			/*	Waiting for drop off to finish										*/
			/************************************************************************/

			case DROP_OFF:
			// Runs once when starting to request drop off for mounted platform
			if (already_dropped == 0)
			{
				//This angle and settings is only used for lift for Gr�nwalls to rotate and drop off items
				angle=180;
				arloNeedsToDrive=true;
				vTaskDelay(200);

				//Requesting drop off
				arlo_drop_object(OBJECT);
				//Set variable to drop off since started
				already_dropped = 1;
				next_state = DROP_OFF;
			}
			else
			{
				//Reading drop off status from platform
				drop_off_status_t = arlo_get_drop_off_status();
				//Checks if drop off done
				if (drop_off_status_t == DROP_OFF_DONE)
				{
					//Changing variable to finished
					com_process_finished=true;

					//Reseting status since it have dropped off item
					drop_off_status_t = 0;
					already_dropped = 0;

					//Reseting to beginning since finished
					next_state = PICK_UP_START;
					//Suspending task
					vTaskSuspend(NULL);
				}
				//Wait for drop off finish
				else
				{
					next_state = DROP_OFF;
				}
			}
			break;
			/************************************************************************/
			/*                             Switch case                              */
			/************************************************************************/
		}
		// Current state is next state for switching between states
		current_state = next_state;

		// Wait for the next cycle after have finished everything
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);
	}
}
