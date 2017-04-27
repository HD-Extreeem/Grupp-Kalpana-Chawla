/*
 * InterruptStepCounter.c
 *
 * Created: 2017-04-19 11:28:36
 *  Author: Yurdaer Dalkic & Hadi Deknache
 *
 * This source file initialize and configure interrupt for pin 51 and 53 on Arduino Due.
 * 
 */ 



#include <asf.h>
#include "InterruptStepCounter.h"
#include "consoleFunctions.h"


// Counter for Arlo Robot encoders for right and left wheel
 uint16_t counter_1 = 0;
 uint16_t counter_2 = 0;


void configInterrupts(void){
	
	//Enable the module clock to the PIOB peripheral
	pmc_enable_periph_clk(ID_PIOB);
	//Enable the module clock to the PIOC peripheral
	pmc_enable_periph_clk(ID_PIOC);	
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

// Handler which calls when pin 53 is toggle.  
void pin12_edge_handler(const uint32_t id, const uint32_t index){
	// Check if pin 53 is high
	if (pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC12)){
		//increase the counter value
		counter_2++;
		//printf("\n c1= %d",counter_1);
	}	
}

// Handler which calls when pin 51 is toggle. 
void pin14_edge_handler(const uint32_t id, const uint32_t index){
	// Checks if pin 51 is high
	if (pio_get(PIOB, PIO_TYPE_PIO_INPUT, PIO_PB14)){
	//Increase the counter value
			counter_1++;
			//printf("\n c2 = %d",counter_2);
	}
}

void reset_Counter(void){
	counter_1=0;
	counter_2=0;
}