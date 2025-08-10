/**
 * @brief Implementation details of ATmega328P GPIO device driver.
 */
#include "callback_array.h"
#include "gpio.h"
#include "utils.h"

namespace driver 
{
namespace atmega328p
{
namespace
{

/**
 * @brief Structure containing indexes for callbacks associated with the I/O ports.
 */
struct CallbackIndex
{
    static constexpr uint8_t PortB{0U}; // Index for callback associated with I/O port B.
    static constexpr uint8_t PortC{1U}; // Index for callback associated with I/O port C.
    static constexpr uint8_t PortD{2U}; // Index for callback associated with I/O port D.
};

/**
 * @brief Struct containing pin offsets, i.e. the discrepancy between the Arduino and the
 *        ATmega328P pin numbers, for each I/O port.
 */
struct PinOffset
{
    static constexpr uint8_t PortB{8U};  // Pin offset for I/O port B.
    static constexpr uint8_t PortC{14U}; // Pin offset for I/O port C.
    static constexpr uint8_t PortD{0U};  // Pin offset for I/O port D.
};

/**
 * @brief Structure holding ATmega328P GPIO parameters.
 */
struct GpioParam
{
    static constexpr uint8_t ioPortCount{3U};               // The number of available I/O ports.
    static constexpr uint8_t pinCount{20U};                 // The number of available GPIO pins.
    static container::CallbackArray<ioPortCount> callbacks; // Pointers to callbacks.
    static uint32_t pinRegistry;                            // Pin registry (1 = reserved).
};

container::CallbackArray<GpioParam::ioPortCount> GpioParam::callbacks{};
uint32_t GpioParam::pinRegistry{};

// -----------------------------------------------------------------------------
constexpr bool isPinNumberValid(const uint8_t pin) noexcept { return pin < GpioParam::pinCount; }

// -----------------------------------------------------------------------------
constexpr bool isPinReserved(const uint8_t pin) noexcept
{
    return isPinNumberValid(pin) ? utils::read(GpioParam::pinRegistry, pin) : false;
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortB(const uint8_t pin) noexcept
{
    return pin >= Gpio::Port::B0 && pin <= Gpio::Port::B5;
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortC(const uint8_t pin) noexcept
{
    return pin >= Gpio::Port::C0 && pin <= Gpio::Port::C5;
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortD(const uint8_t pin) noexcept
{
    return pin >= Gpio::Port::D0 && pin <= Gpio::Port::D7;
}

// -----------------------------------------------------------------------------
constexpr uint8_t getPhysicalPin(const uint8_t pin) noexcept
{
    if (isPinConnectedToPortB(pin))      { return pin - PinOffset::PortB; }  
    else if (isPinConnectedToPortC(pin)) { return pin - PinOffset::PortC; } 
    else if (isPinConnectedToPortD(pin)) { return pin - PinOffset::PortD; }
    else                                 { return static_cast<uint8_t>(-1); }  
}

// -----------------------------------------------------------------------------
constexpr bool isDirectionValid(const Gpio::Direction direction) noexcept
{
    return direction < Gpio::Direction::Count;
}

// -----------------------------------------------------------------------------
inline void invokeCallback(const uint8_t port) noexcept
{
    GpioParam::callbacks.invoke(port);
}

} // namespace

/**
 * @brief Structure for implementation of GPIO hardware.
 */
struct Gpio::Hardware 
{
    volatile uint8_t& dirReg;   // Reference to data direction register.
    volatile uint8_t& portReg;  // Reference to port (output) register.
    volatile uint8_t& pinReg;   // Reference to pin (input) register.
    volatile uint8_t& pcMskReg; // Reference to pin change interrupt mask register.
    const uint8_t pciBit;       // Control bit in the pin change interrupt control register.
    const IoPort port;          // I/O port associated with the device.
};

struct Gpio::Hardware Gpio::myHwPortB
{
    .dirReg   = DDRB,
    .portReg  = PORTB,
    .pinReg   = PINB,
    .pcMskReg = PCMSK0,
    .pciBit   = PCIE0,
    .port     = Gpio::IoPort::B,
};

struct Gpio::Hardware Gpio::myHwPortC
{
    .dirReg   = DDRC,
    .portReg  = PORTC,
    .pinReg   = PINC,
    .pcMskReg = PCMSK1,
    .pciBit   = PCIE1,
    .port     = Gpio::IoPort::C,
};

struct Gpio::Hardware Gpio::myHwPortD
{
    .dirReg   = DDRD,
    .portReg  = PORTD,
    .pinReg   = PIND,
    .pcMskReg = PCMSK2,
    .pciBit   = PCIE2,
    .port     = Gpio::IoPort::D,
};

// -----------------------------------------------------------------------------
Gpio::Gpio(const uint8_t pin, const Direction direction, void (*callback)()) noexcept
    : myHardware{reserve(pin, direction)}
    , myPin{getPhysicalPin(pin)}
{ 
    if (myHardware)
    {
        setDirection(direction);
        if (callback) { setCallback(callback); }
    }
}

// -----------------------------------------------------------------------------
Gpio::~Gpio() noexcept { disable(); }

// -----------------------------------------------------------------------------
Gpio::Gpio(Gpio&& other) noexcept
    : myHardware{other.myHardware}
    , myPin{other.myPin}
{
    other.myHardware = nullptr;
    other.myPin      = static_cast<uint8_t>(-1);
}

// -----------------------------------------------------------------------------
Gpio& Gpio::operator=(Gpio&& other) noexcept
{
    if (&other != this)
    {
        disable();
        myHardware       = other.myHardware;
        myPin            = other.myPin;
        other.myHardware = nullptr;
        other.myPin      = static_cast<uint8_t>(-1);
    }
    return *this;
}

// -----------------------------------------------------------------------------
uint8_t Gpio::operator()() noexcept { return myPin; }

// -----------------------------------------------------------------------------
uint8_t Gpio::pin() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
Gpio::IoPort Gpio::port() const noexcept { return myHardware->port; };

// -----------------------------------------------------------------------------
bool Gpio::isInitialized() const noexcept { return myHardware != nullptr; }

// -----------------------------------------------------------------------------
bool Gpio::read() const noexcept { return utils::read(myHardware->pinReg, myPin); }

// -----------------------------------------------------------------------------
void Gpio::write(const bool output) noexcept
{
    if (output) { utils::set(myHardware->portReg, myPin); }
    else { utils::clear(myHardware->portReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::toggle() noexcept { utils::set(myHardware->pinReg, myPin); }

// -----------------------------------------------------------------------------
void Gpio::enableInterruptOnPort(const bool enable) noexcept 
{ 
    if (enable) { utils::set(PCICR, static_cast<uint8_t>(port())); }
    else { utils::clear(PCICR, static_cast<uint8_t>(port())); }
}

// -----------------------------------------------------------------------------
void Gpio::enableInterrupt(const bool enable) noexcept
{
    if (enable)
    {
        utils::globalInterruptEnable();
        utils::set(PCICR, myHardware->pciBit);
        utils::set(myHardware->pcMskReg, myPin);
    }
    else { utils::clear(myHardware->pcMskReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::blink(const uint16_t& blinkSpeedMs) noexcept
{
    toggle();
    utils::delayMs(blinkSpeedMs);
}

// -----------------------------------------------------------------------------
Gpio::Hardware* Gpio::reserve(const uint8_t pin, const Direction direction) noexcept
{
if (!isPinNumberValid(pin) || isPinReserved(pin) || !isDirectionValid(direction)) { return nullptr; }
    Hardware* hardware{initHardware(pin)};
    if (hardware) { utils::set(GpioParam::pinRegistry, pin); }
    return hardware;
}

// -----------------------------------------------------------------------------
Gpio::Hardware* Gpio::initHardware(const uint8_t pin) noexcept
{
    if (isPinConnectedToPortB(pin))      { return &myHwPortB; }
    else if (isPinConnectedToPortC(pin)) { return &myHwPortC; }
    else if (isPinConnectedToPortD(pin)) { return &myHwPortD; }
    return nullptr;
}

// -----------------------------------------------------------------------------
void Gpio::setDirection(const Direction direction) noexcept
{
    if (direction == Direction::InputPullup) { utils::set(myHardware->portReg, myPin); } 
    else if (direction == Direction::Output) { utils::set(myHardware->dirReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::setCallback(void (*callback)()) const noexcept
{
    if (myHardware->portReg == PORTB)      
    { 
        GpioParam::callbacks.add(callback, CallbackIndex::PortB); 
    }
    else if (myHardware->portReg == PORTC) 
    { 
        GpioParam::callbacks.add(callback, CallbackIndex::PortC); 
    } 
    else if (myHardware->portReg == PORTD) 
    { 
        GpioParam::callbacks.add(callback, CallbackIndex::PortD); 
    }
}

// -----------------------------------------------------------------------------
void Gpio::disable() noexcept
{
    utils::clear(myHardware->dirReg, myPin);
    utils::clear(myHardware->portReg, myPin);
    utils::clear(GpioParam::pinRegistry, myPin);

    enableInterrupt(false);
    myHardware = nullptr;
}

// -----------------------------------------------------------------------------
ISR (PCINT0_vect) { invokeCallback(CallbackIndex::PortB); }

// -----------------------------------------------------------------------------
ISR (PCINT1_vect) { invokeCallback(CallbackIndex::PortC); }

// -----------------------------------------------------------------------------
ISR (PCINT2_vect) { invokeCallback(CallbackIndex::PortD); }

} // namespace atmega328p
} // namespace driver
