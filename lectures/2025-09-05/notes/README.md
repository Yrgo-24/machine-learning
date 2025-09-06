# 2025-09-05 - Anteckningar

Implementering av en prototyp för ett nätverk. 
Två moduler har implementerats, `Router` samt `Sw`, båda med var sitt interface samt en stubklass för testning.
Detta exempel gjordes för att visa hur prototyper till ett nytt projekt, där olika programmerare arbetar
med olika moduler, kan implementeras via interfaces.

## Modulen `Router`
* Filen [router/interface.h](./components/include/components/router/interface.h) 
innehåller klassen `router::Interface`, som är ett interface för routers.
* Filen [router/stub.h](./components/include/components/router/stub.h) innehåller stubklassen `router::Stub`, 
som utgör en testklass för att testa routrar i skarp kod.
* Filen [router/stub.cpp](./components/source/router/stub.cpp) innehåller implementationsdetaljer för klassen `router::Stub`.

## Modulen `Sw`
* Filen [sw/interface.h](./components/include/components/sw/interface.h) 
innehåller klassen `sw::Interface`, som är ett interface för switchar.
* Filen [sw/stub.h](./components/include/components/sw/stub.h) innehåller stubklassen `sw::Stub`, 
som utgör en testklass för att testa switchar i skarp kod. I stället för att verkligen fungera som en switch
genomförs dock utskrifter (för anslutning, aktivering med mera).
* Filen [sw/stub.cpp](./components/source/sw/stub.cpp) innehåller implementationsdetaljer för klassen `sw::Stub`.

## Exempelprogram
I filen [main.cpp](./main.cpp) körs en applikation, där två switchar ansluts till en router. Dessa komponenter
utgörs av interfaces, varvid de underliggande implementationerna enkelt kan bytas ut. I detta exempel används dock de tidigare nämnda stubklasserna som skarpa implementationerr, varvid utskrifter sker i stället för någon riktig anslutning.


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