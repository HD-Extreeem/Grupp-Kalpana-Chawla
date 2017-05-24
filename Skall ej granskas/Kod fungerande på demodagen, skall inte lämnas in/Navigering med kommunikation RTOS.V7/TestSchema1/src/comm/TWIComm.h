/*
 * TWIComm.h
 *
 * Created: 2017-05-11 10:00:14
 * Author: Jonas Eiselt
 */ 

#ifndef TWICOMM_H_
#define TWICOMM_H_

#include "comm/TWICommHandler.h"

#define TX_ARM_LENGTH 3
#define RX_ARM_LENGTH 3

#define TX_NAV_LENGTH 1
#define RX_NAV_LENGTH 5

void twi_init(void);

void twi_send_packet(uint8_t *tx_buffer, uint8_t recipient_addr);
void twi_request_packet(uint8_t *rx_buffer, uint8_t recipient_addr);

void twi_arm_init(TWI_CMD_Init_Req twi_cmd_init_req_t, uint8_t *tx_buffer, uint8_t *rx_buffer);
void twi_nav_init(uint8_t object_id, uint8_t *tx_nav_buffer, uint8_t *rx_nav_buffer);

void twi_control_arm(uint8_t *tx_buffer, uint8_t *rx_buffer);

void twi_start_pick_up(uint8_t *tx_buffer, uint8_t *rx_buffer);
void twi_start_drop_off(uint8_t *tx_buffer, uint8_t *rx_buffer);

void twi_find_object(uint8_t *tx_buffer, uint8_t *rx_buffer);


void twi_reset_arm(uint8_t *tx_buffer, uint8_t *rx_buffer);
void twi_check_drop_off_status(uint8_t *tx_buffer, uint8_t *rx_buffer);
void twi_check_pick_up_status(uint8_t *tx_buffer, uint8_t *rx_buffer);

void twi_indicate(void);

void twi_check_find_object_status(uint8_t *tx_buffer, uint8_t *rx_buffer);

#endif /* TWICOMM_H_ */