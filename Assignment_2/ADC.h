/*
 * ADC.h
 *
 * Created: 2026-05-07 9:47:08 PM
 *  Author: fedbi
 */ 


#ifndef ADC_H_
#define ADC_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdint.h>

void adc_init(void);
uint16_t read_adc(uint8_t channel);



#endif /* ADC_H_ */