# 2025-08-29 - Anteckningar

Implementering av ett HAL-bibliotek med drivrutiner för ATmega328P:
* Filen [led.h](./driver/led.h) innehåller klassen `Led`, som kan användas för lysdioder.
Motsvarande källkod finns i [led.cpp](./driver/led.cpp).
* Filen [serial.h](./driver/serial.h) innehåller klassen `Serial`, som kan användas för seriella
utskrifter. Motsvarande källkod finns i [serial.cpp](./driver/serial.cpp).\
För denna klass lades också ett interface till som försmak på nästa veckas stoff, 
se klassen `SerialInterface` i filen [serial_interface.h](./driver/serial_interface.h)
* I filen [main.cpp](./main.cpp) skapas för tillfället en lysdiod som en instans av klassen `Led`.
Lysdioden blinkas i en while-sats.\
Seriell överföring används för utskrifter, exempelvis för att indikera status gällande initiering av lysdioden.

## Kompilering samt flashning av programmet
Ladda ned katalogen i Windows. Öppna sedan projektfilen [ela24_libatmega.cppproj](./ela24_libatmega.cppproj).
Projektet kommer öppnas i `Microchip Studio`.\
Kompilera samt flasha programmet i vanlig ordning.
