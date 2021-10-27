/***********************************************************************
 * 
 * Stopwatch with LCD display output.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

#ifndef F_CPU
#define F_CPU 16000000L
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include "lcd.h"            // Peter Fleury's LCD library
#include <stdlib.h>         // C library. Needed for conversion function
#include <util/delay.h>

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Update stopwatch value on LCD display when 8-bit 
 *           Timer/Counter2 overflows.
 * Returns:  none
 **********************************************************************/

uint8_t customChars[] = {
    0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000, 0b00000,
    0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000, 0b10000,
    0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000, 0b11000,
    0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100, 0b11100,
    0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110, 0b11110,
    0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111, 0b11111,
};

void store_custom_char(uint8_t addr, uint8_t *custom_char);

int main(void)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);
    
    for (uint8_t i = 0; i < 1; i++) store_custom_char(i, customChars);
    
    lcd_clrscr();
    for (uint8_t i = 0; i < 8; i++) lcd_putc(i);
    
    _delay_ms(1000);
    // Put string(s) at LCD display
    lcd_gotoxy(1, 0);
    lcd_puts("LCD Test");
    lcd_putc('!');
    _delay_ms(1000);
    
    // Configure 8-bit Timer/Counter2 for Stopwatch
    // Set the overflow prescaler to 16 ms and enable interrupt
    lcd_clrscr();
    TIM2_overflow_16384us();
    TIM2_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter2 overflow interrupt
 * Purpose:  Update the stopwatch on LCD display every sixth overflow,
 *           ie approximately every 100 ms (6 x 16 ms = 100 ms).
 **********************************************************************/
ISR(TIMER2_OVF_vect)
{
    static uint8_t number_of_overflows = 0;
    static uint16_t cnt_s = 0;
    static uint16_t cnt_m = 0;
    static int pow10[5] = { 1, 10, 100, 1000, 10000 };
    
    number_of_overflows++;
    if (number_of_overflows >= 6)
    {
        // Do this every 6 x 16 ms = 100 ms
        number_of_overflows = 0;
        char to_display;
        
        lcd_gotoxy(1, 0);
        for (int8_t pos = 1; pos >= 0; pos--) {
            to_display = ((cnt_m % pow10[pos+1]) / (pow10[pos])) + '0';
            lcd_putc(to_display);
            if (pos == 0) lcd_putc(':');
        }
        
        for (int8_t pos = 3; pos >= 0; pos--) {
            to_display = ((cnt_s % pow10[pos+1]) / (pow10[pos])) + '0';
            lcd_putc(to_display);
            if (pos == 2) lcd_putc('.');
        }
         
        lcd_gotoxy(11, 0);
        // seconds to power of 2
        uint8_t seconds = ((cnt_s % pow10[4]) / (pow10[3])) * 10 + ((cnt_s % pow10[3]) / (pow10[2]));
        uint16_t cnt_s_pow_2 = seconds * seconds;
        for (int8_t pos = 3; pos >= 0; pos--) {
            to_display = ((cnt_s_pow_2 % pow10[pos+1]) / (pow10[pos])) + '0';
            lcd_putc(to_display);
        }
        
        lcd_gotoxy(1, 1);
        lcd_putc('b');
        lcd_gotoxy(11, 1);
        lcd_putc('c');
        
        if (cnt_s < 6000) cnt_s += 10;
        else {
            cnt_s = 0;
            if (cnt_m < 60) cnt_m++;
            else cnt_m = 0;
        }            
    }
    // Else do nothing and exit the ISR
}

void store_custom_char(uint8_t addr, uint8_t *custom_char)
{
    // Initialize LCD display
    lcd_init(LCD_DISP_ON);

    // Set pointer to beginning of CGRAM memory
    lcd_command(addr << LCD_CGRAM);
    for (uint8_t i = 0; i < 8; i++)
    {
        // Store all new chars to memory line by line
        lcd_data(custom_char[i+addr]);
    }
    // Set DDRAM address
    lcd_command(1 << LCD_DDRAM);
}