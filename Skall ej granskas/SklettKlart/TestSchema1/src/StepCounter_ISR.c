/*
 * StepCounter_ISR.c
 *
 * Created: 2017-04-24 11:24:47
 *  Author: Desiree Jönsson och Jonas Eiselt
 */ 

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "StepCounter_ISR.h"


xSemaphoreHandle xBinarySemaphore=0;

void task_StepCounter(void *pvParameters)
{
	
	vSemaphoreCreateBinary(xBinarySemaphore);

	while (1)
	{	
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
			
		
		//Write code here
		
		vTaskDelay((200/portTICK_RATE_MS));
		xSemaphoreTake(xBinarySemaphore, 0);
		
	}
}

/*
 * Interrupt handler ( Mastering the Freal Time Kernel p. 200)
 */
void vMoveInterruptHandler(void)
{
	short xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xBinarySemaphore, &xHigherPriorityTaskWoken);
	portYIELD();
}

/*
 * Runs one time when program starts.
 */
void attach_interupt()
{
	pio_set_input(PIOA, PIO_PA6, PIO_PULLUP);
	pio_handler_set(PIOA, ID_PIOA, PIO_PA6, PIO_IT_RISE_EDGE, vMoveInterruptHandler); // Only on rising edge
	pio_enable_interrupt(PIOA, PIO_PA6);											//Enable interrupts. Pin A04
	NVIC_EnableIRQ(PIOA_IRQn);
}
