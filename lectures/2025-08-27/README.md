# 2025-08-27 - Lektion 3

## Dagordning
* Grundläggande objektorientering i C++: klasser, inkapsling med mera.
* Övningsuppgifter innefattande skapande av klasser i C++.

## Mål med lektionen
* Kunna skapa enkla klasser i C++.
* Känna till innebörden av orden såsom *konstruktor* och *inkapsling*.
* Kunna använda enumerationsklasser.

## Instruktioner
* Läs om klasser i detta dokument.
* Se övningsuppgifter i underkatalogen *exercises*.

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Filorganisation för klasser - uppdelning i header- och sourcefiler.
* Kopieringskonstruktorer samt nyckelordet `noexcept`.
* Inlämningsuppgift innefattande skapandet av klasser i C++.

## Klasser i C++ - Del I

### Bakgrund
För att hålla god struktur i större program finns ett ökat behov av att lagra relaterad data som en enhet via enskilda datastrukturer i stället för multipla separata variabler. Som exempel kan det vara fördelaktigt att lagra persondata i form av namn, ålder, adress och dylikt för en person som en enhet i stället för separata variabler för varenda attribut. 

Inom programmering utgörs de vanligaste datastrukturerna för lagring av relaterad data som en enhet av så kallade *struktar* och *klasser:* 
* *Struktar* (strukturer) används främst i C (begreppet klass existerar inte överhuvudtaget i C).
* Klasser används nästan exklusivt i programspråk som skapades efter C, såsom C++, Java och Python. 
* Klasser kan ses som en vidareutveckling av struktar, som är relativt simpla. Sett till uppbyggnad är dock dessa datastrukturer väldigt lika.

Språk som innehar stöd för klasser sägs inneha stöd för objektorientering, eller OOP *(Object Oriented Programming.)* C sägs vara ett procedurellt språk, då det bygger på programmering via funktioner. Via struktar kan dock vissa objektorienterade principer efterliknas, vilket kommer demonstreras nedan.

En del objektorienterade språk, exempelvis Java och C# är rent objektorienterade, vilket innebär att klasser måste användas. Andra programspråk, såsom C++ och Python, sägs utgöra multiparadigmspråk, då det finns möjlighet att programmera båda objektorienterat (såsom i Java) samt procedurellt (såsom i C) eller en kombination av dem.

### Struktar i C
För att förstå hur klasser fungerar är det enklast att börja med den enklaste datastrukturen. Därmed börjar vi med struktar. Struktar i C fungerar likt arrayer, med skillnaden att datatyperna på medlemmarna i en strukt kan vara olika, medan samtliga element i en array måste vara samma. 

Därmed gäller att ett struktobjekt kan inneha en kombination av text, tal, boolvariabler med mera, exempelvis för att lagra en persons namn (text), ålder (heltal), om personen är singel (sant eller falskt) med mera.

Struktar implementeras via nyckelordet *struct.* Nedan visas en strukt döpt *person*, vars syfte är att lagra persondata:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
};
```

Via strukten *person* ovan kan vi lagra namn, ålder, address samt yrke för en given person. Som exempel, för en 42-årig advokat döpt Marie Nilsson som bor på Juristgatan 17 kan vi skapa ett objekt döpt *person1* så som visas nedan. Notera att initieringen är väldigt likt hur vi hade initierat en array i C:

```c
struct person person1 = {"Marie Nilsson", 42U, "Juristgatan 17", "Lawyer"};
```

Notera att vi passerar attributen, såsom "Marie Nilsson" för namnet och 42 för åldern, i samma ordning som de har deklarerats i strukten.

Till skillnad mot arrayer använder vi inte index för att nå innehållet i struktobjektet. I stället använder vi den så kallade punktoperatorn, som utgörs av en punkt mellan objektets namn och attributet vi söker. 

Därmed gäller att om vi vill få tillgång till namnet på *person1* skriver vi *person1.name*, då namnet är lagrat via attributet *name*. På samma sätt gäller att vi skriver *person1.age* för att få tillgång till personens ålder, *person1.address* för att få tillgång till persons adress och så vidare.

Nedan visas hur vi skriver ut persondatan för *person1* i terminalen via anrop av funktionen *print* från standardheadern *stdio.h*:

```c
    struct person person1 = {"Marie Nilsson", 42U, "Juristgatan 17", "Lawyer"};
    printf("Name:\t\t%s\n", person1.name);
    printf("Age:\t\t%u\n", person1.age);
    printf("Address:\t%s\n", person1.address);
    printf("Occupation:\t%s\n", person1.occupation);
