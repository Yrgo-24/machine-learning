/**
 * @brief Implementation details of serial driver.
 */
#include <avr/io.h>

#include "serial.h"

/**
 * @note These lines have to be added to an arbitrary C++ source file in order to use interfaces
 *       for Atmega328P in Microchip Studio.
 */
void operator delete(void* ptr) noexcept { (void) (ptr); }
void operator delete(void* ptr, unsigned int) noexcept { operator delete(ptr); }
extern "C" void __cxa_pure_virtual() { while (1) {} }

namespace driver 
{
// Singleton serial instance.
Serial Serial::myInstance{};

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
    while (0U == (UCSR0A & (1UL << UDRE0)));

    // Put the new character in the transmission register.
    UDR0 = character;
}
} // namespace 

// -----------------------------------------------------------------------------
SerialInterface& Serial::getInstance() noexcept { return myInstance; }

// -----------------------------------------------------------------------------
uint32_t Serial::baudRate_bps() const { return Param::BaudRate_bps; }

// -----------------------------------------------------------------------------
Serial::Serial() noexcept 
{ 
    // Baud rate value corresponding to 9600 kbps.
    constexpr uint16_t baudRateValue{103U};

    // Enable UART transmission.
    UCSR0B |= (1UL << TXEN0);

    // Set the data size to eight bits per byte.
    UCSR0C = ((1UL << UCSZ00) | (1UL << UCSZ01));

    // Set the baud rate to 9600 kbps.
    UBRR0 = baudRateValue;

    // Send carriage return to align the first message left.
    UDR0 = Param::CarriageReturn;
}

// -----------------------------------------------------------------------------
void Serial::print(const char* message) const noexcept
{
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
} // namespace driver