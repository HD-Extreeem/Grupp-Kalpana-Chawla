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
		
	while (1);
}