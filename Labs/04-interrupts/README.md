# Lab 4: Gregor Karetka

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/gkaretka/Digital-electronics-2](https://github.com/gkaretka/Digital-electronics-2)

## Preparation tasks (done before the lab at home)

Consider an n-bit number that we increment based on the clock signal. If we reach its maximum value and try to increase it, the value will be reset. We call this state an **overflow**. The overflow time depends on the frequency of the clock signal, the number of bits, and on the prescaler value:

##### Calculation for 16 MHz, 8 bit, /1 prescaler

&nbsp;
![Timer overflow](Images/equation_calc_16_8_1.gif)
&nbsp;

##### Calculation for 16 MHz, 16 bit, /1 prescaler

&nbsp;
![Timer overflow](Images/equation_calc_16_16_1.gif)
&nbsp;

### Overflow times

1. Complete table with overflow times.

| **Module** | **Number of bits** | **1** | **8** | **32** | **64** | **128** | **256** | **1024** |
| :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: |
| Timer/Counter0 | 8  | 16u | 128u | -- | 1024 us | -- | 4.096 ms | 16.384 ms |
| Timer/Counter1 | 16 | 4.096 ms | 32.768 ms | -- | 262.144 ms | -- | 1.048576 s | 4.194304 s |
| Timer/Counter2 | 8  | 16u | 128u | -- | 1024 us | -- | 4.096 ms | 16.384 ms |

2. Shields are boards that can be attached to an Arduino board, significantly expand its capabilities, and makes prototyping much faster. See schematic of Multi-function shield and find out the connection of four LEDs (D1, D2, D3, D4) and three push buttons (S1-A1, S2-A2, S3-A3).

&nbsp;
![Timer overflow](Images/sch_4_leds.png)
&nbsp;

&nbsp;
![Timer overflow](Images/sch_3_btns.png)
&nbsp;

### Timer library

1. In your words, describe the difference between common C function and interrupt service routine.
   * Function
   * Interrupt service routine

2. Part of the header file listing with syntax highlighting, which defines settings for Timer/Counter0:

```c
/**
 * @name  Definitions of Timer/Counter0
 * @note  F_CPU = 16 MHz
 */
// WRITE YOUR CODE HERE
```

3. Flowchart figure for function `main()` and interrupt service routine `ISR(TIMER1_OVF_vect)` of application that ensures the flashing of one LED in the timer interruption. When the button is pressed, the blinking is faster, when the button is released, it is slower. Use only a timer overflow and not a delay library.

   ![your figure]()


### Knight Rider

1. Scheme of Knight Rider application with four LEDs and a push button, connected according to Multi-function shield. Connect AVR device, LEDs, resistors, push button, and supply voltage. The image can be drawn on a computer or by hand. Always name all components and their values!

   ![your figure]()