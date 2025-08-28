# 2025-08-29 - Lektion 4

## Dagordning
* Filorganisation för klasser - uppdelning i header- och sourcefiler.
* Kopierings- och förflyttningssemantik samt nyckelord `noexcept` och `explicit`.
* Inlämningsuppgift innefattande skapandet av klasser i C++.


## Mål med lektionen
* Kunna dela upp klasser i header- och sourcefiler.
* Känna till och kunna applicera kopierings- och move-konstruktorer, `noexcept` samt `explicit` för en klass.

## Instruktioner
* Läs fördjupad information om klasser i detta dokument.
* Ni kommer få möjlighet att arbeta med inlämningsuppgift 2 under lektionen.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Interfaces och arv i C++.

### Fördjupning gällande klasser

#### Nyckelordet `noexcept`
I C är det vanligt att hantera fel via returkoder. Ofta implementeras funktioner med returtypen `int`, där
0 returneras vid korrekt exekvering, medan en (oftast) icke-negativ felkod returneras vid fel. 

Ett exempel på detta är funktionen `main`, som returnerar 0 som default sedan C99.  Därmed är det
vanligt att se `Hello World`-program såsom visas nedan. Notera att funktionen i fråga har returtypen
`int`, trots att inget värde explicit returneras:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello!\n");

    // 0 is returned by default here to indicate success before terminating the program.
}
```

Tidigare i kursen har returkod 0 alltid returnerats explicit för att tydliggöra detta:

```c
#include <stdio.h>

int main(void)
{
    printf("Hello!\n");

    // Return 0 to indicate success before terminating the program.
    return 0;
}
```

Vi kan därefter returnera värdet som ej är lika med 0 för att indikera att programmet inte
exekverade som tänkt. Se exempelvis programmet nedan, där användaren ombeds mata in ett positivt
heltal. Om ett icke-positivt heltal metas in returnerar felkod -1, annars returneras 0:

```c
#include <stdio.h>

// -----------------------------------------------------------------------------
int read_integer(void)
{
    // Read the input from the terminal, parse to `number`.
    int number = 0;
    scanf("%d", &number);
    
    // Return the entered number.
    return number;
}

// -----------------------------------------------------------------------------
int main(void)
{
    // Ask the user to enter a positive integer.
    printf("Enter a positive integer: ");
    
    // Read the number from the terminal.
    const int number = read_integer();
    
    // Terminate the program with error code -1 if the input is invalid.
    if (0 >= number) 
    { 
        printf("\nInvalid input!\n");
        return -1; 
    }
    
    // Print the number and terminate the program with success code 0.
    printf("Number: %d\n", number);
    return 0;
}
```

Att använda felkoder är väldigt vanligt i C. Det förekommer också en del i C++, speciellt för inbyggda system
och andra minnesbegränsade applikationer. 

Men för "vanliga" appliktioner, både i C++ och många andra programspråk, används i stället så kallade undantag för att hantera fel som uppstår under programmets körning. Som exempel visas en C++-version av ovanstående program.
I detta fall krashar programmet ifall ett icke-positivt heltal matas in från terminalen via ett
kastar ett undantag. Notera att headerfilen `<stdexcept>` har lagts till i detta fall:

```cpp
#include <iostream>
#include <stdexcept>

// -----------------------------------------------------------------------------
int readInteger()
{
     // Read the input from the terminal, parse to `number`.
    int number{};
    std::cin >> number;
    
    // Throw an expection if the input is invalid.
    if (0 >= number) 
    { 
        throw std::invalid_argument("Invalid input!");
    }
    // Return the entered number.
    return number;
}

