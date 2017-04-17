/*
 * main.c
 *
 * Created: 2017-04-16 23:04:34
 *  Author: Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	PWM_init();
	while (1)
	{
		for(uint32_t i=800; i<=1500; i+=50){
			pwm_pin_21(i);
			pwm_pin_22(i);
			//pwm_channel_update_duty(PWM, &PWM_pin_22, i );
		}
	}
	
}