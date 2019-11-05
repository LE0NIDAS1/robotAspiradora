/*
 * usart.c
 *
 * Created: 18/10/2019 0:19:28
 *  Author: leoeg
 */ 

#include "usart.h"
#include <avr/io.h>	

#define RX_BUFF         10

volatile char Rec_Buffer[16];

void USART_init(void){
	
	UBRR0H = (uint8_t)(BAUD_PRESCALLER>>8);
	UBRR0L = (uint8_t)(BAUD_PRESCALLER);
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);
	
	TX_START();
	RX_START();
	
	UCSR0C = (3<<UCSZ00);
	
	
}

unsigned char USART_Receive (void)
{
        while((!(UCSR0A)) & (1<<RXC0));                     // wait while data is being received
        return UDR0;                                      // return 8-bit data
}

void USART_send( unsigned char data){
	
	while(!(UCSR0A & (1<<UDRE0)))
	;

	UDR0 = data;
}

unsigned char recibe_caracter_usart(){
	if(UCSR0A&(1<<7)){  //si el bit7 del registro UCSR0A se ha puesto a 1
		return UDR0;    //devuelve el dato almacenado en el registro UDR0
	}
	else//sino
	return 0 ;//retorna 0
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

uint8_t getByte(void)
{
	// Check to see if something was received
	while (!(UCSR0A & _BV(RXC0)));
	return (uint8_t) UDR0;
}

char* readString(void)
{
	static char rxstr[RX_BUFF];
	static char* temp;
	temp = rxstr;

	while((*temp = getByte()) != '\n')
	{
		++temp;
	}

	return rxstr;
}
