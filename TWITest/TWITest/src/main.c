/**
 * Author: Jonas Eiselt
 * Created: 2017-04-29
 */

#include <asf.h>
#include <inttypes.h>
#include <twi.h>
#include "consoleFunctions.h"

#define TWI_CLK 400000
#define UNO_ADDRESS 0x50
#define DATA_LENGTH 1

int main (void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();
	
	configure_console();
	
	/* Enable TWI peripheral clock */
	pmc_enable_periph_clk(ID_TWI0);
	
	/* Enable TWI master mode */
	twi_enable_master_mode(TWI0);
	
	/* Configure TWI options */
	twi_options_t p_opt;
	p_opt.master_clk = sysclk_get_peripheral_hz();
	p_opt.speed = TWI_CLK;
	
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

	static uint8_t tx_buffer[] = "Hello";
	static uint8_t rx_buffer[16] = {0};
	printf("Here : 1\r\n");
	
	/* Configure packet to be sent */
	twi_packet_t tx_packet;
	tx_packet.chip = 0x50;
	tx_packet.addr[0] = 0>>8;
	tx_packet.addr[1] = 0;
	tx_packet.addr_length = 1;
	tx_packet.buffer = (uint8_t*) tx_buffer;
	tx_packet.length = (sizeof(tx_buffer)/sizeof(uint8_t));
	
	/* Configure packet to be received */
	twi_packet_t rx_packet;
	rx_packet.chip = 0x50;
	rx_packet.addr[0] = 22;
	rx_packet.addr[1] = 0;
	rx_packet.addr[2] = 0;
	rx_packet.addr_length = 1;
	rx_packet.buffer = (uint8_t*) rx_buffer;
	rx_packet.length = (30/sizeof(uint8_t));
	
	printf("Here : 2\r\n");
	
	/* Send data to slave device */
	status = twi_master_write(TWI0, &tx_packet);
	printf("Here : 3\r\n");
	if (status != TWI_SUCCESS)
	{
		printf("Failed to send data\r\n");
	}
	
	/* Receive data from slave device */
	status = twi_master_read(TWI0, &rx_packet);
	if (status == TWI_SUCCESS)
		printf("Received something!");
		
	/*for (uint32_t i = 0; i < (sizeof(tx_data)/sizeof(uint8_t)); i++) 
	{
		// printf("Received: %d\n\r", (uint8_t*) tx_data);
	}*/
	
	while (1);
}