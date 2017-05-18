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
void arlo_nav_init(int16_t *object_buffer);

void arlo_pick_up_object(Object object_t);
void arlo_drop_object(Object object_t);

void arlo_find_object(Object object_t);

void arlo_get_position(int16_t *position_buffer);

Pick_Up_Status arlo_get_pick_up_status(void);
Find_Object_Status arlo_get_find_object_status(void);
Drop_Off_Status arlo_get_drop_off_status(void);

uint8_t arlo_get_collect_status(void);

void arlo_get_object_positions(uint16_t *object_buffer);

#endif /* ARLO_H_ */