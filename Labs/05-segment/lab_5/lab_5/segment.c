/***********************************************************************
 * 
 * Seven-segment display library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000
#endif
#include <util/delay.h>
#include "gpio.h"
#include "segment.h"

/* Variables ---------------------------------------------------------*/
// Active-low digit 0 to 9
uint8_t segment_value[] = {
    // abcdefgDP
    0b00000011, // Digit 0
    0b10011111, // Digit 1
    0b00100101, // Digit 2
    0b00001101, // Digit 3
    0b10011001, // Digit 4
    0b01001001, // Digit 5
    0b01000001, // Digit 6
    0b00011111, // Digit 7
    0b00000001, // Digit 8
    0b00011001  // Digit 9
};

// Active-high position 0 to 3
uint8_t segment_position[] = {
    // p3p2p1p0....
    0b00010000,     // Position 0
    0b00100000,     // Position 1
    0b01000000,     // Position 2
    0b10000000      // Position 3
};

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: SEG_init()
 * Purpose:  Configure SSD signals LATCH, CLK, and DATA as output.
 * Returns:  none
 **********************************************************************/
void SEG_init(void)
{
    /* Configuration of SSD signals */
    GPIO_config_output(&DDRD, SEG_LATCH);
    GPIO_config_output(&DDRD, SEG_CLK);
    GPIO_config_output(&DDRB, SEG_DATA);
}

/**********************************************************************
 * Function: SEG_update_shift_regs()
 * Purpose:  Display segments at one position of the SSD.
 * Input:    segments - Segments to be displayed (abcdefgDP, active low)
 *           position - Position of the display where the segments are to 
 *                      be displayed (p3 p2 p1 p0 xxxx, active high)
 * Returns:  none
 **********************************************************************/
void SEG_update_shift_regs(uint8_t segments, uint8_t position)
{
    uint8_t bit_number;

    // Pull LATCH, CLK, and DATA low
    GPIO_write_low(&PORT_LATCH, SEG_LATCH);
    GPIO_write_low(&PORT_CLK, SEG_CLK);
    GPIO_write_low(&PORT_DATA, SEG_DATA);

    // Wait 1 us
    _delay_us(1);

    // Loop through the 1st byte (segments)
    // a b c d e f g DP (active low values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "segments" by & (faster) or % (slower) and... 
        // ...output DATA value
        PORT_DATA = ((segments & 1) == 0) ? PORT_DATA & ~(1 << SEG_DATA) : PORT_DATA | (1 << SEG_DATA);
        // Shift "segments"
        segments >>= 1;
        
        // Wait 1 us
        _delay_us(1);
        // Pull CLK high
        PORT_CLK |= (1 << SEG_CLK);
        // Wait 1 us
        _delay_us(1);
        // Pull CLK low
        PORT_CLK &= ~(1 << SEG_CLK);
    }

    // Loop through the 2nd byte (position)
    // p3 p2 p1 p0 . . . . (active high values)
    for (bit_number = 0; bit_number < 8; bit_number++)
    {
        // Test LSB of "position" by & (faster) or % (slower) and... 
        // ...output DATA value
        PORT_DATA = ((position & 1) == 0) ? PORT_DATA & ~(1 << SEG_DATA) : PORT_DATA | (1 << SEG_DATA);
        // Shift "position"
        position = position >> 1;
        
        // Wait 1 us
        _delay_us(1);
        // Pull CLK high
        PORT_CLK |= (1 << SEG_CLK);
        // Wait 1 us
        _delay_us(1);
        // Pull CLK low
        PORT_CLK &= ~(1 << SEG_CLK);
    }

    // Pull LATCH high
    PORT_LATCH |= (1 << SEG_LATCH);
    // Wait 1 us
    _delay_us(1);
}

/**********************************************************************
 * Function: SEG_clear()
 **********************************************************************/
void SEG_clear (void)
{
    for (uint8_t i = 0; i < 4; i++)
        SEG_update_shift_regs(~(uint8_t)0, segment_position[i]);        
}

/**********************************************************************
 * Function: SEG_clk_2us()
 **********************************************************************/
void SEG_clk_2us(void)
{
    
}