# `ATmega328P` library - C++ version

## Description
Library containing miscellaneous drivers and containers for microcontroller `ATmega328P`.  
The code is written in C++17 and adapted for Microchip Studio. 

## Content
The library includes the following drivers:  
* `ADC`: Driver for the `ATmega328P` ADC.  
* `EEPROM`: Driver for utilization of `ATmega328P` EEPROM.  
* `GPIO`: Generic driver for GPIO devices.  
* `Timer`: Driver for the `ATmega328P` hardware timers.  
* `Serial`: Driver for serial transmission via USART.  
* `Watchdog`: Driver for the `ATmega328P` watchdog timer.  

The library includes the following generic smart pointers:
* `UniquePtr`: Implementation of unique pointers of any data type.
* `SharedPtr`: Implementation of shared pointers of any data type.

The library includes the following generic containers:  
* `Array`: Implementation of static arrays of any data type.  
* `CallbackArray`: Implementation of callback arrays of arbitrary size.  
* `List`: Implementation of doubly linked lists of any data type.  
* `Pair`: Implementation of pairs containing values of any data type.  
* `Vector`: Implementation of dynamic vectors of any data type.  

The library also includes miscellaneous utility functions, type traits etc.  

## Usage 
This library must be opened in a Windows environment to build.  
Copy the library into a Windows path, such as the C drive, before building.