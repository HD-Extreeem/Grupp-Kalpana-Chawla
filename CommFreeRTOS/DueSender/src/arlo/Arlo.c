/*
 * Arlo.c
 *
 * Created: 2017-05-15 13:13:16
 * Author: Jonas Eiselt
 */ 

#include <asf.h>
#include <inttypes.h>
#include "arlo/Arlo.h"
#include "comm/TWIComm.h"

uint8_t tx_arm_buffer[TX_DATA_LENGTH] = {0};
uint8_t rx_arm_buffer[TX_DATA_LENGTH] = {0};

uint8_t tx_nav_buffer[TX_DATA_LENGTH] = {0};
uint8_t rx_nav_buffer[TX_DATA_LENGTH] = {0};

void arlo_init()
{
	ioport_init();
	twi_init();
	
	/* Initializes arm */
	arlo_arm_init();
	
	/* Initializes nav-system */
}

void arlo_arm_init()
{
	tx_arm_buffer[0] = TWI_CMD_ARM_INIT;
	tx_arm_buffer[1] = TWI_NO_DATA;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_arm_init(TWI_CMD_ARM_REQ_BOX_INFO, tx_arm_buffer, rx_arm_buffer);
	// Do something with rx_arm_buffer
	
	twi_arm_init(TWI_CMD_ARM_REQ_OBJ_INFO, tx_arm_buffer, rx_arm_buffer);
	// Do something with rx_arm_buffer
	
	twi_arm_init(TWI_CMD_ARM_REQ_COLLECT_INFO, tx_arm_buffer, rx_arm_buffer);
	// Do something with rx_arm_buffer
}