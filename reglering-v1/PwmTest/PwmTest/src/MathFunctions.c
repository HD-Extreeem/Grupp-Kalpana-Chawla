/*
 * MathFunctions.c
 *
 * Created: 2017-04-29 20:30:18
 *  Author: Yurdaer Dalkic
 */ 


#include <math.h>
#include "MathFunctions.h"


uint16_t radianToDegree (double radian){
	
	uint16_t degree;
	degree = (uint16_t) (radian*(180/M_PI));
	return degree;
	
}

uint16_t calculateDistance (int Xa, int Ya, int Xb, int Yb){
	uint16_t distance;
	distance = (uint16_t) (sqrt((Xb-Xa)+(Yb-Ya)));
	return distance;
}

uint16_t calculateAzimuthAngle (int Xa, int Ya, int Xb, int Yb){
	uint16_t azimutAngle;
	uint16_t deltaX = Xb-Xa;
	uint16_t deltaY = Yb-Ya;
	
	if ((deltaX == 0)&&(deltaY == 0))
	{
		azimutAngle = 0;
	} 
	else if (deltaX == 0)
	{
		if (deltaY>0)
		{
			azimutAngle = 180;
		} 
		else
		{
			
		}
		
	}
	
	azimutAngle = (uint16_t) ();
	return azimutAngle;
}

uint16_t calcluteRotationAngle (int koordinates [3][3]){
	
}