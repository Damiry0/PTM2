#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#include "LCD_HD44780.h"

#define bit_is_set(sfr, bit) (_SFR_BYTE(sfr) & _BV(bit))
#define bit_is_clear(sfr, bit) (!(_SFR_BYTE(sfr) & _BV(bit)))

#ifndef cbi
#define cbi(reg, bit) reg &= ~(_BV(bit))
#endif

#ifndef sbi
#define sbi(reg, bit) reg |= (_BV(bit))
#endif

struct Stack
{
    int top; //gora stosu
    unsigned capacity; //ilosc elementow stosu
    int *array; // tablica przechowujaca elementy
    int option; // przechowuje wybrana operacje ( +,-,*,/)
};

struct Stack *createStack(unsigned capacity)
{
    struct Stack *stack = (struct Stack *)malloc(sizeof(struct Stack));
    stack->capacity = capacity;
    stack->top = -1;
    stack->array = (int *)malloc(stack->capacity * sizeof(int));
    stack->option = 0;
    return stack;
}

int isFull(struct Stack *stack)
{
    return stack->top == stack->capacity - 1;
}

// Stos jest pusty kiedy top=-1
int isEmpty(struct Stack *stack)
{
    return stack->top == -1;
}

void push(struct Stack *stack, int item)
{
    if (isFull(stack))
        return;
    stack->array[++stack->top] = item;
}

int pop(struct Stack *stack)
{
    if (isEmpty(stack))
        return INT_MIN;
    return stack->array[stack->top--];
}

void getKey(char *str, Stack *stack)
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
                strncat(str, &ch, 1);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(8);
                char ch = '8';
                strncat(str, &ch, 1);
            }
            if (i == 2)
            {
                {
                    LCD_HD44780::showNumber(9);
                    char ch = '9';
                    strncat(str, &ch, 1);
                }
            }
            if (i == 3)
            {
                {
                    LCD_HD44780::writeText("/");
                    if(isEmpty(stack))push(stack, atoi(str));
                    str[0] = '\0';
                    stack->option = 4;
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
                strncat(str, &ch, 1);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(5);
                char ch = '5';
                strncat(str, &ch, 1);
            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(6);
                char ch = '6';
                strncat(str, &ch, 1);
            }
            if (i == 3)
            {
                LCD_HD44780::writeText("*");
                if(isEmpty(stack))push(stack, atoi(str));
                str[0] = '\0';
                stack->option = 3;
            }
        }
        else if (bit_is_clear(PINC, 6))
        {
            sbi(PORTC, i);
            if (i == 0)
            {
                LCD_HD44780::showNumber(1);
                char ch = '1';
                strncat(str, &ch, 1);
            }
            if (i == 1)
            {
                LCD_HD44780::showNumber(2);
                char ch = '2';
                strncat(str, &ch, 1);
            }
            if (i == 2)
            {
                LCD_HD44780::showNumber(3);
                char ch = '3';
                strncat(str, &ch, 1);
            }
            if (i == 3)
            {
                LCD_HD44780::writeText("-");
                if(isEmpty(stack))push(stack, atoi(str));
                str[0] = '\0';
                stack->option = 2;
            }
        }
        else if (bit_is_clear(PINC, 7))
        {
            if (i == 0)
            {
                LCD_HD44780::showNumber(0);
                char ch = '0';
                strncat(str, &ch, 1);
            }
            if (i == 1)
            {
                LCD_HD44780::clear();
                str[0]= '\0';
                while(!isEmpty(stack))pop(stack); //usuwanie wszystkich elementow ze stosu
            }
            if (i == 2)
            {

                switch (stack->option)
                {
                case 1:
                {
                    int sum[2];
                    sum[0] = atoi(str);
                    sum[1] = pop(stack);
                    int result = sum[0] + sum[1];
                    push(stack, result);
                    LCD_HD44780::clear();
                    LCD_HD44780::showNumber(result);
                }
                break;
                case 2:
                {
                    int sum[2];
                    sum[0] = atoi(str);
                    sum[1] = pop(stack);
                    int result = sum[1] - sum[0];
                    push(stack, result);
                    LCD_HD44780::clear();
                    LCD_HD44780::showNumber(result);
                }
                break;
                case 3:
                {
                    int sum[2];
                    sum[0] = atoi(str);
                    sum[1] = pop(stack);
                    int result = sum[1] * sum[0];
                    push(stack, result);
                    LCD_HD44780::clear();
                    LCD_HD44780::showNumber(result);
                }
                break;
                case 4:
                {
                    int sum[2];
                    sum[0] = atoi(str);
                    sum[1] = pop(stack);
                    int result = sum[1] / sum[0];
                    push(stack, result);
                    LCD_HD44780::clear();
                    LCD_HD44780::showNumber(result);
                }
                break;
                }
            }

            if (i == 3)
            {
                LCD_HD44780::writeText("+");
                if(isEmpty(stack))push(stack, atoi(str));
                str[0] = '\0';
                stack->option = 1;
            }
        }
        sbi(PORTC, i);
        sbi(PORTA, i);
    }
}

int main()
{
    struct Stack *stack = createStack(100);
    DDRC = 0b00001111;
    PORTC = 0xff;
    DDRA = 0b00001111;
        PORTA = 0xff;
    LCD_HD44780::init();
    //LCD_HD44780::writeText("ass");
    char str[] = "";
    while (1)
    {
        getKey(str, stack);
        _delay_ms(1000);
    }
}
