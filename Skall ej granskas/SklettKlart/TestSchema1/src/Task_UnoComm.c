/**
* Task_UnoComm.c
*
* Created: 2017-04-20 16:25:09
*  Author: Desiree
*/

#include <asf.h>
#include "Task_UnoComm.h"
#include <FreeRTOS.h>



#define UNO_BUS_ADDR 0x50       // TWI slave bus address
#define TWI_SPEED 100000		// TWI data transfer rate

#define TX_DATA_LENGTH sizeof(tx_buffer)
#define RX_DATA_LENGTH 3

static uint8_t tx_buffer[] = {0x20};
static uint8_t rx_buffer[RX_DATA_LENGTH] = {0};

//void configure_twi(void);
void indicate(void);

Bool crane_init(void);

//void send_data(uint8_t request_byte);
//void request_data(void);
//void check_data(void);

//static twi_package_t tx_packet;
//static twi_package_t rx_packet;

freertos_twi_if freertos_twi;
freertos_peripheral_options_t driver_options;

twi_packet_t tx_packet;
twi_packet_t rx_packet;



void task_unoComm(void *pvParameters)
{



	configure_twi();
	portTickType xLastWakeTime;
	const portTickType xTimeIncrement = 500;
	//const char req[] = "00000000";
	
	xLastWakeTime = xTaskGetTickCount(); // Initialize the xLastWakeTime variable with the current time.
	
	while (1)
	{
		vTaskDelayUntil(&xLastWakeTime, xTimeIncrement);	// Wait for the next cycle.
		printf("UnoComm task\r\n");
		//while (!crane_init());
		//indicate();
		//crane_init();
	
		
		freertos_twi_write_packet(freertos_twi,tx_packet,10);
		//send_data(0x10);
		//request_data();
			printf("halvvägs\n");
		printf("UnoComm tallllllsk\r\n");
	}
}

void configure_twi()
{
	driver_options = {NULL,0,0x0e,TWI_I2C_MASTER,(USE_TX_ACCESS_MUTEX | USE_RX_ACCESS_MUTEX)};
	freertos_twi = freertos_twi_master_init(TWI0, &driver_options);
	if (freertos_twi != NULL)
	{
		twi_set_speed(TWI0,TWI_SPEED,84000000);
	}


	twi_packet_t tx_packet={

		/* Configures packet to be transmitted */
		.addr[0] = 0,							// TWI slave memory address data MSB
		.addr[1] = 0,							// TWI slave memory address data LSB
		.addr_length = 0,						// TWI slave memory address data size
		.chip = UNO_BUS_ADDR,					// TWI slave bus address
		.buffer = (void*) tx_buffer,			// transfer data source buffer
		.length = TX_DATA_LENGTH					// transfer data size (bytes)
	};

}

//-------



/* Initializes TWI master options */
//	twi_master_options_t opt;
//	opt.speed = TWI_SPEED;

/* Initializes the TWI master driver */
//	twi_master_setup(TWI0, &opt);

/* Configures packet to be transmitted */
//	tx_packet.addr[0] = 0;							// TWI slave memory address data MSB
//	tx_packet.addr[1] = 0;							// TWI slave memory address data LSB
//	tx_packet.addr_length = 0;						// TWI slave memory address data size
//	tx_packet.chip = UNO_BUS_ADDR;					// TWI slave bus address
//	tx_packet.buffer = (void*) tx_buffer;			// transfer data source buffer
//	tx_packet.length = TX_DATA_LENGTH;					// transfer data size (bytes)

/* Configures packet to be received */
//	rx_packet.addr[0] = 0;							// TWI slave memory address data MSB
//	rx_packet.addr[1] = 0;							// TWI slave memory address data LSB
//	rx_packet.addr_length = 0;						// TWI slave memory address data size
//	rx_packet.chip = UNO_BUS_ADDR;					// TWI slave bus address
//	rx_packet.buffer = (void*) rx_buffer;		// transfer data destination buffer
//	rx_packet.length = RX_DATA_LENGTH;					// transfer data size (bytes)

///////////////////////----------------


// Declare the variables used as parameters to the
// freertos_twi_master_init() function.
// Handle used to access the initialized port by other FreeRTOS ASF functions.


// Configuration structure.



//freertos_peripheral_options_t twiOptions;
//twiOptions.receive_buffer=(void*) rx_buffer;		// transfer data destination buffer
//twiOptions.receive_buffer_size=RX_DATA_LENGTH;
//twiOptions.operation_mode=TWI_I2C_MASTER;



Bool crane_init()
{
	printf("crane_init\r\n");
	send_data(0x20);
	request_data();

	// indicate();

	/* Check received data */
	check_data();
	request_data();

	return true;
}


void send_data(uint8_t request_byte)
{
	
	printf("crankkkke_init\r\n");
	tx_buffer[0] = request_byte;

	// Performs a multi-byte write access then check the result.
	//while (twi_master_write(TWI0, &tx_packet) != TWI_SUCCESS);
	
	//	while (freertos_twi_master_init(TWI0,&tx_packet)!= TWI_SUCCESS);
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
