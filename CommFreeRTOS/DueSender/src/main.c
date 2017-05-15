/**
* main.c
* I2C communication between Arduino Due and Arduino Uno. Arduino Due sends data and then receives.
*
* Author: Jonas Eiselt
* Created 2017-05-05
*/

#include <asf.h>
#include <stdio_serial.h>
#include <inttypes.h>
#include "conf_board.h"
#include "comm/TWIComm.h"
#include "arlo/Arlo.h"

#define STACK_SIZE (1024/sizeof(portSTACK_TYPE))

void vLEDTask(void *pvParameters);
void vUnoCommTask(void *pvParameters);
void vDueCommTask(void *pvParameters);

static void configure_console(void);

int main(void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();

	/* For debugging purposes only */
	configure_console();

	/* Arlo robot initialization */
	arlo_init();
	
	
	
	/*twi_send_packet(tx_arm_buffer, SLAVE_ADDR_ARM);*/

	/*tx_nav_buffer[0] = 0x20;
	tx_nav_buffer[1] = 0x21;
	tx_nav_buffer[2] = 0x22;
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);
	
	tx_nav_buffer[0] = 0x22;
	tx_nav_buffer[1] = 0x21;
	tx_nav_buffer[2] = 0x22;
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);*/
	
// 	/* Creates task which communicates with Due */
// 	if (xTaskCreate(vDueCommTask, (const signed char * const) "Due Comm task", STACK_SIZE, NULL, 1, NULL) != pdPASS)
// 	{
// 		printf("Failed to create Due Comm task\r\n");
// 	}
// 	
// 	/* Creates task which communicates with Uno */
// 	if (xTaskCreate(vUnoCommTask, (const signed char * const) "Uno Comm task", STACK_SIZE, NULL, 2, NULL) != pdPASS)
// 	{
// 		printf("Failed to create Uno Comm task\r\n");
// 	}
// 	
// 	/* Creates task with blinking LED */
// 	if (xTaskCreate(vLEDTask, (const signed char * const) "Blink task", STACK_SIZE, NULL, 3, NULL) != pdPASS)
// 	{
// 		printf("Failed to create LED task\r\n");
// 	}
// 	
// 	/* Start the FreeRTOS scheduler running all tasks indefinitely */
// 	vTaskStartScheduler();
// 	
// 	/* The program should only end up here if there isn't enough memory to create the idle task */
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

void vUnoCommTask(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */
	
	while (1)
	{
		printf("Entering UnoComm task\r\n");
		
		/*twi_send_packet(0x20, SLAVE_ADDR_ARM);
		twi_request_packet(SLAVE_ADDR_ARM);
		
		twi_check_data(SLAVE_ADDR_ARM);
		twi_request_packet(SLAVE_ADDR_ARM);*/
		
		printf("End of UnoComm task\r\n");
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement); /* Wait for the next cycle. */
	}
	vTaskDelete(NULL);
}

void vDueCommTask(void *pvParameters)
{
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 1000;
	
	xLastWakeTime = xTaskGetTickCount(); /* Initialize the xLastWakeTime variable with the current time. */

	while (1)
	{
		printf("Entering DueComm task\r\n");
		
		/*twi_send_packet(0x21, SLAVE_ADDR_NAV);
		twi_request_packet(SLAVE_ADDR_NAV);
		
		twi_check_data(SLAVE_ADDR_NAV);
		twi_request_packet(SLAVE_ADDR_NAV);*/
		
		printf("End of DueComm task\r\n");
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