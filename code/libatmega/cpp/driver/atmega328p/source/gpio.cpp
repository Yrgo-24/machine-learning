/**
 * @brief GPIO driver implementation details for ATmega328P.
 */
#include <avr/interrupt.h>

#include "driver/atmega328p/gpio.h"
#include "utils/callback_array.h"
#include "utils/utils.h"

namespace driver 
{
namespace atmega328p
{
namespace
{
/**
 * @brief Structure of indexes for callbacks associated with the I/O ports.
 */
struct CallbackIndex
{
    static constexpr uint8_t PortB{0U}; // Index for callback associated with I/O port B.
    static constexpr uint8_t PortC{1U}; // Index for callback associated with I/O port C.
    static constexpr uint8_t PortD{2U}; // Index for callback associated with I/O port D.
};

/**
 * @brief Structure of pin offsets, i.e. the discrepancy between the Arduino and the ATmega328p
 *        pin numbers, for each I/O port.
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
    /** The number of available I/O ports. */
    static constexpr uint8_t IoPortCount{3U};

    /** The number of available GPIO pins. */
    static constexpr uint8_t PinCount{20U};

    /** Pointers to callbacks. */
    static container::CallbackArray<IoPortCount> callbacks;

    /** Pin registry (1 = reserved, 0 = free). */
    static uint32_t pinRegistry;
};

/** Pointers to callbacks. */
container::CallbackArray<GpioParam::IoPortCount> GpioParam::callbacks{};

/** Pin registry (1 = reserved, 0 = free). */
uint32_t GpioParam::pinRegistry{};

// -----------------------------------------------------------------------------
constexpr bool isPinNumberValid(const uint8_t pin) noexcept { return GpioParam::PinCount > pin; }

// -----------------------------------------------------------------------------
constexpr bool isPinReserved(const uint8_t pin) noexcept
{
    return isPinNumberValid(pin) ? utils::read(GpioParam::pinRegistry, pin) : false;
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortB(const uint8_t pin) noexcept
{
    return utils::inRange(pin, Gpio::Port::B0, Gpio::Port::B5);
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortC(const uint8_t pin) noexcept
{
    return utils::inRange(pin, Gpio::Port::C0, Gpio::Port::C5);
}

// -----------------------------------------------------------------------------
constexpr bool isPinConnectedToPortD(const uint8_t pin) noexcept
{
    return utils::inRange(pin, Gpio::Port::D0, Gpio::Port::D5);
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
    return Gpio::Direction::Count > direction;
}

// -----------------------------------------------------------------------------
inline void invokeCallback(const uint8_t port) noexcept
{
    GpioParam::callbacks.invoke(port);
}
} // namespace

/**
 * @brief GPIO hardware structure.
 */
struct Gpio::Hardware 
{
    /** Reference to data direction register (DDRx). */
    volatile uint8_t& dirReg;

    /** Reference to port (output) register (PORTx). */
    volatile uint8_t& portReg;

    /** Reference to pin (input) register (PINx). */
    volatile uint8_t& pinReg;

    /** Reference to pin change interrupt mask register (PCMSKx). */
    volatile uint8_t& pcMskReg;

    /** Control bit in the pin change interrupt control register (PCIEx). */
    const uint8_t pciBit;

    /** I/O port associated with the GPIO. */
    const IoPort port;
};

/** Hardware structure for I/O port B. */
struct Gpio::Hardware Gpio::myHwPortB
{
    .dirReg   = DDRB,
    .portReg  = PORTB,
    .pinReg   = PINB,
    .pcMskReg = PCMSK0,
    .pciBit   = PCIE0,
    .port     = Gpio::IoPort::B,
};

/** Hardware structure for I/O port C. */
struct Gpio::Hardware Gpio::myHwPortC
{
    .dirReg   = DDRC,
    .portReg  = PORTC,
    .pinReg   = PINC,
    .pcMskReg = PCMSK1,
    .pciBit   = PCIE1,
    .port     = Gpio::IoPort::C,
};

/** Hardware structure for I/O port D. */
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
    // Set the data direction (and potentially the callback) if the pin was reserved.
    if (myHardware)
    {
        setDirection(direction);
        if (callback) { setCallback(callback); }
    }
}

// -----------------------------------------------------------------------------
Gpio::~Gpio() noexcept 
{ 
    // Free resources used for the GPIO before deletion.
    utils::clear(myHardware->dirReg, myPin);
    utils::clear(myHardware->portReg, myPin);
    utils::clear(GpioParam::pinRegistry, myPin);

    enableInterrupt(false);
    myHardware = nullptr; 
}

// -----------------------------------------------------------------------------
uint8_t Gpio::operator()() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
uint8_t Gpio::pin() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
Gpio::IoPort Gpio::port() const noexcept { return myHardware->port; };

// -----------------------------------------------------------------------------
bool Gpio::isInitialized() const noexcept { return nullptr != myHardware; }

// -----------------------------------------------------------------------------
bool Gpio::read() const noexcept { return utils::read(myHardware->pinReg, myPin); }

// -----------------------------------------------------------------------------
void Gpio::write(const bool output) noexcept
{
    // Set/clear the output as requested.
    if (output) { utils::set(myHardware->portReg, myPin); }
    else { utils::clear(myHardware->portReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::toggle() noexcept { utils::set(myHardware->pinReg, myPin); }

// -----------------------------------------------------------------------------
void Gpio::enableInterruptOnPort(const bool enable) noexcept 
{ 
    // Enable/disable interrupts on the associated port as requested.
    if (enable) { utils::set(PCICR, myHardware->pciBit); }
    else { utils::clear(PCICR, myHardware->pciBit); }
}

// -----------------------------------------------------------------------------
void Gpio::enableInterrupt(const bool enable) noexcept
{
    // Enable/disable interrupts on the associated pin as requested.
    if (enable)
    {
        utils::globalInterruptEnable();
        utils::set(PCICR, myHardware->pciBit);
        utils::set(myHardware->pcMskReg, myPin);
    }
    else { utils::clear(myHardware->pcMskReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::blink(const uint16_t& blinkSpeed_ms) noexcept
{
    toggle();
    utils::delay_ms(blinkSpeed_ms);
}

// -----------------------------------------------------------------------------
Gpio::Hardware* Gpio::reserve(const uint8_t pin, const Direction direction) noexcept
{
    // Return a nullptr if the given pin is reserved or any of the given parameters are invalid.
    if (!isPinNumberValid(pin) || !isDirectionValid(direction) || isPinReserved(pin)) 
    { 
        return nullptr; 
    }

    // Initialize the hardware, register the given pin on success.
    auto hardware{initHardware(pin)};
    if (hardware) { utils::set(GpioParam::pinRegistry, pin); }

    // Return a pointer to the hardware used, or a nullptr on failure.
    return hardware;
}

// -----------------------------------------------------------------------------
Gpio::Hardware* Gpio::initHardware(const uint8_t pin) noexcept
{
    // Return a pointer to the corresponding structure, or a nullptr if the pin number is invalid.
    if (isPinConnectedToPortB(pin))      { return &myHwPortB; }
    else if (isPinConnectedToPortC(pin)) { return &myHwPortC; }
    else if (isPinConnectedToPortD(pin)) { return &myHwPortD; }
    return nullptr;
}

// -----------------------------------------------------------------------------
void Gpio::setDirection(const Direction direction) noexcept
{
    // Set the GPIO direction by writing to the hardware registers.
    if (direction == Direction::InputPullup) { utils::set(myHardware->portReg, myPin); } 
    else if (direction == Direction::Output) { utils::set(myHardware->dirReg, myPin); }
}

// -----------------------------------------------------------------------------
void Gpio::setCallback(void (*callback)()) const noexcept
{
    // Register the given callback for the associated I/O port.
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
ISR (PCINT0_vect) { invokeCallback(CallbackIndex::PortB); }

// -----------------------------------------------------------------------------
ISR (PCINT1_vect) { invokeCallback(CallbackIndex::PortC); }

// -----------------------------------------------------------------------------
ISR (PCINT2_vect) { invokeCallback(CallbackIndex::PortD); }

} // namespace atmega328p
} // namespace driver
