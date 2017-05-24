/*
 * uartConfig.c
 *
 * Created: 2017-05-05 12:52:06
 *  Author: Yurdaer
 */ 
#include <asf.h>
#include "uartConfig.h"

char str1[4];
char str2[4];
char str3[4];
char str4[4];

int x1 = 0;
int x2 = 0;
int x3 = 0; //irrelevant
int x4 = 0; //irrelevant
char rx[16];
	
	void stringToInt(int *p_variable, char *p_string) {
		int multiplier = 1;
		if(*p_string == '-') {
			multiplier = -1;
			*p_string++;
		}else {
			*p_variable = (*p_string++ - '0') * 1000;
		}
		if(*p_string == '-') {
			multiplier = -1;
			*p_string++;
			}else {
			*p_variable = (*p_string++ - '0') * 100;
		}
		if(*p_string == '-') {
			multiplier = -1;
			*p_string++;
			}else {
			*p_variable = (*p_string++ - '0') * 10;
		}
		if(*p_string == '-') {
			multiplier = -1;
			*p_string++;
			}else {
			*p_variable = (*p_string++ - '0');
		}
		*p_variable = *p_variable * multiplier;
	}
	
	
	double getX(void){
		str1[0] = rx[0];
		str1[1] = rx[1];
		str1[2] = rx[2];
		str1[3] = rx[3];
		
		str2[0] = rx[4];
		str2[1] = rx[5];
		str2[2] = rx[6];
		str2[3] = rx[7];
		
		str3[0] = rx[8];
		str3[1] = rx[9];
		str3[2] = rx[10];
		str3[3] = rx[11];
		
		str4[0] = rx[12];
		str4[1] = rx[13];
		str4[2] = rx[14];
		str4[3] = rx[15];
		
		stringToInt(&x1, str1);//x1
		stringToInt(&x2, str2);//y1
		stringToInt(&x3, str3);
		stringToInt(&x4, str4);
		return (double) x1;
		 
	}
	double getY(void){
		return (double) x2;
	}
	
	

	
