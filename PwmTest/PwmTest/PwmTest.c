/*
 * PwmTest.c
 *
 * Created: 2017-04-14 11:27:48
 *  Author: Hadi Deknache & Yurdaer Dalkic
 */ 


#include "asf.h"

/*Defining pin for PWM*/
#define PWM_PIN IOPORT_CREATE_PIN(PIOC, 21)
pwm_channel_t pwm_channel_instance;
int main(void)
{
    /* Initialize the SAM system */
    SystemInit();
	/*Initialize the board configurations*/
	board_init();
	
	pio_configure_pin(PWM_PIN, PIO_TYPE_PIO_PERIPH_B);
	pmc_enable_periph_clk(ID_PWM);
	
	/*Disable pwm channel for appropriate configuration*/
	pwm_channel_disable(PWM,PWM_CHANNEL_4);
	
	/*Configurating PWM clock*/
	pwm_clock_t PWMDAC_clock_config = {
		.ul_clka = 1000000,
		.ul_clkb = 0,
		.ul_mck = sysclk_get_cpu_hz()
	};
	
	/*Initializing pwm*/
	pwm_init(PWM, &PWMDAC_clock_config);
	
	/*Pwm channel configuration such as channel, alignments, period, duty etc...*/
	pwm_channel_instance.channel = PWM_CHANNEL_4;
	pwm_channel_instance.ul_prescaler = PWM_CMR_CPRE_CLKA;
	pwm_channel_instance.polarity = PWM_HIGH;
	pwm_channel_instance.alignment = PWM_ALIGN_LEFT;
	pwm_channel_instance.ul_period = 2000;
	pwm_channel_instance.ul_duty = 0;
	
	/*Initializing channel after setting things up*/
	pwm_channel_init(PWM, &pwm_channel_instance);
	
	/*Enabling pwm channel after initializing everything correct*/
	pwm_channel_enable(PWM, PWM_CHANNEL_4);

    while (1) 
    {
        //Looping through speed for wheels and setting duty
		for (uint32_t i = 1000; i<=2000;i++)
		{
			pwm_channel_update_duty(PWM, &pwm_channel_instance, i );
			delay_us(5500);
		}
    }
}
