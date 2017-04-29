/**
 * Author: Jonas Eiselt
 * Created: 2017-04-29
 */

#include <asf.h>
#include <inttypes.h>
#include <twi.h>
#include "consoleFunctions.h"

#define TWI_CLK 40000

int main (void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();
	
	configure_console();
	
	/* Enable TWI peripheral clock */
	pmc_enable_periph_clk(TWI0);
	
	/* Enable required TWI PIOs */
	
	/* Enable TWI master mode */
	twi_enable_master_mode(TWI0);
	
	/* Configure TWI options */
	twi_options_t p_opt;
	p_opt.master_clk = sysclk_get_peripheral_hz();
	p_opt.speed = TWI_CLK;
	
	/* Configure TWI as master*/
	uint32_t status = twi_master_init(TWI0, &p_opt);
	
	/* Configure packet to be sent */
	twi_packet_t p_packet;
	p_packet.addr[0] = 0;
	p_packet.addr[1] = 0;
	p_packet.addr[2] = 0;
	p_packet.addr_length = 0;
	p_packet.buffer = tx_data;
	p_packet.chip = UNO_ADDRESS;
	p_packet.length = DATA_LENGTH;
	
	/* Send data to slave device */
	/*status = twi_master_write(TWI0, &p_packet);

	/* Receive data from slave device */
	twi_master_read(TWI0, &p_packet);
	for (uint32_t i = 0; i < DATA_LENGTH; i++) 
	{
		printf("%d", tx_buffer[i]);
	}
	
	while (1);
}