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
#define LED0 PC0
#define LED1 PC1
#define LED2 PC2
#define LED3 PC3
#define LED4 PC4

#define LED_DDR DDRC
#define LED_PORT PORTC

#define BUTTON PB1
#define BUTTON_DDR DDRB
#define BUTTON_PIN PINB

#define PWM_EN 1
#define INVERTED 0
#define BTN_INVERTED 1

#define INVERT(b) ( (INVERTED == 1) ? (~b) : (b) )

#ifndef F_CPU
#define F_CPU 16000000     // CPU frequency in Hz required for delay
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
    LED_DDR |= ((1 << LED4) | (1 << LED3)  | (1 << LED2) | (1 << LED1) | (1 << LED0)); // led outputs
    BUTTON_DDR &= ~(1 << BUTTON); // buttons
    
    uint8_t register_storage = 0;
    uint8_t sample_data = 0;
    uint8_t start = 0;
    uint8_t perform = 0;
    
    // Infinite loop
    while (1)
    {
        // shift register sampler, edge detector
        _delay_ms(50);
        sample_data = (sample_data << 1) | ((((BUTTON_PIN >> BUTTON) & 1) == BTN_INVERTED));
        
        if ((sample_data & 0xF) == 0x0F) start = (start << 1) | 1; // debounce
        else if ((sample_data & 0b0000) == 0b000) start = start << 1;
        
        if (start == 0b0011) perform = 1; // edge detection
        
        if (perform == 1) {
            for (uint8_t i = 0; i < 5; i++) {
                register_storage = 1 << i;
                if (PWM_EN) {
                    // UP
                    for (uint8_t j = 0; j < 255; j++) {
                        LED_PORT = INVERT(LED_PORT | register_storage);
                        for (uint8_t k = 0; k < j/10; k++) _delay_us(10);
                        LED_PORT = INVERT(LED_PORT & ~(register_storage));
                        for (uint8_t k = 0; k < (255-j/10); k++) _delay_us(10);
                    }
                    
                    // DOWN
                    for (uint8_t j = 255; j > 0; j--) {
                        LED_PORT = INVERT(LED_PORT | register_storage);
                        for (uint8_t k = 0; k < j/10; k++) _delay_us(10);
                        LED_PORT = INVERT(LED_PORT & ~(register_storage));
                        for (uint8_t k = 0; k < (255-j/10); k++) _delay_us(10);
                    }
                } else {
                    LED_PORT = INVERT(register_storage);
                    _delay_ms(500);
                }                
            }
                            
            for (uint8_t i = 0; i < 5; i++) {
                register_storage = 1 << (4-i);
                if (PWM_EN) {
                    // UP
                    for (uint8_t j = 0; j < 255; j++) {
                        LED_PORT = INVERT(LED_PORT | register_storage);
                        for (uint8_t k = 0; k < j/10; k++) _delay_us(10);
                        LED_PORT = INVERT(LED_PORT & ~(register_storage));
                        for (uint8_t k = 0; k < (255-j/12); k++) _delay_us(10);
                    }
                    
                    // DOWN
                    for (uint8_t j = 255; j > 0; j--) {
                        LED_PORT = INVERT(LED_PORT | register_storage);
                        for (uint8_t k = 0; k < j/10; k++) _delay_us(10);
                        LED_PORT = INVERT(LED_PORT & ~(register_storage));
                        for (uint8_t k = 0; k < (255-j/10); k++) _delay_us(10);
                    }
                } else {
                    LED_PORT = INVERT(register_storage);
                    _delay_ms(500);
                }
            }
                            
            LED_PORT = INVERT((LED_PORT & ~((1 << LED4) | (1 << LED3)  | (1 << LED2) | (1 << LED1) | (1 << LED0)))); // disable all
            perform = 0; // hold edge detector
            start = 0b10; // ...
        }
    }

    // Will never reach this
    return 0;
}