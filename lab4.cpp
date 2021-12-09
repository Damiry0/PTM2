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

int min=0,sec=0,ms=0, index=0;
bool stop=false;
struct miedzy_czas{
	int miedzy_min;
	int miedzy_sec;
	int miedzy_ms;
};
miedzy_czas miedzy1[10];
bool click=false;
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

            }
            if (i == 1)
            {

            }
            if (i == 2)
            {
                {

                }
            }
            if (i == 3)
            {
                {


                }
            }
            miedzy1[i].miedzy_min=min;
            miedzy1[i].miedzy_ms=ms;
            miedzy1[i].miedzy_sec=sec;
            click=true;
            index=i;

        }
        else if (bit_is_clear(PINC, 5))
        {
            sbi(PORTC, i);
            if (i == 0)
            {



            }
            if (i == 1)
            {


            }
            if (i == 2)
            {


            }
            if (i == 3)
            {

            }
        	if(click==true)
        	{
        		LCD_HD44780::goTo(0,1);
        		LCD_HD44780::showNumber(miedzy1[i].miedzy_min);
        		LCD_HD44780::writeText(":");
        		LCD_HD44780::showNumber(miedzy1[i].miedzy_sec);
        		LCD_HD44780::writeText(":");
        		LCD_HD44780::showNumber(miedzy1[i].miedzy_ms);
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

            	stop=false;
            }
            if (i == 1)
            {
                stop=true;

            }
            if (i == 2)
            {

            }

            if (i == 3)
            {
                ms=0;
                sec=0;
                min=0;
                for(int k=0;k<4;k++)
                {
                	miedzy1[k].miedzy_min=0;
                	miedzy1[k].miedzy_sec=0;
                	miedzy1[k].miedzy_ms=0;
                }

            }
        }
        sbi(PORTC, i);
        sbi(PORTA, i);
    }
}

ISR (TIMER1_OVF_vect)    // Timer1 ISR
{
	PORTD ^= (1 << LED);
	if(stop==false)ms +=100;
	LCD_HD44780::clear();
	//LCD_HD44780::showNumber(a);
	TCNT1 = 64755;   // for 1 sec at 8 MHz
	if(ms == 1000)
	{
		sec += 1;
		ms = 0;
	}
	if(sec == 60)
	{
		min += 1;
		sec = 0;
	}

	LCD_HD44780::showNumber(min);
	LCD_HD44780::writeText(":");
	LCD_HD44780::showNumber(sec);
	LCD_HD44780::writeText(":");

	LCD_HD44780::showNumber(ms);

	//LCD_HD44780::goTo(0,1); // przejscie do 2 linii
	getKey();


}


int main()
{
    DDRC = 0b00001111;
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
while(1){}
 }
