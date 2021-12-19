# Morse code
```c
int main(void)
{
    // Set pin as output in Data Direction Register
    // DDRB = DDRB or 0010 0000
    DDRB |= (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB &= ~(1<<LED_GREEN);
    
    // Infinite loop
    while (1)
    {
        send_dot();
        send_comma();
    }

    // Will never reach this
    return 0;
}

/*
 * Low level send comma by manipulating pin and delaying
 */
void send_comma(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(COMMA_DELAY);
    
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}

/*
 * Low level send dot by manipulating pin and delaying
 */
void send_dot(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(DOT_DELAY);
    
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}
```

1. Code for displaying "DE2" in morse code

```C
// Morse code array A-Z
char *morse_alphabet[26] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--",
    "-.", "---", ".--.", "--.-", ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--.."
};

// Morse code array 0 - 9
char *morse_numbers[10] = {
    "-----", ".----", "..---", "...--", "....-", ".....", "-....", "--...", "---..", "----."
};

#define ONE_SPACE		500

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
    DDRB |= (1<<LED_GREEN);

    // Set pin LOW in Data Register (LED off)
    // PORTB = PORTB and 1101 1111
    PORTB &= ~(1<<LED_GREEN);
    
    // Infinite loop
    while (1)
    {
        /* Char '/' is used for sending additional spaces
         * You can try any combination
         * Tested with: 
         * BPC/DE2/
         * DE2/
         * de2/
         */
        
        char *msg = "DE2/";
        dispaly_message_morse_code(msg);
    }

    // Will never reach this
    return 0;
}

/*
 * Takes *char(string) as input and separates it into individual characters.
 */
void dispaly_message_morse_code(char *msg)
{
    char *msg_ptr = msg;
    while(*msg_ptr != '\0') {
        if (*msg_ptr == '/') {
            send_space(); 
        } else {
            display_char_in_morse_code(*msg_ptr);
            send_space(); 
        }                
        msg_ptr++;
    }
}

/*
 * Look-up table for characters in Morse code alphabet/numbers. 
 */
void display_char_in_morse_code(char c)
{
    if (c >= 65 && c <= 95) { // if char is ASCII A-Z
        ll_display_char_in_morse_code(morse_alphabet[(uint8_t)c - 65]);
    } else if (c >= 97 && c <= 122) { // if char is ASCII a-z
        ll_display_char_in_morse_code(morse_alphabet[(uint8_t)c - 97]);
    } else if (c >= 48 && c <= 57) { // if char is ASCII 0-9
        ll_display_char_in_morse_code(morse_numbers[(uint8_t)c - 48]);
    }
}

/*
 * Low level function for Morse code display, check one by one char and 
 * performs action accordingly by sending COMMA or DOT
 */
void ll_display_char_in_morse_code(char *char_codes)
{
    char *msg_ptr = char_codes;
    while(*msg_ptr != '\0') {
        if (*msg_ptr == COMMA) send_comma();
        else if (*msg_ptr == DOT) send_dot();

        msg_ptr++;
    }        
}

/*
 * Low level send space by manipulating pin and delaying
 */
void send_space(void)
{
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(SPACE_DELAY);
}

/*
 * Low level send comma by manipulating pin and delaying
 */
void send_comma(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(COMMA_DELAY);
    
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}

/*
 * Low level send dot by manipulating pin and delaying
 */
void send_dot(void)
{
    PORTB |= (1 << LED_GREEN);
    _delay_ms(DOT_DELAY);
    
    PORTB &= ~(1 << LED_GREEN);
    _delay_ms(ONE_SPACE);
}
```

# Gpio functions