```

Utskriften blir:

```
Name:           Marie Nilsson
Age:            42
Address:        Juristgatan 17
Occupation:     Lawyer
```

Kortfattat gällande ovanstående kodsnutt:
* **\t** medför ett tabtecken.
* **\n** medför ett nyradstecken.
* **%s** är formatspecificerare för text.
* **%u** utgör formatspecificerare för osignerade heltal.

Vi kan lägga till fler parametrar vid behov. Som exempel kan vi lägga till en boolvariabel *single*, som indikerar ifall personen i fråga är singel eller inte via booleska värden *true* och *false*. För att göra detta måste vi inkludera standardheadern *stdbool:*

```c
#include <stdbool.h>
```

Därefter kan vi lägga till parametern *single* så som visas nedan:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};
```

Vi kan även lägga till parametrar av egna typer också. Anta att vi vill ha en typ för personens kön, där giltiga värden är man, kvinna eller annat. 

I C hade vi kunnat göra detta med en enumeration döpt *gender,* där enumeratorerna föregås av prefixes GENDER för att undvika namnkrockar i större program. En sådan enumeration hade kunnat se ut så som visas nedan:

```c
/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};
```

Vi lägger till parametern *gender* för personens kön efter åldern, så att den mest basala datan (namn, ålder och kön) hamnar först:

```c
/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};
```

Anta att personen Marie Nilsson implementerad via objektet *person1* tidigare är kvinna och singel. I så fall hade associerat objekt *person1* kunnat implementeras så som visas nedan. Notera att vi passerar attributen i samma ordning som de har deklarerats i strukten:

```c
struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE,
                         "Juristgatan 17", "Lawyer", true};
```

#### Exempelprogram

Nedan visas ett program, där persondatan lagrad via ovanstående objekt *person1* skrivs ut i terminalen. Vi implementerar en funktion döpt *gender_str* för att erhålla kön i textform.

```c
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};

/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};

/**
 * @brief Get the given gender as a string.
 * 
 * @param[in] gender The gender in question.
 * 
 * @return The given gender as a string.
 */
const char* gender_str(const enum gender gender)
{
    switch (gender)
    {
        case GENDER_MALE  : return "Male";
        case GENDER_FEMALE: return "Female";
        default           : return "Other";
    }
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create a structure holding personal data.
    struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE,
                             "Juristgatan 17", "Lawyer", true};

    // Print the personal data in the terminal, then terminate the program.
    printf("Name:\t\t%s\n", person1.name);
    printf("Age:\t\t%u\n", person1.age);
    printf("Gender:\t\t%s\n", gender_str(person1.gender));
    printf("Address:\t%s\n", person1.address);
    printf("Occupation:\t%s\n", person1.occupation);
    printf("Single:\t\t%s\n", person1.single ? "Yes" : "No");
    return 0;
}
```

Utskriften blir:

```
Name:           Marie Nilsson
Age:            42
Gender:         Female
Address:        Juristgatan 17
Occupation:     Lawyer
Single:         Yes
```



### Associerade funktioner till struktar
I C är det vanligt att skapa funktioner associerade med en strukt. Dessa funktioner skapas då för att användas med objekt av motsvarande strukt. 

Exempelvis kan vi skapa en funktion döpt *person_print* för att skriva ut persondata i terminalen. Detta kommer både göra programmet enklare att läsa, samtidigt som vi slipper skriva samma kod igen ifall vi vill skriva ut persondata för fler personer.

Vi lägger till prefixet *person* på alla funktioner associerade med strukten *person* av två anledningar:
* **1.** För att tydliggöra att funktionen är associerad med strukten *person.*
* **2.** För att undvika namnkrockar, vilket vi hade fått om en annan C-funktion heter *print.* Därmed ser vi till att samtliga funktioner har undika namn. Som exempel, skapar vi en till struct som heter exempelvis *device* kan vi döpa motsvarande utskriftsfunktion till *device_print.*

Funktionen *person_print* visas nedan:

```c
/**
 * @brief Print personal data in the terminal.
 * 
 * @param[in] self Reference to the person structure holding the data to print.
 */
void person_print(const struct person* self)
{
    // Terminate the function is the pointer is null.
    if (!self) { return; }

    // Print the attributes on separate lines.
    printf("--------------------------------------------------------------------------------\n");
    printf("Name:\t\t%s\n", self->name);
    printf("Age:\t\t%u\n", self->age);
    printf("Gender:\t\t%s\n", gender_str(self->gender));
    printf("Address:\t%s\n", self->address);
    printf("Occupation:\t%s\n", self->occupation);
    printf("Single:\t\t%s\n", self->single ? "Yes" : "No");
    printf("--------------------------------------------------------------------------------\n\n");
}
```

