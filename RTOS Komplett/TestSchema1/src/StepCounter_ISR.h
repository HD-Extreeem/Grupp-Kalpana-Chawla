/*
 * StepCounter_ISR.h
 *
 * Created: 2017-04-24 11:25:15
 *  Author: Desiree
 */ 


#ifndef STEPCOUNTER_ISR_H_
#define STEPCOUNTER_ISR_H_

#define TASK_STEPRIGHT_STACK_SIZE    (1024/sizeof(portSTACK_TYPE))
#define TASK_STEPLEFT_STACK_SIZE    (1024/sizeof(portSTACK_TYPE))
#define TASK_STEP_STACK_PRIORITY     (4)//tskIDLE_PRIORITY

// void task_StepCounterRight(void *pvParameters);
// void task_StepCounterLeft(void *pvParameters);
void attach_interupt(void);
void pin12_edge_handler(void);
/*void pin14_edge_handler(void);*/
void reset_Counter(void);

#endif /* STEPCOUNTER_ISR_H_ */