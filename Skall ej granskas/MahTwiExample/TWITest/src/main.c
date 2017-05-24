#include <asf.h>
#include <inttypes.h>
#include "sam3x8e/twi.h"
#include "sam3x8e/pio.h"
#include "sam3x8e/pmc.h"
#include "consoleFunctions.h"
#include "test/test_twi.h"

static void twi_init(void);

int main(void)
{
	sysclk_init();
	board_init();
	
	configure_console();
	
	uint32_t i;
	uint32_t result;
	uint32_t status;
	uint8_t slave_address = 100;
	uint8_t data_in = 0;
	uint8_t data_out;

	twi_init();

	/*
	* initialize TWI Slave
	*/
	twi_init_slave(TWI1, slave_address);
	printf("Slave initialized\r\n");
	
	/*
	* initialize TWI Master
	*/
	twi_set_clock(TWI0, TWI_FAST_MODE_SPEED, 84000000);
	twi_set_device_address(TWI0, slave_address, 0);
	twi_set_internal_address(TWI0, 0);
	twi_init_master(TWI0);
	// If the slave address on the TWI line matches this slave device
	result = 0;
	for (i = 0; (i < 100 && result == 0); i++) {
		result = TWI1->TWI_SR & TWI_SR_SVACC;
	}
	printf("Result: %d\r\n", result);

	/*
	* send data to slave
	*/
	data_out = 65;
	//twi_write_master(TWI0,65, 1);
	// set Master Write Direction
	TWI0->TWI_MMR |= TWI_MMR_MASTER_WRITE;
	// write data to slave
	TWI0->TWI_THR = data_out;
	printf("Sent: %d\r\n", data_out);
	
	/*
	* read data from master
	*/
	status = TWI1->TWI_SR;
	printf("Status: %d\r\n", status);
	delay_ms(500);
	printf("Now?\r\n");
	// If the slave address on the TWI line matches this slave device
	if (status & TWI_SR_SVACC) {
		printf("Status: %d\r\n", status);
		if ((status & TWI_SR_SVREAD) && (status & TWI_SR_RXRDY)) {
			// read character
			data_in = (uint8_t) TWI1->TWI_RHR;
			printf("Received: %d\r\n", data_in);
		}
	}
	delay_ms(500);
}

static void twi_init(void){
	pmc_enable_peripheral_clock(ID_TWI0);
	pio_conf_pin_to_peripheral(PIOA, PIO_PERIPH_A, 17);	// SDA
	pio_conf_pin_to_peripheral(PIOA, PIO_PERIPH_A, 18);	// SCL

	pmc_enable_peripheral_clock(ID_TWI1);
	pio_conf_pin_to_peripheral(PIOB, PIO_PERIPH_A, 12);	// SDA (pin 20)
	pio_conf_pin_to_peripheral(PIOB, PIO_PERIPH_A, 13);	// SCL (pin 21)
}
