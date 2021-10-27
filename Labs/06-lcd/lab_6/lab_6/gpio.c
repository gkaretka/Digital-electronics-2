/***********************************************************************
 * 
 * GPIO library for AVR-GCC.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2019-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 *
 **********************************************************************/

/* Includes ----------------------------------------------------------*/
#include "gpio.h"

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: GPIO_config_output()
 * Purpose:  Configure one output pin in Data Direction Register.
 * Input:    reg_name - Address of Data Direction Register, such as &DDRB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_config_output(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name | (1<<pin_num);
}

/**********************************************************************
 * Function: GPIO_config_input_nopull()
 * Purpose:  Configure one input pin and do not enable pull-up.
 * Input:    reg_name - Address of Data Direction Register, such as &DDRB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_config_input_nopull (volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num);  // Data Direction Register
    reg_name++;                             // Change pointer to Data Register
    *reg_name = *reg_name & ~(1<<pin_num);  // Data Register -- turn pull-up off
}

/**********************************************************************
 * Function: GPIO_config_input_pullup()
 * Purpose:  Configure one input pin and enable pull-up.
 * Input:    reg_name - Address of Data Direction Register, such as &DDRB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_config_input_pullup(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num);  // Data Direction Register
    reg_name++;                             // Change pointer to Data Register
    *reg_name = *reg_name | (1<<pin_num);   // Data Register -- turn pull-up on
}

/**********************************************************************
 * Function: GPIO_write_low()
 * Purpose:  Write one pin to a low value.
 * Input:    reg_name - Address of Port Register, such as &PORTB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_write_low(volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name = *reg_name & ~(1<<pin_num);
}

/**********************************************************************
 * Function: GPIO_write_high()
 * Purpose:  Write one pin to a high value.
 * Input:    reg_name - Address of Port Register, such as &PORTB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_write_high (volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name |= (1 << pin_num);
}

/**********************************************************************
 * Function: GPIO_toggle()
 * Purpose:  Toggle one pin.
 * Input:    reg_name - Address of Port Register, such as &PORTB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  none
 **********************************************************************/
void GPIO_toggle (volatile uint8_t *reg_name, uint8_t pin_num)
{
    *reg_name ^= (1 << pin_num);
}

/**********************************************************************
 * Function: GPIO_read()
 * Purpose:  Read one pin.
 * Input:    reg_name - Address of Pin Register, such as &PINB
 *           pin_num - Pin designation in the interval 0 to 7
 * Returns:  uint8_t containing 1 or 0
 **********************************************************************/
uint8_t GPIO_read (volatile uint8_t *reg_name, uint8_t pin_num)
{
    return (((*reg_name) >> pin_num) & 1);
}