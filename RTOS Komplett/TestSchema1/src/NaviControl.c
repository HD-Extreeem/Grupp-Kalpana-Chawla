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
int distance;
int angle;


 int updateDistance(int X, int Y){
	distance = calculateDistance(presentPosition[0][0],presentPosition[0][1], X, Y);
    return distance;
 }

 int updateAngle(int X, int Y){
	 angle = calcluteRotationAngle(lastPosition[0][0], lastPosition[0][1], presentPosition[0][0],presentPosition[0][1], X, Y);
	 return angle;
 }
void updatePosition (void){
	lastPosition[0][0]=presentPosition[0][0];
	lastPosition[0][1]=presentPosition[0][1];
// 	presentPosition[0][0]=getX;
// 	presentPosition[0][1]=getY;
}
