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
// 	printf("main");
// 	pio_set_output(PIOB, PIO_PB27, LOW, DISABLE, ENABLE);
// 	pio_set_output(PIOD, PIO_PD8, LOW, DISABLE, ENABLE);
	configInterrupts();
	
// 	for(int i=0; i<=9;i++){
// 	pio_set(PIOD, PIO_PD8);
// 	pio_set(PIOB, PIO_PB27);
// 	delay_ms(25);
//     pio_clear(PIOD, PIO_PD8);
//     pio_clear(PIOB, PIO_PB27);
//     delay_ms(25);

	
	PWM_init();
		pwm_pin_21(1000);
		pwm_pin_22(1000);
		while(1){
			
		}
	/**
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
