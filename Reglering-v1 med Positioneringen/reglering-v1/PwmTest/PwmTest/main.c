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


struct coordinates{
	double presentX;
	double presentY;
	double lastX;
	double lastY;	
	double objectA[2];
	double objectB[2];
	double objectC[2];
	double objectD[2];
};


int main()
{
	struct coordinates coord;
	coord.presentX = 0;
	coord.presentY = 0;
	coord.lastX = 0;
	coord.lastY = -100;
	coord.objectA[0] = 137;
	coord.objectA[1] = 200;
	
	coord.objectB[0] = 56;
	coord.objectB[1] = 363;
	
	coord.objectC[0] = 452;
	coord.objectC[1] = 350;	
	
	coord.objectD[0] = 434;
	coord.objectD[1] = 36;
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
	delay_s(3);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectA[0],coord.objectA[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectA[0],coord.objectA[1]),1);
	delay_s(5);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
	coord.presentX = coord.objectA[0];
	coord.presentY = coord.objectA[1];
	delay_s(1);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectB[0],coord.objectB[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectB[0],coord.objectB[1]),1);
	delay_s(5);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
	coord.presentX = coord.objectB[0];
	coord.presentY = coord.objectB[1];
	delay_s(1);
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectC[0],coord.objectC[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectC[0],coord.objectC[1]),1);
	delay_s(5);
	coord.lastX = coord.presentX;
	coord.lastY = coord.presentY;
coord.presentX = coord.objectC[0];
coord.presentY = coord.objectC[1];
	rotation(calcluteRotationAngle(coord.lastX,coord.lastY,coord.presentX,coord.presentY,coord.objectD[0],coord.objectD[1]),130);
	moveTo(calculateDistance(coord.presentX,coord.presentY,coord.objectD[0],coord.objectD[1]),1);
	


}

