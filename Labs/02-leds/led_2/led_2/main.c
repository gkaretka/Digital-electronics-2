/***********************************************************************
 * 
 * Alternately toggle two LEDs when a push button is pressed.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_RED   PC0     // AVR pin where red LED is connected
#define LED_GREEN PB5
#define BUTTON    PD7

#define BLINK_DELAY 500
#ifndef F_CPU
# define F_CPU 16000000     // CPU frequency in Hz required for delay
#endif

/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions

/* Functions ---------------------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    DDRC |= 1 << LED_RED; // output
    DDRB |= 1 << LED_GREEN; // output
    DDRD &= ~(1 << BUTTON); // input
    
    // Reset
    PORTC &= ~(1 << LED_RED); // out 0
    PORTB |= (1 << LED_GREEN); // out 1
    PORTD |= (1 << BUTTON); // pullup
    
    // Infinite loop
    while (1)
    {
        if (((PIND >> BUTTON) & 1) == 0) {
            _delay_ms(BLINK_DELAY);
            PORTC ^= (1 << LED_RED);
            PORTB ^= (1 << LED_GREEN);    
        }
    }

    // Will never reach this
    return 0;
}