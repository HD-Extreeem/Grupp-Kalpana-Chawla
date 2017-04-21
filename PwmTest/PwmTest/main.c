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
int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();

	configInterrupts();

	
	PWM_init();
	while(1){
		delay_ms(6000);
		pwm_pin_21(1800);
		pwm_pin_22(1800);
	    delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
	    delay_ms(3000);
		pwm_pin_21(1200);
		pwm_pin_22(1200);
		delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1200);
	    delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
	    delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1800);
		delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_ms(3000);
		pwm_pin_21(1200);
		pwm_pin_22(1500);
	    delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_ms(3000);
		pwm_pin_21(1800);
		pwm_pin_22(1500);
		delay_ms(3000);
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		
		
	}
	
}
	

