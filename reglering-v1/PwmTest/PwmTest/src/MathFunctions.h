/*
 * MathFunctions.h
 *
 * Created: 2017-04-29 20:30:40
 *  Author: Yurdaer Dalkic
 *  
 *  Header file to MathFunctions.c
 */ 


#ifndef MATHFUNCTIONS_H_
#define MATHFUNCTIONS_H_

int radianToDegree (int radian);
int calculateDistance (int Xa, int Ya, int Xb, int Yb);
int calculateAzimuthAngle (int Xa, int Ya, int Xb, int Yb);
int calcluteRotationAngle (int Xa, int Ya, int Xb, int Yb, int Xc, int Yc);

#endif /* MATHFUNCTIONS_H_ */