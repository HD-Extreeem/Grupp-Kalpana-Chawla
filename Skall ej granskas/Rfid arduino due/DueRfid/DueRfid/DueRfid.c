/*
 * main.c
 * 
 * Copyright 2013 Shimon <shimon@monistit.com>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * Modified by Hadi Deknache
 * Programmet läser av id-taggar för att sedan skriva ut deras id i terminalen
 * Programmet är modifierat för att skriva ut i terminalen samt läsa av taggar
 * Endast testad med Arduino Uno återstår att se!
 * Avläsningen sker över SPI kommunikation
 */

#include <stdio.h>
//#include <avr/io.h>
#include "consoleFunctions.h"
#include "asf.h"
//#include "lib/spi.h"
//#include <avr/interrupt.h>
#include "lib/mfrc522.h"
//#include "SPI.h"

uint8_t SelfTestBuffer[64];
	
/* Chip select. */
#define SPI_CHIP_SEL 0
#define SPI_CHIP_PCS spi_get_pcs(SPI_CHIP_SEL)

/* Clock polarity. */
#define SPI_CLK_POLARITY 0

/* Clock phase. */
#define SPI_CLK_PHASE 0

/* Delay before SPCK. */
#define SPI_DLYBS 0x40

/* Delay between consecutive transfers. */
#define SPI_DLYBCT 0x10
//F = 16MHz
//add under Project/ Toolchain/ AVR/GNU C Compiler -> Symbols -> add F_CPU=16000000UL

void spi_selector_init(void);
/*Initialisering För utskrift i terminalen för Arduino UNO (Atmega328P)*/
// void init_UART(void)
// {
// 	//Set baud rate 115200
// 	UBRR0H = 0;
// 	UBRR0L = 16;
// 	//Enable receiver and transmitter
// 	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
// 	//Double Clock Speed
// 	UCSR0A = (1<<U2X0);
// 	// Set frame format: 8data, 1stop bit
// 	UCSR0C = (3<<UCSZ00); //(1<<USBS0)|
// }
// /*Skriver ut i terminal när inkommande tecken finns för utskrift*/
// void uart_putchar(char c, FILE *stream){
// 	//wait until buffer empty
// 	while ( !( UCSR0A & (1<<UDRE0)) );
// 	//Put data into buffer
// 	UDR0 = c;
// }

 
int main(void)
{
	SystemInit();
	board_init();
	sysclk_init();
	delay_init();
	configureConsole();
	spi_master_init(SPI0);
	//init_UART();
	//FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	//stdout = &str_uart;
	
	uint8_t byte;
	uint8_t str[MAX_LEN];
	//delay_ms(50);
	//spi_master_init(SPI0);
	delay_ms(1000);
	
	//spi_master_setup_device(SPI0, &spi_device_conf, SPI_MODE_0, 13,0);
	pmc_enable_periph_clk(ID_PIOA);
	pmc_enable_periph_clk(ID_PIOB);
	pmc_enable_periph_clk(ID_SPI0);
	pio_set_output(PIOA,PIO_PA28,LOW,DISABLE,ENABLE);
	
	pio_set_output(PIOB,PIO_PB3,LOW,DISABLE,ENABLE);
	pio_set_output(PIOB,PIO_PB27,LOW,DISABLE,ENABLE);
	
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA25A_SPI0_MISO);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA26A_SPI0_MOSI);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA27A_SPI0_SPCK);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA28A_SPI0_NPCS0);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA29A_SPI0_NPCS1);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA30A_SPI0_NPCS2);
	pio_set_peripheral(PIOA,PIO_PERIPH_A,PIO_PA31A_SPI0_NPCS3);
	
	
	pio_clear(PIOB,PIO_PB27);
	delay_ms(1000);
	pio_set(PIOB,PIO_PB27);
	delay_ms(1000);
	pio_clear(PIOB,PIO_PB27);
	delay_ms(1000);
	pio_set(PIOB,PIO_PB27);
	
	delay_ms(1000);
	//spi_selector_init();
	
	//spi_enable(SPI0);
	//spi_enable_loopback(SPI0);
	//initialiserar Läsaren
	
// 	pio_set_output(PIOB,PIO_PB26,LOW,DISABLE,ENABLE);
// 	pio_set_pin_high(PIO_PB26);
// 	delay_s(2);
// 	pio_set_pin_low(PIO_PB26);
// 	delay_s(2);
// 	pio_set_pin_high(PIO_PB26);
// 	delay_s(2);
// 	pio_set_pin_low(PIO_PB26);
	mfrc522_init();
	//Kollar version på läsaren
		
		byte = mfrc522_read(VersionReg);
		if(byte == 0x92)
		{
			printf("MIFARE RC522v2\n");
			printf("Detected");
		}else if(byte == 0x91 || byte==0x90)
		{
			printf("MIFARE RC522v1\n");
			printf("Detected\n");
		}else
		{
			printf("No reader found\n");
		}
		
		byte = mfrc522_read(ComIEnReg);
		mfrc522_write(ComIEnReg,byte|0x20);
		byte = mfrc522_read(DivIEnReg);
		mfrc522_write(DivIEnReg,byte|0x80);
		
		delay_ms(1500);

	/*Kollar ifall det finns något på läsaren för att skriva ut det*/
	while(1){
		
		byte = mfrc522_request(PICC_REQALL,str);
		
		if(byte == CARD_FOUND)
		{
			byte = mfrc522_get_card_serial(str);
			/*Skriver ut id på taggen/kortet*/
			if(byte == CARD_FOUND)
			{
				for(byte=0;byte<8;byte++)
				printf("%d", str[byte]);
				
				delay_ms(2500);
			}
			else
			{
				printf("Något Gick fel\n");
			}
			printf("\n");
		}
		
		delay_ms(1000);
	} 
}

// void spi_selector_init(void) {
// 	// Initialize ///////////////////////////////////////////
// 	const spi_settings_t setting = { .delay_between_cs = 12, };
// 
// 	// initialize selector 0
// 	spi_selector_settings_t selector_0;
// 	selector_0.selector = SPI_SELECTOR_0;
// 	selector_0.CPOL = SPI_POLARITY_LOW;
// 	selector_0.NCPHA = SPI_PHASE_LOW;
// 	selector_0.baud_rate = 255;
// 	selector_0.bits_pr_transfer = SPI_BITS_8;
// 	selector_0.delay_clk = 492; // 41*12ns = 492 ns
// 	selector_0.delay_transfers = 381;
// 
// 	spi_init(SPI0, &setting);
// 	spi_init_selector(SPI0, &selector_0);
// 	
// 
// 	spi_enable(SPI0);
// 
// 	spi_enable_loopback(SPI0);
// 
// 	//spi_select_slave(SPI0, SPI_SELECTOR_0); // Slave 0
// 	/////////////////////////////
// }



