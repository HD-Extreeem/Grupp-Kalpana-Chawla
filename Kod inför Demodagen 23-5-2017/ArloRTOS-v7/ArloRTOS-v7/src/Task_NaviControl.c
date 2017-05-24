/**
*  @file Task_Move.c
*
*  @brief This source file Handles the navigation of robotic platform which controls wheel based on a PID-control
*		  Task consist of state machine which loop through the process of finding and communicates with mounted lift platform
*
*  @author: Hadi Deknache & Yurdaer Dalkic & Jonas Eiselt(Code in structure) and Desiree Jönsson & Jonas Eiselt (Structure)
*
*  @date 2017-04-20	
*/

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "arlo/Arlo.h"
#include "Task_NaviControl.h"
#include "MathFunctions.h"
#include "PwmFunctions.h"
#include "StepCounter_ISR.h"

extern int distance; //Distance for robot drives
extern int angle; //Angle which robot need to rotate
int direction=1; //Direction Arlo want to drive (-1=backward),(1=forward)
double totalPulses; //Calculate the total pulses needed to move to destination
uint16_t speed = 200; // Set speed for moving the robot
double proportionalError = 0; //P-controller error variable
double referenceValue = 0;	//The calculated reference value
double measurementValue=0;	// The measured value between pulse counters
double controlValue=0; //Variable to store total value for PID-controller
double K=2.5; //Gain for the PID-controller
double totMovement = 0; //Variable to store totalmovement during the transportation
double integral=0; // Variable to use for calculated I-part for PID-control
double derivate=0; // Variable to use for calculated I-part for PID-control
double prevD=0;	 //Variable for last error to hold
double dT=0.1;	 // Variable for sample time
double Td=0.053; //Gain for d-part
double Ti=0.215; //Gain for I-part
int16_t sum=0;   //Sum of error
int course;		 //Variable for which way robot need to drive(backwards/forward)
int rotationSpeed=90;	//Speed set for rotation

Bool newData=false; //Variable to tell when to fetch new position data
extern uint8_t object_counter;//Counter for objects gotten

Bool liftStart = false;//Boolean to check if lift started or not yet

extern xTaskHandle xTaskMove; //Variable to access task for suspending and resuming task move
extern xTaskHandle xTaskCom; //Variable to access task for suspending and resuming task com
extern xTaskHandle xTaskCoordinate; //Variable to access task for suspending and resuming task getcoordinate

typedef enum {START,BEFORE_ROTATE,ROTATE,MOVE,COMM,NAVI,CLOSE,WAIT} states; //Enum with states used for state machine

states currentState = START;//State variable used for switching to current state
states nextState; //State for switching to next state next session

Bool com_process_finished = false; //Variable to check if communication is finished or not yet
Bool arloIsDone = false; //Used for checking if Arlo have rotated/driven
Bool arloNeedsToDrive=false; //Variable to check if Arlo got any distance to drive/to rotate


