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
	pwm_pin_22(1500);
	while(counter_2 <= 183){
		
		pwm_pin_21(1700);
	}
	
	pwm_pin_21(1500);
	delay_s(3);
	printf("\n");
	printf("Nästa\n");
	
	while (counter_1 <= 183)
	{
		pwm_pin_22(1700);
	}
	pwm_pin_22(1500);
	
}
	