```c
/* Includes ----------------------------------------------------------*/
#include <util/delay.h>     // Functions for busy-wait delay loops
#include <avr/io.h>         // AVR device-specific IO definitions
#include "gpio.h"           // GPIO library for AVR-GCC

/* Function definitions ----------------------------------------------*/
/**********************************************************************
 * Function: Main function where the program execution begins
 * Purpose:  Toggle two LEDs when a push button is pressed. Functions 
 *           from user-defined GPIO library is used.
 * Returns:  none
 **********************************************************************/
int main(void)
{
    // Green LED at port B
    GPIO_config_output(&DDRB, LED_GREEN);
    GPIO_write_low(&PORTB, LED_GREEN);

    // Configure the second LED at port C
    GPIO_config_output(&DDRC, LED_RED);
    GPIO_write_high(&PORTC, LED_RED);

    // Configure Push button at port D and enable internal pull-up resistor
    GPIO_config_input_pullup(&DDRD, BUTTON);

    // Infinite loop
    while (1)
    {
        // Pause several milliseconds
        if (GPIO_read(&PIND, BUTTON) == 0) {
            _delay_ms(BLINK_DELAY);
            GPIO_toggle(&PORTC, LED_RED);
            GPIO_toggle(&PORTB, LED_GREEN);
        }
    }

    // Will never reach this
    return 0;
}
```

# Ranges

| **Data type** | **Number of bits** | **Range** | **Description** |
| :-: | :-: | :-: | :-- | 
| `uint8_t`  | 8 | 0, 1, ..., 255 | Unsigned 8-bit integer |
| `int8_t`   | 8 | -128, -127 ... 127 | Signed 8-bit Integer |
| `uint16_t` | 16 | 0, 1, ..., 65535 | Unsigned 16-bit integer |
| `int16_t`  | 16 | −32768, −32767, ... 32767 | Signed 16-bit integer |
| `float`    | 32 | -3.4e+38, ..., 3.4e+38 | Single-precision floating-point |
| `void`     | 0 | 0 | No return type for function / generic pointer |

# Timer

```c
/**
 * @name  Definitions of Timer/Counter0
 * @note  F_CPU = 16 MHz
 */
/**
 * @name  Definitions for 8-bit Timer/Counter0
 * @note  t_OVF = 1/F_CPU * prescaler * 2^n where n = 8, F_CPU = 16 MHz
 */
#define TIM0_stop()                         TCCR0B &= ~((1<<CS02) | (1<<CS01) | (1<<CS00));
/** @brief Set overflow 16us, prescaler 001 --> 1 */
#define TIM0_overflow_16us()                TCCR0B &= ~((1<<CS02) | (1<<CS01)); TCCR0B |= (1<<CS00);
/** @brief Set overflow 128us, prescaler 010 --> 8 */
#define TIM0_overflow_128us()               TCCR0B &= ~((1<<CS02) | (1<<CS00)); TCCR0B |= (1<<CS01);
/** @brief Set overflow 1024 us, prescaler 011 --> 64 */
#define TIM0_overflow_1024us()              TCCR0B &= ~(1<<CS02); TCCR0B |= (1<<CS01) | (1<<CS00);
/** @brief Set overflow 4096us, prescaler 100 --> 256 */
#define TIM0_overflow_4096us()              TCCR0B &= ~((1<<CS01) | (1<<CS00)); TCCR0B |= (1<<CS02);
/** @brief Set overflow 16384 us, prescaler // 101 --> 1024 */
#define TIM0_overflow_16384us()             TCCR0B &= ~(1<<CS01); TCCR0B |= (1<<CS02) | (1<<CS00);
/** @brief Enable overflow interrupt, 1 --> enable */
#define TIM0_overflow_interrupt_enable()    TIMSK0 |= (1<<TOIE0);
/** @brief Disable overflow interrupt, 0 --> disable */
#define TIM0_overflow_interrupt_disable()   TIMSK0 &= ~(1<<TOIE0);
```

```c
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
   GPIO_toggle(&PORTB, LED_D1);
}
```

# Shift register

