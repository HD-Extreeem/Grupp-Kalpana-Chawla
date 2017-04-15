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
#include "asf.h"
#include <stdio.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "lib/spi.h"
#include "lib/mfrc522.h"

uint8_t SelfTestBuffer[64];


//F = 16MHz
//add under Project/ Toolchain/ AVR/GNU C Compiler -> Symbols -> add F_CPU=16000000UL


/*Initialisering För utskrift i terminalen för Arduino UNO (Atmega328P)*/
void init_UART(void)
{
	//Set baud rate 115200
	UBRR0H = 0;
	UBRR0L = 16;
	//Enable receiver and transmitter
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	//Double Clock Speed
	UCSR0A = (1<<U2X0);
	// Set frame format: 8data, 1stop bit
	UCSR0C = (3<<UCSZ00); //(1<<USBS0)|
}
/*Skriver ut i terminal när inkommande tecken finns för utskrift*/
void uart_putchar(char c, FILE *stream){
	//wait until buffer empty
	while ( !( UCSR0A & (1<<UDRE0)) );
	//Put data into buffer
	UDR0 = c;
}

 
int main()
{
	init_UART();
	FILE str_uart = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &str_uart;

	uint8_t byte;
	uint8_t str[MAX_LEN];
	_delay_ms(50);
	
	
	spi_init();
	_delay_ms(1000);
	
	
	//initialiserar Läsaren
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
		
		_delay_ms(1500);

	
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
				
				_delay_ms(2500);
			}
			else
			{
				printf("Något Gick fel\n");
			}
			printf("\n");
		}
		
		_delay_ms(1000);
	} 
}
