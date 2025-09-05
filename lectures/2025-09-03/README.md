# 2025-09-03 - Lektion 5

## Dagordning
* Interfaces och arv i C++.

## Mål med lektionen
* Kunna skapa klasser som ärver andra klasser.
* Kunna skapa interfaces.

## Instruktioner
* Läs om arv samt interfaces nedan.
* Se gärna bifogat interface-exempel för lysdioder [här](./driver/README.md).
* Lektionsanteckningar finns [här](./libatmega/README.md).

## Utvärdering
* Några synpunkter på lektionen i sig?

## Nästa lektion
* Recap gällande interfaces.
* Inlämningsuppgift med interfaces.

## Arv i C++

Arv är en grundläggande princip i C++ som möjliggör att man skapa nya klasser som bygger vidare på befintliga klasser. 
På detta sätt kan man "återanvända" hela eller delar av befintliga klasser och därigenom slippa skriva samma kod för flera
olika klasser.

Rent terminologiskt gäller att:
* En klass som ärver en annan kallas en underklass (eller subklass).
* Den klass som ärvs från kallas en basklass (eller superklass).

Anta att en basklass döpt `Gpio` har skapats för att enkelt kunna styra en GPIO-pin för en mikrokontroller:

```cpp
class Gpio
{
public:
    explicit Gpio(const std::uint8_t pin) noexcept;
    ~Gpio() noexcept;

    bool read() const noexcept;
    void write(const bool enable) noexcept;

    Gpio()                       = delete;
    Gpio(const Gpio&)            = delete;
    Gpio(Gpio&&)                 = delete;
    Gpio& operator=(const Gpio&) = delete;
    Gpio& operator=(Gpio&&)      = delete;

protected:
    // Add GPIO-specific functions for initialization and such here.
    void init() noexcept;

    /** The pin the GPIO is connected to.*/
    uint8_t myPin;
};
```

Notera att nyckelordet `protected` används i stället för `private` i detta fall. Anledningen till detta är
att vi vill låta eventuella underklasser kunna komma åt de delar av klassen som annars är privata (under förutsättning
att så kallats publikt arv används). Skillnaden
mellan privata och skyddade medlemmar är alltså att underklasser kan komma åt skyddade medlemmar, men inte
privata. Varken skyddade eller privata medlemmar är dock åtkomliga i övrigt.

Det finns tre typer av arv i C++:

- **Publikt arv** Den absolut vanligaste typen. Publika och skyddade medlemmar i basklassen behåller sina åtkomsträttigheter i underklassen. Används när underklassen är en specialisering av basklassen.
- **Privat arv:** Alla medlemmar i basklassen blir privata i underklassen. Används när basklassen endast ska användas
internt i underklassen.
- **Skyddat arv** Publika och skyddade medlemmar i basklassen blir skyddade i underklassen. Fungerar alltså som privat arv, med skillnaden att eventuella underklasser till aktuell underklass också får åtkomst till basklassen internt.

Nedan visas ett exempel på publikt arv, där en klass döpt `Led` ärver GPIO-funktionaliten från klassen `Gpio`.
Funktionerna `write` och `read` från basklassen `Gpio` kan därmed användas av instanser av klassen `Led`, alltså
lysdioder. En ny metod, `toggle`, har lagts till för att toggla lysdioden:

```cpp
class Led final : public Gpio
{
public:
    explicit Led(const std::uint8_t pin) noexcept;
    ~Led() noexcept;

    void toggle() noexcept;

    Led()                      = delete;
    Led(const Led&)           = delete;
    Led(Led&&)                = delete;
    Led& operator=(const Led&) = delete;
    Led& operator=(Led&&)      = delete;
};
```

