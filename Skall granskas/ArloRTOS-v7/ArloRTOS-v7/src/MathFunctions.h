/*
 * @file MathFunctions.h
 *
 * @breif This header file contains mathematical functions that can be used for navigation.
 * Angle unit is degree and distance unit is cm.
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
