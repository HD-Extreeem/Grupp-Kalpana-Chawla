/**
* TWIComm.c
*
* Created: 2017-05-11 09:59:55
* Author: Jonas Eiselt
*/

#include <asf.h>
#include "comm/TWIComm.h"

twi_package_t tx_packet;
twi_package_t rx_packet;

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

void twi_send_packet(uint8_t *tx_buffer, uint8_t recipient_addr)
{
	printf("Sending: %d\r\n", tx_buffer[0]);
	
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

void twi_request_packet(uint8_t *rx_buffer, uint8_t recipient_addr)
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

void twi_arm_init(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	TWI_CMD_Init_Req twi_cmd_init_req_t = TWI_CMD_ARM_REQ_BOX_INFO;
	while (twi_cmd_init_req_t != TWI_CMD_ARM_REQ_COLLECT_INFO) 
	{
		tx_buffer[0] = TWI_CMD_ARM_INIT;
		tx_buffer[1] = twi_cmd_init_req_t;
		
		twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
		twi_request_packet(rx_buffer, SLAVE_ADDR_ARM);
		
		/* Do something with the received data */
		
		twi_cmd_init_req_t++;
	}
}

void twi_control_arm(TWI_CMD twi_cmd_t, uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	switch (twi_cmd_t)
	{
		case TWI_CMD_ARM_INIT:
		twi_arm_init(tx_buffer, rx_buffer);
		break;
		case TWI_CMD_DROP_OFF_START:
		twi_start_drop_off(tx_buffer, rx_buffer);
		break;
		case TWI_CMD_PICK_UP_START:
		twi_start_pick_up(tx_buffer, rx_buffer);
		break;
		case TWI_CMD_DROP_OFF_STATUS:
		twi_check_drop_off_status(tx_buffer, rx_buffer);
		break;
		case TWI_CMD_PICK_UP_STATUS:
		twi_check_pick_up_status(tx_buffer, rx_buffer);
		break;
		case TWI_CMD_ERROR:
		twi_reset_arm(tx_buffer, rx_buffer);
		break;
		default:
		break;
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

void twi_start_pick_up(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	tx_buffer[0] = TWI_CMD_PICK_UP_START;
	
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	/* No need to get data back */
}

void twi_start_drop_off(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	tx_buffer[0] = TWI_CMD_DROP_OFF_START;
	
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	/* No need to get data back */
}

void twi_reset_arm(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	/* Re-initialize arm */
	twi_arm_init(tx_buffer, rx_buffer);
}

void twi_check_pick_up_status(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	tx_buffer[0] = TWI_CMD_PICK_UP_STATUS;
	
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	twi_request_packet(rx_buffer, SLAVE_ADDR_ARM);
	
	/* Do something with the received data */
}

void twi_check_drop_off_status(uint8_t *tx_buffer, uint8_t *rx_buffer)
{
	tx_buffer[0] = TWI_CMD_DROP_OFF_STATUS;
	
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	twi_request_packet(rx_buffer, SLAVE_ADDR_ARM);
	
	/* Do something with the received data */
}