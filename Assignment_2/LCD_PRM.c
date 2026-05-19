
#include "LCD_PRM.h"
#define F_CPU 16000000UL


static void lcd_pulse_enable ()
{
	LCD_EN_PORT |= (1<<LCD_EN_PIN);  //setting EN=1
	_delay_ms(1);                    // these delay values come from datasheet
	LCD_EN_PORT &= ~(1<<LCD_EN_PIN);  //setting EN=0
	_delay_ms(50);                    // allow LCD to latch
}



static void lcd_send_nibble (uint8_t nib)
{
	LCD_D_PORT &= ~((1<<LCD_D7_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D4_PIN));  // Making all data pins 0 initially
	
	if (nib & 0x08) LCD_D_PORT |=(1<<LCD_D7_PIN);  //setting bit for D7
	if (nib & 0x04) LCD_D_PORT |=(1<<LCD_D6_PIN);  //setting bit for D6 
	if (nib & 0x02) LCD_D_PORT |=(1<<LCD_D5_PIN);  //setting bit for D5
	if (nib & 0x01) LCD_D_PORT |=(1<<LCD_D4_PIN);  //setting bit for D4
	lcd_pulse_enable();                              //EN = 0
}


static void lcd_send_byte (uint8_t byt, uint8_t rs)     // receive data and rs value
{
	if (rs==1)
	{
		LCD_RS_PORT |=  (1<<LCD_RS_PIN);       // IT’S A Data. SO RS pin =1
	}
	
	else
	{
		LCD_RS_PORT &=  ~(1<<LCD_RS_PIN);       // IT’S A Command. SO RS pin =0
	}
	
	lcd_send_nibble((byt >>4)& 0x0F);         //sending upper nibble
	lcd_send_nibble(byt & 0X0F);      //sending upper nibble
}

void lcd_set_cursor(uint8_t row, uint8_t col)       //function for cursor location
{
	uint8_t addr = (row? 0x40:0x00) + (col & 0x0F);
	lcd_cmd (0x80 | addr);          // send (80 + addr) as a command.
}

void lcd_cmd (uint8_t comd)          // this function sends commands (instructions)
{
	lcd_send_byte(comd,0);
	
	if (comd == 0x01 || comd == 0x02)   // Clear (0x01) and Return Home (0x02) need longer delays in write-only mode
	_delay_ms(2);
}

void lcd_data (uint8_t dt)        // this function sends data (instructions)
{
	lcd_send_byte(dt,1);
}

void lcd_print (const char *str)      //This function allows us to print a string
{
	while(*str)
	{
		lcd_data((uint8_t)*str++);        //Every string ends with ‘\0’. This command checks whether *s=\0
	}
}


void lcd_clear()            // this function allows us to clear the lcd
{
	lcd_cmd(0x01);
}


void lcd_home() // this function allows us to return the cursor to home
{
	lcd_cmd(0x02);
}

 
void lcd_init()
{
	// Make control and data pins outputs
	LCD_RS_DDR |= (1<<LCD_RS_PIN);           // Setting data direction registers for RS and EN pin
	LCD_EN_DDR |= (1<<LCD_EN_PIN);
	LCD_D_DDR  |= (1<<LCD_D7_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D4_PIN); // setting DDR for data pins
	
	LCD_TS_DDR |=(1<<LCD_TS_PIN);
	LCD_TS_PORT &= ~(1<<LCD_TS_PIN);
	
	_delay_ms(25);         //wait for power ON
	
	LCD_EN_PORT &= ~(1<<LCD_EN_PIN);   //RS=0
	
	lcd_send_nibble (0X03);        //Function Set
	_delay_ms(8);
	lcd_send_nibble (0X03);       //Function Set
	_delay_ms(1);
	lcd_send_nibble (0X03);       //Function Set
	_delay_ms(1);
	lcd_send_nibble (0X02);       //Function set  (Set interface to be 4 bits long.)
	_delay_ms(1);
	
	// note that from here onwards its cmd !
	
	lcd_cmd (0X28);              //Function set  (Interface is 4 bits long.  Specify the number of display lines and character font.) (4-bit, 2 lines, 5x8 font)
	
	lcd_cmd (0X08);              //Display off 
	
	lcd_cmd (0X01);              //Display clear
	
	lcd_cmd (0X06);              //Entry mode set
	
	lcd_cmd (0X0C);              //Display on, cursor off, blink off 
	
	
}
// function to print integers. (We use recursion)
void lcd_print_uint16(uint16_t v)                                     //        lcd_print_uint1(1432);
{
	if (v >= 10)
	{
		lcd_print_uint16(v / 10);  		                 	 // print higher digits first
	}
	lcd_data('0' + (v % 10));       				// then print the last digit
}

void lcd_print_float(float value)
{
	// Handle negative numbers
	if (value < 0)
	{
		lcd_data('-');
		value = -value;
	}

	// Integer part
	uint16_t int_part = (uint16_t)value;
	lcd_print_uint16(int_part);

	lcd_data('.');

	// Fractional part (2 digits)
	float frac = value - int_part;
	frac = frac * 100.0f;       // scale to 2 decimal places
	uint16_t frac_part = (uint16_t)(frac + 0.5f);  // rounding

	// Handle leading zero after decimal (e.g., 3.05)
	if (frac_part < 10)
	lcd_data('0');

	lcd_print_uint16(frac_part);
}

	
	
	
