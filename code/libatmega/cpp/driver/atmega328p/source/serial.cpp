/**
 * @brief Implementation details of serial driver.
 */
#include "serial.h"
#include "utils.h"

namespace
{

/**
 * @brief Structure holding ATmega328P serial transmission parameters.
 */
struct SerialParam
{
    static constexpr char carriageReturn{'\r'};    // Carriage return character.
    static constexpr char newLine{'\n'};           // New line character.
    static constexpr uint16_t baudRateValue{103U}; // Baud rate value corresponding to 9600 kbps.
};

// -----------------------------------------------------------------------------
void transmitCharacter(const char character) noexcept
{
    while (utils::read(UCSR0A, UDRE0) == 0U);
    UDR0 = character;
}

// -----------------------------------------------------------------------------
void init() noexcept
{
    static bool initialized{false};
    if (initialized) { return; }
    
    utils::set(UCSR0B, TXEN0);
    utils::set(UCSR0C, UCSZ00, UCSZ01);
    UBRR0       = SerialParam::baudRateValue;
    UDR0        = SerialParam::carriageReturn;
    initialized = true;
}

} // namespace 

namespace driver 
{
namespace atmega328p
{

// -----------------------------------------------------------------------------
Serial::Serial(const bool enable) noexcept 
    : myEnabled{enable} 
{ 
    init(); 
}

// -----------------------------------------------------------------------------
Serial::Serial(Serial&& other) noexcept
    : myEnabled{other.myEnabled}
{
    other.myEnabled = false;
}

// -----------------------------------------------------------------------------
Serial& Serial::operator=(Serial&& other) noexcept
{
    if (&other != this)
    {
        myEnabled       = other.myEnabled;
        other.myEnabled = false;
    }
    return *this;
}

// -----------------------------------------------------------------------------
bool Serial::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Serial::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Serial::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
void Serial::print(const char* message) const noexcept
{
    for (const char* it{message}; *it; ++it)
    {
        transmitCharacter(*it);
        
        if (*it == SerialParam::carriageReturn) 
        { 
            transmitCharacter(SerialParam::newLine); 
        }
    }
}

} // namespace atmega328p
} // namespace driver
