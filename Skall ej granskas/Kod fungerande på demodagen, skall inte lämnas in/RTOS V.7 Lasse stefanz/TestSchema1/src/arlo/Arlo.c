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

// uint16_t object_buffer[6] = {0};

uint8_t arlo_collect_status = 1; // 1 = Arlo can collect all objects without driving to box

int16_t sock_x = 0;
int16_t sock_y = 0;

int16_t cube_x = 0;
int16_t cube_y = 0;

int16_t glass_x = 0;
int16_t glass_y = 0;

int16_t box_x = 0;
int16_t box_y = 0;

/* Comment here */
void arlo_init()
{
	// ioport_init();
	twi_init();
	
	/* Initializes arm */
	arlo_arm_init();
	
	/* Initializes nav-system */
	int16_t object_buffer[8] = {0};
	arlo_nav_init(object_buffer);
	delay_ms(1000);
	arlo_nav_init(object_buffer);
	
	printf("Position for sock: %d, %d\r\n", object_buffer[0], object_buffer[1]);
	printf("Position for cube: %d, %d\r\n", object_buffer[2], object_buffer[3]);
	printf("Position for glass: %d, %d\r\n", object_buffer[4], object_buffer[5]);
	printf("Position for box: %d, %d\r\n", object_buffer[6], object_buffer[7]);
}

/* Comment here */
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
	
	/* Get info about the arm's ability to collect */
	twi_arm_init(TWI_CMD_ARM_REQ_COLLECT_INFO, tx_arm_buffer, rx_arm_buffer);
	arlo_collect_status = rx_arm_buffer[2];
}

/* Comment here */
Pick_Up_Status arlo_get_pick_up_status()
{
	uint8_t tx_buffer[TX_ARM_LENGTH] = {0};
	tx_buffer[0] = TWI_CMD_PICK_UP_STATUS;
	tx_buffer[1] = TWI_NO_DATA;
	tx_buffer[2] = TWI_NO_DATA;
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	
	uint8_t rx_buffer[RX_ARM_LENGTH] = {0};
	twi_request_packet(rx_buffer, SLAVE_ADDR_ARM);
	
	Pick_Up_Status pick_up_status_t = rx_buffer[1];
	printf("Pick up status: %d\r\n", pick_up_status_t);
	
	return pick_up_status_t;
}

/* Comment here */
void arlo_nav_init(int16_t *object_buffer)
{
	/* Get coordinate for sock */
	twi_nav_init(0x52, tx_nav_buffer, rx_nav_buffer);
	
	/* Convert uint8_t to uint16_t */
	// object_buffer[0] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	// object_buffer[1] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	sock_x = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	sock_y = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	/* Get coordinate for cube */
	twi_nav_init(0x53, tx_nav_buffer, rx_nav_buffer);
	
	/* Convert uint8_t to uint16_t */
	// object_buffer[2] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	// object_buffer[3] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	cube_x = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	cube_y = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	/* Get coordinate for glass */
	twi_nav_init(0x54, tx_nav_buffer, rx_nav_buffer);
	
	/* Convert uint8_t to uint16_t */
	// object_buffer[4] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	// object_buffer[5] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	glass_x = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	glass_y = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	/* Get coordinate for box */
	twi_nav_init(0x55, tx_nav_buffer, rx_nav_buffer);
	
	/* Convert uint8_t to uint16_t */
	// object_buffer[4] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	// object_buffer[5] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	box_x = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	box_y = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));

	object_buffer[0] = sock_x;
	object_buffer[1] = sock_y;
	object_buffer[2] = cube_x;
	object_buffer[3] = cube_y;
	object_buffer[4] = glass_x;
	object_buffer[5] = glass_y;
	object_buffer[6] = box_x;
	object_buffer[7] = box_y;
}

/* Comment here */
void arlo_get_position(int16_t *position_buffer)
{
	/* Get first coordinate (x-coordinate) */
	tx_nav_buffer[0] = 0x50;
	
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);
	twi_request_packet(rx_nav_buffer, SLAVE_ADDR_NAV);
	
	// printf("Receiving: %d, %d, %d, %d, %d\r\n", rx_nav_buffer[0], rx_nav_buffer[1], rx_nav_buffer[2], rx_nav_buffer[3], rx_nav_buffer[4]);
	// twi_request_int16_packet(rx_nav_buffer, SLAVE_ADDR_NAV);
	
	/* Convert uint8_t to uint16_t */
	//	position_buffer[0] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	//	position_buffer[1] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));

	int16_t x1 = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	int16_t y1 = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));

	/* Get second coordinate (y-coordinate) */
	tx_nav_buffer[0] = 0x51;
	
	twi_send_packet(tx_nav_buffer, SLAVE_ADDR_NAV);
	twi_request_packet(rx_nav_buffer, SLAVE_ADDR_NAV);
	
	/* Convert uint8_t to uint16_t */
	// position_buffer[2] = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	// position_buffer[3] = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	int16_t x2 = ((rx_nav_buffer[1] << 8) | (rx_nav_buffer[2] << 0));
	int16_t y2 = ((rx_nav_buffer[3] << 8) | (rx_nav_buffer[4] << 0));
	
	position_buffer[0] = x1;
	position_buffer[1] = y1;
	position_buffer[2] = x2;
	position_buffer[3] = y2;
}

/* Comment here */
void arlo_pick_up_object(Object object_t)
{
	tx_arm_buffer[0] = TWI_CMD_PICK_UP_START;
	tx_arm_buffer[1] = object_t;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_control_arm(tx_arm_buffer, rx_arm_buffer);
}

/* Comment here */
void arlo_drop_object(Object object_t)
{
	tx_arm_buffer[0] = TWI_CMD_DROP_OFF_START;
	tx_arm_buffer[1] = object_t;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_control_arm(tx_arm_buffer, rx_arm_buffer);
}

/* Comment here */
uint8_t arlo_get_collect_status()
{
	return arlo_collect_status;
}

/* Comment here */
Drop_Off_Status arlo_get_drop_off_status()
{
	uint8_t tx_buffer[TX_ARM_LENGTH] = {0};
	tx_buffer[0] = TWI_CMD_DROP_OFF_STATUS;
	tx_buffer[1] = TWI_NO_DATA;
	tx_buffer[2] = TWI_NO_DATA;
	twi_send_packet(tx_buffer, SLAVE_ADDR_ARM);
	
	uint8_t rx_buffer[RX_ARM_LENGTH] = {0};
	twi_request_packet(rx_buffer, SLAVE_ADDR_ARM);
	
	Drop_Off_Status drop_off_status_t = rx_buffer[1];
	printf("Drop off status: %d\r\n", drop_off_status_t);
	
	return drop_off_status_t;
}

void arlo_done_drive(Pick_Up_Status pick_up_status_t)
{
	tx_arm_buffer[0] = TWI_CMD_PICK_UP_STATUS;
	tx_arm_buffer[1] = pick_up_status_t;
	tx_arm_buffer[2] = TWI_NO_DATA;
	
	twi_control_arm(tx_arm_buffer, rx_arm_buffer);
}

void arlo_get_object_positions(int16_t *object_buffer)
{
	object_buffer[0] = sock_x;
	object_buffer[1] = sock_y;
	
	object_buffer[2] = cube_x;
	object_buffer[3] = cube_y;
	
	object_buffer[4] = glass_x;
	object_buffer[5] = glass_y;
	
	object_buffer[6] = box_x;
	object_buffer[7] = box_y;
}