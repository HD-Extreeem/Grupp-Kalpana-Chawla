/**
* Author: Jonas Eiselt
* Created: 2017-04-29
*
* Further reading: http://asf.atmel.com/docs/latest/sam3x/html/group__sam__drivers__twi__group.html
*/

#include <asf.h>
#include <inttypes.h>
#include <twi.h>
#include "consoleFunctions.h"

#define TWI_CLK 10000
#define TWI_MASTER_ADDR 8
#define TWI_SLAVE_ADDR 9

#define DATA_LENGTH 8

static uint8_t tx_buffer[] = {0x05, 0x05, 0x05, 0x05, 0x05,	0x05, 0x05,0x05};
static uint8_t rx_buffer[DATA_LENGTH] = {0x00};

void configure_twi(void);
uint8_t pio_conf_pin_to_peripheral(pio_reg_t *port, uint32_t periph, uint8_t pin_number);

int main (void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();
	
	configure_console();
	configure_twi();
	
	while (1);
	
	/* Configure packet to be sent */
	twi_packet_t tx_packet;
	tx_packet.chip = TWI_SLAVE_ADDR;
	tx_packet.addr_length = 1;
	tx_packet.buffer = (void*) tx_buffer;
	tx_packet.length = DATA_LENGTH;
	
	/* Configure packet to be received */
	twi_packet_t rx_packet;
	rx_packet.chip = TWI_SLAVE_ADDR;
	rx_packet.addr_length = 1;
	rx_packet.buffer = (void*) rx_buffer;
	rx_packet.length = DATA_LENGTH;
	
	/* Check if chip answers i2c address */
	/*if (twi_probe(TWI0, TWI_SLAVE_ADDR) != TWI_SUCCESS)
	{
	printf("Failed to get a response from chip\r\n");
	}
	else
	{
	printf("Response gotten from chip\r\n");
	}*/
	
	/*  Alt 1. Send data to slave device */
	while (1)
	{
		for (int i = 0; i < DATA_LENGTH; i++)
		{
			twi_write_byte(TWI0, tx_buffer[i]);
			printf("Sent %d\r\n", tx_buffer[i]);
		}
	}
	
	/* Alt 2. Send data to slave device */
	if (twi_master_write(TWI0, &tx_packet) != TWI_SUCCESS)
	{
		printf("Failed to send data\r\n");
	}
	else
	{
		printf("Data sent\r\n");
	}
	
	/* Receive data from slave device */
	if (twi_master_read(TWI0, &rx_packet) == TWI_SUCCESS)
	{
		for (uint32_t i = 0; i < DATA_LENGTH; i++)
		{
			printf("Received: %d\n\r", (uint8_t*) tx_buffer[i]);
		}
	}
	else
	{
		printf("Didn't receive any data\r\n");
	}
	
	while (1);
}

void configure_twi(void)
{
	/* Enable TWI peripheral clock */
	pmc_enable_periph_clk(ID_TWI0);
	//sysclk_enable_peripheral_clock(ID_TWI0);
	pio_set_peripheral(PIOA, PIO_PERIPH_A, 17);	// SDA
	pio_set_peripheral(PIOA, PIO_PERIPH_A, 18);	// SCL
	
	/* Configure TWI options */
	twi_options_t p_opt;
	p_opt.master_clk = sysclk_get_peripheral_hz();
	p_opt.speed = TWI_CLK;
	p_opt.chip = TWI_MASTER_ADDR;
	
	/* Configure TWI as master*/
	uint32_t status = twi_master_init(TWI0, &p_opt);
	if (status != TWI_SUCCESS)
	{
		printf("Failed to initialize master\r\n");
	}
	else
	{
		printf("Initialized master\r\n");
	}

	/* Enable TWI master mode */
	twi_disable_slave_mode(TWI0);
	twi_enable_master_mode(TWI0);
}

uint8_t pio_conf_pin_to_peripheral(pio_reg_t *port, uint32_t periph, uint8_t pin_number) {
	// Disable interrupts on the pin
	port->PIO_IDR |= (0x1U << pin_number);

	// The pin will be set to peripheral B
	if (periph == PIO_PERIPH_B) { // 0 is peripheral A and 1 is B
		// Set to peripheral B
		port->PIO_ABSR |= (0x1U << pin_number);
		} else if (periph == PIO_PERIPH_A) {
		// Clear for peripheral A
		port->PIO_ABSR &= ~(0x1U << pin_number);
	}
	// The pin will be set in peripheral mode (not controllable by PIO)
	port->PIO_PDR |= (0x1U << pin_number);
	return 1;
}
