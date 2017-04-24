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

extern uint16_t counter_1;
extern uint16_t counter_2;
int setPoint=0;
int m_value=0;
int s_value=0;
int e=0;
uint16_t kp = 1;
uint16_t speed = 1800;

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
	pwm_pin_22(1800);
	
	while (1)
	{
		//setPoint = 2*setPoint;
		m_value = (counter_2-counter_1);
		e = (setPoint - m_value);
		s_value = (kp*e);
		pwm_pin_21((speed+s_value));
		printf("\n e = %d",e);
		printf("\n s_value = %d",s_value);
		printf("\n m_value = %d",m_value);
		
	}
	
}
	

