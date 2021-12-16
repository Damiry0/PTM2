#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<avr/interrupt.h>
#include <stdio.h>

#include "LCD_HD44780.h"

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif


#define LED PD0
#define SYG PA0

static inline void initADC0(void) {
ADMUX |= (1 << REFS0) | (0 << REFS1); //reference voltage on AVCC
ADCSRA |= (1 << ADPS1) | (1 << ADPS0) | (1 << ADPS2); //ADC clock prescaler /8
ADCSRA |= (1 << ADEN); //enables the ADC
}

/*ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	PORTA^= (1 << SYG);
	LCD_HD44780::clear();
	//LCD_HD44780::showNumber(a);
	TCNT1 = 64755;   // for 1 sec at 8 MHz



	LCD_HD44780::showNumber(min);
	LCD_HD44780::writeText(":");
	LCD_HD44780::showNumber(sec);
	LCD_HD44780::writeText(":");

	LCD_HD44780::showNumber(ms);




}*/


int main()
{
	double potentiometerValue;
	uint16_t threshold_level;
	threshold_level= 0b10000000;
	DDRA |= (1 << PA0); //Data Direction Register B: writing a 1 to the bit enables output
	LCD_HD44780::init();
	initADC0();

	while (1) {
	ADCSRA |= (1 << ADSC); //start ADC conversion
	loop_until_bit_is_clear(ADCSRA, ADSC); //wait until ADC conversion is done
	potentiometerValue= ADC; //read ADC value in
	char text[16];
	int gubno = potentiometerValue * 26.3/100;
	int hh = potentiometerValue * 26.3;
	int h = hh%100;
	LCD_HD44780::showNumber(gubno);
	LCD_HD44780::writeText(".");
	LCD_HD44780::showNumber(h);
	_delay_ms(500);
	LCD_HD44780::clear();

	}





  /*  DDRC = 0b00001111;
    PORTC = 0xff;
    DDRA = 0b00001111;
        PORTA = 0xff;
	//OCR1A |= (1 << WGM13) | (1 << WGM12) ;
	//ICR1=100;
	//TCNT1H |=
	LCD_HD44780::init();
	DDRD = (0x01 << LED);     //Configure the PORTD4 as output

	//TCNT1 = 63974;   // for 1 sec at 8 MHz

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG
while(1){}*/


 }
