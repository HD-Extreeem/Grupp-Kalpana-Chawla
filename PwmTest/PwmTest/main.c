/*
 * main.c
 *
 * Created: 2017-04-16 23:04:34
 *  Author: Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	
	configInterrupts();
	
	/**
	PWM_init();
	
	delay_s(5);
		for(uint32_t i=1500; i>=800; i-=50){
			pwm_pin_21(i);
			pwm_pin_22(i);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_s(3);
		for(uint32_t i=1500; i<=2200; i+=50){
			pwm_pin_21(i);
			pwm_pin_22(i);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
	delay_s(3);
	while (1)
	{
		for(uint32_t i=1500; i>=800; i-=50){
			pwm_pin_21(i);
			pwm_pin_22(1500);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_s(3);
		for(uint32_t i=1500; i<=2200; i+=50){
			pwm_pin_21(i);
			pwm_pin_22(1500);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_s(3);
		for(uint32_t i=1500; i>=800; i-=50){
			pwm_pin_21(1500);
			pwm_pin_22(i);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
		delay_s(3);
		for(uint32_t i=1500; i<=2200; i+=50){
			pwm_pin_21(1500);
			pwm_pin_22(i);
			delay_ms(500);
		}
		pwm_pin_21(1500);
		pwm_pin_22(1500);
	}
	*/
}