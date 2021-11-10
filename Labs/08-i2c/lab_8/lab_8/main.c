/***********************************************************************
 * 
 * The I2C bus scanner detects the addresses of the modules that are 
 * connected to the SDA and SCL signals. A simple description of FSM is 
 * used.
 * ATmega328P (Arduino Uno), 16 MHz, AVR 8-bit Toolchain 3.6.2
 *
 * Copyright (c) 2017-Present Tomas Fryza
 * Dept. of Radio Electronics, Brno University of Technology, Czechia
 * This work is licensed under the terms of the MIT license.
 * 
 **********************************************************************/

/* Defines -----------------------------------------------------------*/
#ifndef F_CPU
# define F_CPU 16000000  // CPU frequency in Hz required for UART_BAUD_SELECT
#endif

/* Includes ----------------------------------------------------------*/
#include <avr/io.h>         // AVR device-specific IO definitions
#include <avr/interrupt.h>  // Interrupts standard C library for AVR-GCC
#include "timer.h"          // Timer library for AVR-GCC
#include <stdlib.h>         // C library. Needed for conversion function
#include "uart.h"           // Peter Fleury's UART library
#include "twi.h"            // TWI library for AVR-GCC

/* Variables ---------------------------------------------------------*/
typedef enum {              // FSM scanner declaration
    STATE_IDLE = 1,
    STATE_SEND,
    STATE_ACK,
} state_t;

typedef enum {              // FSM temp hum declaration
    SENSOR_IDLE = 1,
    SENSOR_READ_H0,
    SENSOR_READ_H1,
    SENSOR_READ_T0,
    SENSOR_READ_T1,
} sensor_state_t;

void i2c_scanner(void);
void read_and_send_tmp_hum(void);

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Use Timer/Counter1 and send I2C (TWI) address every 33 ms.
 *           Send information about scanning process to UART.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Initialize I2C (TWI)
    twi_init();

    // Initialize UART to asynchronous, 8N1, 9600
    uart_init(UART_BAUD_SELECT(9600, F_CPU));

    // Configure 16-bit Timer/Counter1 to update FSM
    // Set prescaler to 33 ms and enable interrupt
    TIM1_overflow_262ms();
    TIM1_overflow_interrupt_enable();

    // Enables interrupts by setting the global interrupt mask
    sei();

    // Put strings to ringbuffer for transmitting via UART
    uart_puts("\r\nScan I2C-bus for devices:\r\n");

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
 * Function: Timer/Counter1 overflow interrupt
 * Purpose:  Update Finite State Machine and test I2C slave addresses 
 *           between 8 and 119.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    read_and_send_tmp_hum();
}

void read_and_send_tmp_hum(void)
{
    static sensor_state_t state = SENSOR_IDLE;  // Current state of the FSM
    uint8_t addr = 0xB8 >> 1;
    char uart_string[5] = "00000";         // String for converting numbers by itoa()
    static uint16_t result = 0;
    
    // FSM
    switch (state)
    {
    // Increment I2C slave address
    case SENSOR_IDLE:
        itoa(result, uart_string, 10);
        uart_puts(uart_string);
        uart_puts("\n\r");
        
        twi_start((addr<<1) + TWI_WRITE);   // start write
        twi_write(0x00);                    // byte sel
        
        twi_start((addr<<1) + TWI_READ);    // start read
        result = (uint16_t)twi_read_ack();            // read byte
        result <<= 8;
        twi_stop();
        
        twi_start((addr<<1) + TWI_WRITE);   // start write
        twi_write(0x01);                    // byte sel
                
        twi_start((addr<<1) + TWI_READ);    // start read
        result |= (uint16_t)twi_read_ack();
        twi_stop();
        
        state = SENSOR_READ_H0;
        break;
        
    case SENSOR_READ_H0:
        
        state = SENSOR_READ_H1;
        break; 
        
    case SENSOR_READ_H1:

        state = SENSOR_IDLE;
        break;
    

    default:
        state = SENSOR_IDLE;
        break;
    }
}

void i2c_scanner(void)
{
    static state_t state = STATE_IDLE;  // Current state of the FSM
    static uint8_t addr = 0;            // I2C slave address
    uint8_t result = 1;                 // ACK result from the bus
    char uart_string[2] = "00";         // String for converting numbers by itoa()

    // FSM
    switch (state)
    {
    // Increment I2C slave address
    case STATE_IDLE:
        // If slave address is between 8 and 119 then move to SEND state
        addr++;  
        if (addr >= 8 && addr <= 119) {
            state = STATE_SEND;
        } else if (addr == 0) {
            uart_puts("\n\r\n\r");
        }       
        break;
    
    // Transmit I2C slave address and get result
    case STATE_SEND:
        // I2C address frame:
        // +------------------------+------------+
        // |      from Master       | from Slave |
        // +------------------------+------------+
        // | 7  6  5  4  3  2  1  0 |     ACK    |
        // |a6 a5 a4 a3 a2 a1 a0 R/W|   result   |
        // +------------------------+------------+
        result = twi_start((addr<<1) + TWI_WRITE);
        twi_stop();
           
        /* Test result from I2C bus. If it is 0 then move to ACK state, 
         * otherwise move to IDLE */
        if (result) {
            state = STATE_IDLE;
        } else {
            state = STATE_ACK;
        }            
        
        break;

    // A module connected to the bus was found
    case STATE_ACK:
        // Send info about active I2C slave to UART and move to IDLE
        itoa(addr, uart_string, 10);
        uart_puts(uart_string);
        uart_puts("\n\r");
        
        state = STATE_IDLE;
        break;

    // If something unexpected happens then move to IDLE
    default:
        state = STATE_IDLE;
        break;
    }
}