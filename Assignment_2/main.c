/*
 * LCD_ADC_Timer.c
 *
 * Created: 2026-05-19 8:52:28 AM
 * Author : fedbi
 */ 


#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "LCD_PRM.h"
#include "ADC.h"
#include "delay_timer.h"

int main(void)
{

	lcd_init();
	adc_init();
	delay_init();
	sei();
    lcd_set_cursor(0, 0);           //lcd_set_cursor(1, 0);
    lcd_print("Temperature");         //lcd_print("It works! ");
	
	DDRB |= (1<<DDB5);   //setting output direction

    while (1) 
    {
		uint16_t adc_value = read_adc(0);    //calling adc fuction
		float temp = (adc_value * 0.488);    //converting the digital value to volt and to temp
		lcd_set_cursor(1, 0);               //lcd_set_cursor(1, 0);
		lcd_print_float(temp);              //printing float value
		lcd_data(0xDF);                    // printing 
		lcd_print("C");	
    }
}