Notera att:
* Klassen `Led` ärver klassen `Gpio` publikt genom att vi skriver `: public Gpio` efter klassens namn. 
* Nyckelordet `final` har lagts till direkt efter klassens namn `Led` så att denna klass inte kan
ärvas. Som tumregel är det en god idé att lägga till `final` om klassen inte avses kunna ärvas.
Ibland kan det dock vara önskvärt att kunna ärva implementationen för test och då får detta
nyckelord slopas.

Genom att ärva klassen `Gpio` slapp vi lägga till en medlemsvariabel som håller lysdiodens pin-nummer
samt rutiner för att skriva eller läsa lysdiodens utsignal. I stället kan vi enkelt skapa en lysdiod
via klassen `Led`s konstruktor och sedan använda metoderna `write` och `read` för att styra lysdioden:

```cpp
// Create an LED connected to pin 9.
Led led1{9U};

// Enable the LED.
led1.write(true);
```

På samma sätt hade vi kunnat skapa en underklass döpt `Button` för att kunna styra en knapp.
I detta fall har en metod lagts till för att aktivera/inaktivera avbrott vid nedtryckning
av tryckknappen. Motsvarande metod för att kolla om avbrott är aktiverat har också lagts till.
Eftersom knappen inte ska kunna styras ser vi till att metoden `write` är privat;
vi kan inte radera ärvda metoder, men vi kan ändra synligheten.

```cpp
class Button final : public Gpio
{
public:
    explicit Button(const std::uint8_t pin) noexcept;
    ~Button() noexcept;

    void enableInterrupt(const bool enable) noexcept;
    bool isInterruptEnabled() const noexcept;

    Button()                         = delete;
    Button(const Button&)            = delete;
    Button(Button&&)                 = delete;
    Button& operator=(const Button&) = delete;
    Button& operator=(Button&&)      = delete;

private:
    void write(const bool enable) noexcept;
};
```

Via denna klass kan vi sedan enkelt läsa lysdiodens utsignal, aktivera avbrott och så vidare:

```cpp
// Create a button connected to pin 13.
Button button1{13U};

// Enable interrupt at pressdown.
button1.enableInterrupt(true);

// Toggle led1 if the button is pressed.
if (button1.read()) { led1.toggle(); }
```

## Interfaces i C++

### Vad är ett interface?

Ett interface i C++ är en abstrakt basklass, som endast innehåller rent virtuella metoder, dvs. metoder som saknar implementation.
Syftet med ett interface är att definiera en gemensam  uppsättning funktioner som olika klasser kan implementera på sitt eget sätt
utefter behov. 

Användning av interfaces har ett flertal fördelar:

* **Abstraktion:** Man kan skriva kod som arbetar mot en abstraktion, inte mot en konkret klass. Detta gör det lättare att byta ut implementationer utan att ändra resten av koden.
* **Utbytbarhet:** Olika implementationer kan användas beroende på behov, t.ex. olika hårdvarudrivrutiner eller testklasser.
* **Återanvändbarhet:** Kod som använder interfaces blir mer generell och kan återanvändas i olika sammanhang.
* **Testbarhet:** Genom att använda interfaces kan du enkelt skapa mock- eller fake-klasser för enhetstester.

Nedan visas strukturen för ett interface för timerkretsar. Detta interface har döpts till `TimerInterface`.
För att hålla koden kompakt används inga kommentarer i detta exempel.

```cpp
class TimerInterface
{
public:
    virtual ~TimerInterface() noexcept = default;
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual bool isRunning() const = 0;
    virtual void reset() = 0;
};
```

Via ovanstående interface kan man starta, stoppa och nollställa en timer via metoder `start`, `stop` samt `reset`.
Man kan också kontrollera ifall timern är aktiverad eller inte via metoden `isRunning`.

