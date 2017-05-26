/*
 *  @file Task_GetCoordinates.h
 *
 *  @brief Header file for handle the positioning, updates positioning for Arlo platform
 *
 *  @author: D�sir�e J�nsson och Jonas Eiselt
 *
 *  @date 2017-04-20 14:12:19
 */


#ifndef TASK_GETCOORDINATES_H_
#define TASK_GETCOORDINATES_H_
#define TASK_GET_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_GET_STACK_PRIORITY     (4)//tskIDLE_PRIORITY
void task_getCoordinates( void *pvParameters);

#endif /* TASK_GETCOORDINATES_H_ */
