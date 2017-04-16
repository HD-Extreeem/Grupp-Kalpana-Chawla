/*
 * PwmFunctions.c
 *
 * Created: 2017-04-16 22:13:43
 *  Author: Hadi Deknache & Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"

/*Defining pin for PWM*/
#define PIN_21 IOPORT_CREATE_PIN(PIOC, 21)
#define PIN_37 IOPORT_CREATE_PIN(PIOC, 37)

pwm_channel_t PWM_pin_21;
pwm_channel_t PWM_pin_37;


void PWM_init(void){
	
	
	pmc_enable_periph_clk(ID_PWM);
	
	/*Disable pwm channel for appropriate configuration*/
	pwm_channel_disable(PWM,PWM_CHANNEL_4);
	pwm_channel_disable(PWM,PWM_CHANNEL_3);
	
	/*Configurating PWM clock*/
	pwm_clock_t PWMDAC_clock_config = {
		.ul_clka = 1000000,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	
	/*Initializing pwm*/
	pwm_init(PWM, &PWMDAC_clock_config);
	
	initPin21();
	initPin37();
	
}

void initPin21(void){
	
	pio_configure_pin(PIN_21, PIO_TYPE_PIO_PERIPH_B);
		/*Pwm channel configuration such as channel, alignments, period, duty etc...*/
		PWM_pin_21.channel = PWM_CHANNEL_4;
		PWM_pin_21.ul_prescaler = PWM_CMR_CPRE_CLKA;
		PWM_pin_21.polarity = PWM_HIGH;
		PWM_pin_21.alignment = PWM_ALIGN_LEFT;
		PWM_pin_21.ul_period = 8000;
		PWM_pin_21.ul_duty = 0;
		
	/*Initializing channel after setting things up*/
	pwm_channel_init(PWM, &PWM_pin_21);
	
	/*Enabling pwm channel after initializing everything correct*/
	pwm_channel_enable(PWM, PWM_CHANNEL_4);
}

void initPin37(void){
	
	pio_configure_pin(PIN_37, PIO_TYPE_PIO_PERIPH_B);
		/*Pwm channel configuration such as channel, alignments, period, duty etc...*/
		PWM_pin_37.channel = PWM_CHANNEL_3;
		PWM_pin_37.ul_prescaler = PWM_CMR_CPRE_CLKA;
		PWM_pin_37.polarity = PWM_HIGH;
		PWM_pin_37.alignment = PWM_ALIGN_LEFT;
		PWM_pin_37.ul_period = 8000;
		PWM_pin_37.ul_duty = 0;
		
		/*Initializing channel after setting things up*/
		pwm_channel_init(PWM, &PWM_pin_37);
		
		/*Enabling pwm channel after initializing everything correct*/
		pwm_channel_enable(PWM, PWM_CHANNEL_3);
}

void pwm_pin_21(uint32_t duty){
	if(duty<800){
		duty=800;
	}
	else if(duty>2200){
		duty=2200;
	}
	pwm_channel_update_duty(PWM, &PWM_pin_21, duty );
}

void pwm_pin_37(uint32_t duty){
	if(duty<800){
		duty=800;
	}
	else if(duty>2200){
		duty=2200;
	}
	pwm_channel_update_duty(PWM, &PWM_pin_37, duty );
}