Några saker att notera:
* Destruktorn är markerad som `virtual` för att säkerställa att rätt destruktor anropas när en instans av en subklass raderas via en pekare eller referens till interfacet. Detta är avgörande för att undvika minnesläckor och för att korrekt städa upp resurser i både underklass och basklass.
* Destruktorn är också definierad (`= default`) för att ge en tom, automatisk implementation. Det är nödvändigt att explicit deklarera destruktorn i interfacet, annars blir den inte virtuell och korrekt polymorfisk destruktion uteblir. Eftersom interfacet inte har något att städa upp, räcker det med en default-implementation.
* Alla metoder som ska överlagras i underklasserna markeras med `virtual` och avslutas med `= 0`, vilket gör dem till rent virtuella metoder utan implementation i interfacet.

Nedan visas ett exempel på en basklass `atmega328p::Timer`, som utgör en skarp implementation för timerkretsar för mikrodator ATmega328p:

```cpp
namespace atmega328p
{
class Timer final : public TimerInterface
{
public:
    // Example constructor.
    explicit Timer(const std::size_t durationMs) noexcept;

    // Overridden methods.
    ~Timer() noexcept override;
    void start() noexcept override;
    void stop() noexcept override;
    bool isRunning() const noexcept override;
    void reset() noexcept override;

    // Additional ATmega328P-specific methods.

    Timer()                        = delete;
    Timer(const Timer&)            = delete;
    Timer(Timer&&)                 = delete;
    Timer& operator=(const Timer&) = delete;
    Timer& operator=(Timer&&)      = delete;

private:
    // Timer implementation details.
};
} // namespace atmega328p
```

Några saker vi såg tidigare i avsnittet om arv dyker upp även här:
* Nyckelordet `final` används för att säkerhetsställa att klassen `Timer` inte kan ärvas; detta
är den slutgiltiga implementationen. Detta är som tumregel bra att göra om klassen ej ska ärvas.
Ibland kan det dock vara önskvärt att kunna ärva implementationen för test och då får detta
nyckelord slopas.
* Klassen `atmega328p::Timer` ärver klassen `TimerInterface` genom att vi skriver `: public TimerInterface` efter
klassens namn. Att vi skriver `public` innebär att allt som är publikt (eller skyddat) i interfacet också 
är publikt (eller skyddat) i denna underklass.

Några nya saker att notera:
* De överlagrade metoderna markeras med `override` just för att markera att detta är skarpa implementationer av
överlagrade virtuella metoder. 
* Även destruktorn markeras också med `override` för att tydligt visa att den ersätter den virtuella destruktorn i interfacet. Detta säkerställer att rätt destruktor anropas vid polymorfisk destruktion och hjälper kompilatorn att upptäcka eventuella misstag i signaturen.
* Konstruktorn har inget med interfacet att höra och är varken markerad `virtual` eller `override`.

### Exempel på användning av interfaces

Nedan visas ett interface döpt `LedInterface`. Detta interface ska kunna användas för
diverse lysdioder, exempelvis från olika mikroprocessorer:

```cpp
/**
 * @brief LED interface implementation.
 */
class LedInterface
{
public:
    /**
     * @brief Delete the LED.
     */
    virtual ~LedInterface() noexcept = default;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    virtual std::uint8_t pin() const = 0;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    virtual void setEnabled(const bool enable) = 0;

    /**
     * @brief Toggle the LED.
     */
    virtual void toggle() noexcept = 0;
};
```

Nedan visas underklassen `atmega328p::Led`, som ärver interfacet `LedInterface` och implementeras för att enkelt
styra lysdioder anslutna till mikroprocessor ATmega328P:

```cpp
namespace atmega328p
{
/**
 * @brief LED implementation for ATmega328P.
 */
class Led final : public LedInterface
{
public:
    /**
     * @brief Create a new LED.
     * 
     * @param[in] pin The pin the LED is connected to.
     */
    explicit Led(const std::uint8_t pin) noexcept;

    /**
     * @brief Delete the LED.
     */
    ~Led() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Led()                      = delete; // No default constructor.
    Led(const Led&)            = delete; // No copy constructor.
    Led(Led&&)                 = delete; // No move constructor.
    Led& operator=(const Led&) = delete; // No copy assignment.
    Led& operator=(Led&&)      = delete; // No move assignment.

private:
    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace atmega328p
```

