/*
 *	@file PwmFunctions.h
 *
 * @ The header file configure two PWM pins on Arduino Due, pin 8 and pin 9.
 * The PWM signal has period time 8 ms and duty cycle is between 0.8 ms and 2.2 ms.
 *
 *  @author Yurdaer Dalkic & Hadi Deknache
 *
 *	@date 2017-04-16 22:58:09
 * 
 */ 


#ifndef PWMFUNCTIONS_H_
#define PWMFUNCTIONS_H_

void PWM_init(void);
void initPin21(void);
void initPin22(void);
void rightWheel(uint32_t duty);
void leftWheel(uint32_t duty);
/*Defining pin for PWM*/
#define PIN_21 IOPORT_CREATE_PIN(PIOC, 21)
#define PIN_22 IOPORT_CREATE_PIN(PIOC, 22)

#endif /* PWMFUNCTIONS_H_ */