Notera att:
* Vi sätter ingående argument *self* till en pekare, då person-objekt är relativt stort. Därmed föredrar vi *pass-by-reference* (med pekare) i stället för *pass-by-value* (som hade medfört att vi skapat och sedan skickat en kopia av person-objektet vars innehåll vi vill skriva ut). 
* Eftersom vi bara vill läsa persondatan utan att modifiera något sätter vi person-objektet till en konstant via nyckelordet *const.*
* Vi döper ingående argument till *self* för att indikera att detta är själva objektet som den associerade funktionen är till för. Vanliga andra namn är *this* och *me,* men givetvis kan vi döpa detta argument till *person* om vi föredrar det.
* Eftersom ingående argument *self* är en pekare till person-objektet, inte själva objektet i sig, använder vi piloperatorn -> för att enkelt få tillgång till person-attributen. Som exempel, via piloperatorn kan vi få tillgång till personens namn genom att skriva *self->name* i stället för *(*self).name* för att först läsa innehållet som pekaren *self* pekar på *(.self)*, följt av att använda punktoperatorn för att få tillgång till attributen.

När vi anropar funktionen *person_print* måste vi skicka adressen till person-objektet vi vill skriva ut. Detta gör vi med den så kallade adressoperatorn &. Som exempel, för att kalla på *person_print* med adressen till objektet *person1* kan följande instruktion användas:

```c
person_print(&person1);
```

#### Exempelprogram

Vi lägger till funktionen *person_print* i ovanstående program. Vi lägger också till ytterligare en person, en 37-årig gift man, som heter "Sven Andersson", bor på "Kunskapsgatan 4" och jobbar som lärare. Vi skriver sedan ut persondatan för respektive person genom att anropa *person_print.* Programmet visas nedan:

```c
#include <stdio.h>
#include <stdbool.h>

/**
 * @brief Enumeration of genders.
 */
enum gender
{
    GENDER_MALE,   /** Male. */
    GENDER_FEMALE, /** Female. */
    GENDER_OTHER,  /** Other gender. */
};

/**
 * @brief Structure representing personal data.
 */
struct person 
{
    const char* name;       /** The person's name. */
    unsigned age;           /** The person's age. */
    enum gender gender;     /** The person's gender. */
    const char* address;    /** The person's address. */
    const char* occupation; /** The person's occupation. */
    bool single;            /** Indicate whether the person is single. */
};

/**
 * @brief Get the given gender as a string.
 * 
 * @param[in] gender The gender in question.
 * 
 * @return The given gender as a string.
 */
const char* gender_str(const enum gender gender)
{
    switch (gender)
    {
        case GENDER_MALE  : return "Male";
        case GENDER_FEMALE: return "Female";
        default           : return "Other";
    }
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @param[in] self Reference to the person structure holding the data to print.
 */
void person_print(const struct person* self)
{
    // Terminate the function is the pointer is null.
    if (!self) { return; }

    // Print the attributes on separate lines.
    printf("--------------------------------------------------------------------------------\n");
    printf("Name:\t\t%s\n", self->name);
    printf("Age:\t\t%u\n", self->age);
    printf("Gender:\t\t%s\n", gender_str(self->gender));
    printf("Address:\t%s\n", self->address);
    printf("Occupation:\t%s\n", self->occupation);
    printf("Single:\t\t%s\n", self->single ? "Yes" : "No");
    printf("--------------------------------------------------------------------------------\n\n");
}

/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create structures holding personal data.
    const struct person person1 = {"Marie Nilsson", 42U, GENDER_FEMALE, "Juristgatan 17", 
                                   "Lawyer", true};
    const struct person person2 = {"Sven Andersson", 37U, GENDER_MALE, "Kunskapsgatan 4", 
                                   "Teacher", false};

    // Print the personal data by invoking `person_print`, then terminate the program.
    person_print(&person1);
    person_print(&person2);
    return 0;
}
```

Utskriften visas nedan:

```
--------------------------------------------------------------------------------
Name:           Marie Nilsson
Age:            42
Gender:         Female
Address:        Juristgatan 17
Occupation:     Lawyer
Single:         Yes
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Name:           Sven Andersson
Age:            37
Gender:         Male
Address:        Kunskapsgatan 4
Occupation:     Teacher
Single:         No
--------------------------------------------------------------------------------
```

### Klasser i C++

#### Terminologi
Som skrevs tidigare används klasser i stället för struktar i de flesta konventionella programspråk som skapades efter C. 

