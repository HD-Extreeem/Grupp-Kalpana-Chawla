/*
 *	@file test_pwm.c
 *
 *	@brief This source file contains tests used to ensure that methods used for pwm is working, uses unitytests
 *  @author Hadi Deknache
 *	@date 2017-04
 */ 
#include "asf.h"
#include "unity/unity.h"
#include "pwmfunctions.h"
#include "test_pwm.h"

/* This method is used to check if pwm can be enabled by checking set bit
*
*/
void pwm_enable(void){
	//Assert that pwm channel 4 & 5 is activated after PwmInit() in main.c
	TEST_ASSERT_EQUAL(((1<<PWM_CHANNEL_5)|(1<<PWM_CHANNEL_4)),pwm_channel_get_status(PWM));
}
/* This method used to check if pwm channel can be disabled
*
*/
void pwm_disable(void){
	//Disables the channels and check if 
	pwm_channel_disable(PWM,PWM_CHANNEL_4);
	pwm_channel_disable(PWM,PWM_CHANNEL_5);
	//delay 1s to let pwm disable channels
	delay_s(1);
	//Assert that pwm is disabled
	TEST_ASSERT_EQUAL(0,pwm_channel_get_status(PWM));
}

/* This method checks if duty can be set from methods for wheels
*
*/
void pwm_duty(void){
	//Asserts that duty is set when updating duty cycle, should return 0 if set
	TEST_ASSERT_EQUAL(0,rightWheel(1700));
	TEST_ASSERT_EQUAL(0,leftWheel(1700));
}
