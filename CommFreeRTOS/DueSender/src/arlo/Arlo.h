/*
 * Arlo.h
 *
 * Created: 2017-05-15 13:13:31
 * Author: Jonas Eiselt
 */ 


#ifndef ARLO_H_
#define ARLO_H_

#include "comm/TWIComm.h"

void arlo_init(void);
void arlo_arm_init(void);
void arlo_nav_init(void);

void arlo_lift_object(Object object_t);
void arlo_drop_object(Object object_t);

void arlo_get_position(uint16_t *position_buffer);

#endif /* ARLO_H_ */