I C++ är dock struktar nästintill identiska med klasser; den enda skillnaden är att för klasser är samtliga attribut icke åtkomliga utanför klassen som default. Struktar i C++ kan dock användas på samma sätt som struktar i C. För att minska förvirring i resten av detta avsnitt gäller följande:
* **Strukt** syftar till den enkla datastruktur som har demonstrerats hittills (C-strukt).
* **Klass** syftar till en mer avancerad variant av den datastrukt vi har sett hittills.

#### Skillnader mellan klasser och struktar

Klasser kan ses som en vidareutveckling av struktar. Därmed är klasser och struktar lika sett till den grundläggande uppbyggnaden, men klasser erbjuder mycket mer funktionalitet samt flexibilitet. I detta avsnitt ska vi titta på den mest grundläggande funktionalitet som klasser erbjuder.

Några av de mest betydande skillnaderna mellan klasser och struktar är:
* **Klasser innehar stöd för inkapsling,** vilket innebär att man kan välja vad som är åtkomligt/synligt utanför klassen. Därmed kan hemlig data eller information som inte är relevant utanför klassen (såsom implementationsdetaljer) döljas.
* **Klasser kan innehålla funktioner,** vilket innebär att vi inte behöver implementeras associerade funktioner med struktpekare, så som visades nedan.
* **Klasser erbjuda funktionalitet för arv,** vilket innebär att en ny klass kan erhålla innehållet från en basklass. Detta kan minska mängden kod som behovs avsevärt.
* **Klasser kan implemeteras via så kallade klasstemplates för att göra en eller flera datatyper valbara, likt de funktionstemplates vi såg tidigare. Ett exempel på ett klasstemplate i standardbiblioteket är *std::vector,* som medför att vi kan välja vad som ska lagras i en given vektor.

#### Grundläggande struktur för en klass

Vi implementerar motsvarigheten till strukten *person* i C++ via en klass döpt *Person*. Vi kommer använda oss utav *camelCase* i stället för *snake_case,* vilket medför att:
* Vi använder oss utav stora bokstäver i stället för understreck i funktions- och typnamn. Så i stället för ett funktionsnamn så som *print_numbers* kommer vi döpa denna funktion *printNumbers*.
* Samma mönster följs för typer så som struktar, klasser och enumerationer. En skillnad är dock att namnet på typer kommer börja med en stor bokstav, exempelvis *Person* i stället för *person,* *LedController* i stället för *led_controller* med mera.

##### 1. Lägg till inkluderingsdirektiv
Vi börjar med att inkludera standardheader *iostream,* så att vi kan generera utskrifter samt får tillgång till vanliga typer så som *std::size_t*:

```cpp
#include <iostream>
```

##### 2. Skapa enumerationsklassen *Gender*
Därefter skapar vi enumerationsklassen *Gender* för implementering av kön. Denna enumerationsklass motsvarar enumerationen *gender* vi implementerade i C förut. 

```cpp
/**
 * @brief Enumeration of genders.
 */
enum class Gender 
{ 
    Male,   /** Male. */
    Female, /** Female. */
    Other,  /** Other gender. */
};
```

Anledningen till att vi väljer att implementera *Gender* som en enumerationsklass i stället för en traditionell enumeration är att vi slipper använda prefix för att undvika namnkrockar; i stället är det inbyggt i språket. 

Notera ovan enumeratorerna nu är döpta *Male, Female* samt *Other* i stället för GENDER_MALE, GENDER_FEMALE samt GENDER_OTHER. Med enumerationsklassen ovan skriver vi i stället *Gender::Male, Gender::Female* samt *Gender::Other.* Notera att vi använder separatorn :: mellan prefixet *Gender* samt enumeratorerna *(Male, Female* samt *Other)*.

I allmänhet är det rekommenderat att använda enumerationsklasser i stället för traditionella enumerationer om man programmerar i modern C++, alltså C++11 och framåt.

##### 3. Definiera klassen *Person*
Därefter definierar vi klassen *Person* via nyckelordet *class:*

```cpp
/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:

private:
};
```

Vi har lagt till två nyckelord i klassen, *public* och *private*:
* Allt som faller under nyckelordet *public* är synligt och åtkomligt utanför klassen. Allt som vi vill att "användaren" av klassen ska kunna se och använda ska deklareras här, exempelvis metoder (funktioner) för att läsa personens namn, ålder med mera, samt för att kunna skriva ut person datan i terminalen.
* Allt som faller under nyckelordet *private* är inte synligt eller åtkomligt utanför klassen. Här placerar vi allt som vi vill vara icke åtkomligt för "användaren" av klassen, exempelvis hemlig data eller implementationsdetaljer. I vårt fall placerar vi samtliga medlemsvariabler här, då vi inte vill att "användaren" ska kunna ändra på dessa hur som helst. Vi kan också placera metoder som bara ska användas internt i klassen här. Att döja information på detta sätt kallas *inkapsling.*

