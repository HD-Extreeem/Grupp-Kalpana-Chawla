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


xSemaphoreHandle xBinarySemaphore=0; //(xstatic i jonas kod)

void task_StepCounter(void *pvParameters)
{
	bool value;
	
	printf("Startar Move task\r\n");
	
	vSemaphoreCreateBinary(xBinarySemaphore);
	
	
	uint32_t out = 0;
	
	while (1)
	{
		
		xSemaphoreTake(xBinarySemaphore, portMAX_DELAY);
		ioport_set_pin_level(PIO_PA22_IDX, HIGH);
		printf("TASK StepCounter INNE\r\n");
		ioport_set_pin_level(PIO_PA22_IDX, LOW);
		
		for(int i=0; i<100; i++){
			printf("i=%d",i);
			
		}
		
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
	
	printf("InterruptHandler\n");
}


void attach_interupt()
{
	pio_set_input(PIOA, PIO_PA6, PIO_PULLUP);
	pio_handler_set(PIOA, ID_PIOA, PIO_PA6, PIO_IT_RISE_EDGE, vMoveInterruptHandler); // when bara edge
	pio_enable_interrupt(PIOA, PIO_PA6); //möjligör interruptet
	NVIC_EnableIRQ(PIOA_IRQn);
	printf("attatch\n");
}
