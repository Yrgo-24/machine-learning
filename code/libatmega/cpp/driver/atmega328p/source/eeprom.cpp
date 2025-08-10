    
/**
 * @brief Implementation details of ATmega328P EEPROM stream.
 */
#include "eeprom.h"
#include "utils.h"

namespace driver 
{
namespace atmega328p
{
namespace
{

/**
 * @brief Structure holding ATmega328P EEPROM parameters.
 */
struct EepromParam
{
    static constexpr size_t size{1024U};           // Size of the EEPROM in bytes.
    static constexpr size_t maxAddress{size - 1U}; // Highest address in EEPROM.
};


} // namespace

// -----------------------------------------------------------------------------
Eeprom::Eeprom(const bool enable) noexcept
    : myEnabled{enable} {}

// -----------------------------------------------------------------------------
Eeprom::Eeprom(Eeprom&& other) noexcept
    : myEnabled{other.myEnabled}
{
    other.myEnabled = false;
}

// -----------------------------------------------------------------------------
Eeprom& Eeprom::operator=(Eeprom&& other) noexcept
{
    if (&other != this)
    {
        myEnabled       = other.myEnabled;
        other.myEnabled = false;
    }
    return *this;
}

// -----------------------------------------------------------------------------
bool Eeprom::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Eeprom::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Eeprom::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
bool Eeprom::isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept
{
    return address + dataSize <= EepromParam::maxAddress;
}

// -----------------------------------------------------------------------------
void Eeprom::writeByte(const uint16_t address, const uint8_t data) const noexcept
{
    while (utils::read(EECR, EEPE));
    EEAR = address;
    EEDR = data;
    utils::globalInterruptDisable();
    utils::set(EECR, EEMPE);
    utils::set(EECR, EEPE);
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
uint8_t Eeprom::readByte(const uint16_t address) const noexcept
{
    while (utils::read(EECR, EEPE));
    EEAR = address;
    utils::set(EECR, EERE);
    return EEDR;
}

} // namespace atmega328p
} // namespace driver