# 2025-08-20 - Lektion 1

## Dagordning
* Introduktion - Genomgång av kursens innehåll samt upplägg.
* Övningsuppgifter med vektorer i C++.

## Mål med lektionen
* Kunna använda vektorer i C++ (behövs för implementering av maskininlärningsalgoritmer i denna kurs).

## Instruktioner
* Kolla gärna kursplan samt preliminär planering på Classroom.
* Läs om vektorer nedan.
* Se övningsuppgifter i underkatalogen [exercises](./exercises/README.md).

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Funktionstemplates i C++ (återigen med vektorer).
* Inlämningsuppgift innefattande program med vektorer i C++.

## För samtliga kodexempel

Läs avsnittet [Kompilering samt exekvering av kod](../../README.md#kompilering-samt-exekvering-av-kod) för alternativ
gällande kompilering samt exekvering av kod.

Oavsett om du använder en online-kompilator eller GCC-kompilatorn i Linux, kompilera och kör följande program i en fil döpt `main.cpp`:

```cpp
#include <iostream>

int main()
{
    std::cout << "Hello to C++!\n";
    std::cout << "We're going to use this language for machine learning!\n";
    return 0;
}
```

Utskriften bör bli:  

```
Hello to C++!
We're going to use this language for machine learning!
```

Några saker att notera:
* Vi börjar med att inkludera standardheadern `iostream`, som innehåller funktioner och operatorer för utskrift.
* Allt som ingår i C++ standardbibliotek är implementerat i en så kallad namnrymd döpt `std`, vilket står för *standard* som i *standard library*. Denna namnrymd fungerar lite som en katalog/mapp och minskar bland annat risken för namnkrockar; det gör då inget att vi har multipla typer med samma namn, så länge de ligger i olika namnrymder, på samma sätt som att vi på vår dator kan ha filer med samma namn förutsatt att de ligger i olika kataloger. 
* När vi använder något från standardbiblioteket, vare sig det är en typ, funktioner eller annat måste vi därmed använda prefixet `std`, följt av separatorn `::``, exempelvis `std::vector`, `std::cout`, `std::list` med mera.
* `std::cout` är en operator för utskrift i terminalen *(cout = character out)*, som måste följas av `<<` innan vi lägger till vad vi vill skriva ut.
* `\n` är ett nyradstecken och medför att potentiell nästa utskrift genomförs på nästa rad.

## Vektorer i C++

Vektorer är standardtypen av dynamiska arrayer (tänk listor) i C++.  
För att använda vektorer behöver vi inkludera standardheadern `<vector>`:

```cpp
#include <vector>
```
Som nämndes tidigare gäller att allt som ingår i C++ standardbibliotek är implementerat i namnrymden `std`. När vi skapar en vektor från C++ standardbibliotek måste vi alltså föregå typen `vector` med `std`. Därmed måste vi skriva `std::vector`, vilket kan utläsas som *standardbibliotekets typ vector*. De två kolonnen utgör alltså en separator mellan namnrymden `std` samt typen `vector` (som är implementerad i denna namnrymd).

När vi skapar en vektor måste vi också välja vilken typ av datatyp denna ska innehålla, exempelvis heltal, flyttal eller text (exempelvis via `int`, `double` respektive `const char*`). Detta gör vi genom att deklarera datatypen inuti olikhetstecken `<>` direkt efter `std::vector`, exempelvis `std::vector<int>`. 

Som exempel, för att skapa en tom vektor döpt *numbers* innehållande heltal av datatypen `int` kan följande instruktion användas:

```cpp
std::vector<int> numbers{};
```

### Initiering av vektorer

Om vi vill initiera vektorn *numbers* med något i stället för att låta den vara tom vid start kan vi lägga till heltal mellan måsvingarna, exempelvis heltal 1 - 5 så som visas nedan:

```cpp
std::vector<int> numbers{1, 2, 3, 4, 5};
```

### Operatorn `[]` för vektorer

Vi kan erhålla innehållet i vektorn via index precis som i C. Därmed kan vi använda operatorn `[]`.
I nedanstående program skrivs det första talet (innehållet på index 0) ut i terminalen:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Print the first number of the vector in the terminal.
    std::cout << "First number: " << numbers[0U];

    // Terminate the program with success code 0.
    return 0;
}
```

Notera att vi måste använda operatorn `<<` mellan texten "First number: " samt talet vi vill skriva ut. Om vi inte hade gjort det hade utskriften bokstavligt blivit `numbers[0U]`, i stället för talet på index 0 i vektorn *numbers*.

**OBS!** `0U` betyder talet 0 på osignerad form (U = unsigned). I fall då ett givet tal är naturligt,
alltså 0 och uppåt, är användningen av osignerade heltal rekommenderat. Detta är exempelvis fallet för index
i vektorer, då index inte kan understiga 0.

### Iteration genom vektor via range-baserade for-loopar

Vi kan skriva ut vektorns innehåll genom att iterera från vektorns början till slut.  
Detta kan göras enkelt via en så kallad range-baserad for-loop, såsom visas nedan:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers.
    std::vector<int> numbers{1, 2, 3, 4, 5};
    
    // Prints the numbers on a single line in the terminal.
    for (auto& number : numbers)
    {
        std::cout << number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

Notera att:
* `for (auto& number : numbers)` kan utläsas *för alla tal number i vektorn numbers*. När vi använder operatorn `&` blir *number* en så kallad referens (tänk pekare, fast enklare använt) till varje ett visst tal i vektorn *numbers*. Vi låter kompilatorn välja referensens datatyp genom att skriva `auto`. Vi kommer prata mer om referenser senare.
* Vi lägger till ett blanksteg mellan varje tal genom att skriva ut " " efter varje tal.

### Vanliga metoder tillhörande typ `std::vector`
`std::vector` är en så kallad klass i C++, vilket är en datatyp som innehåller attribut, såsom element (tal) samt vektorns storlek, tillsammans med så kallade *metoder*. Metoder, vanligtvis kallade medlemsfunktioner i C++, är inget annat än funktioner tillhörande en klass. 

Några av de vanligaste metoderna presenteras kortfattat nedan:
* `push_back`: Lägger till ett element längst bak i vektorn.
* `pop_back`: Tar bort det sista elementet i vektorn (om ett sådant finns).
* `size`: Indikerar vektorns storlek i antalet element den rymmer.
* `clear`: Tömmer vektorn, dvs. tar bort samtliga element.
* `empty`: Indikerar om vektorn är tom (returnerar då `true`, annars`false`).
* `resize`: Ändrar vektorns storlek till önskat antal. Det går att ange startvärde för potentiella nya element om man vill.
* `begin`: Returnerar en iterator (tänk en pekare) till början av vektorn, vilket blir det första elementet i vektorn om ett sådant finns.
* `end`: Returnerar en iterator (tänk en pekare) till slutet av elementet i vektorn, alltså till adressen efter det sista elementet om ett sådant finns. Om vektorn är tom kommer denna iterator referera till samma adress som returnerar för metoden `begin`.
* `erase`: Raderar elementet på adressen angiven via en iterator. Det är också möjligt att radera flera konsekutiva element med denna metod.

Det finns många fler metoder tillhörande `std::vector`. Vid intresse, 
se [C++ referensmanual](https://en.cppreference.com/w/cpp/container/vector).

### Lägga till ett element via metoden `push_back`

Typen `std::vector` är dynamisk, vilket innebär att vi kan ändra dess storlek under programmets gång. Som exempel, för att lägga till heltal kan vi använda metoden (medlemsfunktionen) `push_back`. Med denna metod måste vi passera elementet vi vill lägga till, exempelvis ett heltal.

Nedan visas hur vi lägger till heltalet 6 längst bak i vektorn efter initieringen:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers.
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Add the number 6 to the back of the vector.
    numbers.push_back(6);
    
    // Print the numbers on a single line in the terminal.
    for (auto& number : numbers)
    {
        std::cout << number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

### Ta bort vektorns sista element via metoden `pop_back`

På samma sätt kan vi använda metoden `pop_back` för att ta bort det sista elementet i vektorn. När vi använder denna metod behöver vi inte passera något argument.  

Nedan visas hur vi initierar vektorn med heltal 1 - 5. Därefter tas det sista talet (5) ut, följt av
att talet 10 läggs till:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers.
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Remove the last number of the vector.
    numbers.pop_back();

    // Add the number 10 to the back of the vector.
    numbers.push_back(10);
    
    // Print the numbers on a single line in the terminal.
    for (auto& number : numbers)
    {
        std::cout << number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

### Ta reda på vektorns storlek via metoden `size`

Vektorns storlek (antalet element den innehåller) indikeras som ett heltal via metoden `size`.  
När vi kallar på denna metod behöver inget argument passeras.  

I nedanstående program skrivs antalet tal ut genom att kolla vektorns storlek innan talen skrivs ut:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers. 
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Push the number 6 to the back of the vector.
    numbers.push_back(6);

    // Print the size of the vector.
    std::cout << "Printing " << numbers.size() << " numbers:\n";
    
    // Print the numbers on a single line in the terminal.
    for (const auto& number : numbers)
    {
        std::cout << number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

### Iteration genom vektor via index

Nu när vi kan ta radera på vektorns storlek kan vi också iterera genom vektorn via index, så som vi hade gjort i C:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers.
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Push the number 6 to the back of the vector.
    numbers.push_back(6);

    // Print the size of the vector.
    std::cout << "Printing " << numbers.size() << " numbers:\n";
    
    // Print the numbers on a single line in the terminal.
    for (auto i{0U}; i < numbers.size(); ++i)
    {
        std::cout << numbers[i] << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

Notera att:
* Vi behöver inte ange datatyp på iterator `i` i for-loopen, i stället kan vi låta kompilatorn välja datatyp utefter vad vi initierar denna med via nyckelordet `auto`. Eftersom vi initierar `i` med ett heltal sätts datatypen automatiskt till `int`.
* I for-loopen initierar vi heltalet i med heltalet 0 via operatorn `{}`. Det hade gått lika bra att använda operatorn `=`, alltså `auto i = 0`, men i C++ är operatorn `{}` universell för initiering, så den går att använda för att initiera allt, medan operatorn `=` bara fungerar för vissa typer, såsom grundläggande datatyper från C.

### Tömma vektorn via metoden `clear`
En given vektor kan tömmas, vilket innebär att allt dess innehåll raderas, via metoden `clear`.  
Denna metod tar inte emot några ingående argument. 

I nedanstående program demonstreras hur en vektor innehållande fem element töms.  
Vektorns storlek skrivs ut både innan och efter tömningen:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding five numbers.
    std::vector<int> numbers{1, 2, 3, 4, 5};

    // Print the initial size of the vector.
    std::cout << "Initial size: " << numbers.size() << "\n";

    // Empty the vector.
    std::cout << "Emptying vector!\n";
    numbers.clear();

    // Print the vector size after emptying the vector.
    std::cout << "Size after clearing the vector: " << numbers.size() << "\n";

    // Terminate the program with success code 0.
    return 0;
}
```

### Kolla om vektorn är tom via metoden `empty`
Om vi enkelt vill kolla om en vektor är tom kan vi kalla på metoden `empty` utan några ingående argument.  
Denna metod returnerar `true` om vektorn är tom, annars `false`.

I nedanstående program skapar vi en vektor, som startar tom. Därefter tilldelas ett heltal.  
Vi kollar via en if-else sats ifall vektorn är tom både innan och efter heltalet läggs till  
och skriver ut aktuellt tillstånd i terminalen:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create an empty vector.
    std::vector<int> numbers{};

    // Print whether the vector is empty or not.
    std::cout << "State 1: ";
    if (numbers.empty()) { std::cout << "The vector is empty!\n"; }
    else { std::cout << "The vector is not empty!\n"; }
    
    // Push the number 100 to the back of the vector.
    numbers.push_back(100);

    // Print whether the vector is empty or not.
    std::cout << "State 2: ";
    if (numbers.empty()) { std::cout << "The vector is empty!\n"; }
    else { std::cout << "The vector is not empty!\n"; }
    
    // Terminate the program with success code 0.
    return 0;
}
```

### Ändra vektorns storlek via metoden `resize`
Storleken på en given vektor kan enkelt ändras via metoden `resize`. När denna metod anropas måste den nya storleken passeras. Nedan visas hur storleken på en tom vektor kan ändras till att rymma fem element. Vektorns nya element är dock inte initierade, så vad som ligger på dessa adresser (index 0 - 4 i vektorn) är odefinierat:

```cpp
// Empty the vector.
std::vector<int> numbers{};

// Resize the vector so that it has room for five numbers.
numbers.resize(5);
```

Om vi vill kan vi initiera samtliga nya element via ett andra ingående argument när metoden `resize` anropas, vilket är rekommenderat. Vi hade kunnat göra detta i ovanstående exempel genom att lägga till initieringsvärdet efter vektorns storlek. Vi modifierar ovanstående exempel så att samtliga nya element sätts till 0 vid start:

```cpp
// Empty the vector.
std::vector<int> numbers{};

// Resize the vector so that it has room for five numbers.
// Set the initial value of each number to 0.
numbers.resize(5, 0);
```

I nedanstående program skapas en vektor innehållande tre heltal vid start. Därefter ändras vektorns storlek till tio.  
De nya tal som läggs till sätts till 100. Vektorns innehåll skrivs ut i terminalen:

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding three integers.
    std::vector<int> numbers{1, 2, 3};

    // Resize the vector to hold ten elements, set each new element to 100.
    numbers.resize(10, 100);
    
    // Print the numbers on a single line in the terminal.
    for (auto& number : numbers)
    {
        std::cout << number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

#### Erhåll vektorns början och slut via metoder `begin` och `end`
Metoderna `begin` och `end` kan användas för att enkelt ta reda på början och slutet på en given vektor. Detta kan exempelvis användas för att iterera genom vektorn (via en for-sats) eller för att ta bort specifika element (se metoden `erase` nedan).

Som exempel, nedanstående for-sats kan användas för att iterera genom en vektor döpt *numbers*. Varje element skrivs ut följt av ett blanksteg. 

```cpp
#include <iostream>
#include <vector>

int main()
{
    // Create a vector holding three numbers.
    std::vector<int> numbers{1, 2, 3};
    
    // Print the numbers on a single line in the terminal.
    for (auto number{numbers.begin()}; number < numbers.end(); ++number)
    {
        std::cout << *number << " ";
    }
    // Terminate the program with success code 0.
    return 0;
}
```

Notera att:
* En dereferensoperator måste användas för att få tillgång till innehållet på adressen som iteratorn `number` pekar på, på samma sätt som när pekare används i C. `*number` kan därmed avläsas som innehållet på adressen som `number` pekar på.
* Vi kan med fördel använda nyckelordet `auto` för iteratorer. Om vi hade velat skriva ut typen hade det blivit mycket längre, då denna är `std::vector<int>::iterator` i detta fall. För mer komplicerade typer kan det bli väldigt mycket mer att skriva och svårare att läsa.

#### Ta bort specifika element via metoden `erase`
Metoden `erase` fungerar utmärkt för att ta bort element på ett specifikt index. Detta index måste dock specificeras via en iterator. Som exempel, för att ta bort det tredje elementet, alltså elementet på index 2, i en vektor döpt *numbers*, kan vi därmed skriva

```cpp
numbers.erase(numbers.begin() + 2U);
```

Notera att vi specifikt skriver att vi ska radera elementet på startadressen (adressen för elementet på index 0) + 2U för att specificera index 2.

Med denna metod kan vi också ta bort multipla efterföljande element. Som exempel, för att ta bort elementet på index 2 - 5 i en vektor döpt *numbers* kan vi skriva

```cpp
numbers.erase(numbers.begin() + 2U, numbers.begin() + 5U);
```

### Funktioner med vektorer som ingående argument

#### Skapande av egna namnrymder

I detta fall kommer vi skapa några funktioner som är anpassade specifikt för vektorer. Vi kan placera dessa funktioner i en egen namnrymd döpt `vector`. För att skapa en namnrymd lägger via till ett så kallat `namespace` döpt `vector`, vars början och slut implementeras via måsvingar:

```cpp
namespace vector
{
    
} // namespace vector
```

#### Passera vektorer effektivt via referens

Vid funktionsanrop av någon av våra vektor-funktioner måste vi då använda prefixet `vector`, exempelvis `vector::print` för att kalla på en funktion döpt `print` i denna namnrymd, på samma sätt som att vi tidigare har behövt lägga till prefixet *std* för att använda vektorer samt operatorn `std::cout` från C++ standardbibliotek.

Vektorer är relativt stora objekt. Bland annat innehåller dessa:
* En pekare till det dynamiska fältet där elementen lagras. Så vektorns storlek är som tur är inte beroende av fältets storlek; annars hade vektorer kunnat bli väldigt stora. 
* En variabel som håller reda på antalet element i fältet.

I C hade motsvarande struct *int_vector* kunnat implementeras så som visas nedan:

```c

struct int_vector
{
    /** Pointer to field holding stored elements. */
    int* data;

    /** The size of the field in number of held elements. */
    size_t size;
};
```

**Tips:** 
*Vid intresse av hur vektorer kan implementeras i C++, se filer `vector.h` samt `vector_impl.h` tillhörande
ATMega328P-biblioteket [i detta repo](../../code/libatmega/cpp/README.md).*

Om ett 64-bitars system används kommer både pekare samt variabler av datatypen size_t allokera 8 byte (64 bitar) varv. Därmed kommer en vektor uppta minst 16 byte. I praktiken är vektorer ännu större på grund av ytterligare attribut, exempelvis 24 byte när vi använder tidigare nämnd online-kompilator.

Att vektorer är relativt stora objekt medför att vi inte bör passera kopior av dessa *(pass-by-value)* vid funktionsanrop. I stället bör vi passera dessa via referens *(pass-by-reference)*, alltså via dess adress. 

Detta medför att vid ett funktionsanrop där vi passerar en vektor kan vi passera en C++-referens eller pekare (8 byte på ett 64-bitars system) i stället för att först skapa en 24-bitars kopia av vektorn (i praktiken en klon), för att sedan passera hela denna kopia. Detta är mycket effektivare, då vi
* Inte behöver kopiera vektorn, vilket hade medfört overhead (det tar upp instruktioner och tid att genomföra kopieringen).
* Det är mycket smidigare att skicka en liten referens/pekare i stället för att skicka hela kopian på stacken. Om ett 64-bitars system används hade referensen/pekaren kunnat lagras på en enda minnesadress, medan vektorkopian hade krävt tre.

*Som tumregel kan du komma ihåg att passera allt som inte är en grundläggande datatyp, så som int och double, via referens, då referensen som regel kommer vara mindre än en eventuell kopia.*

I C++ är det mycket enkelt att passera objekt via referens; vi behöver bara lägga till ett `&` (dvs. en adressoperator) på objektet som ska passeras via referens, , i övrigt fungerar det samma som en vanlig variabel. C++-referenser kan därmed tänkas fungera (ungefär) som pekare, men används som vanliga variabler. 

För att passera en vektor innehållande heltal till en funktion döpt `print` kan vi därmed använda följande deklaration:

```cpp
void print(std::vector<int>& numbers);
```

Om vi bara vill läsa innehållet ur vektorn utan att råka ändra det bör
vi deklarera att vektorn är konstant via nyckelordet `const`:


```cpp
void print(const std::vector<int>& numbers);
```

I funktionen `print` kan vi sedan iterera så som demonstrerat tidigare
för att skriva ut vektorns innehåll:

```cpp
void print(const std::vector<int>& numbers)
{
    // Print the numbers on separate lines in the terminal.
    for (const auto& number : numbers)
    {
        std::cout << number << "\n";
    }
}
```

Vi kan också använda bindestreck för att ge ett "tak" samt "golv" på utskriften. Vi kan också se till att dessa inte skrivs ut om vektorn är tom (då avslutas funktionen direkt):

```cpp
void print(const std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Print the opening line separator.
    std::cout << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (const auto& number : numbers)
    {
        std::cout << number << "\n";
    }
    // Print the terminating line separator.
    std::cout << "--------------------------------------------------------------------------------\n\n";
}
```

##### Exempelprogram

I nedanstående program skapas en vektor innehållande heltal 0 - 9.
Vektorns innehåll skrivs sedan ut genom att vi kallar på funktionen `vector::print`, alltså funktionen `print` i namnrymden *vector:*

```cpp
#include <iostream>
#include <vector>

namespace vector
{
/**
 * @brief Print numbers in given vector. Each number is printed on a separate line.
 * 
 * @param[in] numbers Vector holding the numbers to print.
 */
void print(const std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Print the opening line separator.
    std::cout << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (auto& number : numbers)
    {
        std::cout << number << "\n";
    }
    // Print the terminating line separator.
    std::cout << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

int main()
{
    // Create a vector holding ten ingers.
    std::vector<int> numbers{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Print the numbers in the terminal.
    vector::print(numbers);

    // Terminate the program with success code 0.
    return 0;
}
```

#### Användning av defaultargument

I C++ kan vi implementera ingående argument till funktioner som är valfria. Om inget värde anges används i stället ett förvalt värde, ett så kallat default-värde. Detta implementeras genom att skriva defaultvärdet direkt efter det ingående argumentet i argumentlistan.

Nedan visas deklaration av en funktion döpt `assign`, vars syfte är att tilldela ett heltal till refererad vektor *numbers*:

```cpp
void assign(std::vector<int>& numbers, const std::size_t size, 
            const int startVal = 0, const int stepVal = 1);
```
Notera att:
* Vektorns storlek efter tilldelningen kan väljas via argumentet `size`. Vi sätter detta argument till `std::size_t`, alltså en osignerad typ, så att man inte ska kunna ange ett negativt heltal.
* Startvärdet, alltså det första heltal som ska läggas till, kan väljas via ingående argument *startVal*, som har defaultvärdet 0 (därav = 0 direkt efter deklarationen). Det går att välja startvärde efter behov, men om man inte anger något startvärde specifikt används 0 som startvärde.
* Stegvärdet, alltså skillnaden mellan varje tillagt heltal, kan väljas via ingående argument *stepVal*, som har defaultvärdet 1 (därav = 1 direkt efter deklarationen). Det går att välja stegvärde efter behov, men om man inte anger något startvärde specifikt används 1. Om man används 0 som startvärde kommer då heltal {0, 1, 2, 3...} tilldelats. Däremot om man hade valt stegvärde 2 hade heltal {0, 2, 4, 6...} tilldelats.

Defaultvärden ger oss därmed möjligheten att göra vissa parametrar valfri att ange. Den enda haken med defaultvärden är att de måste placeras sist i argumentlistan.

Funktionen `assign` kan exempelvis implementeras i namnrymden `vector` så som visas nedan:

```cpp
void assign(std::vector<int>& numbers, const std::size_t size, 
            const int startVal = 0, const int stepVal = 1)
{
    // Resize the vector to the desired size.
    numbers.resize(size);

    // Initialize the next value to assign.
    auto nextNumber{startVal};
    
    // Assign values to the vector, incrementing by stepVal each time.
    for (auto& number : numbers)
    {
        number = nextNumber;
        nextNumber += stepVal;
    }
}
```

Anta att vi deklarerar tre tomma vektorer *v1, v2* och *v3* så som visas nedan:

```cpp
std::vector<int> v1{}, v2{}, v3{};
```

Vi kan därefter tilldela heltal till vektorerna så som visas nedan:

```cpp
// Assign {0, 1, 2...9}.
vector::assign(v1, 10);

// Assign {0, 2, 4}.
vector::assign(v2, 3, 0, 2);

// Assign {100, 101...119}.
vector::assign(v3, 20, 100);
```

Storlek, start- och stegvärde för respektive vektor vi presenteras nedan:
 
|    Vektor    |    Storlek   |  Startvärde  |  Stegvärde   |
| :----------: | :----------: | :----------: | :----------: |
|      v1      |      10      |      0       |      1       |
|      v2      |      3       |      0       |      2       |
|      v3      |      20      |      100     |      1       |


##### Exempelprogram

I nedanstående program tilldelas 50 heltal {100, 198} via anrop
av funktionen `assign`. Vektorns innehåll skrivs slutligen ut via anrop
av funktionen `print`:

```cpp
#include <iostream>
#include <vector>

namespace vector
{ 
/**
 * @brief Assign numbers to given vector.
 * 
 * @param[in] numbers  The vector for which to assign.
 * @param[in] size     The desired size of the vector in number of elements.
 * @param[in] startVal The value of the first element (default = 0).
 * @param[in] stepVal  The difference between each added element (default = 1).
 */  
void assign(std::vector<int>& numbers, const std::size_t size, 
            const int startVal = 0, const int stepVal = 1)
{
    // Resize the vector to the desired size.
    numbers.resize(size);

    // Initialize the next value to assign.
    auto nextNumber{startVal};
    
    // Assign values to the vector, incrementing by stepVal each time.
    for (auto& number : numbers)
    {
        number = nextNumber;
        nextNumber += stepVal;
    }
}

/**
 * @brief Print numbers in given vector. Each number is printed on a separate line.
 * 
 * @param[in] numbers Vector holding the numbers to print.
 */
void print(const std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Print the opening line separator.
    std::cout << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (auto& number : numbers)
    {
        std::cout << number << "\n";
    }
    // Print the terminating line separator.
    std::cout << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

int main()
{
    // Create a vector holding twenty integers in the range [100, 198].
    std::vector<int> numbers{};
    vector::assign(numbers, 50, 100, 2);

    // Print the numbers in the terminal and terminate the program.
    vector::print(numbers);
    return 0;
}
```

#### Ett sista exempel

Som ett sista exempel sätts utströmmen som används för utskrift
i vår funktion `vector::print` till valbar, där terminalutskrift används som default. Därmed kan vi välja mellan att skriva ut i terminalen eller till en fil.

Vi ändrar därmed funktionsdeklarationen så att
denna innehåller ett andra ingående argument *ostream*, som utgör en referens till den utström via ska använda. För att utskrift som default ska ske till terminalen sätts defaultvärdet till `std::cout`:

```cpp
void print(const std::vector<int>& numbers, std::ostream& ostream = std::cout);
```

Vi modiferar sedan funktionens innehåll så att vi använder vald utström *ostream* i stället för terminalutskrift `std::cout`:

```cpp
void print(const std::vector<int>& numbers, std::ostream& ostream = std::cout)
{
    // Terminate the function is the vector is empty.
    if (numbers.empty()) { return; }

    // Print the opening line separator.
    ostream << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (const auto& number : numbers)
    {
        ostream << number << "\n";
    }
    // Print the terminating line separator.
    ostream << "--------------------------------------------------------------------------------\n\n";
}
```

Om vi kallar på funktionen så som tidigare, alltså bara med en vektor som ingående argument, sker terminalutskrift:

```cpp
int main()
{
    // Create a vector holding three integers.
    std::vector<int> numbers{1, 2, 3};

    // Print the numbers in the terminal (using the default output stream).
    vector::print(numbers);

    // Terminate the program with success code 0.
    return 0;
}
```

Däremot om vi lägger till standardheadern *fstream* och skapar ett objekt av typen `std::ofstream`, kan vi skriva till en fil:

```cpp
#include <fstream>
```

Som exempel, för att öppna en fil döpt `numbers.txt` för skrivning kan ett objekt av typen `std::ofstream` döpt *ostream* skapas, så som visas nedan. Om `numbers.txt` inte finns skapas den när denna instruktion exekverar:

```cpp
// Open the file `numbers.txt` for writing.
std::ofstream ostream{"numbers.txt"};
```

Vi hade kunnat skriva direkt till denna fil därefter, antingen text eller tal:

```cpp
// Open the file `numbers.txt` for writing.
std::ofstream ostream{"numbers.txt"};

// Print content to the file via the associated file stream.
ostream << "This is written to a file named numbers.txt";
ostream << "Writing number " << 5 << "\n";
```

Vi kan också använda objektet *ostream* vid funktionsanropet av `vector::print` för att skriva vektorns innehåll till filen `numbers.txt`, såsom visas nedan:

```cpp
int main()
{
    // Create a vector holding three integers.
    std::vector<int> numbers{1, 2, 3};

    // Open the file `numbers.txt` for writing.
    std::ofstream ostream{"numbers.txt"};

    // Write the numbers to the file by passing the associated output stream to the print function.
    vector::print(numbers, ostream);

    // Terminate the program with success code 0.
    return 0;
}

##### Exempelprogram

I nedanstående program skrivs tilldelas en vektor 50 heltal {100, 102, 104... 198}. Dessa tal skrivs både ut i terminalen samt till en fil döpt `numbers.txt` genom att kalla på funktionen `vector::print` två gånger:
* Vid det första anropet används default-utström `std::cout`, vilket medför att vektorns innehåll skrivs ut i terminalen.
* Vid det andra anropet används utströmmen *ostream*, som pekar på filen *number.txt*, som utström. Därmed skrivs vektorns innehåll till filen `numbers.txt`:

```cpp
#include <fstream>
#include <iostream>
#include <vector>

namespace vector
{ 
/**
 * @brief Assign numbers to given vector.
 * 
 * @param[in] numbers  The vector for which to assign.
 * @param[in] size     The desired size of the vector in number of elements.
 * @param[in] startVal The value of the first element (default = 0).
 * @param[in] stepVal  The difference between each added element (default = 1).
 */  
void assign(std::vector<int>& numbers, const std::size_t size, 
            const int startVal = 0, const int stepVal = 1)
{
    // Resize the vector to the desired size.
    numbers.resize(size);

    // Initialize the next value to assign.
    auto nextNumber{startVal};
    
    // Assign values to the vector, incrementing by stepVal each time.
    for (auto& number : numbers)
    {
        number = nextNumber;
        nextNumber += stepVal;
    }
}

/**
 * @brief Print numbers in given vector. Each number is printed on a separate line.
 * 
 * @param[in] numbers Vector holding the numbers to print.
 */
void print(const std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Print the opening line separator.
    std::cout << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (auto& number : numbers)
    {
        std::cout << number << "\n";
    }
    // Print the terminating line separator.
    std::cout << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

/**
 * @brief Print numbers in the terminal and to a file by invoking the `vector::print` function.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create a vector holding twenty integers in the range [100, 198].
    std::vector<int> numbers{};
    vector::assign(numbers, 50, 100, 2);

    // Print the numbers in the terminal.
    vector::print(numbers);

    // Write the numbers to a file named `numbers.txt`.
    std::ofstream ostream{"numbers.txt"};
    vector::print(numbers, ostream);

    // Terminate the program with success code 0.
    return 0;
}
```

Ovanstående program finns tillgänglig tillsammans med en makefil för kompilering och körning av koden i underkatalogen `vector`.  
Instruktioner för byggande samt körande av koden finns i bifogad README-fil.

### Ytterligare information
**1.** Under nästa lektion kommer vi gå igenom hur man kan implementera så kallade funktionstemplates för att möjliggöra att funktioner kan ta emot vektorer av olika datatyper. Vi kan också lägga till så kallade *constraints* (begränsningar) på vilka typer som kan användas, exempelvis så att funktionen `vector::assign` enbart kan användas för numeriska typer såsom `int`, `double` samt `std::size_t`.

**2.** Lite mer kortfattad information om vektorer finns [här](https://www.geeksforgeeks.org/vector-in-cpp-stl/).  

**3.** Mer detaljerad information om vektorer finns [här](https://en.cppreference.com/w/cpp/container/vector).  
