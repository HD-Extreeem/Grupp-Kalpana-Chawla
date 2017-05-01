/*
 * MathFunctions.c
 *
 * Created: 2017-04-29 20:30:18
 *  Author: Yurdaer Dalkic
 */ 


#include <math.h>
#include "MathFunctions.h"


double radianToDegree (double radian){
	double degree;
	degree = radian*(180/M_PI);
	return degree;
}

double calculateDistance (double Xa, double Ya, double Xb, double Yb){
	double distance;
	double deltaX = Xb - Xa;
	double deltaY = Yb - Ya;
	distance = sqrt((deltaX*deltaX) + (deltaY*deltaY));
	return distance;
}

double calculateAzimuthAngle (double Xa, double Ya, double Xb, double Yb){
	double azimutAngle = 0;
	double deltaX = Xb-Xa;
	double deltaY = Yb-Ya;
	
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
			azimutAngle =  radianToDegree (atan (deltaX/deltaY));
		} 
		else if ( (deltaX > 0) && (deltaY < 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY))  + 180);
		}
		else if ( (deltaX < 0) && (deltaY < 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY) ) + 180);
		}
		else if ( (deltaX < 0) && (deltaY > 0 ) )
		{
			azimutAngle = (radianToDegree (atan (deltaX/deltaY) ) + 360);
		}
	}
	
	return azimutAngle;
}

double calcluteRotationAngle (double Xa, double Ya, double Xb, double Yb, double Xc, double Yc){
	
	double AoB = calculateAzimuthAngle(Xa, Ya, Xb, Yb);
	double BoC = calculateAzimuthAngle(Xb, Yb, Xc, Yc);
	double rotationAngle = BoC-AoB;
	if (rotationAngle > 180)
	{
		rotationAngle =  rotationAngle - 360;
	}

	return rotationAngle;
}