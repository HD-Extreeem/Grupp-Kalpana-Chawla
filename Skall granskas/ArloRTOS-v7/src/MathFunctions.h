/*
 * @file MathFunctions.h
 *
 * @breif Header file to MathFunctions.c
 * 
 * @author Yurdaer Dalkic
 *
 * @date 2017-04-29 20:30:40 
 *  
 */ 


#ifndef MATHFUNCTIONS_H_
#define MATHFUNCTIONS_H_

double radianToDegree (double radian);
double calculateDistance (double Xa, double Ya, double Xb, double Yb);
double calculateAzimuthAngle (double Xa, double Ya, double Xb, double Yb);
double calcluteRotationAngle (double Xa, double Ya, double Xb, double Yb, double Xc, double Yc);

#endif /* MATHFUNCTIONS_H_ */