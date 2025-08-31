# `ATmega328P` library - C version

## Description
Library containing miscellaneous drivers for microcontroller `ATmega328P`.  
The code is written in C99 and adapted for Microchip Studio. 

## Content
The library includes the following drivers:  
* `ADC`: Driver for the `ATmega328P` ADC.  
* `Config`: Global configuration settings.
* `EEPROM`: Driver for utilization of `ATmega328P` EEPROM.  
* `Factory`: Driver factory used to create driver components.
* `GPIO`: Generic driver for GPIO devices.  
* `Serial`: Driver for serial transmission via UART. 
* `Timer`: Driver for the `ATmega328P` hardware timers.   
* `Watchdog`: Driver for the `ATmega328P` watchdog timer.  

The library also includes miscellaneous utility functions.

A test program is implemented.

## Usage 
This library must be opened in a Windows environment to build.  
Copy the library into a Windows path, such as the C drive, before building.