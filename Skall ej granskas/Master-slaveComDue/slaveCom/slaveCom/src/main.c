/**
 * \file
 *
 * \brief TWI SLAVE Example for SAM.
 *
 * Copyright (c) 2011-2016 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

/**
 * \mainpage TWI SLAVE Example
 *
 * \section intro Introduction
 *
 * The application demonstrates how to use use the SAM TWI peripheral in slave mode.
 *
 * \section Requirements
 *
 * This package can be used with SAM evaluation kits.
 *
 * In addition, another device will be needed to act as the TWI master. The
 * twi_eeprom_example can be used for that, in which case a second kit
 * supported by that project is needed.
 * -# Connect TWD (SDA) for the 2 boards.
 * -# Connect TWCK (SCL) for the 2 boards.
 * -# Connect GND for the 2 boards.
 * -# Make sure there is a pull up resistor on TWD and TWCK.
 *
 * \section files Main files:
 *  - twi.c SAM Two-Wire Interface driver implementation.
 *  - twi.h SAM Two-Wire Interface driver definitions.
 *  - twi_slave_example.c Example application.
 *
 * \section exampledescription Description of the Example
 * After launching the program, the device will act as a simple TWI-enabled
 * serial memory containing 512 bytes. This enables this project to be used
 * with the twi_eeprom_example project as the master.
 *
 * To write in the memory, the TWI master must address the device first, then
 * send two bytes containing the memory address to access. Additional bytes are
 * treated as the data to write.
 *
 * Reading is done in the same fashion, except that after receiving the memory
 * address, the device will start outputting data until a STOP condition is
 * sent by the master.
 * The default address for the TWI slave is fixed to 0x40. If the board has a TWI
 * component with this address, you can change the define AT24C_ADDRESS in
 * twi_eeprom_example.c of twi_eeprom_example project, and the define
 * SLAVE_ADDRESS in twi_slave_example.c of twi_slave_exmaple project.
 *
 * \section compinfo Compilation Info
 * This software was written for the GNU GCC and IAR EWARM.
 * Other compilers may or may not work.
 *
 * \section contactinfo Contact Information
 * For further information, visit
 * <A href="http://www.atmel.com/">Atmel</A>.\n
 * Support and FAQ: http://www.atmel.com/design-support/
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include "asf.h"
#include "conf_twi_slave_example.h"

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** Device address of slave */
#define SLAVE_ADDRESS       0x50
/** Memory size in bytes */
#define MEMORY_SIZE         512

#define STRING_EOL    "\r"
#define STRING_HEADER "--TWI SLAVE Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL
/** The slave device instance*/
	
// uint8_t navigationRead[8] = {0};
// uint16_t coordinates[8]={0};
	
// for (int h=0;h<=8;h++)
// {
// //L�s in data
// }
// for (int h=0;h<=8;h+=2)
// {
// 	coordinates[h]=(navigationRead[h]<<8)|(navigationRead[h+1])
// }


uint8_t data_received[10] = {0};


twi_packet_t packet_received = {
	.addr[0]      = 0xaa, // TWI slave memory address data MSB
	//.addr[1]      = 0,      // TWI slave memory address data LSB
	.addr_length  = sizeof (uint16_t),    // TWI slave memory address data size
	.buffer       = data_received,        // transfer data destination buffer
	.length       = sizeof(data_received) ,  // transfer data size (bytes)
	.chip         = SLAVE_ADDRESS
};
typedef struct _slave_device_t {
	/** PageAddress of the slave device*/
	uint16_t us_page_address;
	/** Offset of the memory access*/
	uint16_t us_offset_memory;
	/** Read address of the request*/
	uint8_t uc_acquire_address;
	/** Memory buffer*/
	uint8_t uc_memory[MEMORY_SIZE];
} slave_device_t;

slave_device_t emulate_driver;