// -----------------------------------------------------------------------------
int main()
{
    // Ask the user to enter a positive integer.
    std::cout << "Enter a positive integer: ";
    
    // Print the number and terminate the program with success code 0.
    printf("Number: %d\n", readInteger());
    return 0;
}
```

Undantag är mycket vanligt i C++ standarbibliotek och ersätter i de flesta fall returkoder så som
används i C. Undantag kräver dock mer resurser, vilket är anledningen till att det inte är så förekommande
i inbyggda system.

* Nyckelordet `noexcept` används för att ange att en funktion inte kommer att kasta några så kallade undantag. Detta kan medföra att kompilatorn kan göra diverse optimeringar, såsom att inte behöva förbereda för så kallad stack unwinding (att "rulla tillbaka" stacken vid ett undantag äger rum), vilket gör att koden kan bli både snabbare och mindre. 

Nyckelordet `noexcept` läggs direkt efter ingående argument i en given funktion.
Som exempel, nedan används nyckelordet `noexcept` för ovanstående funktion `square`, som returnerar
kvadraten av ett givet tal `x`:

```cpp
double square(const double x) noexcept
{ 
    return x * x; 
}
```

Det är rekommderat att använda `noexcept` för samtliga funktioner som inte kastar ett undtantag direkt
eller indirekt. Därmed måste man ha i åtanke om funktionerna man kallas på kan kasta ett undantag;
är man osäker kan man söka på funktionen ifråga, eller fråga Chat GPT. 

Värt att tänka på är att samtliga C-funktioner inte kastar några undantag, då detta koncept inte
existerar i C. Som exempel, funktionerna i nedanstående kod kan säkert markeras `noexcept`, då C-funktionerna
`std::malloc` samt `std::free` från `<cstdlib>` används:

```cpp
#include <cstdlib>
#include <iostream>

/**
 * @brief Simple vector structure.
 * 
 * @tparam T The vector type.
 */
template<typename T>
struct Vector
{
    /** Pointer to field holding stored elements. */
    T* data{nullptr};

    /** The size of the vector in number of elements. */
    std::size_t size{};

    /**
     * @brief Clear allocated resources before deleting the vector.
     */
    ~Vector() noexcept { clear(); }

    /**
     * @brief Clear the vector.
     */
    void clear() noexcept
    {
        // Free allocated resources.
        std::free(data);

        // Reinitialize the member variables.
        data = nullptr;
        size = 0U;
    }

    /**
     * @brief Push an element to the vector of the vector.
     * 
     * @param[in] element The element to add.
     * 
     * @return True if the value was added, false otherwise.
     */
    bool pushBack(const T& element) noexcept
    {
        // Create a copy of the data field with space for one more element.
        auto copy{static_cast<T*>(std::realloc(data, sizeof(T) * (size + 1U)))};

        // Return false if the copy couldn't be created (instead of throwing an exception).
        if (!copy) { return false; }

        // Add the new element to the back of the copy vector, then increment the vector size.
        copy[size++] = element;

        // Set the data pointer to the new field (the old field was freed when the copy was made).
        data = copy;

        // Return true to indicate that the value was added.
        return true;
    }
};

/**
 * @brief Example using C functions to implement a simple vector structure.
 *        The `noexcept` keyword is used, since C functions are used for the memory handling.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create an empty integer vector.
    Vector<int> numbers{};

    // Push ten values to the vector, then print the vector content.
    for (int i{}; 10 > i; ++i)
    {
        numbers.pushBack(i);
    }

    // Print each number on a separate line.
    for (std::size_t i{}; numbers.size > i; ++i)
    {
        std::cout << numbers.data[i] << "\n";
    }

    // The destructor of the `Vector` struct releases allocated resources automatically when
    // the `numbers` vector goes out of scope.
    return 0;
}
```

För vår klass `Person` från föregående lektion kan samtliga metoder förutom utskriftsfunktionen
`print` markeras `noexcept`, då enbart tilldelningar samt retur av strängar och tal görs. Utskriftsoperatorn `std::cout` kommer normalt inte generera undantag vid fel, men det kan göras om undantag har aktiverats, såsom visas nedan:

```cpp
// Enable expections for `std::cout`.
std::cout.exceptions(std::ios::failbit | std::ios::badbit);
``` 

För att ta det säkra före det osäkra markerar vi inte metoden `print` `noexcept`, då vi
inte alltid kan garentera att denna metod kastar ett undantag, hur icke-troligt det
må vara.

Klassen `Person` med nyckelordet `noexcept` tillagt på samtliga lämpliga metoder visas nedan:

```cpp
/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true) noexcept
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }

    /**
     * @brief Delete person instance.
     */
    ~Person() noexcept = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const noexcept { return myName; }

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const noexcept { return myAge; }
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const noexcept { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const noexcept { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const noexcept { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const noexcept { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) noexcept { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) noexcept { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) noexcept { myIsSingle = single; }
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const
	{
		ostream << "--------------------------------------------------------------------------------\n";
		ostream << "Name:\t\t" << myName << "\n";
		ostream << "Age:\t\t" << myAge << "\n";
		ostream << "Gender:\t\t" << genderStr() << "\n";
		ostream << "Address:\t" << myAddress << "\n";
		ostream << "Occupation:\t" << myOccupation << "\n";
		ostream << "Single:\t\t" << isSingleStr() << "\n"; 
		ostream << "--------------------------------------------------------------------------------\n\n";
	}
	
	Person() = delete; // No default constructor.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const noexcept
	{
		if (myGender == Gender::Male) { return "Male"; }
		else if (myGender == Gender::Female) { return "Female"; }
		else { return "Other"; }
	}

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const noexcept { return myIsSingle ? "Yes" : "No"; }
	
    /** The person's name. */
    const char* myName;

    /** The person's age. */
    unsigned myAge;

    /** The person's gender. */
    Gender myGender;

    /** The person's address. */
    const char* myAddress;

    /** The person's occupation. */
    const char* myOccupation;

    /** Indicate whether the person is single. */
    bool myIsSingle;
};

```

#### Nyckelordet `explicit`

Under föregående lektion implementerades följande konstruktor för klassen `Person`:

```cpp
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true)
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }
};
```

Via ovanstående konstruktor kan vi skapa en `Person`-instans och passera denna via referens
till en funktion `personPrint`, som visas nedan:

```cpp
/**
 * @brief Print personal data about the given person.
 * 
 * @param[in] person The person in question.
 */
