/**
* @file main.c
*
* @brief This source file is the main source file, which configures needed settings for PWM, 
*  interrupts, pins and communication with Arlo robot platform
*  initialize task for communication and navigation
*		
* @Author Désirée Jönsson, Jonas Eiselt, Hadi Deknache & Yurdaer Dalkic
*
* @date 2017-04-20
*
*/

#include <asf.h>
#include <inttypes.h>
#include "../unity/unity.h"
#include "tests/test_pwm.h"
#include "Task_NaviControl.h"
#include "Task_GetCoordinates.h"
#include "Task_UnoComm.h"
#include "consoleFunctions.h"
#include "StepCounter_ISR.h"
#include "PwmFunctions.h"
#include "conf_board.h"
#include "arlo/Arlo.h"

int main (void)
{
	sysclk_init();
	/* Board initialization, for communication and printouts */
	board_init();
	//delay_s(3);
	/* Serial communication initialization, for printouts */
	configureConsole();
	/* Interrupt initialization */
	attach_interrupt();
	/* PWM initialize */
	PWM_init();
	delay_s(3);
	/* Arlo robot initialization */
	//arlo_init();
	coordinatesInit();
	/* Print info in terminal Window */
	printf("-- %s\n\r", BOARD_NAME);
	printf("-- Compiled: %s %s --\n\r", __DATE__, __TIME__);
	
	UnityBegin("../tests/test_pwm.c");
	RUN_TEST(pwm_enable,0);
	RUN_TEST(pwm_disable,10);
	RUN_TEST(pwm_duty,20);
	UnityEnd();
	
}
