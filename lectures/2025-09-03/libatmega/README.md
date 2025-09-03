# 2025-09-03 - Anteckningar

Implementering av ett HAL-bibliotek med drivrutiner för ATmega328P. Tre moduler har implementerats, `Led`, `Button` samt `Serial`, alla med var sitt interface samt en skarp implementation.

## Modulen `Led`
* Filen [led_interface.h](./driver/led_interface.h) innehåller klassen `LedInterface`, som är ett interface för
lysdioder. 
* Filen [led.h](./driver/led.h) innehåller underklassen `Led`, som utgör en skarp implementation av `LedInterface`,
anpassad för ATmega328P.\
* Filen [led.cpp](./driver/led.cpp) innehåller implementationsdetaljer för klassen `Led`.

## Modulen `Button`
* Filen [button_interface.h](./driver/button_interface.h) innehåller klassen `ButtonInterface`, som är ett interface för
tryckknappar.
* Filen [button.h](./driver/led.h) innehåller klassen `Button`, som utgör en skarp implementation av `ButtonInterface`,
anpassad för ATmega328P.\
* Filen [button.cpp](./driver/button.cpp) innehåller implementationsdetaljer för klassen `Button`.

## Modulen `Serial`
* Filen [serial_interface.h](./driver/serial_interface.h) innehåller klassen `SerialInterface`, som är ett interface för
serialla enheter. 
* Filen [serial.h](./driver/serial.h) innehåller klassen `Serial`, som utgör en skarp implementation av `SerialInterface`,
anpassad för ATmega328P.
* Filen [serial.cpp](./driver/serial.cpp) innehåller implementationsdetaljer för klassen `Serial`.

## Exempelprogram
I filen [main.cpp](./main.cpp) skapas en lysdiod, en tryckknapp samt en seriell instans via klasser
`Led`, `Button` samt `Serial`. Dessa instanser passeras till en funktion döpt `runProgram`, som förväntar
sig (referenser till) instanser av motsvarande interface `LedInterface`, `ButtonInterface` samt `SerialInterface`. Vid nedtryckning
av tryckknappen blinkar lysdioden. Knappevent loggas i terminalen. 

Genom att funktionen `runProgram` förväntar sig instanser av motsvarande interface kan vi enkelt 
byta ut de skarpa implementationerna som används, exempelvis om processorn byts ut, utan att ändra i logiken för denna funktion. Det enda som krävs är att de nya skarpa implementationer vi använder också ärver motsvarande interfaces.

## Kompilering samt flashning av programmet
Ladda ned katalogen i Windows. Öppna sedan projektfilen [ela24_libatmega.cppproj](./ela24_libatmega.cppproj).
Projektet kommer öppnas i `Microchip Studio`.\
Kompilera samt flasha programmet i vanlig ordning.
