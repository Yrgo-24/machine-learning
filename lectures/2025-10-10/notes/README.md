# 2025-10-10 - Anteckningar

Slutförd implementation av klassen `DenseLayer` - en skarp implementation av dense-lager.
* Filen [ml/types.h](./ml/types.h) innehåller typedefinitioner för maskininlärning, i detta fall enbart aktiveringsfunktioner.
* Filen [ml/dense_layer/dense_layer.h](./ml/dense_layer/dense_layer.h) innehåller deklaration av klassen `DenseLayer`.
* Filen [ml/dense_layer/dense_layer.cpp](./ml/dense_layer/dense_layer.cpp) innehåller implementationsdetaljer
av klassen `DenseLayer`.
* Filen [main.cpp](./main.cpp) innehåller ett test av ett neuralt nätverk med dense-lager:
    * Det neurala nätverket består av klassen `SingleLayer`, som skapades i inlämningsuppgift 5.
    * Dense-lagrerna består av instanser av den nyimplementerade klassen `DenseLayer`.
    * Nätverket tränas att prediktera ett tvåbitars XOR-mönster. Efter att träningen är slutförd skrivs
    resultatet ut i terminalen.
