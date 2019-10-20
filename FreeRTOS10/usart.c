/*
 * usart.c
 *
 * Created: 18/10/2019 0:19:28
 *  Author: leoeg
 */ 

#include "usart.h"
#include <avr/io.h>	

void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	TX_START();
	RX_START();
	
	UCSR0C = (3<<UCSZ00);
	
	
}

unsigned char USART_receive(void){
	
	while(!(UCSR0A & (1<<RXC0)))
	;

	return UDR0;
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)))
	;

	UDR0 = data;
}

void putByte(unsigned char data)
{
	// Stay here until data buffer is empty
	while (!(UCSR0A & _BV(UDRE0)));
	UDR0 = (unsigned char) data;

}

void USART_putstring(char* StringPtr){
	
	while(*StringPtr != 0x00){
		USART_send(*StringPtr);
		StringPtr++;
	}
}