Som default är allt i en klass privat. Normalt placeras allt som är publikt först i klassen, så att "användaren" vid behov enkelt kan kolla in vad för metoder och annat som hen har till sitt förfogande. Implementationsdetaljerna placerar vi efteråt i den privata delen av klassen och meningen är att användaren inte ens ska behöva (eller vilja) läsa ned hit. 

Oftast dokumenteras enbart den publika delen av klassen; resten av klassen är implementationsdetaljer som enbart är av intresse för utvecklaren av klassen och dokumenteras därmed inte. Eftersom detta är ett typexempel dokumenteras dock all kod.

##### 4. Lägg till medlemsvariabler
Under den privata delen lägger vi till medlemsvariablerna, alltså de variabler i klassen som lagrar personens namn, ålder, kön med mera.
Notera att vi använder prefixet *my*, för att undvika namnkrockar med senare metoder vi kommer lägga till (annars behöver vi använda pekaren *this* likt hur vi använde struktpekaren *self* i C tidigare):

```cpp
/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:

private:
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

Medlemsvariablerna är privata, så att "användaren" inte kan läsa och skriva dem hur som helst. Vi kommer senare lägga till så kallade get- och set-metoder för att styra vad "användaren" kan läsa och skriva. Som exempel bör man kunna ändra personens singelstatus eller yrke, men nödvändigtvis inte hens namn eller kön (inte i denna implementering i alla fall).

##### 5. Lägg till konstruktorer och destruktorer
Men innan vi lägger till några get- och set-metoder lägger vi till konstruktorer, vilket kan ses som initieringsrutiner som anropas automatiskt när ett objekt av klassen skapas.

Först och främst raderar vi den så kallade default-konstruktorn, så att man inte kan skapa ett person-objekt utan att skicka med namn, ålder med mera.

Längst ned i den publika delen (precis innan `private`) lägger vi därmed till följande:

```cpp
public:
    Person() = delete; // No default constructor.
```

Man kallar denna konstruktor för default-konstruktor, då den inte har några ingående argument. Därmed är det inte möjligt att skapa ett tomt objekt så som visas nedan:

```cpp
// Won't compile, since the default constructor is deleted.
Person person{};
```

Vi kan också lägga till en destruktor, vilket kan ses som en funktion som kallas på automatiskt precis innan en instans av den givna klassen raderas. Om vi skapar en instans av klassen `Person` längst upp i funktionen `main` kommer detta äga 
rum när `main` avslutas.

Destruktorn ser ut som default-konstruktorn, med skillnaden att negationstecknet ~ placeras framför klassens
namn. Som exempel, för att skriva ut `Deleting person instance!` i terminalen när en given person-instans
raderas kan vi implementera destruktorn så som visas nedan:

```cpp
/**
 * @brief Delete person instance.
 */
~Person()
{
    std::cout << "Deleting person instance!\n";
}
```

Om vi inte är i behov av att göra något speciellt innan objektet raderas kan vi slopa destruktorn;
kompilatorn kommer då skapa en default-destruktor åt oss, i praktiken en tom konstruktor. 
Vill vi ändå skapa en destruktor för tydlighets skull (rekommenderat) kan vi explicit sätta destruktorn till default med nyckelordet `default`.

```cpp
/**
 * @brief Delete person instance.
 */
~Person() = default;
```

Alternativt kan vi låta destruktorn vara tom:

```cpp
/**
 * @brief Delete person instance.
 */
~Person() {}
```

I modern C++ rekommenderas dock nyckelordet `default`, vilket kommer användas i detta exempel.

Destruktorn placeras normalt sett direkt nedanför icke-raderade konstruktorer. Därmed placerar vi konstruktorn
ovanför den raderade default-konstruktorn:

```cpp
public:
    // Add additional constructors here!

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;

    Person() = delete; // No default constructor.
```

Klassen ser nu ut så här:

```cpp
/**
 * @brief Class for representing a person and their personal data.
 */
class Person
{
public:
    /**
     * @brief Delete person instance.
     */
    ~Person() = default;

