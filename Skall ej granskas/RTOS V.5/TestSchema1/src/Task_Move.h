/*
 * Task_Move.h
 *
 * Created: 2017-04-20 14:11:43
 *  Author: Hadi Deknache && Yurdaer Dalkic
    Desiree J?nsson och Jonas Eiselt
*/

#ifndef TASK_MOVE_H_
#define TASK_MOVE_H_

#define TASK_MOVE_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_MOVE_STACK_PRIORITY     (6)//tskIDLE_PRIORITY
void task_move(void *pvParameters);
void move(void);
typedef struct{
	double presentX,presentY,lastX,lastY,targetX,targetY;
	double sock[2],cube[2],glass[2],box[2];
}coordinates;

coordinates coord;
void coordinatesInit (void);
void calculateAngleDistance(void);
void updateLastPresent(void);
void updateNextPosLasse(void);
void updateNextPosGL(void);
void updateNextPosition(void);

#endif /* TASK_MOVE_H_ */