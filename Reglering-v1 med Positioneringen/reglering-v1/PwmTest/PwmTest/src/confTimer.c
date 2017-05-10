/*
 * conf_tc.c
 *
 * Created: 2013-12-10 08:37:41
 *  Author: Tommy
 */ 

#include <asf.h>
#include "confTimer.h"

void configureTC(void)
{
//	sysclk_init();
//	tc_enable_interrupt()
	
	
	/* Configure PMC */
//	pmc_enable_periph_clk(ID_TC0);
	/** Configure TC for a 10 kHz frequency and trigger on RC compare. */
//	tc_init(TC0, 0, 0 | TC_CMR_BURST_XC0);	//Timer_clock_1 - MCK/2 - 42 MHz
//	tc_write_rc(TC0, 0, 4200);					//4200 corresponds to fs = 10 kHz
//	tc_start(TC0, 0);
}

