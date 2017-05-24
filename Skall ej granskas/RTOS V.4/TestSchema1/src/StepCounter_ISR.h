/*
* StepCounter_ISR.h
*
* Created: 2017-04-24 11:25:15
*  Author: Desiree
*/


#ifndef STEPCOUNTER_ISR_H_
#define STEPCOUNTER_ISR_H_

uint16_t counter_1;
uint16_t counter_2;

void attach_interupt(void);
void pin12_edge_handler(const uint32_t id, const uint32_t index);
void pin14_edge_handler(const uint32_t id, const uint32_t index);
void reset_Counter(void);

#endif /* STEPCOUNTER_ISR_H_ */