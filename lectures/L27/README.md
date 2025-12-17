# L27 - Konvolutionella neurala nätverk (del V)

## Dagordning
* Implementation av flatten-lager i mjukvara.
* Fortsatt arbete med `Inlämningsuppgift 6 - Konvolutionellt neuralt nätverk.`

## Mål med lektionen
* Kunna implementera enkla flatten-lager i mjukvara.

## Instruktioner
* Öppna filen [flatten_demo.cpp](./flatten_layer/cpp/flatten_demo.cpp). 
En strukt döpt `ml::FlattenLayer` ska läggas till i denna fil att realisera enkla flatten-lager. \
Se mer information i [bilaga A](#bilaga-a---skapande-av-enkla-flatten-lager-i-c) nedan.
* Som referens, se motsvarande implementation i Python [här](./flatten_layer/python/flatten_demo.py). \
Testkör programmet via kommandot `python3 flatten_demo.py` i terminalen i motsvarande katalog.
* Om ni är intresserade finns en något mer utvecklad implementation i C [här](./flatten_layer/c/source/flatten_demo.c).
    * I denna implementation demonstreras inkapsling genom att strukten `flatten_layer_t` bara implementeras i källkodsfilen 
    [flatten_layer.c](./flatten_layer/c/source/ml/flatten_layer.c). 
    * Motsvarande headerfil  [flatten_layer.h](./flatten_layer/c/include/ml/flatten_layer.h) innehåller endast funktioner 
    för att använda flatten-lager.
    * Struktens medlemsvariabler hålls därmed privata, på samma sätt som nyckelordet `private` används i C++. 
    * Här demonstreras även hur matriser kan implementeras i C via strukten [matrix_t](./flatten_layer/c/include/ml/matrix.h).

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Slutförande av `Inlämningsuppgift 6 - Konvolutionellt neuralt nätverk.`
* Kursavslut.

## Bilaga A - Skapande av enkla flatten-lager i C++

### Uppgiftsbeskrivning

---

En strukt döpt `ml::FlattenLayer` ska läggas till i [flatten_demo.cpp](./flatten_layer/cpp/flatten_demo.cpp)
för att realisera enkla flatten-lager. \
För att hålla det så enkelt som möjligt implementerar vi en strukt och slopar get- och set-metoder, 
raderande av copy- och move-konstruktorer med mera.

Studera koden i funktionen `main`. Er implementation ska skrivas så att denna kod fungerar.\
Därmed ska följande kod fungera för att skapa och använda ett flatten-lager döpt `flattenLayer`:

```cpp
// Create a flatten layer: 4x4 input, produces 1x16 output.
constexpr std::size_t inputSize{4U};
FlattenLayer flattenLayer{inputSize};

// Example 4x4 input matrix (could represent an image or feature map).
const Matrix2d input{{2, 1, 6, 1},
                     {3, 0, 4, 6},
                     {1, 2, 4, 5},
                     {3, 4, 7, 7}};

// Perform feedforward (flatten the input).
flattenLayer.feedforward(input);

// Example output gradients (same shape as flattened output, used for backpropagation demo).
const Matrix1d outputGradients{1, 2, 3, 4, 8, 7, 6, 5, 0, 2, 4, 8, 9, 7, 5, 3};

// Perform backpropagation (unflatten the output gradients).
flattenLayer.backpropagate(outputGradients);
```

### Kompilering samt exekvering av programmet

---

Som vanligt kan programmet köras genom att skriva kommandot `make` i terminalen:

```bash
make
```

När implementationen är färdig, avkommentera kompilatorflaggorna `CXX_FLAGS` i [makefilen](./flatten_layer/cpp/makefile).
Ändra alltså från följande:

```bash
# TODO: Uncomment this line once the implementation is finished.
#CXX_FLAGS := -std=c++17 -Wall -Werror -DFLATTEN_LAYER_IMPLEMENTED
```
till

```bash
CXX_FLAGS := -std=c++17 -Wall -Werror -DFLATTEN_LAYER_IMPLEMENTED
```

Ni kan då också ta bort header guards `FLATTEN_LAYER_IMPLEMENTED` ur 
[flatten_demo.cpp](./flatten_layer/cpp/flatten_demo.cpp) om ni vill. \
I så fall, ändra alltså från följande:

```cpp
/**
 * @brief Create and demonstrate a simple flatten layer.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{
//! @todo Remove this header guard (and/or uncomment the compiler flags in the makefile) once the
//        implementation is finished.
#ifdef FLATTEN_LAYER_IMPLEMENTED
    // Function content.
    return 0;

//! @todo Remove this header guard (and/or uncomment the compiler flags in the makefile) once the
//        implementation is finished.
#endif /** FLATTEN_LAYER_IMPLEMENTED */
}
```

till

```cpp
/**
 * @brief Create and demonstrate a simple flatten layer.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{
    // Function content.
    return 0;
}
```

---
