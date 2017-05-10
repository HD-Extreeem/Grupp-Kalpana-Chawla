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
extern int lastPosition [0][0]={0,-100};
extern int presentPosition [0][0]={0,0};

struct coordinates{
	int presentX;
	int presentY;
	int lastX;
	int lastY;	
	int objectA[2];
	int objectB[2];
	int objectC[2];
};


int main()
{
	struct coordinates coord;
	coord.presentX = 0;
	coord.presentY = 0;
	coord.lastX = 0;
	coord.lastY = -100;
	coord.objectA[0] = 100;
	coord.objectA[1] = 100;
	
	coord.objectB[0] = -100;
	coord.objectB[0] = 200;
	
	coord.objectC[0] = 100;
	coord.objectC[1] = 300;	
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
	
    delay_s(2);
	
	
	delay_s(3);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectA[0],coord.objectA[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectA[0],coord.objectA[1]),1);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
	coord.presentX = coord.objectA[0];
	coord.presentY = coord.objectA[1];
	delay_s(5);
	//rotation(calcluteRotationAngle(0,0,100,100,-100,300),130);
	//moveTo(calculateDistance(100,100,-100,300),1);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectB[0],coord.objectB[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectB[0],coord.objectB[1]),1);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
	coord.presentX = coord.objectB[0];
	coord.presentY = coord.objectB[1];
	delay_s(5);

// 	rotation(calcluteRotationAngle(100,100,-100,300,100,600),130);
// 	moveTo(calculateDistance(-100,300,100,600),1);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectC[0],coord.objectC[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectC[0],coord.objectC[1]),1);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
	coord.presentX = coord.objectC[0];
	coord.presentY = coord.objectC[1];
	
// 	lastPosition[0][0]= presentPosition[0][0];
// 	lastPosition[0][1]= presentPosition[0][1];
// 	presentPosition[0][0] = objects[2][0];
// 	presentPosition[0][1] = objects[2][1];
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

