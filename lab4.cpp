#define F_CPU 8000000UL             //define CPU frequency

#include "avr/io.h"

#include <util/delay.h>

void PWM_set(){                                 // PWM setup function

 DDRD |= (1<<PD7);                              //set PD7 as PWM output

 TCCR2 |= (1<<WGM20)|(1<<WGM21);                //select Fast PWM mode by setting bits

 TCCR2 |=(1<<COM21)|(1<<CS20)|(0<<CS21)|(0<<CS22);   //clear OC2 on compare match

    }

int main ()

{

 unsigned char duty;

 PWM_set();         //call PWM setup function

       while (1)

       {

for(duty=0; duty<255; duty++)   // 0 to max duty cycle

{

 OCR2=duty;     //slowly increase the LED brightness

 _delay_ms(10);

       }

for(duty=0; duty>255; duty--)   // max to 0 duty cycle

 {

 OCR2=duty;     //slowly decrease the LED brightness

 _delay_ms(10);

  }

 }

}