void personPrint(const Person& person)
{
    person.print();
}

/**
 * @brief Create a person instance and send a reference to this instance for printing.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    Person p1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
    personPrint(p1);
    return 0;
}
```

Vi kan också skapa objekt via kopieringsinitiering, såsom visas nedan. Notera att vi inte har skapat
en `Person`-instans i detta fall - en lista med argument passeras till `Person-print`. Dessa argument
konverteras implicit till en `Person`-instans.

```cpp
/**
 * @brief Print personal data about the given person.
 * 
 * @param[in] person The person in question.
 */
void personPrint(const Person& person)
{
    person.print();
}

/**
 * @brief Pass a list holding personal data to `personPrint` for printing.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // The list is implicitly converted to a `Person` instance, which is error-prone.
    personPrint({"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"});
    return 0;
}
```

Implicit omvandling på detta sätt kan medföra svårdetekterade buggar och rekommderas därmed inte.
Därmed är det bra att se till att implicit konvertering av `Person`-instanser inte är tillåtet. 
Detta kan åstadkommas genom att lägga till nyckelordet `explicit` framför konstruktorn, såsom visas nedan:

```cpp
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
                    const char* occupation, const bool single = true)
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }
};
```

#### Kopierings- och flyttningssemantik
För en given klass kan man lägga till så kallade kopierings- och förflyttningskonstruktorer.
Man kan också skapa tilldelningsoperatorer för kopiering och förflyttning. *Värt att notera är att
samtliga dessa fyra typer kommer skapas automatiskt av kompilatorn om de inte explicit skapas eller raderas.*

**Kopieringskonstruktorer** används för att skapa en kopia av ett givet objekt. Nedan visas hur man kan skapa 
en kopia `p2` av ett objekt `p1`:

```cpp
// Create original `Person` instance.
Person p1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

// Create a copy of `p1`.
Person p2{p1};

// The same name will be printed twice.
std::cout << "Name of p1: " << p1.name << "\n";
std::cout << "Name of p2: " << p2.name << "\n";
```

En explicit kopieringskonstruktor för klassen `Person` visas för demonstrations skull nedan.
Notera att vi kopierar innehållet från `other` till den nya instansen:

```cpp
/**
 * @brief Copy another instance.
 * 
 * @param[in] other Reference to the instance to copy.
 */
Person(const Person& other) noexcept
    : myName{other.myName}
    , myAge{other.myAge}
    , myGender{other.myGender}
    , myAddress{other.myAddress}
    , myOccupation{other.myOccupation}
    , myIsSingle{other.myIsSingle}
{}
```

**Förflyttningskonstruktorer** används för att flytta resurser från ett objekt till ett annat.
Original-objektet efter förflyttningen tom. Nedan visas hur resurser flyttas från ett objekt `p1` till ett objekt `p2` via funktionen `std::move` från headerfilen `<utility>`:

```cpp

#include <utility>

// Create original `Person` instance.
Person p1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

// Move resourses from `p1` to a new instane called `p2`.
Person p2{std::move(p1)};

// `p1` is now empty - no name will be printed.
// The name of `p2` is "Marie Nilsson".
std::cout << "Name of p1: " << p1.name << "\n";
std::cout << "Name of p2: " << p2.name << "\n";
```

En explicit förflyttningskonstruktor för klassen `Person` visas nedan för demonstrations skull.
Notera att vi kopierar innehållet från `other`, därefter nollställer vi `other`:

```cpp
/**
 * @brief Move resources from another instance.
 * 
 * @param[in] other Reference to the other instance from which to move resources.
 */
