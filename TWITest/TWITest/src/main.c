/**
 * Author: Jonas Eiselt
 * Created: 2017-04-29
 */

#include <asf.h>
#include <inttypes.h>

int main (void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();
	
	/* TWI structure */
	twi_init();
	twi_begin_transmission(slave_address);
	twi_send_byte(255);
	twi_end_transmission();
	
	while (1);
}