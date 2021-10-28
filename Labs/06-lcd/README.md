# Lab 6: Gregor Karetka

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/gkaretka/Digital-electronics-2](https://github.com/gkaretka/Digital-electronics-2)

# Lab 6: Display devices, LCD display

### Learning objectives

After completing this lab you will be able to:
   * Use text-based LCD
   * Understand the digital communication between MCU and HD44780
   * Use library functions for LCD
   * Generate custom characters on LCD

The purpose of the laboratory exercise is to understand the serial control of Hitachi HD44780-based LCD character display and how to define custom characters. Another goal is to learn how to read documentation for library functions and use them in your own project.

<a name="preparation"></a>
## Preparation tasks (done before the lab at home)

Use schematic of the [LCD keypad shield](../../Docs/arduino_shield.pdf) and find out the connection of LCD display. What data and control signals are used? What is the meaning of these signals?

![Timing parallel](Images/lcd_schematic.png)

   | **LCD signal(s)** | **AVR pin(s)** | **Description** |
   | :-: | :-: | :-- |
   | RS | PB0 | Register selection signal. Selection between Instruction register (RS=0) and Data register (RS=1) |
   | R/W | GND | Read/write (active low) selector |
   | E | PB1 | Enable: Starts data read/write |
   | D[3:0] | N/C (we use 4-bit mode) |  Four low order bidirectional tristate data bus pins. Used for data transfer and receive between the MPU and the HD44780U. These pins are not used during 4-bit operation |
   | D[7:4] | PD4, PD5, PD6, PD7 |  Four high order bidirectional tristate data bus pins. Used for data transfer and receive between the MPU and the HD44780U. DB7 can be used as a busy flag. |

What is the ASCII table? What are the codes/values for uppercase letters `A` to `Z`, lowercase letters `a` to `z`, and numbers `0` to `9` in this table?

   ![Timing parallel](Images/ascii_table.png)

<a name="part3"></a>
## Part 3: Library for HD44780 based LCDs

In the lab, we are using [LCD library for HD44780 based LCDs](http://www.peterfleury.epizy.com/avr-software.html) developed by Peter Fleury. Use the online manual of LCD library and add the input parameters and description of the following functions.

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `lcd_init` | `LCD_DISP_OFF`<br>`LCD_DISP_ON`<br>`LCD_DISP_ON_CURSOR`<br>`LCD_DISP_ON_CURSOR_BLINK` | Display off<br>Display on, cursor off <br>Display on, cursor on<br>display on, cursor on, blink char |
   | `lcd_clrscr` | none | Clear display and set cursor to home position.| `lcd_clrscr();` |
   | `lcd_gotoxy` | uint8_t x position, uint8_t y position | Set cursor to specified position. | `lcd_gotoxy(0, 1);` |
   | `lcd_putc` | char c, to be displayed | Display character at current cursor position. | `lcd_putc('x');` |
   | `lcd_puts` | char *s, array of chars | Display string without auto linefeed. | `lcd_puts("msg");` |
   | `lcd_command` |LCD_CLR<br>LCD_HOME<br>etc ... (see) | Send LCD controller instruction command. | `lcd_command(LCD_CLR);` |
   | `lcd_data` | uint8_t data, to be sent | Send data byte to LCD controller. | `lcd_data('c');` |


### LCD display module

1. In your words, describe what ASCII table is.
   * ASCII - is table that defines number value for standard characters (letters, numbers, control characters like new line/delete etc.)

2. (Hand-drawn) picture of time signals between ATmega328P and LCD keypad shield (HD44780 driver) when transmitting three character data `De2`.

   ![Timing parallel](Images/de2_timing.jpg)


### Stopwatch

1. Flowchart figure for `TIMER2_OVF_vect` interrupt service routine which overflows every 16&nbsp;ms but it updates the stopwatch LCD approximately every 100&nbsp;ms (6 x 16&nbsp;ms = 100&nbsp;ms). Display tenths of a second and seconds `00:seconds.tenths`. Let the stopwatch counts from `00:00.0` to `00:59.9` and then starts again. The image can be drawn on a computer or by hand. Use clear descriptions of the individual steps of the algorithms.

   ![Stopwatch](Images/stopwatch.png)


### Custom characters

1. Code listing with syntax highlighting of two custom character definition:

```c
/* Variables ---------------------------------------------------------*/
// Custom character definition
uint8_t custom_char_1[8] = {
    0b00111,
    0b01110,
    0b11100,
    0b11000,
    0b11100,
    0b01110,
    0b00111,
    0b00011
};

uint8_t custom_char_2[8] = {
    0b01100,
    0b01100,
    0b01100,
    0b01100,
    0b01100,
    0b00000,
    0b01100,
    0b01100
};
```


### Kitchen alarm

Consider a kitchen alarm with an LCD, one LED and three push buttons: start, +1 minute, -1 minute. Use the +1/-1 minute buttons to increment/decrement the timer value. After pressing the Start button, the countdown starts. The countdown value is shown on the display in the form of mm.ss (minutes.seconds). At the end of the countdown, the LED will start blinking.

1. Scheme of kitchen alarm; do not forget the supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values.

   ![Kitchen timer sch](Images/kitchen_timer_sch.png)