Person(Person&& other) noexcept
    : myName{other.myName}
    , myAge{other.myAge}
    , myGender{other.myGender}
    , myAddress{other.myAddress}
    , myOccupation{other.myOccupation}
    , myIsSingle{other.myIsSingle}
{
    // Clear `other` after the resources have been copied.
    other.myName       = nullptr;
    other.myAge        = {};
    other.myGender     = {};
    other.myAddress    = nullptr;
    other.myOccupation = nullptr;
    other.myIsSingle   = false;
}
```

Notera ovan att två adressoperatorer används för ingående argument `other` i detta fall. Detta är en så kallad
`rvalue-reference`, en speciell typ av referens som medför att kompilatorn kan ta över äganderätten över objektet; därmed kan resurserna flyttas.

Beroende på klassens funktion kan det vara fördelaktigt att kunna kopiera och/eller flytta resurser såsom visas ovan.
Ofta vill man dock förhindra att objekt kopieras eller flyttas, t.ex. om klassen hanterar resurser som inte får dupliceras.
Om kopiering eller flyttning tillåts kan det annars leda till dubbla frigöringar av resurser, minnesläckor eller andra svårupptäckta fel.

Så länge man inte explicit vill att instanser ska kunna kopieras rekommenderas därmed att radera kopierings- och
förflyttningskonstruktorn, vilket gör via nyckelordet `delete`. När vi gör detta kan vi slopa namnet på det
ingående argumentet samt `noexcept`:

```cpp
Person(const Person&) = delete; // No copy constructor.
Person(Person&&)      = delete; // No move constructor.
```

Som nämndes tidigare finns det också **tilldelningsoperatorer för kopiering samt flyttning**.
Som exempel, i följande fall skapas en kopia `p2` av ett objekt `p1` via en tilldelning:

```cpp
// Create original `Person` instance.
Person p1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

// Create a copy of `p1` via copy assignment.
Person p2 = p1;

// The same name will be printed twice.
std::cout << "Name of p1: " << p1.name << "\n";
std::cout << "Name of p2: " << p2.name << "\n";
```

En explicit tilldelningsoperator för kopiering visas nedan. Resurserna från `other` kopieras enbart
om `other` inte är samma instans som `this` (objektet vi vill kopiera till):

```cpp
/**
 * @brief Copy another instance via copy assignment.
 * 
 * @param[in] other Reference to the instance to copy.
 * 
 * @return Reference to this instance.
 */
Person& operator=(const Person& other) noexcept
{
    // Copy resources only if `other` isn't referring to this object.
    if (this != &other)
    {
        myName       = other.myName;
        myAge        = other.myAge;
        myGender     = other.myGender;
        myAddress    = other.myAddress;
        myOccupation = other.myOccupation;
        myIsSingle   = other.myIsSingle;
    }
    // Return a reference to this instance.
    return *this;
}
```

Vi kan också förflytta data via tilldelning via tilldelningsoperatorn för förflyttning.
I följande fall förflyttas resurser från en instance `p1` till `p2` via en tilldelning:

```cpp
#include <utility>

// Create original `Person` instance.
Person p1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

// Move resources from `p2` to `p1` via move assignment.
Person p2 = std::move(p1);

// `p1` is now empty - no name will be printed.
// The name of `p2` is "Marie Nilsson".
std::cout << "Name of p1: " << p1.name << "\n";
std::cout << "Name of p2: " << p2.name << "\n";
```

En explicit tilldelningsoperator för förflyttning visas nedan. Resurserna från `other` flyttas enbart
om `other` samt `this` inte är samma instans:

```cpp
/**
 * @brief Move resources from another instance.
 * 
 * @param[in] other Reference to the other instance from which to move resources.
 * 
 * @return Reference to this instance.
 */