void task_NaviControl(void *pvParameters)
{
	portTickType xLastWakeTime; //Variable to hold current time
	const portTickType xTimeIncrement = 200; //Time given for the task to complete work...
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle after have finished everything
		
		switch (currentState)
		{
			/************************************************************************/
			/*								START									*/
			/*	This case checks if all items have been picked or not				*/
			/*	checks also which mounted lift on Arlo								*/
			/************************************************************************/
			case START:
			/* If Arlo can pick up all objects without going to box in between pick-ups */
			if (arlo_get_collect_status() == 1)
			{
				updateNextPosition(); // Updates data position for next run
				
				//Checks if driven to every position yet
				if (object_counter > 5)
				{
					nextState = CLOSE;
				}
				//Else continue to next position
				else
				{
					nextState = BEFORE_ROTATE;
				}
			}
			//Else Arlo can only pick one item at a time
			else
			{
				updateNextPosition();// Updates data position for next run
				//Checks if driven to every position yet
				if (object_counter > 7)
				{
					nextState = CLOSE;
				}
				//Else continue to next position
				else
				{
					nextState = BEFORE_ROTATE;
				}
			}
			
			break;
			/************************************************************************/
			/*								Navi									*/
			/*	Case Navi checks if new position data need to be fetched			*/
			/*	Also checks if communication with lift need to begin				*/
			/*	else navigates to next position										*/
			/************************************************************************/
			case NAVI:
			//checks if new data is needed
			if (newData)
			{
				vTaskResume(xTaskCoordinate);//resumes task for getting data
				nextState=WAIT; //jumps to WAIT case to fetch data
			}
			//Startar kommunikationen när roboten är framme
			if (liftStart)
			{
				liftStart=false;	//Sets liftstart to false to prevent from entering again
				nextState = COMM;	//Switches state to check com status
				vTaskResume(xTaskCom);	//Resumes communication with mounted lift
			}
			//else continues its route
			else{
				nextState = MOVE;
			}
			
			break;
			/************************************************************************/
			/*							       MOVE									*/
			/*	Case MOVE checks if Arlo have reached its destination				*/
			/*	calculates total pulses needed to reach destination from encoders	*/
			/************************************************************************/
			case MOVE:
			totalPulses = (distance*direction/1.45);//calculates total pulses needed to reach
			/*Checks if reached its destination to stop and reset every variable for next run later*/
			if (totMovement+2>=totalPulses)
			{
				rightWheel(1500);//Stop rightWheel
				leftWheel(1500);//Stop leftWheel
				controlValue=0; //reset controll value
				measurementValue=0; //reset measurment value
				totMovement=0;//reset total movement of robot
				proportionalError=0; //reset error of p-control part
				angle=0;	//resets angle value
				sum=0;		//resets sum
				speed = 200;	//set speed back to 200
				distance=0;		//set distance to 0
				reset_Counter();	//resets encoder value countered
				liftStart=true;		//sets variable to true to begin communication
				newData=true; //Sets variable to true for fetching position data from system
				nextState = NAVI; //goes back to navi case
			}
			//else if not yet finished runs move for checking wheels so they don't get of course and control
			else
			{
				move();//method to control
				nextState = MOVE;
			}
			break;
			/************************************************************************/
			/*							 Before_rotate								*/
			/*	Case before rotate calculates the rotation angle and pulses needed	*/
			/*																		*/
			/************************************************************************/
			case BEFORE_ROTATE:
			printf("BEFORE ROTATE");
			course=1;     // rotation course, 1 to right -1 to left
			if (angle<0)
			{
				course=-1;
			}
			// total number of pulses required for rotation
			totalPulses=((angle*course)/2);
			rightWheel(1500);//stops right wheel
			leftWheel(1500);//stops left wheel
			reset_Counter();//resets counter for encoders on wheels
			nextState = ROTATE;
			break;
			/************************************************************************/
			/*							       ROTATE								*/
			/*	Case Rotate rotates Arlo platform on its own axis					*/
			/*																		*/
			/************************************************************************/
			case ROTATE:
			//CHecks if rotation is finished
			if ((counter_1+counter_2+1) >= totalPulses)
			{
				//  stop wheels
				rightWheel(1500);
				leftWheel(1500);
				
				reset_Counter(); //resets counter for encoders
				angle=0; 
				totalPulses=0;
				totMovement=0;
				//checks if command came from communication to continue communication
				if (arloNeedsToDrive)
				{
					arloIsDone=false;
					nextState=COMM;
				}
				//else continues to move
				else
				{
					nextState = MOVE;
				}
				
			}
			//else continues the rotation of the wheels
			else
			{
				leftWheel(1500 + ( rotationSpeed*course) );
				rightWheel(1500 - ( rotationSpeed*course) );
				nextState = ROTATE;
			}
			break;
			/************************************************************************/
			/*							       CLOSE								*/
			/*	Case CLOSE is a deadlock case where is set when route and pick up	*/
			/*	process is finished													*/
			/************************************************************************/
			case CLOSE:
			nextState = CLOSE;
			break;
			/************************************************************************/
			/*							       COMM									*/
			/*	Case Comm handles the communication and commands sent from the lift	*/
			/*	Checks if lift have finished its process to continue				*/
			/************************************************************************/
			case COMM:
			//checks whether lift have finished it process or not yet
			if (com_process_finished)
			{
				com_process_finished=false;//resets variable for process to be triggered next time
				nextState = START;
			}
			//checks if needed to rotate
			else if (arloNeedsToDrive)
			{
				nextState=BEFORE_ROTATE;
			}
			//Else wait on command or finishing communication
			else
			{
				nextState = COMM;
			}
			
			break;
			/************************************************************************/
			/*							       WAIT									*/
			/*	Checks if new position data have been fetched yet					*/
			/*																		*/
			/************************************************************************/
			case WAIT:
			//checks if data have been fetched
			if (!newData)
			{
				nextState=NAVI;
			}
			//else waits
			else
			{
				nextState = WAIT;
			}
			
			break;
			/************************************************************************/
			/*                        end of state machine			                */
			/************************************************************************/
		}
		//switches state to next state for next run
		currentState = nextState;
	}
}

