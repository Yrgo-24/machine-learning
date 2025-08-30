/**
 * @brief EEPROM stream implementation details for ATmega328P.
 */
#include <avr/interrupt.h>

#include "driver/atmega328p/eeprom.h"
#include "utils/utils.h"

namespace driver 
{
namespace atmega328p
{
namespace
{
/**
 * @brief Structure of ATmega328P EEPROM parameters.
 */
struct EepromParam
{
    /** Size of the EEPROM in bytes. */
    static constexpr size_t Size{1024U};

    /** Highest EEPROM address. */
    static constexpr size_t MaxAddress{Size - 1U};
};


} // namespace

// -----------------------------------------------------------------------------
EepromInterface& Eeprom::getInstance() noexcept
{
    // Create and initialize the singleton EEPROM instance (once only).
    static Eeprom myInstance{};

    // Return a reference to the singleton EEPROM instance, cast to the corresponding interface.
    return myInstance; 
}

// -----------------------------------------------------------------------------
bool Eeprom::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Eeprom::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Eeprom::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
Eeprom::Eeprom() noexcept
    : myEnabled{false} 
{}

// -----------------------------------------------------------------------------
bool Eeprom::isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept
{
    return EepromParam::MaxAddress >= address + dataSize;
}

// -----------------------------------------------------------------------------
void Eeprom::writeByte(const uint16_t address, const uint8_t data) const noexcept
{
    // Wait until EEPROM is ready to send the next byte.
    while (utils::read(EECR, EEPE));

    // Set the address and data to write.
    EEAR = address;
    EEDR = data;

    // Perform write, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    utils::set(EECR, EEMPE);
    utils::set(EECR, EEPE);

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
uint8_t Eeprom::readByte(const uint16_t address) const noexcept
{
    // Wait until EEPROM is ready to read the next byte.
    while (utils::read(EECR, EEPE));

    // Set the address from which to read.
    EEAR = address;

    // Read and return the value of the given address.
    utils::set(EECR, EERE);
    return EEDR;
}
} // namespace atmega328p
} // namespace driver