Genom att använda pekare eller referenser till `LedInterface` kan man skriva kod som fungerar med alla underklasser. Som exempel, nedan demonstreras en funktion döpt `blinkLed`, som används för att blinka en given LED. Notera
att ingående argument `led` utgörs av en referens till ett `LedInterface`, därmed kan lysdioden utgöras av en instans
en godtycklig underklass. Exempelvis kan detta vara en instans av den tidigare demonstrerade klassen `atmega::Led`, alternativt kan det vara en instans av klass för en lysdiod på en STM32-processor:

```cpp
/**
 * @brief Blink the given LED.
 * 
 * @param[in] led The LED to blink.
 * @param[in] blinkTimeMs The blink time in milliseconds.
 */
void blinkLed(LedInterface& led, const std::size_t blinkTimeMs) noexcept
{
    // Toggle the LED, then delay the calling thread.
    led.toggle();
    std::this_thread::sleep_for(std::chrono::milliseconds(blinkTimeMs));
    
    // Toggle the LED again, then delay the calling thread.
    led.toggle();
    std::this_thread::sleep_for(std::chrono::milliseconds(blinkTimeMs));
}
```

När funktionen ovan anropas kan vi använda en av underklasserna direkt, exempelvis vår klass
`atmega328p::led`. Antag att vi har implementerat en lysdiod ansluten till pin 9 på en Arduino Uno 
via en instans döpt `led1`:

```cpp
atmega328p::Led led1{9U};
```

Vi kan blinka denna lysdiod med en blinktid på exempelvis 1000 ms genom att kalla på funktionen `blinkLed`.
Eftersom `atmega328p::led` utgör en underklass till `LedInterface` kan vi passera `led1` direkt:

```cpp
blinkLed(led1, 1000U);
```

Anta att vi också har skapat en underklass `stm32F103::led` för att implementera lysdioder för en 
STM32f103-processor, såsom visas nedan. 
Notera att implementationen är lite annorlunda denna gång; framförallt har användaren möjlighet att sätta
lysdiodens startvärde direkt via anrop av konstruktorn. Denna klass har också en privat metod döpt `init`. 
Detta demonstreras för att visa att underklasserna kan skräddarsys efter behov:

```cpp
namespace stm32f103
{
/**
 * @brief LED implementation for STM32F103.
 */
class Led final : public LedInterface
{
public:
    /**
     * @brief Create a new LED.
     * 
     * @param[in] pin The pin the LED is connected to.
     * @param[in] startVal Initial value of the LED (default = off).
     */
    explicit Led(const std::uint8_t pin, const bool startVal = false) noexcept;

    /**
     * @brief Delete the LED.
     */
    ~Led() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Led()                      = delete; // No default constructor.
    Led(const Led&)            = delete; // No copy constructor.
    Led(Led&&)                 = delete; // No move constructor.
    Led& operator=(const Led&) = delete; // No copy assignment.
    Led& operator=(Led&&)      = delete; // No move assignment.

private:
    /**
     * @brief Initialize the LED.
     */
    void init(const bool startVal) noexcept;

    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace stm32f103
```

Antag att vi har implementerat en lysdiod ansluten till pin 20 på en STM32F103-processor via en instans döpt `led2`.
Vi håller lysdioden tänd vid start:

```cpp
stm32f103::Led led2{20U, true};
```

Vi kan även blinka denna lysdiod via anrop av funktionen `blinkLed`, eftersom klassen `stm32::led` 
utgör en underklass till `LedInterface`. Som exempel, för att blinka denna lysdiod var 500:e millisekund
kan följande anrop göras:

```cpp
blinkLed(led2, 500U);
```

Se det fullständiga exemplet [här](./driver/README.md).