    Person() = delete; // No default constructor.

private:
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

Vi vill att användaren ska ange persondatan direkt när ett person-objekt skapas. Vi skapar då en konstruktor som tar personens namn, ålder, kön, adress, yrke samt singel-status som ingående argument. Vi sätter singelstatusen till default-värdet *true,* så "användaren" behöver inte ange just denna (om hen inte anger något antar vi att personen är singel):

```cpp
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
    {
    }
```

I konstruktorn ovan initierar vi medlemsvariablerna så att:
* *myName* tilldelas namnet som refereras till av ingående argument *name.*
* *myAge* tilldelas värdet som lagras av ingående argument *age.*
* *myGender* tilldelas värdet som lagras av ingående argument *gender.*
* *myAddress* tilldelas adressen som refereras till av ingående argument *address.*
* *myOccupation* tilldelas yrket som refereras till av ingående argument *ocupation.*
* *myIsSingle* tilldelas värdet som lagras av ingående argument *single.* Som default är *single* satt till *true*.

Detta åstadkommer vi genom att lägga till en initieringsdel  mellan funktionshuvudet samt funktionskroppen (markerad via måsvingar {}). Notera att vi börjar initieringsdelen med ett kolon, initierar med måsvingar (så *myName{name}* betyder i praktiken att *myName = name)* och separerar initieringen av varje medlemsvariabel med ett kommatecken.

```cpp
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
```

Om vi behöver göra något ytterligare efter initieringen av medlemsvariablerna hade vi kunnat placera detta i konstruktorns funktionskropp. I detta fall vill vi dock enbart initiera medlemsvariablerna, så vi behöver inte lägga till något ytterligare. Vi placerar därmed med fördel måsvingarna på sista initieringsraden för att inte uppta onödigt mycket blanka rader. 

Man kan nu använda konstruktorn som visas ovan för att initiera person-objekt. Nedan visas hur vi initierar objektet *person1* för att lagra persondata gällande den 42-åriga juristen Marie Nilsson, som är singel:

```cpp
Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
```
                   
Eftersom Marie är singel och default-värdet för det ingående argumentet som lagrar singelstatusen är *true* struntade vi att tilldela ett värde gällande singelstatusen explicit ovan. Vi hade dock kunnat lägga till *true* som sista ingående argument:

```cpp
Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer", true};
```

Vi kan också använda konstruktorn för att initiera objektet *person2* för att lagra persondata gällande den 37-åriga läraren Sven Andersson.
Eftersom han inte är singel sätter vi värdet *false* på singelstatusen (det sista ingående argumentet) explicit.

```cpp
Person person2{"Sven Andersson", 37U, Gender::Male, "Kunskapsgatan 4", "Teacher", false};
```

Klassen ser nu åt så som visas nedan:

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

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;
	
