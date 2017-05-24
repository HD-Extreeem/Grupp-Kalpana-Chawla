/*
 * task_regulator.h
 *
 * Author: Désirée Jönsson
 */ 


#ifndef TASK_REGULATOR_H
#define TASK_REGULATOR_H
#define TASK_REGULATOR_STACK_SIZE     (1024/sizeof(portSTACK_TYPE))
#define TASK_REGULATOR_STACK_PRIORITY     (3)//tskIDLE_PRIORITY

#define M 3

void task_regulator(void *pvParameters);
void regulator(void);
void updatematlab(int16_t new_PWM_value,int distance,int16_t error,uint16_t bv);


#endif /* TASK_REGULATOR_H_ */