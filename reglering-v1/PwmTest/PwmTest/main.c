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
extern int lastPosition [1][1]={0,-100};
extern int presentPosition [1][1]={0,0};


int main()
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
    delay_s(2);
moveTo(100,1);
delay_s(10);
rotation(calcluteRotationAngle(0,-100,0,0,100,100),130);
moveTo(calculateDistance(0,0,100,100),1);

delay_s(5);
rotation(calcluteRotationAngle(0,0,100,100,-100,300),130);
moveTo(calculateDistance(100,100,-100,300),1);

delay_s(5);

rotation(calcluteRotationAngle(100,100,-100,300,100,600),130);
moveTo(calculateDistance(-100,300,100,600),1);
lastPosition[0][0]= presentPosition[0][0];
lastPosition[0][1]= presentPosition[0][1];
presentPosition[0][0] = objects[2][0];
presentPosition[0][1] = objects[2][1];
delay_s(5);
// 	 delay_s(1);
// 	 getDirection();
// 	 driveTo(objects[0][0],objects[0][1]);
// 	 delay_s(10);
// 	 getDirection();
// 	 driveTo(objects[1][0],objects[1][1]);
// 	 delay_s(10);
// 	 getDirection();
// 	 driveTo(objects[2][0],objects[2][1]);




// int i =200;
//  	 while (1)
//  	 {
// 		  moveTo(i,1);
// 		  delay_s(3);
// 		  rotation(90,130);
// 		  delay_s(3);
//  	 }
// 	
}

