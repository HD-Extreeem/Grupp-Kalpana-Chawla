/**
 * @file consoleFunctions.c
 *
 * @brief Code to be used if there is a need to print on the terminal window
 *
 * @author Kalpana
 *
 * @date 2017-04-11
 */

#include <stdio_serial.h>
#include <asf.h>
#include "conf_board.h"
#include "consoleFunctions.h"

void configureConsole(void)
/* Configuration for printouts, baudrates and paritytype  */
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
		.paritytype = CONF_UART_PARITY
	};
	/* Configure console UART, and initialize. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}