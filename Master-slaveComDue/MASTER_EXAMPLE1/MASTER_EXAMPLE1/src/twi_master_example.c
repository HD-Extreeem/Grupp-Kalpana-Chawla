/**
* twi_master_example.c
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
#define EEPROM_MEM_ADDR 0x00        //!< TWI slave memory address
#define TWI_SPEED 50000				//!< TWI data transfer rate

#define PATTERN_TEST_LENGTH sizeof(test_pattern)
const uint8_t test_pattern[] = {
	0x57,
	0x4F,
	0x48,
	0x4F,
	0x4F
};

int main(void)
{
	sysclk_init();
	board_init();
	configure_console();
	printf("Startar");
	
	// configure_console();
	
	/* irq_initialize_vectors(); */
	
	
	/* TWI master initialization options. */
	twi_master_options_t opt;
	opt.speed = TWI_SPEED;

	/* Initialize the TWI master driver. */
	twi_master_setup(TWI0, &opt);
	
	twi_package_t packet = {
		.addr[0]      = 0, // TWI slave memory address data MSB
		.addr[1]      = 0,      // TWI slave memory address data LSB
		.addr_length  = 0 ,    // TWI slave memory address data size
		.chip         = EEPROM_BUS_ADDR,      // TWI slave bus address
		.buffer       = (void*) test_pattern, // transfer data source buffer
		.length       = sizeof(test_pattern)   // transfer data size (bytes)
	};

	
	// Perform a multi-byte write access then check the result.
	while (twi_master_write(TWI0, &packet) != TWI_SUCCESS);

	uint8_t data_received[sizeof(test_pattern)];

	twi_package_t packet_received = {
		.addr[0]      = 0, // TWI slave memory address data MSB
		.addr[1]      = 0,      // TWI slave memory address data LSB
		.addr_length  = 0,    // TWI slave memory address data size
		.chip         = EEPROM_BUS_ADDR,      // TWI slave bus address
		.buffer       = data_received,        // transfer data destination buffer
		.length       = sizeof(test_pattern)   // transfer data size (bytes)
	};

	// Perform a multi-byte read access then check the result.
	while(twi_master_read(TWI0, &packet_received) != TWI_SUCCESS);
	
		for (uint32_t i = 0 ; i < sizeof(data_received); i++) {
			printf("Tar emot:%c\n\r", data_received[i]);
			if (data_received[i]==test_pattern[i])
			{
				printf("Data Correct\n\r");
			}
			else{
				printf("Data incorrect\n\r");
			}
		}
	

	LED_On(LED0_GPIO);

	while(1);
}