```c
/**********************************************************************
 * Function: Timer/Counter1 overflow interrupt, running at couinting frequency
 * Purpose:  Increment counter value from 00 to 9999.
 **********************************************************************/
ISR(TIMER1_OVF_vect)
{
    val++;
    if (val == 9999) val = 0;
}
```

```c
/**********************************************************************
 * Function: Timer/Counter0 overflow interrupt, running at switching frequency
 * Purpose:  Display tens and units of a counter at SSD.
 **********************************************************************/

ISR(TIMER0_OVF_vect)
{
    static uint8_t pos = 0;
    // precalculated powers of 10
    static int pow10[5] = { 1, 10, 100, 1000, 10000 };
    
    // calculate digit from number and pos
    uint16_t loc_val = (val % pow10[pos+1]) / (pow10[pos]);
    
    // Update segment
    SEG_update_shift_regs(loc_val, pos);
    
    // Increment to go to next position
    pos++;
    if (pos == 4) pos = 0; // If position exceeded go to first position
}
```

# LCD display

   | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Read/write (active low) selector |
   | E | PB1 | Enable: Starts data read/write |
   | D[3:0] | N/C (we use 4-bit mode) |  Four low order bidirectional tristate data bus pins. Used for data transfer and receive between the MPU and the HD44780U. These pins are not used during 4-bit operation |
   | D[7:4] | PD4, PD5, PD6, PD7 |  Four high order bidirectional tristate data bus pins. Used for data transfer and receive between the MPU and the HD44780U. DB7 can be used as a busy flag. |

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `lcd_init` | `LCD_DISP_OFF`<br>`LCD_DISP_ON`<br>`LCD_DISP_ON_CURSOR`<br>`LCD_DISP_ON_CURSOR_BLINK` | Display off<br>Display on, cursor off <br>Display on, cursor on<br>display on, cursor on, blink char | `lcd_init(LCD_DISP_ON_CURSOR)` |
   | `lcd_clrscr` | none | Clear display and set cursor to home position.| `lcd_clrscr();` |
   | `lcd_gotoxy` | uint8_t x position, uint8_t y position | Set cursor to specified position. | `lcd_gotoxy(0, 1);` |
   | `lcd_putc` | char c, to be displayed | Display character at current cursor position. | `lcd_putc('x');` |
   | `lcd_puts` | char *s, array of chars | Display string without auto linefeed. | `lcd_puts("msg");` |
   | `lcd_command` |LCD_CLR<br>LCD_HOME<br>etc ... (see) | Send LCD controller instruction command. | `lcd_command(LCD_CLR);` |
   | `lcd_data` | uint8_t data, to be sent | Send data byte to LCD controller. | `lcd_data('c');` |

# ADC + UART
  | **Operation** | **Register(s)** | **Bit(s)** | **Description** |
   | :-- | :-: | :-: | :-- |
   | Voltage reference    | ADMUX | REFS1:0 | 01: AVcc voltage reference (5V), ... |
   | Input channel        | ADMUX | MUX3:0 | 0000: ADC0, 0001: ADC1, ... |
   | ADC enable           | ADCSRA | ADEN | 1: ADC Enable, 0: ADC Disabled |
   | Start conversion     | ADCSRA | ADSC | 1: Start first conversion/start conversion, 0: stop conversion |
   | ADC interrupt enable | ADCSRA | ADIE | 1: Enable interrupt, 0: Disable interrupt |
   | ADC clock prescaler  | ADCSRA | ADPS2:0 | 000: Division factor 2, 001: 2, 010: 4, ...|
   | ADC 10-bit result    | ADMUX | ADLAR | 0: 10-bit mode(ADCH:L), 1: 8-bit mode(values stored only in ADCH) |

   | **Function name** | **Function parameter(s)** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `uart_init` | `UART_BAUD_SELECT(9600, F_CPU)` | Initialize UART to 8N1 and set baudrate to 9600&nbsp;Bd | `uart_init(UART_BAUD_SELECT(9600, F_CPU));` |
   | `uart_getc` | `void` | Get received byte from ringbuffer. | `uint8_t c = uart_getc();`
   | `uart_putc` | `unsigned char data` | Put byte to ringbuffer for transmitting via UART. | `uart_putc('c');`
   | `uart_puts` | `const char *s` | Put string to ringbuffer for transmitting via UART. | `uart_puts("msg");`

