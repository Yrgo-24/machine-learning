# 2025-10-10 - Lektion 18

## Dagordning
* Implementering av dense-lager i mjukvara (del II) - feedforward, backpropagation och optimering.

## Mål med lektionen
* Ha implementerat metoderna `feedforward`, `backpropagate` och `optimize` i dense-lager-klassen.
* Ha skapat en fullt fungerande dense-lager-implementation.
* Ha testat implementationen med ett komplett neuralt nätverk (det som skapades i inlämningsuppgift 5).

## Instruktioner
* Fortsätt där vi arbetade senast - implementera de tre metoderna som lämnades tomma förra lektionen.
* Översätt de matematiska operationerna för feedforward, backpropagation och optimering som vi genomfört för hand till kod, se bilaga A nedan för mer detaljer.
* Testa implementationen genom att träna det neurala nätverk på befintlig träningsdata, justera vid behov antalet genomförda epoker och/eller lärhastigheten.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion (i januari)
* Lite repetition av neurala nätverk för att förbereda er inför resterande delar av kursen:
    * **Projekt 2** – Implementering av dynamiskt neuralt nätverk på ett inbyggt system.
    * Konvolutionella neurala nätverk för bildklassificering – kernels, padding, pooling-lager med mera.

## Övrigt
* Lycka till på LIA 1, vi ses i januari!

## Bilaga A - Implementering av klassen `DenseLayer` (del II)

**1.** I filen `source/dense_layer/dense_layer.cpp`, skapa en anonym namnrymd. I denna namnrymd, definiera
följande hjälpfunktioner:

* `actFuncOutput`: Funktion för att beräkna utdata ur en given aktiveringsfunktion.
    * **Parametrar:**
        * `actFunc`: Aktiveringsfunktionen som ska användas (av typen `ActFunc`).
        * `input`: Indatavärde till aktiveringsfunktionen (flyttal).
    * **Returvärde:** 
        * Utdatan ur aktiveringsfunktionen (flyttal).
    * **Implementation:**
        * Använd en switch-sats för att beräkna utdatan beroende på angiven aktiveringsfunktion:
            * `ActFunc::Relu`: Returnera `input` om `input > 0.0`, annars `0.0`.
            * `ActFunc::Tanh`: Returnera `std::tanh(input)` (kräver `#include <cmath>`).
            * Default-fall: Kasta ett undantag av typ `std::invalid_argument` med meddelandet `"Invalid activation function!"`.

* `actFuncDelta`: Funktion för att beräkna derivatan av en given aktiveringsfunktion.
    * **Parametrar:**
        * `actFunc`: Aktiveringsfunktionen som ska användas (av typen `ActFunc`).
        * `input`: Indatavärde till aktiveringsfunktionen (flyttal).
    * **Returvärde:** 
        * Derivatan av aktiveringsfunktionen (flyttal).
    * **Implementation:**
        * Använd en switch-sats för att beräkna derivatan beroende på angiven aktiveringsfunktion:
            * `ActFunc::Relu`: Returnera `1.0` om `input > 0.0`, annars `0.0`.
            * `ActFunc::Tanh`: Beräkna `const auto tanhOutput = std::tanh(input)` och returnera `1.0 - tanhOutput * tanhOutput`.
            * Default-fall: Kasta ett undantag av typ `std::invalid_argument` med meddelandet `"Invalid activation function!"`.

**2.** Implementera funktionen `feedforward`:

**Indatakontroll:**
* Kontrollera att dimensionerna på given input matchar antalet vikter per nod i lagret (`input.size() == weightCount()`).
* Om dimensionerna inte matchar: skriv ut felmeddelandet `"Input dimension mismatch: expected X, actual: Y!"` och returnera `false`.
* **OBS:** I denna funktion kastar vi inte undantag för att hålla koden optimerad (anropas ofta vid träning/prediktion).

**Beräkning för varje nod:**
* Iterera genom samtliga noder i lagret med en for-loop: `for (std::size_t i{}; i < nodeCount(); ++i)`.
* För varje nod `i`, beräkna den viktade summan:
    1. Starta med nodens bias-värde: `auto sum{myBias[i]}`.
    2. Lägg till varje vikt multiplicerat med motsvarande input: `for (std::size_t j{}; j < weightCount(); ++j)` där `sum += myWeights[i][j] * input[j]`.
* Applicera aktiveringsfunktionen på summan: `myOutput[i] = actFuncOutput(myActFunc, sum)`.

**Returvärde:** `true` vid lyckad feedforward.

**3.** Implementera funktionen `backpropagate` för utgångslager (med referensvärden):

**Indatakontroll:**
* Kontrollera att dimensionerna på referensvektorns storlek matchar antalet noder (`reference.size() == nodeCount()`).
* Om dimensionerna inte matchar: skriv ut felmeddelandet `"Output dimension mismatch: expected X, actual: Y!"` och returnera `false`.

**Felberäkning för varje nod:**
* Iterera genom samtliga noder i lagret: `for (std::size_t i{}; i < nodeCount(); ++i)`.
* För varje nod `i`:
    1. Beräkna det råa felet: `const auto err{reference[i] - myOutput[i]}`.
    2. Beräkna gradientfelet: `myError[i] = err * actFuncDelta(myActFunc, myOutput[i])`.

**Returvärde:** `true` vid lyckad backpropagation.

**4.** Implementera funktionen `backpropagate` för dolda lager (med fel och vikter från nästa lager):

**Indatakontroll:**
* Kontrollera att nästa lagers viktantal matchar detta lagers nodantal (`nextLayer.weightCount() == nodeCount()`).
* Om dimensionerna inte matchar: skriv ut felmeddelandet `"Layer dimension mismatch: expected X, actual: Y!"` och returnera `false`.

**Felberäkning för varje nod:**
* Iterera genom samtliga noder i detta lager: `for (std::size_t i{}; i < nodeCount(); ++i)`.
* För varje nod `i`:
    1. Initiera variabel som lagrar det beräknade råa felet: `double err{}`.
    2. Summera samtliga fel från nästa lager: `for (std::size_t j{}; j < nextLayer.nodeCount(); ++j)`
        * `err += nextLayer.error()[j] * nextLayer.weights()[j][i]`.
    3. Beräkna gradientfelet: `myError[i] = err * actFuncDelta(myActFunc, myOutput[i])`.

**Returvärde:** `true` vid lyckad backpropagation.

**5.** Implementera funktionen `optimize`:

**Indatakontroll:**
* Kontrollera att lärhastigheten är giltig (`learningRate > 0.0`).
* Om lärhastigheten är ogiltig: skriv ut felmeddelandet `"Invalid learning rate X!"` och returnera `false`.
* Kontrollera att inputstorleken matchar antalet vikter per nod (`input.size() == weightCount()`).
* Om dimensionerna inte matchar: skriv ut felmeddelandet `"Input dimension mismatch: expected X, actual: Y!"` och returnera `false`.

**Parameteruppdatering för varje nod:**
* Iterera genom samtliga noder i lagret: `for (std::size_t i{}; i < nodeCount(); ++i)`.
* För varje nod `i`:
    1. Uppdatera bias: `myBias[i] += myError[i] * learningRate`.
    2. Uppdatera alla vikter: `for (std::size_t j{}; j < weightCount(); ++j)`
        * `myWeights[i][j] += myError[i] * learningRate * input[j]`

**Returvärde:** `true` vid lyckad optimering.
