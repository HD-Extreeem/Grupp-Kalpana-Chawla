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
	
	for(int i=800; i<=2200; i=i+100){
		pwm_pin_21(i);
		pwm_pin_37(i);
	}
}