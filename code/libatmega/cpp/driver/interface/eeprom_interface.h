/**
 * @brief Implementation of EEPROM (Electrically Erasable Programmable ROM) stream interface.
 */
#pragma once

#include <stdint.h>

#include "type_traits.h"

namespace driver 
{

/**
 * @brief Class for implementation of EEPROM stream interface.
 */
class EepromInterface
{
public:

    /**
     * @brief Delete EEPROM stream.
     */
    virtual ~EepromInterface() noexcept = default;

    /**
     * @brief Check whether the EEPROM stream is initialized.
     * 
     * @return True if the EEPROM stream is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Indicate whether the EEPROM stream is enabled.
     * 
     * @return True if the EEPROM stream is enabled, otherwise false.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Set enablement of EEPROM stream.
     * 
     * @param enable Indicate whether to enable the EEPROM stream.
     */
    virtual void setEnabled(const bool enable) noexcept = 0;

    /**
     * @brief Write data to given address in EEPROM. If more than one byte is to be written, 
     *        the other bytes are written to the consecutive addresses until all bytes are stored.
     * 
     * @tparam T The data type of the data to write. Must be unsigned.
     *
     * @param address The destination address.
     * @param data    The data to write to the destination address.
     *
     * @return True upon successful write, otherwise false.
     */
    template <typename T = uint8_t>
    bool write(const uint16_t address, const T& data) const noexcept;

    /**
     * @brief Read data from given address in EEPROM. If more than one byte is to be read,
     *        the consecutive addresses are read until all bytes are read.
     *
     * @tparam T The data type of the data to read. Must be unsigned.
     * 
     * @param address The destination address.
     * @param data    Reference to variable for storing the data read from given address.
     *
     * @return True upon successful read, otherwise false.
     */
    template <typename T = uint8_t>
    bool read(const uint16_t address, T& data) const noexcept;

private: 
    virtual bool isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept = 0;
    virtual void writeByte(const uint16_t address, const uint8_t data) const noexcept = 0;
    virtual uint8_t readByte(const uint16_t address) const noexcept = 0;
};

// -----------------------------------------------------------------------------
template <typename T>
bool EepromInterface::write(const uint16_t address, const T& data) const noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, 
                  "EEPROM write only supported for unsigned data types!");
    if (!isAddressValid(address, sizeof(T)) || !isEnabled()) { return false; }
    
    for (uint8_t i{}; i < sizeof(T); ++i)
    {
        writeByte(address + i, static_cast<uint8_t>(data >> (8U * i)));
    }
    return true;
}

// -----------------------------------------------------------------------------
template <typename T>
bool EepromInterface::read(const uint16_t address, T& data) const noexcept
{
    static_assert(type_traits::is_unsigned<T>::value, 
        "EEPROM read only supported for unsigned data types!");
    if (!isAddressValid(address, sizeof(T)) || !isEnabled()) { return false; }
    data = {};

    for (uint8_t i{}; i < sizeof(T); ++i) 
    { 
        data |= static_cast<T>(readByte(address + i) << (8U * i));
    }
    return true;
}

} // namespace driver
