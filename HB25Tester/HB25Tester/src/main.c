/**
* Author: Hadi Deknache and Jonas Eiselt
* Created: 2017-04-10
*/

#include <asf.h>
#include <inttypes.h>

#define piob_baseAdress 0x400E1000U

uint32_t *const p_PIOB_PER = (uint32_t*) (piob_baseAdress + 0x0000U);
uint32_t *const p_PIOB_OER  = (uint32_t*) (piob_baseAdress + 0x0010U);
uint32_t *const p_PIOB_CODR = (uint32_t*) (piob_baseAdress + 0x0034U);
uint32_t *const p_PIOB_SODR = (uint32_t*) (piob_baseAdress + 0x0030U);

/*
* Full reverse: 1.0 ms
* Neutral (off): 1.5 ms
* Full forward: 2.0 ms
*/
int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	
	delay_ms(3000);
	
	/* Insert application code here, after the board has been initialized. */
	*p_PIOB_PER |= (1<<26);
	*p_PIOB_OER |= (1<<26);
	
	while (1)
	{
		for (uint32_t i = 1000; i<=1500;i++)
		{
			/* Send pulse to first motor controller */
			*p_PIOB_SODR |= (1<<26);
			delay_us(i);
			*p_PIOB_CODR |= (1<<26);
			
			/* Mode 2 */
			delay_us(1100);
			
			/* Send pulse to second motor controller */
			*p_PIOB_SODR |= (1<<26);
			delay_us(i);
			*p_PIOB_CODR |= (1<<26);
			
			delay_us(5250);
		}
		
		
		delay_s(3);
		
		for (uint32_t i = 1500; i<=2000;i++)
		{
			/* Send pulse to first motor controller */
			*p_PIOB_SODR |= (1<<26);
			delay_us(i);
			*p_PIOB_CODR |= (1<<26);
			
			/* Mode 2 */
			delay_us(1100);
			
			/* Send pulse to second motor controller */
			*p_PIOB_SODR |= (1<<26);
			delay_us(i);
			*p_PIOB_CODR |= (1<<26);
			
			delay_us(5250);
		}
		
		/* Send pulse to first motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_us(1500);
		*p_PIOB_CODR |= (1<<26);
		
		/* Mode 2 */
		delay_us(1100);
		
		/* Send pulse to second motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_us(1500);
		*p_PIOB_CODR |= (1<<26);
		
		delay_s(3);
	}
	
	
	while (1)
	{
		/* Send pulse to first motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_ms(1);
		*p_PIOB_CODR |= (1<<26);
		
		delay_s(2);
		
		/* Send pulse to first motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_us(1500);
		*p_PIOB_CODR |= (1<<26);
		
		delay_s(2);
		
		/* Send pulse to first motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_ms(2);
		*p_PIOB_CODR |= (1<<26);
		
		delay_s(2);
		
		/* Send pulse to first motor controller */
		*p_PIOB_SODR |= (1<<26);
		delay_us(1500);
		*p_PIOB_CODR |= (1<<26);
		
		delay_s(2);
	}
	
}
