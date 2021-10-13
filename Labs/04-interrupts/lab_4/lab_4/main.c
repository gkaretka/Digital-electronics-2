/***********************************************************************
 * 
 * Control LEDs using functions from GPIO and Timer libraries. Do not 
 * use delay library any more.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2018-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#define LED_D1  PB5
#define LED_D2  PB4
#define LED_D3  PB3
#define LED_D4  PB2

#define BUTTON  PC1

#ifndef F_CPU           // Preprocessor directive allows for conditional
// compilation. The #ifndef means "if not defined".
#define F_CPU 16000000  // CPU frequency in Hz required for delay
#endif                  // The #ifndef directive must be closed by #endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include <util/delay.h>     // Functions for busy-wait delay loops
#include "gpio.h"           // GPIO library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC

static inline void knight_rider();

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle one LED on the Multi-function shield using 
             the internal 8- or 16-bit Timer/Counter.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Configuration of LED(s) at port B - active low
    GPIO_config_output(&DDRB, LED_D1);
    GPIO_write_high(&PORTB, LED_D1); // high
    
    GPIO_config_output(&DDRB, LED_D2);
    GPIO_write_low(&PORTB, LED_D2); // low
        
    GPIO_config_output(&DDRB, LED_D3);
    GPIO_write_low(&PORTB, LED_D3); // low
            
    GPIO_config_output(&DDRB, LED_D4);
    GPIO_write_low(&PORTB, LED_D4); // low
    
    GPIO_config_input_pullup(&DDRC, BUTTON);

    // Configuration of 16-bit Timer/Counter1 for LED blinking
    // Set the overflow prescaler to 262 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();
    
    enum speed_states {SPEED_4MS = 0, SPEED_33MS = 1, SPEED_262MS = 2, SPEED_1S = 3, SPEED_4S = 4};
    uint8_t speed_state = SPEED_262MS;
    
    // Infinite loop
    while (1)
    {
        /* Empty loop. All subsequent operations are performed exclusively 
         * inside interrupt service routines ISRs */
        
        if (GPIO_read(&PINC, BUTTON) == 0) {
            switch(speed_state) {
                case (SPEED_4S):
                    speed_state = SPEED_1S;
                    TIM1_overflow_1s();
                    break;
                    
                case (SPEED_1S):
                    speed_state = SPEED_262MS;
                    TIM1_overflow_262ms();
                    break;
                
                case (SPEED_262MS):
                    speed_state = SPEED_33MS;
                    TIM1_overflow_33ms();
                    break;
                
                case (SPEED_33MS):
                    speed_state = SPEED_4MS;
                    TIM1_overflow_4ms();
                    break;
                
                case (SPEED_4MS):
                    speed_state = SPEED_4S;
                    TIM1_overflow_4s();
                    break;
                    
                default:
                    speed_state = SPEED_4S;
                    TIM1_overflow_4s();
                    break;
            }
            
            _delay_ms(500);
        }
    }

    // Will never reach this
    return 0;
}

/* Interrupt service routines ----------------------------------------*/
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Toggle D1 LED on Multi-function shield.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    knight_rider();
}

static inline void knight_rider()
{
    static uint8_t state = (1 << LED_D1);
    static uint8_t dir = 0;
    if (dir == 0) {
        state =  (state >> 1);
        PORTB = ~state;
        if (state == (1 << LED_D4)) dir = 1;
    } else {
        state = (state << 1);
        PORTB = ~state;
        if (state == (1 << LED_D1)) dir = 0;
    }
}