/** This method uses a PID-control to run wheels simultaneously
*	calculates total pulses wheels have run and values for error and corrections of wheels
**/
void move (void)
{
	//Check if need to go backwards
	if (distance < 0)
	{
		direction =-1;
	}
	//else set direction forward
	else
	{
		direction =1;
	}
	//calculates and stores the total movement
	totMovement = ((counter_1+counter_2)/2);
	measurementValue = (counter_2-counter_1);// Calculates the error differences
	proportionalError = (referenceValue - measurementValue); // Calculates p-controller gain
	//checks if error and referencevalue out of range
	if ((referenceValue > 0) && (proportionalError < 0))
	{
		referenceValue = 0;
		proportionalError=0;
		sum=0;
		prevD=0;
		counter_1 = (counter_1+counter_2)/2;
		counter_2=counter_1;
	}
	//checks if error and referencevalue out of range
	if ((referenceValue < 0) && (proportionalError > 0))
	{
		referenceValue = 0;
		proportionalError=0;
		sum=0;
		prevD=0;
		counter_1 = (counter_1+counter_2)/2;
		counter_2=counter_1;
	}
	//calculates the sum of errors
	sum = (sum + prevD);
	//calculates the error of I-part
	integral= (sum * (dT/Ti));
	//calculates the error of the error in the future to correct
	derivate = ((Td/dT) * (proportionalError-prevD));
	//total error to correct for PID-control
	controlValue =(K*(proportionalError+integral+ derivate)); 
	//stores the previous error
	prevD=proportionalError;
	//	Check if almost reached the destination to slow down and make a smoother brake
	if (((totMovement/totalPulses) >= 0.90) || ((totMovement/totalPulses) <= 0.05))
	{
		speed = 130;
	}
	//Else same speed set
	else
	{
		speed = 200;
	}
	//checks if control value going out of bounds
	if (controlValue>70)
	{
		controlValue=70;
	}
	//checks if control value going out of bounds
	else if (controlValue<-70)
	{
		controlValue=-70;
	}
	
	rightWheel(1500 + ((speed+controlValue)*direction));//set new speed for rightWheel
	leftWheel( 1500 + ((speed-controlValue)*direction));//set new speed for leftWheel
	
}
/** This method initialize the coordinates for the objects and where Arlo stands
*	Positions of objects is got from the positioning system
**/
void coordinatesInit (void)
{
	int16_t object_buffer[8] = {0}; //arrayy used to be stored coordinates objects
	arlo_get_object_positions(object_buffer); //request coordinates fromn positionings system
	
	//Robot platform present positions x and y coordinates
	coord.presentX = 200;
	coord.presentY = 0;
	//Robot platform last positions x and y coordinates
	coord.lastX = 200;
	coord.lastY = -100;

	coord.sock[0] = object_buffer[0];//X coordinate sock
	coord.sock[1] = object_buffer[1];//Y coordinate sock

	coord.cube[0] = object_buffer[2];//X coordinate cube
	coord.cube[1] = object_buffer[3];//Y coordinate cube

	coord.glass[0] = object_buffer[4];//X coordinate glass
	coord.glass[1] = object_buffer[5];//Y coordinate glass
	
	coord.box[0] = 50; //X coordinate box (hardcooded)
	coord.box[1] = -10; //Y coordinate box (hardcooded)
	
	//coord.box[0] = object_buffer[6]; // X coordinate box
	//coord.box[1] = object_buffer[7]; //Y coordinate box
}


