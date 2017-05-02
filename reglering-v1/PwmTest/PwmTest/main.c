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

	while (1)
	{
		moveTo(500,0);
		printf("Klar!\n");
		rightWheel(1500);
		leftWheel(1500);
		delay_s(2);
// 		printf("0 %d\n",calculateDistance(0,0,0,0));
// 		
// 		printf("212 %d\n",calculateDistance(500,500,620,675));
// 		
// 		printf("167 %d\n",calculateDistance(500,500,425,650));
// 		
// 		printf("185 %d\n",calculateDistance(500,500,425,330));

// 		printf("0 %d\n",radianToDegree(0));		
// 		printf("30 %d\n",radianToDegree(0.52359));		
// 		printf("45 %d\n",radianToDegree(0.785398));		
// 		printf("60 %d\n",radianToDegree(1.0471));		
// 		printf("90 %d\n",radianToDegree(1.5707));		
// 		printf("130 %d\n",radianToDegree(2.26892));
// 		printf("180 %d\n",radianToDegree(3.14159));
// 		printf("270 %d\n",radianToDegree(4.71238));
// 		printf("360 %d\n",radianToDegree(6.265732));
		
// 		  rotation(30,100);
// 		  delay_s(3);
// 		  rotation(30,100);
// 		  delay_s(3);
	//	  rotation(-90);
		//setPoint = 2*setPoint;
// 		m_value = (counter_2-counter_1);
// 		e = (setPoint - m_value);
// 		s_value = (kp*e);
// 		pwm_pin_21((speed+s_value));
// 		pwm_pin_22((speed-s_value));
// 		if (loop_counter == 100)
// 		{
// 			printf("counter1: %d\n",counter_1);
// 			printf("counter2: %d\n",counter_2);
// 			printf("loop_counter: %d\n",loop_counter);
// 		}
		//loop_counter++;
		//printf("\n e = %d",e);
		//printf("\n s_value = %d",s_value);
		//printf("\n m_value = %d",m_value);
		//delay_us(46200*3);
		
	}
	
}
	

