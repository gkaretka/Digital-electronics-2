# Lab 8: Gregor Karetka

Link to your `Digital-electronics-2` GitHub repository:

   [https://github.com/gkaretka/Digital-electronics-2](https://github.com/gkaretka/Digital-electronics-2)

# Lab 8: I2C/TWI serial communication

<a name="preparation"></a>

## Preparation tasks (done before the lab at home)

1. Use schematic of the [Arduino Uno](../../Docs/arduino_shield.pdf) board and find out to which pins the SDA and SCL signals are connected.

   | **Signal** | **MCU pin** | **Arduino pin(s)** |
   | :-: | :-: | :-: |
   | SDA (data)  | PC4 | A4/SDA |
   | SCL (clock) | PC5 | A5/SCL |

2. What is the general structure of I2C address and data frames?

   | **Frame type** | **8** | **7** | **6** | **5** | **4** | **3** | **2** | **1** | **0** | **Description** |
   | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-: | :-- |
   | Address | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | R/W bit | ACK/NACK | Address of device the communication is started with. ACK by slave with correct address present on the line. |
   | Data    | Bit 7 | Bit 6 | Bit 5 | Bit 4 | Bit 3 | Bit 2 | Bit 1 | Bit 0 | ACK/NACK | Data to be send. ACK by slave if data received otherwise NACK.

   1. Use the [`twi.h`](../../Examples/library/include/twi.h) header file from the I2C/TWI library to complete the description of the functions in the following table.

   | **Function name** | **Function parameters** | **Description** | **Example** |
   | :-- | :-- | :-- | :-- |
   | `twi_init` | None | Initialize TWI, enable internal pull-up resistors, and set SCL frequency | `twi_init();` |
   | `twi_start` | Device address | Start communication on TWI bus and send address of TWI slave. | `twi_start((addr<<1)+TWI_READ);` | 
   | `twi_write` | 8 bit data to be sent` | Send one data byte to TWI slave device. | `twi_write(127);` |
   | `twi_read_ack` | None | Read one byte from TWI slave device and acknowledge it by ACK. | `twi_read_ack();` |
   | `twi_read_nack` | None | Read one byte from TWI slave device and acknowledge it by NACK. | `twi_read_nack();` |
   | `twi_stop` | None | Generates stop condition on TWI bus. | `twi_stop();` |