void BOARD_TWI_Handler(void)
{
	printf("Fick meddelande fr�n master\n");
	uint32_t status;

	status = twi_get_interrupt_status(BOARD_BASE_TWI_SLAVE);
	//printf("Status: %u \n",status);
// 	uint32_t count = twi_slave_read(BOARD_BASE_TWI_SLAVE,&packet_received);
// 	printf("Count:%u\n\r",count);
// 	for (uint32_t i = 0 ; i <= 512 ;i++){
// 		printf("Tar emot: %u\n", data_received[i]);
// 		printf("Char = %c\n\r", data_received[i]);
// 	}//	twi_slave_write(BOARD_BASE_TWI_SLAVE,&data_received
// 	twi_disable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_IDR_SVACC);
// 	twi_enable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_IER_RXRDY | TWI_IER_GACC
// 	 				| TWI_IER_NACK | TWI_IER_EOSACC | TWI_IER_SCL_WS);
//  	while (twi_slave_read(TWI1, &packet_received) != TWI_SUCCESS);
// 	for (uint32_t i = 0 ; i <= MEMORY_SIZE; i++) {
//		printf("%02X\n",data_received[i]);
//	}
	
	
	if (((status & TWI_SR_SVACC) == TWI_SR_SVACC)
			&& (emulate_driver.uc_acquire_address == 0)) {
		twi_disable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_IDR_SVACC);
		twi_enable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_IER_RXRDY | TWI_IER_GACC
				| TWI_IER_NACK | TWI_IER_EOSACC | TWI_IER_SCL_WS);
		emulate_driver.uc_acquire_address++;
		emulate_driver.us_page_address = 0;
		emulate_driver.us_offset_memory = 0;
	}
	

	if ((status & TWI_SR_GACC) == TWI_SR_GACC) {
		puts("General Call Treatment\n\r");
		puts("not treated");
	}

	if (((status & TWI_SR_SVACC) == TWI_SR_SVACC) && ((status & TWI_SR_GACC) == 0)
			&& ((status & TWI_SR_RXRDY) == TWI_SR_RXRDY)) {

		if (emulate_driver.uc_acquire_address == 1) {
			/* Acquire MSB address */
			emulate_driver.us_page_address =
					(twi_read_byte(BOARD_BASE_TWI_SLAVE) & 0xFF) << 8;
			emulate_driver.uc_acquire_address++;
		} else {
			if (emulate_driver.uc_acquire_address == 2) {
				/* Acquire LSB address */
				emulate_driver.us_page_address |=
						(twi_read_byte(BOARD_BASE_TWI_SLAVE) & 0xFF);
				emulate_driver.uc_acquire_address++;
			} else {
				/* Read one byte of data from master to slave device */
				emulate_driver.uc_memory[emulate_driver.us_page_address +
					emulate_driver.us_offset_memory] =
						(twi_read_byte(BOARD_BASE_TWI_SLAVE) & 0xFF);

				emulate_driver.us_offset_memory++;
			}
		}
	} else {
		if (((status & TWI_SR_TXRDY) == TWI_SR_TXRDY)
				&& ((status & TWI_SR_TXCOMP) == TWI_SR_TXCOMP)
				&& ((status & TWI_SR_EOSACC) == TWI_SR_EOSACC)) {
			//printf("else if");
			/* End of transfer, end of slave access */

			emulate_driver.us_offset_memory = 0;
			emulate_driver.uc_acquire_address = 0;
			emulate_driver.us_page_address = 0;
			twi_enable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_SR_SVACC);
			twi_disable_interrupt(BOARD_BASE_TWI_SLAVE,
					TWI_IDR_RXRDY | TWI_IDR_GACC |
					TWI_IDR_NACK | TWI_IDR_EOSACC | TWI_IDR_SCL_WS);
		} else {
			//printf("Else,else\n");
			if (((status & TWI_SR_SVACC) == TWI_SR_SVACC)
					&& ((status & TWI_SR_GACC) == 0)
					&& (emulate_driver.uc_acquire_address == 3)
					&& ((status & TWI_SR_SVREAD) == TWI_SR_SVREAD)
					&& ((status & TWI_SR_TXRDY) == TWI_SR_TXRDY)
					&& ((status & TWI_SR_NACK) == 0)) {
				/* Write one byte of data from slave to master device */
				
				twi_write_byte(BOARD_BASE_TWI_SLAVE,emulate_driver.uc_memory[emulate_driver.us_page_address+ emulate_driver.us_offset_memory]);
				printf("%02X\n",(emulate_driver.uc_memory[emulate_driver.us_page_address+ emulate_driver.us_offset_memory]));
				emulate_driver.us_offset_memory++;
				printf("Char= %c\n\r", (emulate_driver.uc_memory[emulate_driver.us_page_address+ emulate_driver.us_offset_memory]));
				printf("Offsetmem = %c\n\r",emulate_driver.uc_memory[emulate_driver.us_offset_memory]);
				printf("us page adress = %c\n\r",emulate_driver.uc_memory[emulate_driver.us_page_address]);
			}
		}
	}
	
	
}

/**
 *  \brief Configure the Console UART.
 */
static void configure_console(void)
{
	const usart_serial_options_t uart_serial_options = {
		.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
		.charlength = CONF_UART_CHAR_LENGTH,
#endif
		.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
		.stopbits = CONF_UART_STOP_BITS,
#endif
	};

	/* Configure console UART. */
	sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
	stdio_serial_init(CONF_UART, &uart_serial_options);
}

/**
 * \brief Application entry point for TWI Slave example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t i;

	/* Initialize the SAM system */
	sysclk_init();

	/* Initialize the board */
	board_init();

	/* Initialize the console UART */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);
	printf("Startar nu!!!");
#if (SAMG55)
	/* Enable the peripheral and set TWI mode. */
	flexcom_enable(BOARD_FLEXCOM_TWI);
	flexcom_set_opmode(BOARD_FLEXCOM_TWI, FLEXCOM_TWI);
#else
	/* Enable the peripheral clock for TWI */
	pmc_enable_periph_clk(BOARD_ID_TWI_SLAVE);
#endif

	for (i = 0; i < MEMORY_SIZE; i++) {
		emulate_driver.uc_memory[i] = 0;
	}
	emulate_driver.us_offset_memory = 0;
	emulate_driver.uc_acquire_address = 0;
	emulate_driver.us_page_address = 0;

	/* Configure TWI as slave */
	puts("\n-I- Configuring the TWI in slave mode\n\r");
	
	twi_slave_init(BOARD_BASE_TWI_SLAVE, SLAVE_ADDRESS);
	/* Clear receipt buffer */
	twi_read_byte(BOARD_BASE_TWI_SLAVE);
	twi_enable_slave_mode(BOARD_BASE_TWI_SLAVE);
	/* Configure TWI interrupts */
	NVIC_DisableIRQ(BOARD_TWI_IRQn);
	NVIC_ClearPendingIRQ(BOARD_TWI_IRQn);
	NVIC_SetPriority(BOARD_TWI_IRQn, 0);
	NVIC_EnableIRQ(BOARD_TWI_IRQn);
	twi_enable_interrupt(BOARD_BASE_TWI_SLAVE, TWI_SR_SVACC);

	while (1) {
	}
}

/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond


