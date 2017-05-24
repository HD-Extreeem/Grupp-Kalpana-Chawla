/*
 * task_regulator.c
 *
 * Author: D�sir�e J�nsson
 */ 

#include <asf.h>
#include "task_regulator.h"
#include "pwmInit.h"
#include <FreeRTOS.h>
#include <inttypes.h>
#include "task_comm.h"

//Variabler PID-regleringen men ocks� f�r veta var objektet befinner sig samt f�r uppdetara PWM signalen och plotta till Matlab
int16_t prevD=0;
uint16_t bv = 0;
uint16_t derivate=0;
uint16_t intergral=0;
double kp=0;
double Td=0;
double Ti=0;
double dT=0.1;
int16_t error=0;
int16_t new_PWM_value=0;
int distance=0;
int32_t sum;


void task_regulator( void *pvParameters)
{

   portTickType xLastWakeTime;
   const portTickType xTimeIncrement = 100;
	xLastWakeTime = xTaskGetTickCount(); //Initialise the xLastWakeTime variable with the current time. 

    while (1) {
    
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); // Wait for the next cycle.
		ioport_set_pin_dir (PIO_PC22_IDX, IOPORT_DIR_OUTPUT);
		ioport_set_pin_dir (PIO_PD7_IDX, IOPORT_DIR_INPUT);
		ioport_set_pin_level(PIO_PC22_IDX,0);
		ioport_set_pin_level(PIO_PD7_IDX,0);	
		analogInit(0);
		
		
		int invalue= analogRead(0);
		double intergral=0;
		error=(distance-bv);										//Distance = avst�ndet fr�n sensorn till var bollen befanns sig just d� p� banan. Bv det �nskade v�rdet.

		
		
		//PID utr�ckningen som anv�nds f�r regleringen.
		sum = (sum + prevD);
		intergral = (sum * (dT/Ti));
		derivate = ((error-prevD) * (Td / dT));
		new_PWM_value =(kp* (error + intergral + derivate));		
		prevD= error;

		if(new_PWM_value >100){
			new_PWM_value=100;										//Full spead on the regulator (fan) so the actual value can go upp the bv (wanted vaule)
		}
		if(new_PWM_value <0){
			new_PWM_value=0;										//Zero spead on the regulator(fan) so the actual value can go down the bv (wanted vaule)
		}
			update_dutyCycle(new_PWM_value); 
			updatematlab(new_PWM_value,distance,error,bv); 			//Send to matlab to draw plot
    }
}