	Person() = delete; // No default constructor.

private:
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

##### 5. Lägg till get- och set-metoder
För att vi ska kunna styra vilka av medlemsvariablerna vars innehåll enbart kan läsas och vilka som både kan läsas och skrivas använder vi så kallade get- och setmetoder:
* Get-metoder ger oss möjlighet att läsa innehållet av en medlemsvariabel. Som exempel, en get-metod döpt *name* kan användas för att låta "användaren" av klassen ta reda på personens namn.
* Set-metoder ger oss möjlighet att skriva/ändra innehållet på en medlemsvariabel. Som exempel, en set-metod döpt *setOccupation* hade kunnat användas för att byta personens yrke från "Lawyer" till "Judge".

Get-metoder returnerar värdet som ska läsas, exempelvis personens namn, medan set-metoder tar det nya värdet som ska skrivas som ingående argument.

För att get- och set-metoderna ska vara åtkomliga utanför klassen placeras dessa i den publika delen av klassen, direkt efter konstruktorn. 

Nedan visas exempel på get-metoder *name* samt *age,* som returnerar det lagrade namnet samt åldern:

```cpp
/**
 * @brief Get the person's name.
 * 
 * @return The person's name as a string.
 */
const char* name() const { return myName; }

/**
 * @brief Get the person's age.
 * 
 * @return The person's age as an integer.
 */
unsigned age() const { return myAge; }
```

Notera att vi skriver nyckelordet *const* direkt efter funktionshuvudet, då vi ser till att klassen innehåll bara ska kunna läsas. Vi kan därmed inte av misstag råka ändra någon av klassmedlemmarna (hade vi gjort det hade ett kompileringsfel genererats).

Vi hade kunnat använda get-metoderna ovan för att läsa och skriva ut åldern på Marie Nilsson, som tidigare implementerades i mjukvaran via objektet *person1:*

```cpp
/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create an object holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

    // Print the person's name and age in the terminal, then terminate the program.
    std::cout << "Name:\t" << person1.name() << "\n";
    std::cout << "Age:\t" << person1.age() << "\n";
    return 0;
}
```

Utskriften blir följande:

```
Name:   Marie Nilsson
Age:    42
```

Vi lägger till get-metoder för övriga medlemsvariabler. Vi döper dessa till *gender, address, occupation* samt *isSingle:*

```cpp
/**
 * @brief Get the person's gender.
 * 
 * @return The person's gender as an enumerator.
 */
Gender gender() const { return myGender; }

/**
 * @brief Get the person's home address.
 * 
 * @return The person's home address as a string.
 */
const char* address() const { return myAddress; }

/**
 * @brief Get the person's occupation.
 * 
 * @return The person's occupation as a string.
 */
const char* occupation() const { return myOccupation; }

/**
 * @brief Check whether the person is single.
 * 
 * @return True if the person is single, false otherwise.
 */
bool isSingle() const { return myIsSingle; }
```

Vi lägger därefter till set-metoder för att kunna ändra personens hemadress, yrke samt singelstatus. Vi lägger till prefixet set på dessa för att tydliggöra att dessa ska användas för att sätta ett värde (det hade dock fungerat utmärkt att döpa dessa samma som motsvarande get-metoder, då C++-kompilatorn kan tolka vilken av metoderna du avser att anropa utefter argumentlistan):

```cpp
/**
 * @brief Set the person's home address.
 * 
 * @param[in] address The new home address.
 */
void setAddress(const char* address) { myAddress = address; }

/**
 * @brief Set the person's occupation.
 * 
 * @param[in] occupation The new occupation.
 */
void setOccupation(const char* occupation) { myOccupation = occupation; }

/**
 * @brief Updates the person's single status.f
 * 
 * @param[in] single True if the person is single, false otherwise.
 */
void setSingle(const bool single) { myIsSingle = single; }
```

Om vi exempelvis kallar på *setAddress* ser vi till att medlemsvariabeln *myAddress* tilldelas den nya adress som refereras till av ingående argument *newAddress*. 

Vi kallar på *setAddress* för att ändra Maria Nilssons hemadress från Juristgatan 17 till Kaggeledsgatan 3*. Vi skriver ut hennes hemadress både innan och efter anropet:

```cpp
/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create an object holding personal data.
    Person person1{"Marie Nilsson", 42, Gender::Female, "Juristgatan 17", "Lawyer"};

    // Print the person's name, age, and old address.
    std::cout << "Name:\t\t" << person1.name() << "\n";
    std::cout << "Age:\t\t" << person1.age() << "\n";
    std::cout << "Old address:\t" << person1.address() << "\n";
    
    // Change the person's home address, then print it and terminate the program.
    person1.setAddress("Kaggeledsgatan 3");
    std::cout << "New address:\t" << person1.address() << "\n";
    return 0;
}
```

Utskriften blir följande:

```
Name:           Marie Nilsson
Age:            42
Old address:    Juristgatan 17
New address:    Kaggeledsgatan 3
```

Efter tillägg av get- och set-metoderna ser klassen *Person* ut så här:

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

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) { myIsSingle = single; }
	
	Person() = delete; // No default constructor.

private:
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

##### 6. Lägg till generella metoder
Vi kan sedan lägga till övriga metoder för att hantera eller använda persondatan. Vi lägger därmed till metoden *print* för att skriva ut den lagrade persondatan:

```cpp
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
```

Notera att vi använder ett ingående argument i form av en referens till en utström, *ostream*, för att kunna välja vart datan ska skrivas. Som default skriver vi till terminalen, så defaultvärdet sätts till *std::cout*. Men om man vi kan man exempelvis skriva till en fil.

Notera i detta fall att vi kallar på egna metoder *genderStr* samt *isSingleStr* för att erhålla personens kön samt om hen är singel som text. Dessa metoder ska enbart användas internt för användning i metoden *print* och bör bör då med fördel implementeras som privata metoder. 

Vi lägger därmed till dessa metoder under nyckelordet *private,* ovanför medlemsvariablerna:

```cpp
private:
/**
 * @brief Get the person's gender as a string.
 * 
 * @return The person's gender as a string.
 */
const char* genderStr() const
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
const char* isSingleStr() const { return myIsSingle ? "Yes" : "No"; }
```

Vi kan sedan enkelt skriva ut Marie Nilssons persondata genom att kalla på metoden *print* i stället för att göra terminalutskriften i *main:*

```cpp
/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create an object holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};

    // Print the personal data in the terminal, the terminate the program.
    person1.print();
    return 0;
}
```

Klassen *Person* ser nu ut så som visas nedan:

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

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) { myIsSingle = single; }
	
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
	const char* genderStr() const
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
	const char* isSingleStr() const { return myIsSingle ? "Yes" : "No"; }
	
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