Person& operator=(Person&& other) noexcept
{
    // Copy resources only if `other` isn't referring to this object.
    if (this != &other)
    {
        myName       = other.myName;
        myAge        = other.myAge;
        myGender     = other.myGender;
        myAddress    = other.myAddress;
        myOccupation = other.myOccupation;
        myIsSingle   = other.myIsSingle;

        // Clear `other` after the resources have been copied.
        other.myName       = nullptr;
        other.myAge        = {};
        other.myGender     = {};
        other.myAddress    = nullptr;
        other.myOccupation = nullptr;
        other.myIsSingle   = false;
    }
    // Return a reference to this instance.
    return *this;
}
```

Precis som var fallet för kopierings- och förflyttningskonstruktorn demonstrerade tidigare skapas 
tilldelningsoperatorer för kopiering samt förflyttning automatiskt om vi inte explicit skapar dem. 
Vill vi radera denna görs detta med nyckelordet `delete`:

```cpp
Person& operator=(const Person&) = delete; // No copy assignment.
Person& operator=(Person&&)      = delete; // No move assignment.
```

Så länge det inte finns en anledning till att kunna kopiera eller förflytta resurser bör därmed
dessa konstruktorer och tilldelningsoperatorer raderas. Länst ned i den publika delen av en klass,
i detta fall `Person`, kan vi då lägga följande fyra rader:

```cpp
Person(const Person&)            = delete; // No copy constructor.
Person(Person&&)                 = delete; // No move constructor.
Person& operator=(const Person&) = delete; // No copy assignment.
Person& operator=(Person&&)      = delete; // No move assignment.
```

Efter att ha raderat dessa konstruktorer samt tilldelningsoperatorer ser vår klass `Person` ut så som visas nedan:

```cpp
/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true) noexcept
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }

    /**
     * @brief Delete person instance.
     */
    ~Person() noexcept = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const noexcept { return myName; }

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const noexcept { return myAge; }
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const noexcept { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const noexcept { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const noexcept { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const noexcept { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) noexcept { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) noexcept { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) noexcept { myIsSingle = single; }
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const
	{
		ostream << "--------------------------------------------------------------------------------\n";
		ostream << "Name:\t\t" << myName << "\n";
		ostream << "Age:\t\t" << myAge << "\n";
		ostream << "Gender:\t\t" << genderStr() << "\n";
		ostream << "Address:\t" << myAddress << "\n";
		ostream << "Occupation:\t" << myOccupation << "\n";
		ostream << "Single:\t\t" << isSingleStr() << "\n"; 
		ostream << "--------------------------------------------------------------------------------\n\n";
	}
	
	Person()                         = delete; // No default constructor.
    Person(const Person&)            = delete; // No copy constructor.
    Person(Person&&)                 = delete; // No move constructor.
    Person& operator=(const Person&) = delete; // No copy assignment.
    Person& operator=(Person&&)      = delete; // No move assignment.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const noexcept
	{
		if (myGender == Gender::Male) { return "Male"; }
		else if (myGender == Gender::Female) { return "Female"; }
		else { return "Other"; }
	}

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const noexcept { return myIsSingle ? "Yes" : "No"; }
	
    /** The person's name. */
    const char* myName;

    /** The person's age. */
    unsigned myAge;

    /** The person's gender. */
    Gender myGender;

    /** The person's address. */
    const char* myAddress;

    /** The person's occupation. */
    const char* myOccupation;

    /** Indicate whether the person is single. */
    bool myIsSingle;
};
```

#### Uppdelning i header- och källkodsfiler för klasser
Ofta används objekt av många olika klasser i ett program. Dessa klasser är dessutom ofta större än klassen *person* som demonstrerades tidigare. Det är därmed opraktiskt att implementera samtliga klasser i en enda fil. 

Oftast skapar main en headerfil dedikerad för en specifik klass. Som exempel, för klassen *Person* kan vi med fördel lägga till headerfilen *person.h* och implementera denna klass där i.

Längst upp i *person.h* placerar vi direktivet #pragma once, som ser att vi inte råkar definiera multipla kopior av innehållet i denna headerfil om den inkluderas i flera olika filer, på samma sätt som header guards fungerar i C. Innehållet i *person.h* kommer därmed endast defineras en enda gång:

**Filen *person.h***

```cpp
#pragma once
```

Vi klistrar sedan in klassen *Person* samt enumerationsklassen `Gender` i denna fil. Vi lägger också till standardheader *iostream,* annars kommer inte *std::ostream* samt *std::cout* i metoden *print* att kännas igen, vilket kommer leda till ett kompileringsfel:

```cpp
/**
 * @brief Personal data implementation.
 */
#pragma once

#include <iostream>

/**
 * @brief Enumeration of genders.
 */
enum class Gender 
{ 
    Male,   /** Male. */
    Female, /** Female. */
    Other,  /** Other gender. */
};

/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true) noexcept
        : myName{name}
        , myAge{age}
        , myGender{gender}
        , myAddress{address}
        , myOccupation{occupation}
        , myIsSingle{single} 
    {
    }

    /**
     * @brief Delete person instance.
     */
    ~Person() noexcept = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const noexcept { return myName; }

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const noexcept { return myAge; }
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const noexcept { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const noexcept { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const noexcept { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const noexcept { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) noexcept { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) noexcept { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) noexcept { myIsSingle = single; }
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const
	{
		ostream << "--------------------------------------------------------------------------------\n";
		ostream << "Name:\t\t" << myName << "\n";
		ostream << "Age:\t\t" << myAge << "\n";
		ostream << "Gender:\t\t" << genderStr() << "\n";
		ostream << "Address:\t" << myAddress << "\n";
		ostream << "Occupation:\t" << myOccupation << "\n";
		ostream << "Single:\t\t" << isSingleStr() << "\n"; 
		ostream << "--------------------------------------------------------------------------------\n\n";
	}
	
	Person()                         = delete; // No default constructor.
    Person(const Person&)            = delete; // No copy constructor.
    Person(Person&&)                 = delete; // No move constructor.
    Person& operator=(const Person&) = delete; // No copy assignment.
    Person& operator=(Person&&)      = delete; // No move assignment.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const noexcept
	{
		if (myGender == Gender::Male) { return "Male"; }
		else if (myGender == Gender::Female) { return "Female"; }
		else { return "Other"; }
	}

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const noexcept { return myIsSingle ? "Yes" : "No"; }
	
    /** The person's name. */
    const char* myName;

    /** The person's age. */
    unsigned myAge;

    /** The person's gender. */
    Gender myGender;

    /** The person's address. */
    const char* myAddress;

    /** The person's occupation. */
    const char* myOccupation;

    /** Indicate whether the person is single. */
    bool myIsSingle;
};
```

**Filen *main.cpp***

Vi inkluderar sedan filen *person.h* i samma fil som funktionen *main* ligger i, alltså *main.cpp*. Innehållet i denna fil kan då reduceras till följande:

```cpp
/**
 * @brief Example program demonstrating a simple class with an associated enum class.
 */
#include <fstream>
#include <iostream>

#include "person.h"

/**
 * @brief Print personal data in the terminal and to a file named `person.txt`.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create objects holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
    Person person2{"Sven Andersson", 37U, Gender::Male, "Kunskapsgatan 4", "Teacher", false};
    
    // Print stored personal data in the terminal.
    person1.print();
    person2.print();

    // Open file `person.txt` for writing.
    std::ofstream ofstream{"person.txt"};

    // Write the personal data to `person.txt`, then terminate the program.
    person1.print(ofstream);
    person2.print(ofstream);
    return 0;
}
```

Programmet fungerar i övrigt på samma sätt som tidigare.

**Metoddefinitioner i filen *person.cpp***

Ofta placeras klassens metoddefinitioner i en separat källkodsfil för att hålla headerfilen mer läsbar. Som exempel, för att hålla filen *person.h* läsbar kan vi med fördel placera metoddefinitionerna i en källkodsfil döpt *person.cpp*. 

Längst upp i denna källkodsfil inkluderar vi headerfilen *person.h*:

```cpp
#include "person.h"
```

Därefter placerar vi metoddefinitionerna från klassen *Person* i denna fil.
Vi kopierar därmed samtliga metoder från klassen *Person* till denna fil. Värt att notera är att:
* För att kompilatorn ska förstå att respektive metod tillhör klassen *Person* måste vi använder prefixet *Person. Så metoden *print* måste definieras som *Person::print* som exempel. 
* Defaultvärden på ingående argument ska endast skrivas i metoddeklarationen i headerfilen.
* Nyckelord framför metodernas namn, såsom `explicit`, ska inte skrivas med i metoddefinitionen.
* Konstruktorer, destruktorer med mera märkta `default` samt `delete` behöver inte placeras i källkodsfilen.

Vi börjar med metoden *print*. I headerfilen *person.h* låter vi metodhuvudet inklusive dokumentationen kvarstå:

```cpp
/**
 * @brief Print personal data about the person.
 * 
 * @param[in] ostream The output stream to use (default = terminal print).
 */
void print(std::ostream& ostream = std::cout) const;
```

I källkodsfilen *person.cpp* placerar vi sedan metoddefinitionen:

```cpp
// -----------------------------------------------------------------------------
void Person::print(std::ostream& ostream) const
{
    ostream << "--------------------------------------------------------------------------------\n";
    ostream << "Name:\t\t" << myName << "\n";
    ostream << "Age:\t\t" << myAge << "\n";
    ostream << "Gender:\t\t" << genderStr() << "\n";
    ostream << "Address:\t" << myAddress << "\n";
    ostream << "Occupation:\t" << myOccupation << "\n";
    ostream << "Single:\t\t" << isSingleStr() << "\n"; 
    ostream << "--------------------------------------------------------------------------------\n\n";
}
```

Notera att defaultvärdet på ingående argument *ostream* endast definieras i metoddeklarationen i headerfilen. Om vi hade haft kvar defaultvärdet i metoddeklarationen när de är separerade hade ett kompilatorfel genererats.

Vi implementerar definitioner för övriga metoder i källkodsfilen *person.cpp.* Vi placerar en rad med bindestreck mellan varje metod för att visuellt separera dessa nu när metoddokumentationen inte är med. 

**Filen *person.cpp***

Efter att ha lagt till samtliga metoddefinitioner ser filen *person.cpp* ut så här:

```cpp
/**
 * @brief Personal data implementation details.
 */
#include <iostream>

#include "person.h"

// -----------------------------------------------------------------------------
Person::Person(const char* name, const unsigned age, const Gender gender, const char* address, 
               const char* occupation, const bool single) noexcept
    : myName{name}
    , myAge{age}
    , myGender{gender}
    , myAddress{address}
    , myOccupation{occupation}
    , myIsSingle{single} 
{
}

// -----------------------------------------------------------------------------
const char* Person::name() const noexcept { return myName; }

// -----------------------------------------------------------------------------
unsigned Person::age() const noexcept { return myAge; }

// -----------------------------------------------------------------------------
Gender Person::gender() const noexcept { return myGender; }

// -----------------------------------------------------------------------------
const char* Person::address() const noexcept { return myAddress; }

// -----------------------------------------------------------------------------
const char* Person::occupation() const noexcept { return myOccupation; }

// -----------------------------------------------------------------------------
bool Person::isSingle() const noexcept { return myIsSingle; }

// -----------------------------------------------------------------------------
void Person::setAddress(const char* address) noexcept { myAddress = address; }

// -----------------------------------------------------------------------------
void Person::setOccupation(const char* occupation) noexcept { myOccupation = occupation; }

// -----------------------------------------------------------------------------
void Person::setSingle(const bool single) noexcept { myIsSingle = single; }

// -----------------------------------------------------------------------------
void Person::print(std::ostream& ostream) const
{
    ostream << "--------------------------------------------------------------------------------\n";
    ostream << "Name:\t\t" << myName << "\n";
    ostream << "Age:\t\t" << myAge << "\n";
    ostream << "Gender:\t\t" << genderStr() << "\n";
    ostream << "Address:\t" << myAddress << "\n";
    ostream << "Occupation:\t" << myOccupation << "\n";
    ostream << "Single:\t\t" << isSingleStr() << "\n"; 
    ostream << "--------------------------------------------------------------------------------\n\n";
}

// -----------------------------------------------------------------------------
const char* Person::genderStr() const noexcept
{
    if (myGender == Gender::Male) { return "Male"; }
    else if (myGender == Gender::Female) { return "Female"; }
    else { return "Other"; }
}

// -----------------------------------------------------------------------------
const char* Person::isSingleStr() const noexcept { return myIsSingle ? "Yes" : "No"; }
```

**Filen *person.h***

Läsbarheten i headerfilen *person.h* ökar nu, då vi har tagit bort en stor del av implementeringsdetaljerna i och med metoddefinitionerna. Vi måste dock fortfarande ha kvar metoddeklarationer samt definition av medlemsvariablerna innanför klassen:

```cpp
/**
 * @brief Personal data implementation.
 */
#pragma once

#include <iostream>

/**
 * @brief Enumeration of genders.
 */
enum class Gender 
{ 
    Male,   /** Male. */
    Female, /** Female. */
    Other,  /** Other gender. */
};

/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Create a new person.
     * 
     * @param[in] name The person's name.
     * @param[in] name The person's age.
     * @param[in] gender The person's gender.
     * @param[in] address The person's home address.
     * @param[in] occupation The person's occupation.
     * @param[in] single Indicate whether the person is single (default = true).
     */
    explicit Person(const char* name, const unsigned age, const Gender gender, const char* address, 
           const char* occupation, const bool single = true) noexcept;

    /**
     * @brief Delete person instance.
     */
    ~Person() noexcept = default;

	/**
     * @brief Get the person's name.
     * 
     * @return The person's name as a string.
     */
    const char* name() const noexcept;

    /**
     * @brief Get the person's age.
     * 
     * @return The person's age as an integer.
     */
    unsigned age() const noexcept;
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const noexcept;

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const noexcept;

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const noexcept;

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const noexcept;
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) noexcept;

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) noexcept;

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) noexcept;
	
	/**
	 * @brief Print personal data about the person.
	 * 
	 * @param[in] ostream The output stream to use (default = terminal print).
	 */
	void print(std::ostream& ostream = std::cout) const;
	
	Person()                         = delete; // No default constructor.
    Person(const Person&)            = delete; // No copy constructor.
    Person(Person&&)                 = delete; // No move constructor.
    Person& operator=(const Person&) = delete; // No copy assignment.
    Person& operator=(Person&&)      = delete; // No move assignment.

