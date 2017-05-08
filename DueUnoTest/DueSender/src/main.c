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

#define UNO_BUS_ADDR 0x50       // TWI slave bus address
#define TWI_SPEED 100000		// TWI data transfer rate

#define TX_DATA_LENGTH sizeof(tx_buffer)
#define RX_DATA_LENGTH 3

static uint8_t tx_buffer[] = {0x20};
static uint8_t rx_buffer[RX_DATA_LENGTH] = {0};

void configure_twi(void);
void indicate(void);

Bool crane_init(void);

void send_data(uint8_t request_byte);
void request_data(void);
void check_data(void);

static twi_package_t tx_packet;
static twi_package_t rx_packet;

int main(void)
{
	sysclk_init();
	board_init();

	configure_twi();
	// indicate();
	
	/* Crane init */
	while (!crane_init());
	indicate();
	
	// request_data();
	// check_data();
	
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
	tx_packet.length = TX_DATA_LENGTH;					// transfer data size (bytes)

	/* Configures packet to be received */
	rx_packet.addr[0] = 0;							// TWI slave memory address data MSB
	rx_packet.addr[1] = 0;							// TWI slave memory address data LSB
	rx_packet.addr_length = 0;						// TWI slave memory address data size
	rx_packet.chip = UNO_BUS_ADDR;					// TWI slave bus address
	rx_packet.buffer = (void*) rx_buffer;		// transfer data destination buffer
	rx_packet.length = RX_DATA_LENGTH;					// transfer data size (bytes)
}

Bool crane_init()
{
	send_data(0x20);
	request_data();

	// indicate();
	
	/* Check received data */
	check_data();
	request_data();
	
	return true;
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

void send_data(uint8_t request_byte)
{
	tx_buffer[0] = request_byte;
	
	// Performs a multi-byte write access then check the result.
	while (twi_master_write(TWI0, &tx_packet) != TWI_SUCCESS);
	// indicate();
}

void request_data()
{
	// Performs a multi-byte read access then check the result.
	while (twi_master_read(TWI0, &rx_packet) != TWI_SUCCESS);
	// indicate();
}

void check_data()
{
	switch (rx_buffer[0])
	{
		case 0x10:
		/* Identification */
		send_data(0x21);
		break;
		case 0x11:
		/* Postion relative to object */
		send_data(0x11);
		break;
		case 0x12:
		/* Position relative to box-edge */
		send_data(0x12);
		break;
		case 0x13:
		/* Max velocity (mm/s) */
		send_data(0x13);
		break;
		case 0x14:
		/* Lift success */
		send_data(0x14);
		break;
		case 0x15:
		/* Lift failed */
		send_data(0x15);
		break;
		case 0x16:
		/* Return success */
		send_data(0x16);
		break;
		case 0x17:
		/* Return failed */
		send_data(0x17);
		break;
		case 0x18:
		/* Adjust position */
		send_data(0x18);
		break;
		case 0x19:
		/* Cancel adjustment */
		send_data(0x19);
		break;
		default:
		// Do nothing...
		break;
	}
}