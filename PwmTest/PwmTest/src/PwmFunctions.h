/*
 * PwmFunctions.h
 *
 * Created: 2017-04-16 22:58:09
 *  Author: Yurdaer Dalkic & Hadi Deknache
 */ 


#ifndef PWMFUNCTIONS_H_
#define PWMFUNCTIONS_H_

void PWM_init(void);
void initPin21(void);
void initPin37(void);
void pwm_pin_21(uint32_t duty);
void pwm_pin_37(uint32_t duty);

#endif /* PWMFUNCTIONS_H_ */