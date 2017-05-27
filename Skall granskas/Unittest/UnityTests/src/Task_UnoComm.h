/*
 *  @file Task_UnoComm.h
 *
 *	@brief Header file for Task_UnoComm.c
 *
 *  @author: Desiree Jönsson
 *
 *	@date 2017-04-20 16:25:25
 */ 


#ifndef TASK_UNOCOMM_H_
#define TASK_UNOCOMM_H_

#define TASK_UNO_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_UNO_STACK_PRIORITY     (3)//tskIDLE_PRIORITY

void task_unoComm( void *pvParameters);

#endif /* TASK_UNOCOMM_H_ */