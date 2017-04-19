/*
 * InterruptStepCounter.c
 *
 * Created: 2017-04-19 11:28:36
 *  Author: Yurdaer Dalkic
 */ 
#include <asf.h>
#include "InterruptStepCounter.h"
#include "consoleFunctions.h"




void configInterrupts(void){
	printf("config");
	pmc_set_writeprotect(0);
	//Enable the module clock to the PIOB peripheral
	pmc_enable_periph_clk(ID_PIOB);
	//Enable the module clock to the PIOC peripheral
	pmc_enable_periph_clk(ID_PIOC);
	
		//test
		
		
	//Set pin 12 direction on PIOC as input, with pullup
	pio_set_input(PIOC,PIO_PC12,PIO_PULLUP);
	//Set pin 14 direction on PIOB as input, with pullup
	pio_set_input(PIOB,PIO_PB14,PIO_PULLUP);
	//Configure the input pin 12 interrupt mode and handler
	pio_handler_set(PIOC, ID_PIOC, PIO_PC12, PIO_IT_EDGE, pin12_edge_handler);
	//Configure the input pin 14 interrupt mode and handler
	pio_handler_set(PIOB, ID_PIOB, PIO_PB14, PIO_IT_EDGE, pin14_edge_handler);
	//Enable the interrupt for the configured input pin 12
	pio_enable_interrupt(PIOC,PIO_PC12);
	//Enable the interrupt for the configured input pin 12
	pio_enable_interrupt(PIOB,PIO_PB14);
	//Enable interrupt handling from the PIOC module:
	NVIC_EnableIRQ(PIOC_IRQn);
	//Enable interrupt handling from the PIOB module:
	NVIC_EnableIRQ(PIOB_IRQn);
	

}

void pin12_edge_handler(const uint32_t id, const uint32_t index){
	if (pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC12)){
		printf("pin51");
	}
	
	
}
void pin14_edge_handler(const uint32_t id, const uint32_t index){
	if (pio_get(PIOB, PIO_TYPE_PIO_INPUT, PIO_PB14)){
			printf("pin 53");
	}
	


}