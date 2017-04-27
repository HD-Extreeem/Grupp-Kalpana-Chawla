/*
 * PID_move.c
 *
 * Created: 2017-04-26 14:38:37
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 
#include <asf.h>
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"
#include "PID_move.h"

extern uint16_t counter_1;
extern uint16_t counter_2;
uint16_t loop_counter;
int setPoint=0;
int m_value=0;
int s_value=0;
int e=1;
double kp = 1;
uint16_t speed = 1500;
int faktor;
int rotationSpeed=100;
int rotationValue;

void move (int distance, int degree){
// 	setPoint = 2*setPoint;
// 	 		m_value = (counter_2-counter_1);
// 	 		e = (setPoint - m_value);
// 	 		s_value = (kp*e);
// 	 		pwm_pin_21((speed+s_value));
// 	 		pwm_pin_22((speed-s_value));
//     		if (loop_counter == 100)
// 	 		{
// 	 			printf("counter1: %d\n",counter_1);
// 	 			printf("counter2: %d\n",counter_2);
// 	 			printf("loop_counter: %d\n",loop_counter);
// 	
// 	 		}
	//loop_counter++;
	//printf("\n e = %d",e);
	//printf("\n s_value = %d",s_value);
	//printf("\n m_value = %d",m_value);
	//delay_us(46200*3);
}

void rotation (int degree){
	
	pwm_pin_21(1500);
	pwm_pin_22(1500);
	if(degree>180){
	    degree=degree-360;
		faktor=-1;
	}
    rotationValue=((degree*faktor)/2);
	reset_Counter();
	while ((counter_1+counter_2) != rotationValue)
	{
		pwm_pin_21(1500+(rotationSpeed*faktor));
		pwm_pin_22(1500-(rotationSpeed*faktor));
	}
	
	pwm_pin_21(1500);
	pwm_pin_22(1500);
	reset_Counter();
	faktor=1;
}

