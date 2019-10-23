/*
 * hc_sr04.c
 *
 * Created: 20/10/2019 17:24:13
 *  Author: leoeg
 */ 
#include "hcsr04.h"

void Port_Init()
{
	
	DDRD |= 0b00001000; //	Set all pins of the PORTD as output, PORTD3 as input.
	PORTD = 0b00000000; // Set all pins of PORTD low which turns it off.
	
	EIMSK |= (1<<INT0); //enable external interrupt
	EICRA |= (1<<ISC00); // Any logical change on INT0 generates an interrupt request.

	//Config and enable Timer1
	TCCR1B = (0<<CS12)|(1<<CS11)|(1<<CS10);
}

ISR( INT0_vect )
{
	
	if(PIND & (1 << 2)) {TCNT1 = 0;}/* Clear Timer counter */
	else  {hc_sr04_cnt = TCNT1;}
	

}

unsigned char hc_sr04_meas( void )
{
	
	hc_sr04_cnt = 0;
	
	PORTD |=  (1 << 3);
	_delay_us( 10 );
	PORTD &= ~(1 << 3);


	while( hc_sr04_cnt == 0 );
	
	if (hc_sr04_cnt<3700) {return 0.000004 * hc_sr04_cnt/2 * 34300;} else {return 41;}
	
}