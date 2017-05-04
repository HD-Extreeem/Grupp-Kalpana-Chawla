/**
 * main.c
 *
 * Author: Hadi Deknache and Jonas Eiselt
 * Created 2017-05-03
 *
 * Note: modified ASF-example
 */

#include <asf.h>
#include <inttypes.h>
#include "twi_master.h"
#include "led.h"

//#include "conf_board.h"
#include "consoleFunctions.h"

#ifndef EEPROM_BUS_ADDR
#define EEPROM_BUS_ADDR 0x50        //!< TWI slave bus address
#endif
#define EEPROM_MEM_ADDR 0        //!< TWI slave memory address
#define TWI_SPEED 50000				//!< TWI data transfer rate

#define PATTERN_TEST_LENGTH sizeof(test_pattern)
const uint8_t test_pattern[] = {0x57, 0x4F, 0x48, 0x4F, 0x4F};

int main(void)
{
	sysclk_init();
	board_init();
	
	// configure_console();
	
	/* irq_initialize_vectors(); */
	for (int i = 0; i < 10; i++)
	{
		LED_On(LED0_GPIO);
		delay_ms(100);
		LED_Off(LED0_GPIO);
		delay_ms(100);
	}
	
	delay_ms(1000);
	
	
	/* TWI master initialization options. */
	twi_master_options_t opt;
	opt.speed = TWI_SPEED;

	/* Initialize the TWI master driver. */
	twi_master_setup(TWI0, &opt);
	for (int i = 0; i < 10; i++)
	{
		LED_On(LED0_GPIO);
		delay_ms(100);
		LED_Off(LED0_GPIO);
		delay_ms(100);
	}
	delay_ms(1000);
	
	/* Initialize the platform LED's. */
	twi_package_t packet = {
		.addr[0]      = 0, // TWI slave memory address data MSB
		.addr[1]      = 0,      // TWI slave memory address data LSB
		.addr_length  = 0,    // TWI slave memory address data size
		.chip         = EEPROM_BUS_ADDR,      // TWI slave bus address
		.buffer       = (void*) test_pattern, // transfer data source buffer
		.length       = PATTERN_TEST_LENGTH   // transfer data size (bytes)
	};

	for (int i = 0; i < 10; i++)
	{
		LED_On(LED0_GPIO);
		delay_ms(100);
		LED_Off(LED0_GPIO);
		delay_ms(100);
	}
	
	// Perform a multi-byte write access then check the result.
	while (twi_master_write(TWI0, &packet) != TWI_SUCCESS);
	for (int i = 0; i < 10; i++)
	{
		LED_On(LED0_GPIO);
		delay_ms(100);
		LED_Off(LED0_GPIO);
		delay_ms(100);
	}
	uint8_t data_received[PATTERN_TEST_LENGTH] = {0};

	twi_package_t packet_received = {
		.addr[0]      = EEPROM_MEM_ADDR >> 8, // TWI slave memory address data MSB
		.addr[1]      = EEPROM_MEM_ADDR,      // TWI slave memory address data LSB
		.addr_length  = sizeof (uint16_t),    // TWI slave memory address data size
		.chip         = EEPROM_BUS_ADDR,      // TWI slave bus address
		.buffer       = data_received,        // transfer data destination buffer
		.length       = PATTERN_TEST_LENGTH   // transfer data size (bytes)
	};

	// Perform a multi-byte read access then check the result.
	while (twi_master_read(TWI0, &packet_received) != TWI_SUCCESS);

	// Verify that the received data matches the sent data.
	for (uint32_t i = 0 ; i < PATTERN_TEST_LENGTH; ++i) {

		if (data_received[i] != test_pattern[i]) {
			// Error
			while(1);
		}
	}

	LED_On(LED0_GPIO);

	while(1);
}