/** This method is used for updating next position coordinate to drive to
*	checks which mounted lift, The lift with box can pick all at once
*	the lift that only can take one item at a time need to go back and forth
**/
void updateNextPosition()
{
	//If Arlo can pick up all items at time
	if (arlo_get_collect_status() == 1)
	{
		//first item to drive to
		if (object_counter == 1)
		{
			coord.targetX=(double)coord.sock[0];//sets target to sock x coordinate
			coord.targetY=(double)coord.sock[1];//sets target to sock y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//second item to drive to
		else if (object_counter == 2)
		{
			coord.lastX=(double) 200; //update last place arlo stood x coordinate
			coord.lastY=(double) 0; //update last place arlo stood y coordinate
			coord.targetX=(double)coord.cube[0]; //update target to cube x coordinate
			coord.targetY=(double)coord.cube[1]; //update target to cube y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//third item to drive to
		else if (object_counter == 3)
		{
			coord.lastX=(double)coord.sock[0];//update last place arlo stood x coordinate
			coord.lastY=(double)coord.sock[1];//update last place arlo stood y coordinate
			coord.targetX=(double)coord.glass[0];//update target to glass x coordinate
			coord.targetY=(double)coord.glass[1];//update target to glass y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//last place to drive to box
		else if(object_counter == 4)
		{
			coord.lastX=(double)coord.cube[0];//update last place arlo stood x coordinate
			coord.lastY=(double)coord.cube[1];//update last place arlo stood y coordinate
			coord.targetX=(double)coord.box[0];//update target to box x coordinate
			coord.targetY=(double)coord.box[1];//update target to box y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		object_counter++;
	}
	//else if Arlo only can pick one item at a time
	else
	{
		//Drive to first object
		if (object_counter == 1)
		{
			coord.targetX=coord.sock[0];//sets target to sock x coordinate
			coord.targetY=coord.sock[1];//sets target to sock x coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//Drive to box to leave item
		else if (object_counter == 2)
		{
			coord.lastX=(double) 0;//update last place arlo stood y coordinate
			coord.lastY=(double) 0;//update last place arlo stood y coordinate
			coord.targetX=coord.box[0];//sets target to box x coordinate
			coord.targetY=coord.box[1];//sets target to box y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//drive to second item to get
		else if(object_counter == 3)
		{
			coord.lastX=(double)coord.sock[0];//update last place arlo stood y coordinate
			coord.lastY=(double)coord.sock[1];//update last place arlo stood y coordinate
			coord.targetX=coord.cube[0];//sets target to cube x coordinate
			coord.targetY=coord.cube[1];//sets target to cube y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//drive to box to leave item
		else if (object_counter == 4)
		{
			coord.lastX=(double)coord.box[0];//update last place arlo stood y coordinate
			coord.lastY=(double)coord.box[1];//update last place arlo stood y coordinate
			coord.targetX=coord.box[0];//sets target to box x coordinate
			coord.targetY=coord.box[1];//sets target to box y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//drive to last item to get
		else if (object_counter == 5)
		{
			coord.lastX=(double)coord.cube[0];//update last place arlo stood y coordinate
			coord.lastY=(double)coord.cube[1];//update last place arlo stood y coordinate
			coord.targetX=coord.glass[0];//sets target to glass x coordinate
			coord.targetY=coord.glass[1];//sets target to glass y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//drive to box last time to leave
		else if (object_counter == 6)
		{
			coord.lastX=(double)coord.box[0];//update last place arlo stood y coordinate
			coord.lastY=(double)coord.box[1];//update last place arlo stood y coordinate
			coord.targetX=coord.box[0];//sets target to box x coordinate
			coord.targetY=coord.box[1];//sets target to box y coordinate
			calculateAngleDistance();//calculates the distance and angle
		}
		//increase object counter
		object_counter++;
	}
}
/** This method was used before for dead reckoning since no data was fetched between positions
*	Sets present position to last position Arlo was standing on
**/
void updateLastPresent()
{
	coord.lastX=(double)coord.presentX;
	coord.lastY=(double)coord.presentY;
}
/**This method is used to calculate the angle and distance needed for arlo to travel to next item
*	Used methods in mathfunction.c to make its calculations
**/
void calculateAngleDistance(void){
	angle = calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.targetX,coord.targetY);
	distance = calculateDistance(coord.presentX,coord.presentY,coord.targetX,coord.targetY);
}