/*
 * PID_move.c
 *
 * Created: 2017-04-26 14:38:37
 *  Author: Yurdaer Dalkic && Hadi Deknache
 */ 
#include <asf.h>
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"

extern uint16_t counter_1;
extern uint16_t counter_2;
uint16_t loop_counter;
int setPoint=0;
int m_value=0;
int s_value=0;
int e=1;
double kp = 1;
uint16_t speed = 1500;

void move(int distance, int degree){
	if ((degree>0)||(degree<0))
	{
	setPoint = (degree/2);
	while (1)
	{
		printf("e_value: %d\n",e);
		m_value = (counter_2-counter_1);
		e = (setPoint - m_value);
		s_value = (kp*e);
		if (s_value<100)
		{
			s_value=100;
		}
		
		printf("e_value: %d\n",e);
		pwm_pin_21((speed+s_value));
		pwm_pin_22((speed-s_value));
		delay_us(46200);
		if (e==0)
		{
			pwm_pin_21(1500);
			pwm_pin_22(1500);
			reset_Counter();
			break;
			
		}
		
	}
	
	}

}

