/*
 * Task_Move.h
 *
 * Created: 2017-04-20 14:11:43
 *  Author: D�sir�e J�nsson och Jonas Eiselt
 */ 


#ifndef TASK_MOVE_H_
#define TASK_MOVE_H_


#define TASK_MOVE_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_MOVE_STACK_PRIORITY     (2)//tskIDLE_PRIORITY
int objects[3][2];
int lastPosition [1][1];
int presentPosition [1][1];
void task_move(void *pvParameters);

#endif /* TASK_MOVE_H_ */