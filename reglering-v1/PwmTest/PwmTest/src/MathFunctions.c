/*
 * MathFunctions.c
 *
 * Created: 2017-04-29 20:30:18
 *  Author: Yurdaer Dalkic
 */ 


#include <math.h>
#include "MathFunctions.h"


int radianToDegree (double radian){
	
	int degree;
	degree = (int) (radian*(180/M_PI));
	return degree;
	
}

int calculateDistance (int Xa, int Ya, int Xb, int Yb){
	int distance;
	int deltaX = Xb - Xa;
	int deltaY = Yb - Ya;
	distance = (int) (sqrt((deltaX*deltaX) + (deltaY*deltaY)));
	return distance;
}

int calculateAzimuthAngle (int Xa, int Ya, int Xb, int Yb){
	int azimutAngle = 0;
	int deltaX = Xb-Xa;
	int deltaY = Yb-Ya;
	
	if ( (deltaX == 0) && (deltaY == 0) )
	{
		azimutAngle = 0;
	} 
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
	{
		if ( (deltaX > 0) && (deltaY > 0 ) )
		{
			azimutAngle = (int) (atan (deltaX/deltaY));
		} 
		else if ( (deltaX > 0) && (deltaY < 0 ) )
		{
			azimutAngle = (int) (atan (deltaX/deltaY) + 180);
		}
		else if ( (deltaX < 0) && (deltaY < 0 ) )
		{
			azimutAngle = (int) (atan (deltaX/deltaY) + 180);
		}
		else if ( (deltaX < 0) && (deltaY > 0 ) )
		{
			azimutAngle = (int) (atan (deltaX/deltaY) + 360);
		}
	}
	
	return azimutAngle;
}

int calcluteRotationAngle (int koordinates [3][3]){
	
	int AoB = calculateAzimuthAngle(koordinates [0][0], koordinates [0][1], koordinates [1][0], koordinates [1][1]);
	int BoC = calculateAzimuthAngle(koordinates [2][1], koordinates [2][2], koordinates [3][1], koordinates [3][2]);
	int rotationAngle = BoC-AoB;
	if (rotationAngle > 180)
	{
		rotationAngle = 360 - rotationAngle;
	}
	return rotationAngle;
}