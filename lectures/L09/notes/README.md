# L09 - Anteckningar

Träning samt prediktion av enkla linjär regressionsmodeller via struktar:
* Katalogen [cpp](./cpp/README.md) innehåller en C++-implementation med strukten `LinReg` samt associerade funktioner.
* Katalogen [c](./c/README.md) innehåller en C-implementation med strukten `linreg_t` samt associerade funktioner.

I dessa implementationer används randominsering för att randomisera ordningsföljden på träningsdatan
varje epok, samt för att initiera modellernas parametras till flyttal mellan 0.0 - 1.0.
