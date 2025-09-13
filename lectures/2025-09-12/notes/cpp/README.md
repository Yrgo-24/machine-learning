# # Linjär regression i C++ med strukten `Linreg`

Träning samt prediktion av en enkel linjär regressionsmodell i C++ via strukten `LinReg`. Implementationen har genomförts
i filen [main.cpp](./main.cpp). Träningsdatan definieras i funktionen `main`. Träning genomförs under 1000 epoker eller
tills modellen predikterar inom $1 \times 10^{-6}$. Efter träningen är slutförd genomförs prediktion med indatavärden
från träningsuppsättningarna.

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