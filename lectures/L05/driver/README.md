# Demonstration av ett interface för lysdioder

Demonstration av ett interface döpt `LedInterface`, som används för att enkelt kunna använda
olika typer av lysdioder, exempelvis lysdioder från olika processorer, via underklasser.

* [led_interface.h](./include/driver/led_interface.h) utgör själva interfacet i form av basklassen `LedInterface`.
* [atmega328p_led.h](./include/driver/atmega328p_led.h) utgör en implementation av lysdioder för processorn
`ATmega328P` via underklassen `atmega328p::Led`. Motsvarande källkod finns [här](./source/atmega328p_led.cpp).
* [stm32f103_led.h](./include/driver/stm32f103_led.h) utgör en implementation av lysdioder för en STM32F103-processor
via underklassen `stm32f103::Led`. Motsvarande källkod finns [här](./source/main.cpp).
* I [main.cpp](./source/main.cpp) skapas lysdioder av vardera processor (via de tidigare nämnda underklasserna) 
och blinkar med olika frekvens. Detta åstakommes via en delad blink-funktion, som förväntar sig en referens
till ett `LedInterface`. Eftersom båda subklasser ärver `LedInterface` fungerar det utmärkt.

## Kompilering samt körning av programmet
För att kunna kompilera koden, se till att du har GCC-kompilatorn samt *make* installerat. Installera därmed paketen *build-essential* samt *make:*

```bash
sudo apt -y update
sudo apt -y install build-essential make
```

Tack vara den bifogade makefilen kan du sedan när som helst kompilera och köra programmet via följande kommando (i denna katalog):

```bash
make
```

Du kan också enbart bygga programmet utan att köra det efteråt via följande kommando:

```bash
make build
```

Du kan köra programmet utan att kompilera innan via följande kommando:

```bash
make run
```

Du kan också ta bort kompilerade filer via följande kommando:

```bash
make clean
```