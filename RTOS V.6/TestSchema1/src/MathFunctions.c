/*
* MathFunctions.c
*
* Created: 2017-04-29 20:30:18
*  Author: Yurdaer Dalkic
*
* This source file contains mathematical functions that can be used for navigation.
* Angle unit is degree and distance unit is cm.
*/


#include <math.h>
#include "MathFunctions.h"

/** Converts radian x in degree
@param radian the angle value in radian
return degree the angle value in degree
**/
double radianToDegree (double radian){
	double degree;
	degree =  radian*(180/M_PI);
	return degree;
}

/** Calculates the shortest distance between two coordinates A and B
@param Xa the x coordinate for A
@param Xb the x coordinate for B
@param Ya the y coordinate for A
@param Yb the y coordinate for B
return distance the distance between A and B
**/
double calculateDistance (double Xa, double Ya, double Xb, double Yb){
	double distance;
	double deltaX = Xb - Xa;
	double deltaY = Yb - Ya;
	distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));
	return distance;
}

/** Calculates the azimuth angle from position A to position B
@param Xa the x coordinate for A
@param Xb the x coordinate for B
@param Ya the y coordinate for A
@param Yb the y coordinate for B
return azimutAngle the azimuth angle from A to B
**/
double calculateAzimuthAngle (double Xa, double Ya, double Xb, double Yb){
	double azimutAngle = 0;
	double deltaX = Xb-Xa;
	double deltaY = Yb-Ya;
	
	// if A and B has same coordinates
	if ( (deltaX == 0) && (deltaY == 0) )
	{
		azimutAngle = 0;
	}
	// if A and B has same x coordinate
	else if (deltaX == 0)
	{
		if (deltaY>0)
		{
			azimutAngle = 0;
		}
		else
		{
			azimutAngle = 180;
		}
		
	}
	// if A and B has same y coordinate
	else if (deltaY == 0)
	{
		if (deltaX>0)
		{
			azimutAngle = 90;
		}
		else
		{
			azimutAngle = 270;
		}
		
	}
	else
	{   // angle is between 0 to 90 degree
		if ( (deltaX > 0) && (deltaY > 0 ) )
		{
			azimutAngle =  radianToDegree (atan (deltaX/deltaY));
		}
		// the angle is between 90 to 180 degree
		else if ( (deltaX > 0) && (deltaY < 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY))  + 180);
		}
		// the angle is between 180 to 270 degree
		else if ( (deltaX < 0) && (deltaY < 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY) ) + 180);
		}
		// the angle is between 270 to 360 degree
		else if ( (deltaX < 0) && (deltaY > 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY) ) + 360);
		}
	}
	
	return azimutAngle;
}

/** Calculates the rotation angle. The direction is from A to B and the target position is C.
A is start position, B is present position and C is target position.
@param Xa the x coordinate for A
@param Xb the x coordinate for B
@param Xc the x coordinate for C
@param Ya the y coordinate for A
@param Yb the x coordinate for B
@param Yc the y coordinate for C
return rotationAngle the rotation angle from A to target C
**/
double calcluteRotationAngle (double Xa, double Ya, double Xb, double Yb, double Xc, double Yc){
	
	double AoB = calculateAzimuthAngle(Xa, Ya, Xb, Yb);
	double BoC = calculateAzimuthAngle(Xb, Yb, Xc, Yc);
	double rotationAngle = BoC-AoB;
	if (rotationAngle > 180)
	{
		rotationAngle =  rotationAngle - 360;
	}
	if (rotationAngle < -180)
	{
		rotationAngle =  rotationAngle + 360;
	}
	return rotationAngle;
}