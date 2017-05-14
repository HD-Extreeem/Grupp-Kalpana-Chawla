/*
 * TWIComm.h
 *
 * Created: 2017-05-11 10:00:14
 * Author: Jonas Eiselt
 */ 

#ifndef TWICOMM_H_
#define TWICOMM_H_

#define TX_DATA_LENGTH 1
#define RX_DATA_LENGTH 3

void twi_init(void);

void twi_send_packet(uint8_t request_byte, uint8_t recipient_addr);
void twi_request_packet(uint8_t recipient_addr);

Bool twi_crane_init(void);
void twi_check_data(uint8_t recipient_addr);

void twi_indicate(void);

#endif /* TWICOMM_H_ */