/*
 * main.c
 *
 * Created: 2017-04-16 23:04:34
 *  Author: Yurdaer Dalkic
 */ 
#include "asf.h"
#include "PwmFunctions.h"
#include "InterruptStepCounter.h"
#include "consoleFunctions.h"
#include "PID_move.h"
#include "MathFunctions.h"

// extern uint16_t counter_1;
// extern uint16_t counter_2;
// uint16_t loop_counter;
// int setPoint=0;
// int m_value=0;
// double s_value=0;
// int e=0;
// double kp = 2;
// uint16_t speed = 1700;

int main(void)
{
	/* Initialize the SAM system */
	SystemInit();
	/*Initialize the board configurations*/
	board_init();
	configureConsole();
	configInterrupts();
	PWM_init();
	printf("Startar");
    delay_s(2);
//     rightWheel(1500);
//     leftWheel(1500);

	rotation(26,150);
	
	while (1)
	{
// 		moveTo(100,0);
// 		printf("Klar!\n");
// 		rightWheel(1500);
// 		leftWheel(1500);
// 		delay_s(2);
		printf("Start!");
		int rot = calcluteRotationAngle(0,0,0,100,100,300);
		printf("rot=%d\n",rot);
		rotation(rot,200);
		int dis = calculateDistance(0,100,100,300);
		printf("distance=%d\n",dis);
		moveTo(dis,1);
		delay_s(3);
        

// 		printf("0 :%d",(int) calculateDistance(0,0,0,0));
// 		printf("212 :%d",(int) calculateDistance(500,500,620,675));
// 		printf("167 :%d",(int) calculateDistance(500,500,425,650));
// 		printf("185 :%d",(int) calculateDistance(500,500,425,330)); 
// 		printf("0 :%d\n",(int)radianToDegree(0));		
// 		printf("30 :%d\n",(int) radianToDegree(0.5235987756));		
// 		printf("45 :%d\n",(int) radianToDegree(0.7853981634));		
// 		printf("60 :%d\n",(int) radianToDegree(1.0471975512));		
// 		printf("90 :%d\n",(int) radianToDegree(1.5707963268));		
// 		printf("130 :%d\n",(int) radianToDegree(2.2689280276));
// 		printf("180 :%d\n",(int)radianToDegree(3.1415926536));
// 		printf("270 :%d\n",(int)radianToDegree(4.7123889804));
// 		printf("360 :%d\n",(int)radianToDegree(6.2831853072));
//      printf("0 :%d\n",(int)calculateAzimuthAngle(0,0,0,0));
// 		printf("0 :%d\n",(int)calculateAzimuthAngle(0,0,0,10));
// 		printf("180 :%d\n",(int)calculateAzimuthAngle(0,10,0,0));
// 		printf("90 :%d\n",(int)calculateAzimuthAngle(0,0,10,0));
// 		printf("270 :%d\n",(int)calculateAzimuthAngle(10,0,0,0));
// 		printf("38 :%d\n",(int)calculateAzimuthAngle(3521,2563,3543,2591));
// 		printf("213 :%d\n",(int)calculateAzimuthAngle(600,700,500,550));
// 		printf("137 :%d\n",(int)calculateAzimuthAngle(600,700,710,580));
// 		printf("217 :%d\n",(int)calculateAzimuthAngle(725,1625,258,1015));
// 		printf("315 :%d\n",(int)calculateAzimuthAngle(300,100,100,300));
//         printf("0 :%d\n",(int)calcluteRotationAngle(0,0,0,0,0,0));
// 	    printf("0 :%d\n",(int)calcluteRotationAngle(100,0,200,0,300,0));
//         printf("0 :%d\n",(int)calcluteRotationAngle(0,100,0,200,0,300));
// 		printf("-90 :%d\n",(int)calcluteRotationAngle(0,0,100,0,100,100));
// 		printf("90 :%d\n",(int)calcluteRotationAngle(0,0,100,0,100,-100));
// 	    printf("-61 :%d\n",(int)calcluteRotationAngle(870,560,600,700,300,500));
// 	    printf("145 :%d\n",(int)calcluteRotationAngle(3800,4000,3870,4150,3920,3860));

 
// 		setPoint = 2*setPoint;
//  		m_value = (counter_2-counter_1);
//  		e = (setPoint - m_value);
//  		s_value = (kp*e);
//  		pwm_pin_21((speed+s_value));
//  		pwm_pin_22((speed-s_value));
//  		if (loop_counter == 100)
//  		{
//  			printf("counter1: %d\n",counter_1);
//  			printf("counter2: %d\n",counter_2);
// 			printf("loop_counter: %d\n",loop_counter);
// 		}
// 		loop_counter++;
// 		printf("\n e = %d",e);
// 		printf("\n s_value = %d",s_value);
// 		printf("\n m_value = %d",m_value);
// 		delay_us(46200*3);

	}
}
	

