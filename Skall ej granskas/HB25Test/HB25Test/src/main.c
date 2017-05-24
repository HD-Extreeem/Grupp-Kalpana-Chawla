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

void testCase1_2(void);
void testCase3(void);
void testCase4(void);
void testCase5(void);
void testCase6(void);
void sendPulse(uint32_t);

/*
 * Full reverse: 1.0 ms (1000 us)
 * Neutral (off): 1.5 ms (1500 us)
 * Full forward: 2.0 ms (2000 us)
 */
int main (void)
{
	/* System clock and board initialization */
	sysclk_init();
	board_init();
	
	/* Pin initialization */
	*p_PIOB_PER |= (1<<26);
	*p_PIOB_OER |= (1<<26);
	
	delay_ms(3000);
	
	/* Wheel should be spinning */
	testCase1_2();
	
	/* Both wheels should be spinning */
	testCase3();
	
	/* Both wheels should spin full forward */
	testCase4();
	
	/* Both wheels should spin full reverse */
	testCase5();
	
	/* Both wheels should spin full reverse->stop->full forward */
	testCase6();
	
	while (1);
}

void testCase1_2()
{
	sendPulse(1200);
}

void testCase3()
{
	/* Send pulse to first motor controller */
	sendPulse(1200);
	
	/* Mode 2 */
	delay_us(1100);
	
	/* Send pulse to second motor controller */
	sendPulse(1200);
}

void testCase4()
{
	sendPulse(1000);
	delay_us(1100);
	sendPulse(1000);
}

void testCase5()
{
	sendPulse(2000);
	delay_us(1100);
	sendPulse(2000);
}

void testCase6()
{
	for (uint32_t i = 1000; i <= 1500; i++)
	{
		sendPulse(i);
		delay_us(1100);
		sendPulse(i);
			
		delay_us(5250);
	}
	
	delay_s(3);
	
	for (uint32_t i = 1500; i <= 2000; i++)
	{
		sendPulse(i);
		delay_us(1100);
		sendPulse(i);
		
		delay_us(5250);
	}
}

/* Sets pin 22 and clears it after a given time (pulse_time) */
void sendPulse(uint32_t pulse_time)
{
	*p_PIOB_SODR |= (1<<26);
	delay_us(pulse_time);
	*p_PIOB_CODR |= (1<<26);	
}