```c
/**********************************************************************
 * Function: ADC complete interrupt
 * Purpose:  Display value on LCD and send it to UART.
 **********************************************************************/
ISR(ADC_vect)
{
    uint16_t value = 0;
    char lcd_string[4] = "0000";
    value = ADC;                  // Copy ADC result to 16-bit variable
    
    // Clear display and convert to deca (and show)
    lcd_gotoxy(8, 0);
    lcd_puts("    ");
    itoa(value, lcd_string, 10);  // Convert decimal value to string
    lcd_gotoxy(8, 0);
    lcd_puts(lcd_string);
    
    // UART: " value: xxxx "
    uart_puts("\033[4;32m");
    uart_puts(" value: ");
    uart_puts(lcd_string);
    uart_puts(" ");
    
    // Clear display and display hexa value
    lcd_gotoxy(13, 0);
    lcd_puts("    ");
    itoa(value, lcd_string, 16);  // Convert decimal value to string
    lcd_gotoxy(13, 0);
    lcd_puts(lcd_string);
    
    // hexa string + newline
    uart_puts(lcd_string);
    uart_puts("\r\n   key: ");
    
    // clear key display
    lcd_gotoxy(8, 1);
    lcd_puts("      ");
    
    // Display key on LCD and serial 
    lcd_gotoxy(8, 1);
    if (value < 75) {
        lcd_puts("Right");
        uart_puts("Right");
    } else if (value < 150) {
        lcd_puts("Up");
        uart_puts("Up");
    } else if (value < 350) {
        lcd_puts("Down");
        uart_puts("Down");
    } else if (value < 550) {
        lcd_puts("Left");
        uart_puts("Left");
    } else if (value < 800) {
        lcd_puts("Select");
        uart_puts("Select");
    } else {
        lcd_puts("None");
        uart_puts("None");
    }
    
    // UART 2xnewlines
    uart_puts("\r\n\r\n");
}
```

# Get parity

```c
uint8_t get_parity(uint8_t data, uint8_t type)
{
    uint8_t pbl = type;

    for (int8_t i = 0; i < 8; i++) {
        pbl = (data & 1 ) ^ pbl;
        data >>= 1;
    }

    return pbl;
}
```

# Adruino TWI (I2C)

   | **Signal** | **MCU pin** | **Arduino pin(s)** |
   | :-: | :-: | :-: |
   | SDA (data)  | PC4 | A4/SDA |
   | SCL (clock) | PC5 | A5/SCL |

   | **Frame type** | **8** | **7** | **6** | **5** | **4** | **3** | **2** | **1** | **0** | **Description** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-- |
   | Address | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | R/W bit | ACK/NACK | Address of device the communication is started with. ACK by slave with correct address present on the line. |
   | Data    | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | ACK/NACK | Data to be send. ACK by slave if data received otherwise NACK.

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `twi_init` | None | Initialize TWI, enable internal pull-up resistors, and set SCL frequency | `twi_init();` |
   | `twi_start` | Device address | Start communication on TWI bus and send address of TWI slave. | `twi_start((addr<<1)+TWI_READ);` | 
   | `twi_write` | 8 bit data to be sent` | Send one data byte to TWI slave device. | `twi_write(127);` |
   | `twi_read_ack` | None | Read one byte from TWI slave device and acknowledge it by ACK. | `twi_read_ack();` |
   | `twi_read_nack` | None | Read one byte from TWI slave device and acknowledge it by NACK. | `twi_read_nack();` |
   | `twi_stop` | None | Generates stop condition on TWI bus. | `twi_stop();` |