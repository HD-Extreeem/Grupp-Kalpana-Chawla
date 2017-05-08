/*
 * StepCounter_ISR.c
 *
 * Created: 2017-04-24 11:24:47
 *  Author: Desiree J�nsson och Jonas Eiselt
 */

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "StepCounter_ISR.h"
#include "consoleFunctions.h"


// Counter for Arlo Robot encoders for right and left wheel
 uint16_t counter_1 = 0;
 uint16_t counter_2 = 0;
 uint8_t c_counter = 0;
 char rx[16];
 bool c1Loop = true;
 bool c2Loop = true;

xSemaphoreHandle xBinarySemaphoreRight = 0;
xSemaphoreHandle xBinarySemaphoreLeft = 0;

void task_StepCounterRight(void *pvParameters)
{

	vSemaphoreCreateBinary(xBinarySemaphoreRight);

	while (1)
	{
		xSemaphoreTake(xBinarySemaphoreRight, portMAX_DELAY);


		// Checks if pin 51 is high
		if (pio_get(PIOB, PIO_TYPE_PIO_INPUT, PIO_PB14)){
		//Increase the counter value
				counter_1++;
				c1Loop = true;
				//printf("\n c2 = %d",counter_2);
		}
		vTaskDelay((50/portTICK_RATE_MS));
		xSemaphoreTake(xBinarySemaphoreRight, 0);

	}
}

void task_StepCounterLeft(void *pvParameters)
{

	vSemaphoreCreateBinary(xBinarySemaphoreLeft);

	while (1)
	{
		xSemaphoreTake(xBinarySemaphoreLeft, portMAX_DELAY);

		// Check if pin 53 is high
		if (pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC12)){
			//increase the counter value
			counter_2++;
			c2Loop = true;
			//printf("\n c1= %d",counter_1);
		}

		vTaskDelay((50/portTICK_RATE_MS));
		xSemaphoreTake(xBinarySemaphoreLeft, 0);

	}
}

/*
 * Interrupt handler ( Mastering the Freal Time Kernel p. 200)
 */
void pin12_edge_handler(void)
{
	long xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xBinarySemaphoreRight, &xHigherPriorityTaskWoken);
	portYIELD();
}

void pin14_edge_handler(void)
{
	long xHigherPriorityTaskWoken = pdFALSE;
	xSemaphoreGiveFromISR(xBinarySemaphoreLeft, &xHigherPriorityTaskWoken);
	portYIELD();
}

/*
 * Runs one time when program starts.
 */
void attach_interupt(void)
{

		//Enable the module clock to the PIOB peripheral
		pmc_enable_periph_clk(ID_PIOB);
		//Enable the module clock to the PIOC peripheral
		pmc_enable_periph_clk(ID_PIOC);
		//Set pin 12 direction on PIOC as input, with pullup
		pio_set_input(PIOC,PIO_PC12,PIO_PULLUP);
		//Set pin 14 direction on PIOB as input, with pullup
		pio_set_input(PIOB,PIO_PB14,PIO_PULLUP);
		//Configure the input pin 12 interrupt mode and handler pin51
		pio_handler_set(PIOC, ID_PIOC, PIO_PC12, PIO_IT_RISE_EDGE, pin12_edge_handler);
		//Configure the input pin 14 interrupt mode and handler pin53
		pio_handler_set(PIOB, ID_PIOB, PIO_PB14, PIO_IT_RISE_EDGE, pin14_edge_handler);
		//Enable the interrupt for the configured input pin 12
		pio_enable_interrupt(PIOC,PIO_PC12);
		//Enable the interrupt for the configured input pin 12
		pio_enable_interrupt(PIOB,PIO_PB14);
		//Enable interrupt handling from the PIOC module:
		NVIC_EnableIRQ(PIOC_IRQn);
		//Enable interrupt handling from the PIOB module:
		NVIC_EnableIRQ(PIOB_IRQn);

	// 	NVIC_EnableIRQ((IRQn_Type) ID_USART1);
	// 	usart_enable_interrupt(CONF_UART, UART_IER_RXRDY);

}

// void USART1_Handler() {
// 	CONF_UART->US_CR |= (1 << US_CR_RSTRX);
// 	rx[c_counter++] = CONF_UART->US_RHR & US_RHR_RXCHR_Msk;
// 	if (c_counter > 15)
// 	{
// 		c_counter = 0;
// 	}
// }

void reset_Counter(void){
 counter_1=0;
 counter_2=0;
}
