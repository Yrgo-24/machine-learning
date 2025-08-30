/**
 * @brief Implementation details of serial driver.
 */
#include <avr/io.h>

#include "driver/atmega328p/serial.h"
#include "utils/utils.h"

namespace driver 
{
namespace atmega328p
{
namespace
{
/**
 * @brief Structure holding serial parameters.
 */
struct Param
{
    // Baud rate in bps.
    static constexpr uint32_t BaudRate_bps{9600U};

    // Carriage return character.
    static constexpr char CarriageReturn{'\r'};

    // New line character.
    static constexpr char NewLine{'\n'};
};

// -----------------------------------------------------------------------------
void transmitChar(const char character) noexcept
{
    // Wait until the previous character has been sent.
    while (!utils::read(UCSR0A, UDRE0));

    // Put the new character in the transmission register.
    UDR0 = character;
}
} // namespace 

// -----------------------------------------------------------------------------
SerialInterface& Serial::getInstance() noexcept
{ 
    // Create and initialize the singleton serial instance (once only).
    static Serial myInstance{};

    // Return a reference to the singleton serial instance, cast to the corresponding interface.
    return myInstance; 
}

// -----------------------------------------------------------------------------
uint32_t Serial::baudRate_bps() const { return Param::BaudRate_bps; }

// -----------------------------------------------------------------------------
bool Serial::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Serial::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Serial::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
Serial::Serial() noexcept 
    : myEnabled{false}
{ 
    // Baud rate value corresponding to 9600 kbps.
    constexpr uint16_t baudRateValue{103U};

    // Enable UART transmission.
    utils::set(UCSR0B, TXEN0);

    // Set the data size to eight bits per byte.
    utils::set(UCSR0C, UCSZ00, UCSZ01);

    // Set the baud rate to 9600 kbps.
    UBRR0 = baudRateValue;

    // Send carriage return to align the first message left.
    UDR0 = Param::CarriageReturn;
}

// -----------------------------------------------------------------------------
void Serial::print(const char* message) const noexcept
{
    // Terminate the function if serial transmission isn't enabled.
    if (!myEnabled) { return; }

    // Transmit each character of the string one by one.
    for (const char* it{message}; *it; ++it)
    {
        transmitChar(*it);
        
        // Send new line characters instead of carriage returns.
        if (Param::CarriageReturn == *it) 
        { 
            transmitChar(Param::NewLine); 
        }
    }
}
} // namespace atmega328p
} // namespace driver
