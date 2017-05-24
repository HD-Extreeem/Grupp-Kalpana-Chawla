/*
 * PwmFunctions.h
 *
 * Created: 2017-04-16 22:58:09
 *  Author: Yurdaer Dalkic & Hadi Deknache
 *
 * Header file to PwmFunctions.c
 */ 


#ifndef PWMFUNCTIONS_H_
#define PWMFUNCTIONS_H_

void PWM_init(void);
void initPin21(void);
void initPin22(void);
void rightWheel(uint32_t duty);
void leftWheel(uint32_t duty);

#endif /* PWMFUNCTIONS_H_ */