#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include<avr/interrupt.h>

#include "LCD_HD44780.h"

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif


#define LED PD4

void getKey()
{
    for (int i = 0; i < 4; i++)
    {
        cbi(PORTC, i);
        cbi(PORTA, i);
        if (bit_is_clear(PINC, 4))
        {
            sbi(PORTA, i);
            sbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(7);
                char ch = '7';
               // strncat(str, &ch, 1);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(8);
                char ch = '8';
              //  strncat(str, &ch, 1);
            }
            if (i == 2)
            {
                {
                    LCD_HD44780::showNumber(9);
                    char ch = '9';
                  //  strncat(str, &ch, 1);
                }
            }
            if (i == 3)
            {
                {
                    LCD_HD44780::writeText("/");

                }
            }
        }
        else if (bit_is_clear(PINC, 5))
        {
            sbi(PORTC, i);
            if (i == 0)
            {

                LCD_HD44780::showNumber(4);
                char ch = '4';

            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(5);
                char ch = '5';

            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(6);
                char ch = '6';

            }
            if (i == 3)
            {
                LCD_HD44780::writeText("*");

            }
        }
        else if (bit_is_clear(PINC, 6))
        {
            sbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(1);

            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(2);

            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(3);

            }
            if (i == 3)
            {
                LCD_HD44780::writeText("-");

            }
        }
        else if (bit_is_clear(PINC, 7))
        {
            if (i == 0)
            {
                LCD_HD44780::showNumber(0);

            }
            if (i == 1)
            {
                LCD_HD44780::clear();

            }
            if (i == 2)
            {

            }

            if (i == 3)
            {
                LCD_HD44780::writeText("+");

            }
        }
        sbi(PORTC, i);
        sbi(PORTA, i);
    }
}

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	PORTD ^= (1 << LED);
	TCNT1 = 61630;   // for 1 sec at 16 MHz
}

int main()
{
	//OCR1A |= (1 << WGM13) | (1 << WGM12) ;
	//ICR1=100;
	//TCNT1H |=
	DDRD = (0x01 << LED);     //Configure the PORTD4 as output

	//TCNT1 = 63974;   // for 1 sec at 16 MHz

	TCCR1A = 0x00;
	TCCR1B = (1<<CS10) | (1<<CS12);;  // Timer mode with 1024 prescler
	TIMSK = (1 << TOIE1) ;   // Enable timer1 overflow interrupt(TOIE1)
	sei();        // Enable global interrupts by setting global interrupt enable bit in SREG

 while(1)
 {



 }
 }