Vi hade också kunnat skriva persondatan till filen *person.txt* via en filström av datatypen *std::ofstream* från standardheader *fstream*. Vi inkluderar därmed *fstream* i aktuell fil:

```cpp
#include <fstream>
```

I funktionen *main* skapar vi sedan en filström döpt *ofstream* för att öppna filen *person.txt* för skrivning. Om filen inte finns skapas den nu:

```cpp
std::ofstream ofstream{"person.txt"};
```

Vi skriver sedan persondatan till filen *person.txt* genom att anropa metoden *print* med *ofstream* som ingående argument:

```cpp
person1.print(ofstream);
```

Nedan visas hur vi kan skriva persondatan både till terminalen samt till filen *person.txt* via två anrop av metoden *print*:

```cpp
/**
 * @brief Print personal data in the terminal.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create an object holding personal data.
    Person person1{"Marie Nilsson", 42, Gender::Female, "Juristgatan 17", "Lawyer"};

    // Print the personal data in the terminal.
    person1.print();

    // Open file `person.txt` for writing.
    std::ofstream ofstream{"person.txt"};

    // Print the personal data to `person.txt`, then terminate the program.
    person1.print(ofstream);
    return 0;
}
```

##### Exempelprogram

Nedan visas ett program, där Marie Nilssons och Sven Anderssons personuppgifter skrivs ut i terminalen samt till en fil döpt *person.txt.* Allt är implementerat i en enda fil:

```cpp
/**
 * @brief Example program demonstrating a simple class with an associated enum class.
 */
#include <fstream>
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

    /**
     * @brief Delete person instance.
     */
    ~Person() = default;
	
	/**
	 * @brief Get the person's gender.
	 * 
	 * @return The person's gender as an enumerator.
	 */
	Gender gender() const { return myGender; }

	/**
	 * @brief Get the person's home address.
	 * 
	 * @return The person's home address as a string.
	 */
	const char* address() const { return myAddress; }

	/**
	 * @brief Get the person's occupation.
	 * 
	 * @return The person's occupation as a string.
	 */
	const char* occupation() const { return myOccupation; }

	/**
	 * @brief Check whether the person is single.
	 * 
	 * @return True if the person is single, false otherwise.
	 */
	bool isSingle() const { return myIsSingle; }
	
	/**
	 * @brief Set the person's home address.
	 * 
	 * @param[in] address The new home address.
	 */
	void setAddress(const char* address) { myAddress = address; }

	/**
	 * @brief Set the person's occupation.
	 * 
	 * @param[in] occupation The new occupation.
	 */
	void setOccupation(const char* occupation) { myOccupation = occupation; }

	/**
	 * @brief Updates the person's single status.f
	 * 
	 * @param[in] single True if the person is single, false otherwise.
	 */
	void setSingle(const bool single) { myIsSingle = single; }
	
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
	const char* genderStr() const
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
	const char* isSingleStr() const { return myIsSingle ? "Yes" : "No"; }
	
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

Utskriften i terminalen samt filinnehållet i filen *person.txt* blir följande:

```
--------------------------------------------------------------------------------
Name:		Marie Nilsson
Age:		42
Gender:		Female
Address:	Juristgatan 17
Occupation:	Lawyer
Single:		Yes
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
Name:		Sven Andersson
Age:		37
Gender:		Male
Address:	Kunskapsgatan 4
Occupation:	Teacher
Single:		No
--------------------------------------------------------------------------------
```

#### Slutförd komimplementation
Ovanstående program finns tillgängligt i underkatalogen [person](./person/README.md), tillsammans med en makefil för kompilering och körning av koden.\
Instruktioner för kompilering samt exekvering av koden finns i bifogad README-fil.

#### Sammanfattning
Efter att ha gått igenom detta stycke bör du som läsare ha tillräckligt med kunskap för att skapa en mindre klass.
Du bör kunna:
* Styra vad som är synligt respektive inte synligt utanför en klass via inkapsling.
* Skapa konstruktorer för att initiera objekt av klassen i fråga.
* Styra vad som kan läsas samt vad som kan läsas och skrivas via get- och set-metoder.
* Skapa enumerationsklasser.

Testa gärna dina kunskaper genom att genomföra övningsuppgiften i underkatalogen *exercises.* 

##### Fördjupning
Under nästa föreläsning ska vi gå igenom några ytterligare delar gällande klasser, såsom uppdelning
i header- och source-filer, kopierings- och förflyttningskonstruktorer. Även nyckelordet `noexcept` kommer behandlas.