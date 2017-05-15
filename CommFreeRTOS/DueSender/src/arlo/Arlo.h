/*
 * Arlo.h
 *
 * Created: 2017-05-15 13:13:31
 * Author: Jonas Eiselt
 */ 


#ifndef ARLO_H_
#define ARLO_H_

#include "comm/TWICommHandler.h"

void arlo_init(void);
void arlo_arm_init(void);

void arlo_lift_object(Object object_t);
void arlo_drop_object(Object object_t);

#endif /* ARLO_H_ */