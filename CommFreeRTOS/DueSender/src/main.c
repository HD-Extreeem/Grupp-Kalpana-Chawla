/**
* main.c
* I2C communication between Arduino Due and Arduino Uno. Arduino Due sends data and then receives.
*
* Author: Jonas Eiselt
* Created 2017-05-05
*
* Note: modified ASF-example
*/

#include <asf.h>
#include <stdio_serial.h>
#include <inttypes.h>
#include "conf_board.h"
#include "twi.h"
#include "comm/TWIComm.h"

#define STACK_SIZE (1024/sizeof(portSTACK_TYPE))

void vLEDTask(void *pvParameters);
void vCommTask(void *pvParameters);

static void configure_console(void);

int main(void)
{
	sysclk_init();
	board_init();

	configure_console();
	
	ioport_init();
	
	/* Creates task with blinking LED */
	if (xTaskCreate(vLEDTask, (const signed char * const) "Blink task", STACK_SIZE, NULL, 2, NULL) != pdPASS)
	{
		printf("Failed to create LED task\r\n");
	}
	
	/* Creates task which communicates with Uno */
	if (xTaskCreate(vCommTask, (const signed char * const) "Comm task", STACK_SIZE, NULL, 1, NULL) != pdPASS)
	{
		printf("Failed to create Comm task\r\n");
	}

	/* Start the FreeRTOS scheduler running all tasks indefinitely */
	vTaskStartScheduler();
	
	/* The program should only end up here if there isn't enough memory to create the idle task */
	while (1);
}

/**
*  This tasks makes the on-board LED blink every second
*/
void vLEDTask(void *pvParameters)
{
	ioport_set_pin_dir(LED0_GPIO, IOPORT_DIR_OUTPUT);
	
	printf("Entering LED task\r\n");
	
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */
	
	static uint32_t toggle = 0;
	while (1)
	{
		toggle = !toggle;
		if (toggle)
		{
			ioport_set_pin_level(LED0_GPIO, true);
		}
		else
		{
			ioport_set_pin_level(LED0_GPIO, false);
		}
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); /* Wait for the next cycle. */
	}
	/* Should never go there */
	vTaskDelete(NULL);
}

void vCommTask(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 100;
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */

	twi_init();
		
	while (1)
	{
		printf("Entering Comm task\r\n");
		
		twi_send_packet(0x20);
		twi_request_packet();
		
		check_data();
		twi_request_packet();
	
		printf("End of Comm task\r\n");
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); /* Wait for the next cycle. */
	}
	vTaskDelete(NULL);
}

static void configure_console()
{
	const usart_serial_options_t usart_serial_options =
	{
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &usart_serial_options);
}