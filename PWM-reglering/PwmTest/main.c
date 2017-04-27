/*
 * main.c
 *
 * Created: 2017-04-16 23:04:34
 *  Author: Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"
#include "consoleFunctions.h"
#include "PID_move.h"

// extern uint16_t counter_1;
// extern uint16_t counter_2;
// uint16_t loop_counter;
// int setPoint=0;
// int m_value=0;
// double s_value=0;
// int e=0;
// double kp = 2;
// uint16_t speed = 1700;

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
	printf("Startar");
	delay_s(3);
 	pwm_pin_22(1700);
 	pwm_pin_21(1700);
   delay_s(3);
   pwm_pin_22(1500);
   pwm_pin_21(1500);
	while (1)
	{
		  move(0,-30);
		  delay_s(3);
		//setPoint = 2*setPoint;
// 		m_value = (counter_2-counter_1);
// 		e = (setPoint - m_value);
// 		s_value = (kp*e);
// 		pwm_pin_21((speed+s_value));
// 		pwm_pin_22((speed-s_value));
// 		if (loop_counter == 100)
// 		{
// 			printf("counter1: %d\n",counter_1);
// 			printf("counter2: %d\n",counter_2);
// 			printf("loop_counter: %d\n",loop_counter);
// 		}
		//loop_counter++;
		//printf("\n e = %d",e);
		//printf("\n s_value = %d",s_value);
		//printf("\n m_value = %d",m_value);
		//delay_us(46200*3);
		
	}
	
}
	

