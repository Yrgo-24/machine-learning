# Klassen *Person*

Implementering av klassen *Person* i C++. Denna klass används för att lagra samt skriva ut persondata i terminalen samt till en fil döpt *person.txt.*

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