private:
	/**
	 * @brief Get the person's gender as a string.
	 * 
	 * @return The person's gender as a string.
	 */
	const char* genderStr() const noexcept;

	/**
	 * @brief Get string indicating whether the person is single.
	 * 
	 * @return "Yes" if the person is single, else "No".
	 */
	const char* isSingleStr() const noexcept;
	
    /** The person's name. */
    const char* myName;

    /** The person's age. */
    unsigned myAge;

    /** The person's gender. */
    Gender myGender;

    /** The person's address. */
    const char* myAddress;

    /** The person's occupation. */
    const char* myOccupation;

    /** Indicate whether the person is single. */
    bool myIsSingle;
};
```

**Filen *main.cpp**

Att vi delade upp klassen *Person* i en headerfil *person.h* samt källkodsfil *person.cpp* har ingen påverkan på filen *main.cpp.* Återigen behöver vi enbart inkludera filen *person.h,* därefter kan vi skapa personobjekt, skriva ut innehåll med mera:

```cpp
/**
 * @brief Example program demonstrating a simple class with an associated enum class.
 */
#include <fstream>
#include <iostream>

#include "person.h"

/**
 * @brief Print personal data in the terminal and to a file named `person.txt`.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create objects holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
    Person person2{"Sven Andersson", 37U, Gender::Male, "Kunskapsgatan 4", "Teacher", false};
    
    // Print stored personal data in the terminal.
    person1.print();
    person2.print();

    // Open file `person.txt` for writing.
    std::ofstream ofstream{"person.txt"};

    // Write the personal data to `person.txt`, then terminate the program.
    person1.print(ofstream);
    person2.print(ofstream);
    return 0;
}
```

Det enda vi behöver tänka på är att lägga till filen `person.cpp` i vår makefil:

```make
# Builds and runs the application as default.
default: build run

# Builds the application.
build:
	@g++ main.cpp person.cpp -o main -Wall -Werror

# Runs the application.
run:
	@./main

# Cleans the application.
clean:
	@rm -f main
```

#### Slutförd kodimplementation
Filer [person.h](./person/person.h), [person.cpp](./person/person.cpp) samt [main.cpp](./person/main.cpp) finns tillgängliga i underkatalogen [person](./person/README.md), tillsammans med en [makefil](./person/makefile) för 
kompilering och körning av koden.

#### Sammanfattning
Efter att ha gått igenom materialet från de två senaste föreläsningarna bör du som läsare kunna:
* Styra vad som är synligt respektive inte synligt utanför en klass via inkapsling.
* Skapa konstruktorer för att initiera objekt av klassen i fråga.
* Styra vad som kan läsas samt vad som kan läsas och skrivas via get- och set-metoder.
* Skapa enumerationsklasser.
* Dela upp en klass i en headerfil samt en källkodsfil.
* Använda nyckelord såsom `explicit` samt `noexcept`.
* Radera kopierings- och förflyttningskonstruktorer samt tilldelningsoperator för kopiering och förflyttning.

#### Fördjupning
Det finns mycket mer att lära sig om klasser och objektorientering än vad som presenterades här; detta ämne hade kunnat täcka en hel kurs. 
För mer information:
* Se boken [*The C++ Programming Language*](https://www.adlibris.com/se/bok/c-programming-language-the-9780321958327) av Bjarne Stroustrup, skapare av C++. Denna bok täcker det mesta som behöver kännas till om modern C++ (förutom sådant som har kommit till efter att den senaste upplagan släpptes), inte bara klasser. 
* En hel del fördjupad information finns också i klass-avsnittet på [*cplusplus.com*](https://cplusplus.com/doc/tutorial/classes/).  
* Vi kommer se mer på arv och interfaces under nästa föreläsning.
