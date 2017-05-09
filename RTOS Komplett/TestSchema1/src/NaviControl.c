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
#include "Task_Navi.h"

extern int lastPosition [1][1];
extern int presentPosition [1][1];
int Distance;
int Angle;

 int updateDistance(int X, int Y){
	Distance = calculateDistance(presentPosition[0][0],presentPosition[0][1], X, Y);
    return Distance;
 }

 int updateAngle(int X, int Y){
	 Angle = calcluteRotationAngle(lastPosition[0][0], lastPosition[0][1], presentPosition[0][0],presentPosition[0][1], X, Y);
	 return Angle;
 }
void updatePosition (void){
	lastPosition[0][0]=presentPosition[0][0];
	lastPosition[0][1]=presentPosition[0][1];
// 	presentPosition[0][0]=getX;
// 	presentPosition[0][1]=getY;
}
