/***********************************************************************
 * 
 * Blink a LED and use function from the delay library.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
/* The preprocessor will process directives that are inserted into the C
 * source code. These directives allow additional actions to be taken on
 * the C source code before it is compiled into object code. Directives
 * are not part of the C language itself.
 *
 * Preprocessor directives begin with a pound (#) symbol and may have 
 * several arguments. Do NOT put a semicolon character at the end of 
 * directives. This is a common mistake.
 */
#define LED_GREEN   PB5 // AVR pin where green LED is connected
#define SHORT_DELAY 12 // Delay in milliseconds
#ifndef F_CPU           // Preprocessor directive allows for conditional
                        // compilation. The #ifndef means "if not defined".
# define F_CPU 16000000 // CPU frequency in Hz required for delay
#endif                  // The #ifndef directive must be closed by #endif

/* Includes ----------------------------------------------------------*/
/* Include another C language file into the current file at the location
 * of the #include statement prior to compiling the source code.
 */
#include <util/delay.h> // Functions for busy-wait delay loops
#include <avr/io.h>     // AVR device-specific IO definitions

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED and use delay library.
 * Returns:  none
 **********************************************************************/

// Morse code array A-Z
char *morse_alphabet[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
    "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

// Morse code array 0 - 9
char *morse_numbers[10] = {
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

#define ONE_SPACE		250

#define DOT '.'
#define DOT_DELAY		ONE_SPACE

#define COMMA '-'
#define COMMA_DELAY	    (ONE_SPACE*3)

#define SPACE '/'
#define SPACE_DELAY	    (ONE_SPACE*3)

void dispaly_message_morse_code(char *msg);
void display_char_in_morse_code(char c);
void ll_display_char_in_morse_code(char *c);

void send_space(void);
void send_comma(void);
void send_dot(void);

int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB = DDRB | (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB = PORTB & ~(1<<LED_GREEN);

    // Infinite loop
    while (1)
    {
        dispaly_message_morse_code("DE2/");
    }

    // Will never reach this
    return 0;
}

void dispaly_message_morse_code(char *msg)
{
    char *msg_ptr = msg;
    while(*msg_ptr != '\0') {
        if (*msg_ptr == '/') {
            send_space(); 
        } else {
            display_char_in_morse_code(*msg_ptr);
        }                
        msg_ptr++;
    }
}

void display_char_in_morse_code(char c)
{
    if (c >= 65 && c <= 95) {
        ll_display_char_in_morse_code(morse_alphabet[(uint8_t)c - 65]);
    } else if (c >= 97 && c <= 122) {
        ll_display_char_in_morse_code(morse_alphabet[(uint8_t)c - 97]);
    } else if (c >= 48 && c <= 57) {
        ll_display_char_in_morse_code(morse_numbers[(uint8_t)c - 48]);
    }
}

void ll_display_char_in_morse_code(char *char_codes)
{
    char *msg_ptr = char_codes;
    while(*msg_ptr != '\0') {
        if (*msg_ptr == COMMA)
            send_comma();
        else if (*msg_ptr == DOT)
            send_dot();
        msg_ptr++;
    }        
}

void send_space(void)
{
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(SPACE_DELAY);
}

void send_comma(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(COMMA_DELAY);
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}

void send_dot(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(DOT_DELAY);
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}