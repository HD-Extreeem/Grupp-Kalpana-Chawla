/*
 * InterruptStepCounter.h
 *
 * Created: 2017-04-19 11:29:16
 *  Author: Yurdaer Dalkic & Hadi Deknache
 *  Header file to InterruptStepCounter.c
 */ 


#ifndef INTERRUPTCOUNTER_H_
#define INTERRUPTCOUNTER_H_

void configInterrupts (void);
void pin12_edge_handler(const uint32_t id, const uint32_t index);
void pin14_edge_handler(const uint32_t id, const uint32_t index);

#endif /* INTERRUPTCOUNTER_H_ */