/*
 * TWI.h
 *
 * Created: 2017-04-29 12:42:35
 * Author: Jonas Eiselt
 *
 * Further reading: 
 *	(1) Atmel-11057C-ATARM-SAM3X-SAM3A-Datasheet_23-Mar-15
 */ 


#ifndef TWI_H_
#define TWI_H_

/* Product mapping (1, p. 31) */
#define TWI0 0x4008C000U
#define TWI1 0x40090000U

/* Register mappings (1, p. 735) */
#define TWI_CR 0x00		// Control register
#define TWI_MMR 0x04	// Master mode register
#define TWI_SMR 0x08	// Slave mode register
#define TWI_IADR 0x0C	// Internal address register
#define TWI_CWGR 0x10	// Clock waveform generator register
#define TWI_SR 0x20		// Status register
#define TWI_IER 0x24	// Interrupt enable register
#define TWI_IDR 0x28	// Interrupt disable register
#define TWI_IMR 0x2C	// Interrupt mask register
#define TWI_RHR 0x30	// Receive Holding register
#define TWI_THR 0x34	// Transmit holding register



#endif /* TWI_H_ */