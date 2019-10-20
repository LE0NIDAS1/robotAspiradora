/*
 * usart.h
 *
 * Created: 18/10/2019 0:56:30
 *  Author: leoeg
 */ 


#ifndef USART_H_
#define USART_H_

#define F_CPU 16000000UL
#define BAUDRATE 9600
#define BAUD_PRESCALLER ((F_CPU	/ (BAUDRATE * 16UL)))

#define RX_START()      UCSR0B |= _BV(RXEN0)    // Enable RX
#define RX_STOP()       UCSR0B &= ~_BV(RXEN0)   // Disable RX
#define TX_START()      UCSR0B |= _BV(TXEN0)    // Enable TX
#define TX_STOP()       UCSR0B &= ~_BV(TXEN0)   // Disable TX

void USART_init(void);
unsigned char USART_receive(void);
void USART_send( unsigned char data);
void putByte(unsigned char data);
void USART_putstring(char* StringPtr);


#endif /* USART_H_ */