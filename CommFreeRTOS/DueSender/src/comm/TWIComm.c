/**
* TWIComm.c
*
* Created: 2017-05-11 09:59:55
* Author: Jonas Eiselt
*/

#include <asf.h>
#include "comm/TWIComm.h"
#include "comm/TWICommHandler.h"

static uint8_t tx_buffer[TX_DATA_LENGTH] = {};
static uint8_t rx_buffer[RX_DATA_LENGTH] = {};

twi_package_t tx_packet;
twi_package_t rx_packet;

Arm_Info arm_info_t;
TWI_CMD twi_cmd_t = TWI_CMD_ARM_INIT;
TWI_CMD_Init_Req twi_cmd_init_req_t = TWI_CMD_ARM_REQ_BOX_INFO;
Pick_Up_Status pick_up_status_t = PICK_UP_IDLE;
Drop_Off_Status drop_off_status_t;

void twi_init()
{
	/* Initializes TWI master options */
	twi_master_options_t opt;
	opt.speed = TWI_SPEED;
	
	/* Initializes the TWI master driver */
	if (twi_master_setup(TWI_PORT, &opt) == TWI_SUCCESS)
	{
		printf("Master initialized\r\n");
	}
}

void twi_send_packet(uint8_t request_byte, uint8_t recipient_addr)
{
	printf("Sending: %d\r\n", request_byte);
	tx_buffer[0] = request_byte;
	
	tx_packet.addr[0] = 0;							// TWI slave memory address data MSB
	tx_packet.addr[1] = 0;							// TWI slave memory address data LSB
	tx_packet.addr_length = 0;						// TWI slave memory address data size
	tx_packet.chip = recipient_addr;				// TWI slave bus address
	tx_packet.buffer = (void*) tx_buffer;			// transfer data source buffer
	tx_packet.length = TX_DATA_LENGTH;				// transfer data size (bytes)

	/* Performs a multi-byte write access then checks the result */
	uint32_t status = twi_master_write(TWI_PORT, &tx_packet);
	printf("Status: %d\r\n", (int) status);
}

void twi_request_packet(uint8_t recipient_addr)
{
	rx_packet.addr[0] = 0;							// TWI slave memory address data MSB
	rx_packet.addr[1] = 0;							// TWI slave memory address data LSB
	rx_packet.addr_length = 0;						// TWI slave memory address data size
	rx_packet.chip = recipient_addr;				// TWI slave bus address
	rx_packet.buffer = (void*) rx_buffer;			// transfer data source buffer
	rx_packet.length = RX_DATA_LENGTH;				// transfer data size (bytes)
	
	/* Performs a multi-byte read access then checks the result */
	while (twi_master_read(TWI_PORT, &rx_packet) != TWI_SUCCESS);
	// indicate();
	
	for (int i = 0; i < RX_DATA_LENGTH; i++)
	{
		if (recipient_addr == SLAVE_ADDR_ARM)
		{
			printf("Got from Uno: %d\r\n", rx_buffer[i]);
		}
		else
		{
			printf("Got from Due: %d\r\n", rx_buffer[i]);
		}
	}
}

Bool twi_crane_init()
{
	twi_send_packet(0x20, SLAVE_ADDR_ARM);
	twi_request_packet(SLAVE_ADDR_ARM);

	/* Checks received data */
	twi_check_data(SLAVE_ADDR_ARM);
	twi_request_packet(SLAVE_ADDR_ARM);

	return true;
}

void twi_check_data(uint8_t recipient_addr)
{
	uint8_t objects_left = 2; /* Don't forget to declare as global! */
	
	if (recipient_addr == SLAVE_ADDR_ARM)
	{
		switch (twi_cmd_t)
		{
			case TWI_CMD_ARM_INIT:
			twi_cmd_init_req_t = rx_buffer[0];
			if (twi_cmd_init_req_t == TWI_CMD_ARM_REQ_COLLECT_INFO)
			{
				twi_cmd_t = TWI_CMD_PICK_UP_START;
				twi_send_packet(TWI_CMD_PICK_UP_START, SLAVE_ADDR_ARM);
			}
			else
			{
				twi_send_packet(TWI_CMD_ARM_INIT, SLAVE_ADDR_ARM);
			}
			break;
			case TWI_CMD_PICK_UP_START:
			pick_up_status_t = rx_buffer[0];
			switch (pick_up_status_t)
			{
				case PICK_UP_DONE:
				if (arm_info_t.collect_all && objects_left > 0)
				{
					twi_cmd_t = TWI_CMD_PICK_UP_START;
				}
				else
				{
					twi_cmd_t = TWI_CMD_DROP_OFF_START;
				}
				twi_send_packet(twi_cmd_t, SLAVE_ADDR_ARM);
				break;
				case PICK_UP_FORWARD:
				/* ... */
				break;
				case PICK_UP_BACKWARD:
				/* ... */
				break;
				case PICK_UP_RUNNING:
				/* ... */
				break;
				case PICK_UP_FAILED:
				/* ... */
				break;
				case PICK_UP_DONE_DRIVE:
				/* ... */
				break;
				case PICK_UP_IDLE:
				/* ... */
				break;
				default:
				break;
			}
			break;
			case TWI_CMD_DROP_OFF_START:
			drop_off_status_t = rx_buffer[0];
			switch (drop_off_status_t)
			{
				case DROP_OFF_DONE:
				objects_left = objects_left - 1;
				if (objects_left > 0)
				{
					twi_cmd_t = TWI_CMD_PICK_UP_START;
				}
				else
				{
					/* No more objects to pick up */
					while (1);
				}
				break;
				default:
				break;
			}
			break;
			case TWI_CMD_ERROR:
			twi_cmd_t = TWI_CMD_ARM_INIT;
			twi_send_packet(twi_cmd_t, SLAVE_ADDR_ARM);
			break;
			default:
			break;
		}
	}
	else if (recipient_addr == SLAVE_ADDR_NAV)
	{
		/* Get coordinate data */
	}
	else
	{
		// Something went wrong...
	}
}

void twi_indicate()
{
	for (int i = 0; i < 10; i++)
	{
		gpio_toggle_pin(LED0_GPIO);
		delay_ms(100);
	}
}