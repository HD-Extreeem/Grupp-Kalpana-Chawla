/*
 * MathFunctions.h
 *
 * Created: 2017-04-29 20:30:40
 *  Author: Yurdaer
 */ 


#ifndef MATHFUNCTIONS_H_
#define MATHFUNCTIONS_H_

int radianToDegree (double radian);
int calculateDistance (int Xa, int Ya, int Xb, int Yb);
int calculateAzimuthAngle (int Xa, int Ya, int Xb, int Yb);
int calcluteRotationAngle (int koordinates [3][3]);

#endif /* MATHFUNCTIONS_H_ */