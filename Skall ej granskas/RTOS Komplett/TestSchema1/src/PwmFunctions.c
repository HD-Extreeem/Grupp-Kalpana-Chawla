/*
 * PwmFunctions.c
 *
 * Created: 2017-04-16 22:13:43
 *  Author: Hadi Deknache & Yurdaer Dalkic
 *
 * The source file configure two PWM pins on Arduino Due, pin 8 and pin 9.
 * The PWM signal has period time 8 ms and duty cycle is between 0.8 ms and 2.2 ms.
 *
 */ 
#include "asf.h"
#include "PwmFunctions.h"


/*Define the PWM channel instance in order to configure channel 4 and 5*/
pwm_channel_t PWM_pin_21;
pwm_channel_t PWM_pin_22;


/* This method configure PWM clock   */
void PWM_init(void){
	
	//Enable the module clock for the PWM peripheral
	pmc_enable_periph_clk(ID_PWM);
	
	/*Disable PWM channels for appropriate configuration*/
	pwm_channel_disable(PWM,PWM_CHANNEL_4);
	pwm_channel_disable(PWM,PWM_CHANNEL_5);
	
	/*Setup clock for PWM module*/
	pwm_clock_t PWMDAC_clock_config = {
		.ul_clka = 1000000,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	pwm_init(PWM, &PWMDAC_clock_config);
	
	//Methods for initializing PWM for pin 8 and 9
	initPin21();
	initPin22();
	
}

/* This method initialize channel instance and configure PWM channel 4 for pin 9,
 selecting clock A as its source clock, setting the period at 8 ms and setting the duty cycle at 0% */
void initPin21(void){
	
	    pio_configure_pin(PIN_21, PIO_TYPE_PIO_PERIPH_B);
		/*Pwm channel configuration such as channel, alignments, period, duty etc...*/
		PWM_pin_21.channel = PWM_CHANNEL_4;
		PWM_pin_21.ul_prescaler = PWM_CMR_CPRE_CLKA;
		PWM_pin_21.polarity = PWM_LOW;
		PWM_pin_21.alignment = PWM_ALIGN_LEFT;
		PWM_pin_21.ul_period = 7500;
		PWM_pin_21.ul_duty = 0;
		
	/*Initializing channel after setting things up*/
	pwm_channel_init(PWM, &PWM_pin_21);
	
	/*Enabling pwm channel after initializing everything correct*/
	pwm_channel_enable(PWM, PWM_CHANNEL_4);
}

/* This method initialize channel instance and configure PWM channel 5 for pin 8,
 selecting clock A as its source clock, setting the period at 8 ms and setting the duty cycle at 0% */
void initPin22(void){
	   
	    pio_configure_pin(PIN_22, PIO_TYPE_PIO_PERIPH_B);
		/*Pwm channel configuration such as channel, alignments, period, duty etc...*/
		PWM_pin_22.channel = PWM_CHANNEL_5;
		PWM_pin_22.ul_prescaler = PWM_CMR_CPRE_CLKA;
		PWM_pin_22.polarity = PWM_LOW;
		PWM_pin_22.alignment = PWM_ALIGN_LEFT;
		PWM_pin_22.ul_period = 7500;
		PWM_pin_22.ul_duty = 0;
		
		/*Initializing channel after setting things up*/
		pwm_channel_init(PWM, &PWM_pin_22);
		
		/*Enabling pwm channel after initializing everything correct*/
		pwm_channel_enable(PWM, PWM_CHANNEL_5);
}
 /* This method changes the duty cycle of PWM signal on pin 9. The duty cycle is limited between 0.8 ms and 2.2 ms  */
void leftWheel(uint32_t duty){
	if(duty<800){
		duty=800;
	}
	else if(duty>2200){
		duty=2200;
	}
	// Change the duty cycle of the PWM channel
	pwm_channel_update_duty(PWM, &PWM_pin_21, duty );
}

 /* This method changes the duty cycle of PWM signal on pin 8. The duty cycle is limited between 0.8 ms and 2.2 ms  */
void rightWheel(uint32_t duty){
	
	if(duty<800){
		duty=800;
	}
	else if(duty>2200){
		duty=2200;
	}
	// Change the duty cycle of the PWM channel
	pwm_channel_update_duty(PWM, &PWM_pin_22, duty );
	
}