/*
 * main.c
 *
 * Created: 2017-04-16 23:04:34
 *  Author: Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"
#include "ConfigInterupts.h"
#include "consoleFunctions.h"
#include "PID_move.h"
#include "MathFunctions.h"
#include "uartConfig.h"
#include "NaviControl.h"

extern int objects[3][2];



void main()
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
    delay_s(2);

	 delay_s(1);
	 getDirection();
	 driveTo(objects[0][0],objects[0][1]);
	 delay_s(10);
	 getDirection();
	 driveTo(objects[1][0],objects[1][1]);
	 delay_s(10);
	 getDirection();
	 driveTo(objects[2][0],objects[2][1]);
	 while (1)
	 {
	 }
	
}

