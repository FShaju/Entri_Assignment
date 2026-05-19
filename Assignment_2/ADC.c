/*
 * ADC.c
 *
 * Created: 2026-05-07 9:46:53 PM
 *  Author: fedbi
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include "ADC.h"

void adc_init(void)
{
	//step 1 : setting the reference voltage (5 V) 
	ADMUX |= /*((1<<REFS1)|(1<<REFS0))*/(1<<REFS0);
	//step 2 : ADC enable
	
	ADCSRA |= (1<<ADEN);
	//step 3: choosing prescaler value 128-125 Khz
	ADCSRA |= ((1<<ADPS2) | (1<<ADPS1)| (1<<ADPS0));
	
}
/* read adc vlaue from the corresponding given channel (0-7)*/

uint16_t read_adc(uint8_t channel)
{
	/* our channel limit 0-7
	8 (0000 1000) or 12 (0000 1100)
	for 8 channel 0 select .for 12 channel 4 select */
	
	channel &= 0x07;
	
	//clear previous channel and update the register with new channel
	ADMUX = (ADMUX & 0xF8)|channel;
	// Start ADC conversion;
	ADCSRA |= (1<< ADSC);
	// Step 7: wait until the conversion is done or completed
	while (ADCSRA & (1<<ADSC));
	// Step 8: return ADC
	return ADC;
}