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

uint8_t tx_arm_buffer[TX_ARM_LENGTH] = {0};
uint8_t rx_arm_buffer[RX_ARM_LENGTH] = {0};

uint8_t tx_nav_buffer[TX_NAV_LENGTH] = {0};
uint8_t rx_nav_buffer[RX_NAV_LENGTH] = {0};

void arlo_init()
{
	// ioport_init();
	twi_init();
	
	/* Initializes arm */
	arlo_arm_init();
	
	/* Initializes nav-system */
	arlo_nav_init();
}

void arlo_arm_init()
{
	printf("Arm initialized?\r\n");
	
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

void arlo_nav_init()
{
	/* Get coordinate for sock */
	twi_nav_init(0x52, tx_nav_buffer, rx_nav_buffer);
	
	/* Get coordinate for cube */
	twi_nav_init(0x53, tx_nav_buffer, rx_nav_buffer);
	
	/* Get coordinate for glass */
	twi_nav_init(0x54, tx_nav_buffer, rx_nav_buffer);
}

void arlo_get_position(uint16_t *position_buffer)
{
	/* Get first coordinate (x-coordinate) */
	tx_nav_buffer[0] = 0x50;
	
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);
	twi_request_packet(rx_nav_buffer, SLAVE_ADDR_NAV);
	
	/* Convert uint8_t to uint16_t */
	position_buffer[0] = (uint16_t) ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	
	/* Get second coordinate (y-coordinate) */
	tx_nav_buffer[0] = 0x51;
	
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);
	twi_request_packet(rx_nav_buffer, SLAVE_ADDR_NAV);
	
	/* Convert uint8_t to uint16_t */
	position_buffer[1] = (uint16_t) ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
}

void arlo_lift_object(Object object_t)
{
	tx_arm_buffer[0] = TWI_CMD_PICK_UP_START;
	tx_arm_buffer[1] = object_t;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_control_arm(tx_arm_buffer, rx_arm_buffer);
}

void arlo_drop_object(Object object_t)
{
	tx_arm_buffer[0] = TWI_CMD_DROP_OFF_START;
	tx_arm_buffer[1] = object_t;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_control_arm(tx_arm_buffer, rx_arm_buffer);
}