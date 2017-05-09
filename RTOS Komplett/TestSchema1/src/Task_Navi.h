/*
 * Task_Navi.h
 *
 * Created: 2017-05-09 10:13:45
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 


#ifndef TASK_NAVI_H_
#define TASK_NAVI_H_

#define TASK_Navi_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_Navi_STACK_PRIORITY     (3)//tskIDLE_PRIORITY
void Task_Navi(void *pvParameters);
int objects[3][2];
int lastPosition [1][1];
int presentPosition [1][1];


#endif /* TASK_NAVI_H_ */