/**
 * Author: Hadi Deknache and Jonas Eiselt
 * Created: 2017-04-10
 */

#include <asf.h>
#include <inttypes.h>

#define piob_baseAdress 0x400E1000U

uint32_t *p_PIOB_PER = (uint32_t*) (piob_baseAdress + 0x0000U);
uint32_t *p_PIOB_OER  = (uint32_t*) (piob_baseAdress + 0x0010U);
uint32_t *p_PIOB_CODR = (uint32_t*) (piob_baseAdress + 0x0034U);
uint32_t *p_PIOB_SODR = (uint32_t*) (piob_baseAdress + 0x0030U);

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	delay_ms(3000);
	
	/* Insert application code here, after the board has been initialized. */
	*p_PIOB_PER |= (1<<26);
	*p_PIOB_OER |= (1<<26);
	
	/* Send pulse to first motor controller */
	*p_PIOB_SODR |= (1<<26);
	delay_ms(1);
	*p_PIOB_CODR |= (1<<26);
	
	/* Mode 2 */
	delay_ms(1.10);
	
	/* Send pulse to second motor controller */
	*p_PIOB_SODR |= (1<<26);
	delay_ms(1);
	*p_PIOB_CODR |= (1<<26);
	
	while (1);
}
