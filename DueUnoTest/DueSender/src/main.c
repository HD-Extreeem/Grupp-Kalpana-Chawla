/**
* main.c
* I2C communication between Arduino Due and Arduino Uno. Arduino Due sends data and then receives.
*
* Author: Jonas Eiselt
* Created 2017-05-05
*
* Note: modified ASF-example
*/

#include <asf.h>
#include <inttypes.h>
#include "twi.h"
#include "led.h"

#define UNO_BUS_ADDR 0x50        // TWI slave bus address
#define TWI_SPEED 50000			 // TWI data transfer rate

#define DATA_LENGTH sizeof(tx_buffer)
static const uint8_t tx_buffer[] = {0x55, 0x55};
static uint8_t rx_buffer[DATA_LENGTH] = {0};

void configure_twi(void);
void indicate(void);

static twi_package_t tx_packet;
static twi_package_t rx_packet;

int main(void)
{
	sysclk_init();
	board_init();

	configure_twi();
	// indicate();
	
	// Performs a multi-byte write access then check the result.
	while (twi_master_write(TWI0, &tx_packet) != TWI_SUCCESS);
	// indicate();
	
	// Performs a multi-byte read access then check the result.
	while (twi_master_read(TWI0, &rx_packet) != TWI_SUCCESS);
	// indicate();
	
	/*if (rx_buffer[0] == 0)
	{
	indicate();
	}*/
	
	while (twi_master_write(TWI0, &rx_packet) != TWI_SUCCESS);
	indicate();
	
	while(1);
}

void configure_twi()
{
	/* Initializes TWI master options */
	twi_master_options_t opt;
	opt.speed = TWI_SPEED;

	/* Initializes the TWI master driver */
	twi_master_setup(TWI0, &opt);
	
	/* Configures packet to be transmitted */
	tx_packet.addr[0] = 0;							// TWI slave memory address data MSB
	tx_packet.addr[1] = 0;							// TWI slave memory address data LSB
	tx_packet.addr_length = 0;						// TWI slave memory address data size
	tx_packet.chip = UNO_BUS_ADDR;					// TWI slave bus address
	tx_packet.buffer = (void*) tx_buffer;			// transfer data source buffer
	tx_packet.length = DATA_LENGTH;					// transfer data size (bytes)

	/* Configures packet to be received */
	rx_packet.addr[0] = 0;							// TWI slave memory address data MSB
	rx_packet.addr[1] = 0;							// TWI slave memory address data LSB
	rx_packet.addr_length = 0;						// TWI slave memory address data size
	rx_packet.chip = UNO_BUS_ADDR;					// TWI slave bus address
	rx_packet.buffer = (void*) rx_buffer;		// transfer data destination buffer
	rx_packet.length = DATA_LENGTH;					// transfer data size (bytes)
}

/* Signals the user via the on-board LED (marked L) */
void indicate()
{
	for (int i = 0; i < 10; i++)
	{
		LED_On(LED0_GPIO);
		delay_ms(100);
		LED_Off(LED0_GPIO);
		delay_ms(100);
	}
}