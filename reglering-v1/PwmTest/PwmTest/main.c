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
#include "confTimer.h"

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
	configureTC();
    delay_s(2);



	
	while (1)
	{

        printf("%d\n", tc_read_rc(TC0,0));
		delay_us(300);
       

// 		printf("Start!");
// 		int rot = calcluteRotationAngle(0,100,0,0,100,300);
// 		printf("rot=%d\n",rot);
// 		rotation(rot,130);
// 		delay_s(1);
// 		int dis = calculateDistance(0,0,100,300);
// 		printf("distance=%d\n",dis);
// 		moveTo(dis,1);
// 		delay_s(3);
        

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



	}
}
	

