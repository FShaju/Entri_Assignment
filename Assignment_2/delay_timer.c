/*
 * delay_timer.c
 *
 * Created: 2026-05-18 12:40:15 PM
 *  Author: fedbi
 */ 
#define F_CPU 16000000UL
#include "delay_timer.h"
#include <avr/io.h>
#include <stdint.h>
#include <avr/interrupt.h>

volatile uint16_t stay =0;

void delay_init (void)
{
	 TCCR0A = 0;
	 TCCR0B = 0;
	
	TCCR0A |= (1<<WGM01);                 // timer0 set to CTC mode
	TCCR0A &= ~(1<<WGM00); 
	
	TCCR0B |= ((1<<CS01)|(1<<CS00));         // Setting prescale to 64
	TCCR0B &= ~(1<<CS02); 
	
	TCNT0 = 0;                              //set TCNT0 with 0
	OCR0A = 249;                            //  setting OCR0A value 250-1
	
	TIMSK0 |= (1<<OCIE0A);             // enabling timer 0 interrupt COMPARE MATCH
	                          //enabling global interrupt

}


ISR (TIMER0_COMPA_vect)
{
	stay++;
	if (stay >= 500)
	{
		PORTB ^= (1<<PORTB5);
		stay = 0;
	}
}

