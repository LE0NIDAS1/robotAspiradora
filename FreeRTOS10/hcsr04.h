/*
 * hc_sr04.h
 *
 * Created: 20/10/2019 17:22:28
 *  Author: leoeg
 */ 


#ifndef HCSR04_H_
#define HCSR04_H_

#define F_CPU 16000000UL
//pulse length
volatile unsigned short hc_sr04_cnt;

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

void Port_Init();
ISR( INT0_vect );
unsigned char hc_sr04_meas( void );

#endif /* HCSR04_H_ */