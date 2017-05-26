/*
*	@file Task_NaviControl.h
*
* @brief This header file Handles the navigation of robotic platform which controls wheel based on a PID-control
*		  Task consist of state machine which loop through the process of finding and communicates with mounted lift platform
*
*	@author Hadi Deknache & Yurdaer Dalkic(code in structure),
*	Desiree J?nsson & Jonas Eiselt(structure)
*
*	@date 2017-04-20 14:11:43
*
*/

#ifndef TASK_NaviControl_H_
#define TASK_NaviControl_H_

#define TASK_NaviControl_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_NaviControl_STACK_PRIORITY     (6)//tskIDLE_PRIORITY
void task_NaviControl(void *pvParameters);
void move(void);
//struct used for storing coordinates for objects and Arlo
typedef struct{
	int16_t presentX,presentY,lastX,lastY,targetX,targetY;
	double sock[2],cube[2],glass[2],box[2];
}coordinates;

coordinates coord; //coordinates struct initialization
void coordinatesInit (void);
void calculateAngleDistance(void);
void updateLastPresent(void);
void updateNextPosLasse(void);
void updateNextPosGL(void);
void updateNextPosition(void);

#endif /* TASK_NaviControl_H_ */
