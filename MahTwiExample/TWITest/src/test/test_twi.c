/*
 * Two Wire Interface (TWI) unit tests
 *
 * Authors:	Jonathan Bjarnason
 * 			Mathias Beckius
 *
 * Date:	20 October 2014
 */

#include "sam3x8e/twi.h"
#include "sam3x8e/pio.h"
#include "sam3x8e/pmc.h"
#include "test/test_twi.h"
#include "consoleFunctions.h"

static void twi_init(void){
	pmc_enable_peripheral_clock(ID_TWI0);
	pio_conf_pin_to_peripheral(PIOA, PIO_PERIPH_A, 17);	// SDA
	pio_conf_pin_to_peripheral(PIOA, PIO_PERIPH_A, 18);	// SCL

	pmc_enable_peripheral_clock(ID_TWI1);
	pio_conf_pin_to_peripheral(PIOB, PIO_PERIPH_A, 12);	// SDA (pin 20)
	pio_conf_pin_to_peripheral(PIOB, PIO_PERIPH_A, 13);	// SCL (pin 21)
}

/*
 * Tests if the Slave Address can be set.
 */
void test_twi_init_slave(void) {
	twi_init_slave(TWI0, 100);
	twi_init_slave(TWI1, 100);
}

/*
 * This test performs a control of:
 * 	1) is current Master Read Direction modified?
 * 	2) is Device Address set?
 * 	3) is a 8-bit Device Address ignored?
 * 	4) is Internal Address Size set?
 * 	5) can the setting for the Internal Address Size be larger than 2 bits?
 */
void test_twi_set_device_address(void) {
	// Set Device Address and Internal Address Size
	twi_set_device_address(TWI0, 0xFF, 7);
	twi_set_device_address(TWI1, 0xFF, 7);
	// Device Address should be set
}

/*
 * This test performs a control of:
 * 	1) can the TWI_IADR register be set for TWI0 and TWI1?
 * 	2) if an address is larger than 3 bytes, will the rest be ignored?
 */
void test_twi_set_internal_address(void) {
	// set 4-byte address
	twi_set_internal_address(TWI0, 0xFFFFFFFF);
	// address shouldn't be larger than 3 bytes
	// set 4-byte address
	twi_set_internal_address(TWI1, 0xFFFFFFFF);
	// address shouldn't be larger than 3 bytes
}

/*
 * Will invalid parameters be denied?
 */
void test_twi_set_clock_invalid_parameters(void) {
	uint8_t error;
	// too large bus speed...
	error = twi_set_clock(TWI0, 400001, 84000000);
	// ...shouldn't be accepted!
	// too small MCK in relation to bus speed...
	error = twi_set_clock(TWI0, 400000, 800000);
	// ...shouldn't be accepted!
	// too small bus speed...
	error = twi_set_clock(TWI0, 0, 84000000);
	// ...shouldn't be accepted!
}

/*
 * This test performs a control of:
 * 	1) will valid parameters be accepted?
 * 	2) will this render in a correct register setting?
 */
void test_twi_set_clock_valid_parameters(void) {
	uint32_t reg0, reg1;
	uint8_t error;
	// too large bus speed...
	error = twi_set_clock(TWI0, TWI_FAST_MODE_SPEED, 84000000) |
			twi_set_clock(TWI1, TWI_FAST_MODE_SPEED, 84000000);
	// ...should be accepted!
	
	// store register values
	reg0 = TWI0->TWI_CWGR;
	reg1 = TWI1->TWI_CWGR;
	/*
	 * According to the given formula (see datasheet),
	 * divider for Clock Low and Clock High should be 101,
	 * Clock Divider should be zero.
	 */
}

/*
 * This test is semi-automatic and requires that SDA and SCL of
 * both TWI peripherals are connected.
 */
void test_twi_send_receive_SEMI_AUTOMATIC(void) {
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
	// printf("Slave initialized\r\n");
	
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

	/*
	 * send data to slave
	 */
	data_out = 65;
	// set Master Write Direction
	TWI0->TWI_MMR |= TWI_MMR_MASTER_WRITE;
	// write data to slave
	TWI0->TWI_THR = data_out;
	
	/*
	 * read data from master
	 */
	status = TWI1->TWI_SR;
	// If the slave address on the TWI line matches this slave device
	if (status & TWI_SR_SVACC) {
		if ((status & TWI_SR_SVREAD) && (status & TWI_SR_RXRDY)) {
			// read character
			data_in = (uint8_t) TWI1->TWI_RHR;
		}
	}

/*
	// Wait for last acknowledge
	while (1) {
		status = twi->TWI_SR;
		// If last byte has not been acknowledged
		if (status & TWI_SR_NACK) {
			return 1; // indicate "failure"
		}
		// ready to send data?
		if (status & TWI_SR_TXRDY) {
			break;
		}
	}
	// send STOP command
	twi->TWI_CR = TWI_CR_STOP;
	// Wait for Transmission Completed flag to set
	while (!(twi->TWI_SR & TWI_SR_TXCOMP));


	TEST_ASSERT_TRUE(data_in == data_out);*/
}
/*

void test_twi_master_init(twi_reg_t *twi){
	test_twi_init();

	const twi_settings_t settings = {
		.baudrate = 9200,
		.master_clk = 30
	};

	TEST_ASSERT_TRUE(twi_master_init(twi, settings));

}

void test_twi_master_write(twi_reg_t *twi){
	// Spells 'twi' in ASCII code
	uint8_t data_array[3] = { 0x74, 0x77, 0x69 };

	const twi_packet_t packet = {
		.address[3] = {0x40u, 0x80u, 0x120u},
		.address_length = 3,
		.buffer = data_array[0],
		.data_length = 3,
		.chip
	};


	//TEST_ASSERT_TRUE(twi_master_write(twi, packet));
}

void test_twi_master_read(twi_reg_t *twi){

}

void test_twi_slave_init(twi_reg_t *twi){
	//twi_master_init(twi, );

}

void test_twi_slave_write(twi_reg_t *twi){

}

void test_twi_slave_read(twi_reg_t *twi){

}
*/
