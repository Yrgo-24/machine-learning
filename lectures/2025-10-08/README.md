# 2025-10-08 - Lektion 17

## Dagordning
* Implementering av dense-lager i mjukvara (del I) (med programkoden från inlämningsuppgift 5):
    * Skapande av en skarp underklass `DenseLayer`, som ersätter befintlig stubbklass.
    * Tillägg av ett fåtal aktiveringsfunktioner.

## Mål med lektionen
* Ha skapat en skarp underklass som ärver det befintliga dense-lager-interfacet.
* Ha implementerat samtliga metoder **förutom** `feedforward`, `backpropagate` samt `optimize` - Dessa kommer att implementeras under nästa lektion.

## Instruktioner
* Använd er inlämnade programkod för inlämningsuppgift 5.
* Skapa en konkret implementation som ärver `ml::dense_layer::Interface`, se bilaga A nedan för mer detaljer.
* Implementera samtliga metoder förutom de ovannämnda - returnera enbart `true` i dessa.
* Lektionsanteckningar finns [här](./notes/README.md).

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Implementering av dense-lager i mjukvara (del II) - feedforward, backpropagation och optimering.

## Bilaga A - Implementering av klassen `DenseLayer` (del I)

**1.** Skapa en ny fil döpt `types.h` direkt i `include/ml`. Filsökvägen ska alltså vara `include/ml/types.h` och
filen ska kunna inkluderas såsom visas nedan:

```cpp
#include "ml/types.h"
```

**2.** I `ml/types.h` definiera en enumerationsklass döpt `ActFunc`. Denna enumerationsklass ska kunna användas för
att välja aktiveringsfunktion för ett givet dense-layer. Följande enumeratorer ska läggas till:
* `Relu`: För ReLU (`Rectified Linear Unit`), som för en given input x returnerar x om x > 0 annars 0.
* `Tanh`: För hyperbolisk tangent, som ger utdata i intervallet [-1, 1].

**3.** Skapa en ny fil döpt `dense_layer.h` i `include/ml/dense_layer`. Filsökvägen ska alltså vara `include/ml/dense_layer/dense_layer.h` och
filen ska kunna inkluderas såsom visas nedan:

```cpp
#include "ml/dense_layer/dense_layer.h"
```

**4.** Skapa en ny fil döpt `dense_layer.cpp` i `source/dense_layer`. Filsökvägen ska alltså vara `source/dense_layer/dense_layer.cpp`.\
**OBS! Glöm inte att lägga till denna filen i din makefil, se nedan!**

```makefile
# Application target.
TARGET := app

# Source files.
SOURCE_FILES := source/dense_layer/dense_layer.cpp # Här läggs filen till i bygget.
                source/dense_layer/stub.cpp\
                source/main.cpp\
				source/neural_network/single_layer.cpp\

# Include directory.
INCLUDE_DIR := include

# Compiler flags.
COMPILER_FLAGS := -Wall -Werror -std=c++17

# Build and run the application as default.
default: build run

# Build the application.
build:
	@g++ $(SOURCE_FILES) -o $(TARGET) -I$(INCLUDE_DIR) $(COMPILER_FLAGS)

# Run the application.
run:
	@./$(TARGET)

# Clean the application.
clean:
	@rm -f $(TARGET)
```

**5.** I headerfilen `ml/dense_layer/dense_layer.h`, lägg till klass döpt `DenseLayer`, som ärver motsvarande interface,
se filen `include/ml/dense_layer/interface.h`. Använd publikt arv och se till att denna underklass i sig inte kan ärvas vidare.
Överlagra samtliga metoder från interfacet, inklusive destruktorn.

**Tips**: Kopiera allt innehåll i interfacet och klistra in i den nya filen. Anpassa sedan koden för de nya underklassen `DenseLayer` (inga `virtual` eller `= 0`, använd `override` med mera).

**6.** Radera följande:
* Defaultkonstruktorn.
* Kopieringskonstruktorn.
* Förflyttningskonstruktorn.
* Kopieringsoperatorn.
* Förflyttningsoperatorn.

**7.** Skapa en explicit konstruktor, som ska kunna användas för att skapa ett dense-lager med godtyckliga dimensioner
samt en valbar aktiveringsfunktion. 
* Parametrar:
    * `nodeCount`: Antalet noder i lagret (osignerat heltal).
    * `weightCount`: Antalet vikter per nod i lagret (osignerat heltal).
    * `actFunc`: Aktiveringsfunktionen som ska användas (av typen `ActFunc`). Som default ska `ReLU` användas.

Denna funktion ska inte markeras `noexcept`; förutom att allokera minne för noderna (vilket kan generera undantag)
ska ett undantag genereras om `nodeCount` eller `weightCount` är lika med 0.

**8.** Lägg till följande privata medlemsvariabler i klassen:
* `myOutput`: Vektor innehållande nodernas output (flyttal). Ska ha utrymme för `nodeCount` värden.
* `myError`: Vektor innehållande nodernas fel (flyttal). Ska ha utrymme för `nodeCount` värden.
* `myBias`: Vektorn innehållande nodernas bias-värden (flyttal). Ska ha utrymme för `nodeCount` värden.
* `myWeights`: Tvådimensionell vektor innehållande nodernas vikter. Ska ha utrymme för `nodeCount` x `weightCount` värden.
* `myActFunc`: Lagrets aktiveringsfunktion (av typen `ActFunc`).

**9.** Definiera samtliga metoder, konstruktorer med mera som ej är markerade delete eller default i filen `source/dense_layer/dense_layer.cpp`:

**Konstruktor:**
* I konstruktorn ska samtliga medlemsvariabler initieras:
    * `myOutput` samt `myError` ska innehålla `nodeCount` flyttal som är lika med 0.0 vid start.
    * `myBias` ska innehålla `nodeCount` flyttal randomiserade mellan 0.0 - 1.0.
    * `myWeights` ska innehålla `nodeCount` x `weightCount` flyttal randomiserade mellan 0.0 - 1.0.
    * `myActFunc` ska tilldelas angiven aktiveringsfunktion.
* Om `nodeCount` eller `weightCount` är lika med 0 ska ett undantag av typen `std::invalid_argument` kastas med felmeddelandet `Invalid dense layer parameters: nodeCount and weightCount must be greater than 0!`, såsom visas nedan:

```cpp
#include <stdexcept>

if ((0U == nodeCount) || (0U == weightCount))
{
    throw std::invalid_argument(
        "Invalid dense layer parameters: nodeCount and weightCount must be greater than 0!");
}
```

**Övriga metoder:**
* Implementera samtliga metoder från interfacet förutom `feedforward`, `backpropagate` samt `optimize`.
* Följ beskrivningarna i `include/ml/dense_layer/interface.h`:
    * Metoder som `nodeCount`, `weightCount` ska returnera antalet noder respektive vikter per nod i lagret.
    * Getter-metoder såsom `output` samt `error` ska returnera referenser till motsvarande medlemsvariabler.
    * För de tre ej implementerade metoderna ska enbart `true` returneras.
