/*
 * Task_GetCordinates.h
 *
 * Created: 2017-04-20 14:12:19
 *  Author: D�sir�e J�nsson och Jonas Eiselt
 */


#ifndef TASK_GETCORDINATES_H_
#define TASK_GETCORDINATES_H_
#define TASK_GET_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_GET_STACK_PRIORITY     (2)//tskIDLE_PRIORITY

void task_getCordinates( void *pvParameters);

#endif /* TASK_GETCORDINATES_H_ */
