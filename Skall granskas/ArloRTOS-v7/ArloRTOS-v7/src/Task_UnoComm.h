/*
 *  @file Task_UnoComm.h
 *
 *  @brief This header file handle the whole process of communication between robotic platform and mounted lift platform
 *		 Status is being requested from mounted lift and checks if finished or still running pick up/drop off
 *
 *  @author: Desiree J�nsson
 *
 *	@date 2017-04-20 16:25:25
 */


#ifndef TASK_UNOCOMM_H_
#define TASK_UNOCOMM_H_

#define TASK_UNO_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_UNO_STACK_PRIORITY     (3)//tskIDLE_PRIORITY

void task_unoComm( void *pvParameters);

#endif /* TASK_UNOCOMM_H_ */
