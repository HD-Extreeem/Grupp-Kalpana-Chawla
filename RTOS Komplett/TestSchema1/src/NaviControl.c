/*
 * naviControl.c
 *
 * Created: 2017-05-05 08:55:48
 *  Author: Yurdaer Dalkic
 */ 
#include <asf.h>
#include "NaviControl.h"
#include "MathFunctions.h"
#include "StepCounter_ISR.h"
#include "PID_move.h"

int objects[3][2] = {{100,100},{-100,200},{100,400}};
int lastPosition [1][1];
int presentPosition [1][1];



 void driveTo(int X, int Y){
	double distance = calculateDistance(presentPosition[0][0],presentPosition[0][1], X, Y);
	double rotationAngle ;
//	while (distance >= 10)
//	{
		rotationAngle = calcluteRotationAngle(lastPosition [0][0],lastPosition [0][1],presentPosition [0][0],presentPosition [0][1], X, Y);
		rotation(rotationAngle,130);
		moveTo(distance,1);
		delay_ms(1000);
// 		lastPosition[0][0] = presentPosition[0][0];
// 		lastPosition[0][1] = presentPosition[0][1];
// 		presentPosition [0][0]= getX();
// 		presentPosition [0][1]= getY();
// 		distance = calculateDistance(presentPosition[0][0],presentPosition[0][1], X, Y);
//	}
}

//  void getDirection(void){
// 	lastPosition [0][0]= getX();
// 	lastPosition [0][1]= getY();
//     moveTo(50,1);
// 	delay_s(1);
// 	presentPosition [0][0]= getX();
// 	presentPosition [0][1]= getY();
// }
