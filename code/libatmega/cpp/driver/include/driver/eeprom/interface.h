/**
 * @brief EEPROM (Electrically Erasable Programmable ROM) stream interface.
 */
#pragma once

#include <stdint.h>

#include "utils/type_traits.h"

namespace driver 
{
/**
 * @brief EEPROM (Electrically Erasable Programmable ROM) stream interface.
 */
class EepromInterface
{
public:
    /**
     * @brief Delete the EEPROM stream.
     */
    virtual ~EepromInterface() noexcept = default;

    /**
     * @brief Check whether the EEPROM stream is initialized.
     * 
     * @return True if the EEPROM stream is initialized, false otherwise.
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief Indicate whether the EEPROM stream is enabled.
     * 
     * @return True if the EEPROM stream is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Set enablement of EEPROM stream.
     * 
     * @param[in] enable Indicate whether to enable the EEPROM stream.
     */
    virtual void setEnabled(const bool enable) = 0;

    /**
     * @brief Write data to given address in EEPROM. If more than one byte is to be written, 
     *        the other bytes are written to the consecutive addresses until all bytes are stored.
     * 
     * @tparam T The data type of the data to write. Must be unsigned.
     *
     * @param[in] address The destination address.
     * @param[in] data The data to write to the destination address.
     *
     * @return True upon successful write, false otherwise.
     */
    template <typename T = uint8_t>
    bool write(const uint16_t address, const T& data) const noexcept;

    /**
     * @brief Read data from given address in EEPROM. If more than one byte is to be read,
     *        the consecutive addresses are read until all bytes are read.
     *
     * @tparam T The data type of the data to read. Must be unsigned.
     * 
     * @param[in] address The destination address.
     * @param[out] data Reference to variable for storing the data read from given address.
     *
     * @return True upon successful read, false otherwise.
     */
    template <typename T = uint8_t>
    bool read(const uint16_t address, T& data) const noexcept;

private: 
    virtual bool isAddressValid(const uint16_t address, const uint8_t dataSize) const = 0;
    virtual void writeByte(const uint16_t address, const uint8_t data) const = 0;
    virtual uint8_t readByte(const uint16_t address) const = 0;
};

// -----------------------------------------------------------------------------
template <typename T>
bool EepromInterface::write(const uint16_t address, const T& data) const noexcept
{
    // Generate a compiler error if the given type isn't of unsigned type.
    static_assert(type_traits::is_unsigned<T>::value, 
        "EEPROM write only supported for unsigned data types!");

    // Return false is the given address in invalid or if the EEPROM stream isn't enabled.
    if (!isAddressValid(address, sizeof(T)) || !isEnabled()) { return false; }
    
    // Write each byte to EEPROM, one at a time.
    for (uint8_t i{}; i < sizeof(T); ++i)
    {
        writeByte(address + i, static_cast<uint8_t>(data >> (8U * i)));
    }
    // Return true to indicate success.
    return true;
}

// -----------------------------------------------------------------------------
template <typename T>
bool EepromInterface::read(const uint16_t address, T& data) const noexcept
{
    // Generate a compiler error if the given type isn't of unsigned type.
    static_assert(type_traits::is_unsigned<T>::value, 
        "EEPROM read only supported for unsigned data types!");

    // Return false is the given address in invalid or if the EEPROM stream isn't enabled.
    if (!isAddressValid(address, sizeof(T)) || !isEnabled()) { return false; }
    data = {};

    // Read each byte from EEPROM, one at a type.
    for (uint8_t i{}; i < sizeof(T); ++i) 
    { 
        data |= static_cast<T>(readByte(address + i) << (8U * i));
    }
    // Return true to indicate success.
    return true;
}
} // namespace driver
