/*
*  @file StepCounter_ISR.c
*
*  @brief Source file which handle interrupt and initialize and attach interrupts
* 
*  @author Desiree J�nsson och Jonas Eiselt(Skelettet),Hadi Deknache och Yurdaer Dalkic (kod)
*
*  @date 2017-04-24 11:24:47
*/

#include <asf.h>
#include <FreeRTOS.h>
#include <inttypes.h>
#include "StepCounter_ISR.h"
#include "consoleFunctions.h"

/** Handler for triggered interrupt on pin 53
	increases a counter if rising edge
	@param id for for which port
	@param index for which pin triggered
**/
void pin12_edge_handler(const uint32_t id, const uint32_t index)
{
	if (pio_get(PIOC, PIO_TYPE_PIO_INPUT, PIO_PC12)){
		//increase the counter value
		counter_2++;
	}
	
}
/** Handler for triggered interrupt on pin 51
	increases a counter if rising edge
	@param id for for which port
	@param index for which pin triggered
**/
void pin14_edge_handler(const uint32_t id, const uint32_t index)
{
	// Checks if pin 51 is high
	if (pio_get(PIOB, PIO_TYPE_PIO_INPUT, PIO_PB14)){
		//Increase the counter value
		counter_1++;
	}
}

/*
* Runs once, when program starts.
* Configures pins to attach interrupts and when to trigger interrupts
*/
void attach_interrupt(void)
{

	//Enable the module clock to the PIOB peripheral
	pmc_enable_periph_clk(ID_PIOB);
	//Enable the module clock to the PIOC peripheral
	pmc_enable_periph_clk(ID_PIOC);
	//Set pin 12 direction on PIOC as input, with pullup
	pio_set_input(PIOC,PIO_PC12,PIO_PULLUP);
	//Set pin 14 direction on PIOB as input, with pullup
	pio_set_input(PIOB,PIO_PB14,PIO_PULLUP);
	//Configure the input pin 12 interrupt mode and handler pin51
	pio_handler_set(PIOC, ID_PIOC, PIO_PC12, PIO_IT_RISE_EDGE, pin12_edge_handler);
	//Configure the input pin 14 interrupt mode and handler pin53
	pio_handler_set(PIOB, ID_PIOB, PIO_PB14, PIO_IT_RISE_EDGE, pin14_edge_handler);
	//Enable the interrupt for the configured input pin 12
	pio_enable_interrupt(PIOC,PIO_PC12);
	//Enable the interrupt for the configured input pin 12
	pio_enable_interrupt(PIOB,PIO_PB14);
	//Enable interrupt handling from the PIOC module:
	NVIC_EnableIRQ(PIOC_IRQn);
	//Enable interrupt handling from the PIOB module:
	NVIC_EnableIRQ(PIOB_IRQn);
}

/*
* Method for reset of counter when needed for pid control and rotation
*/
void reset_Counter(void){
	counter_1=0;
	counter_2=0;
}
