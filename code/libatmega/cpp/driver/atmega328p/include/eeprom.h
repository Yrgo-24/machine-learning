/**
 * @brief Implementation of ATmega328P EEPROM stream.
 */
#pragma once

#include <stdint.h>

#include "eeprom_interface.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for implementation of ATmega328P EEPROM stream.
 * 
 *        This class is non-copyable.
 */
class Eeprom : public EepromInterface
{
public:

    /**
     * @brief Create new EEPROM stream.
     * 
     * @param enable Indicate whether to enable serial transmission (default = true).
     */
    explicit Eeprom(const bool enable = true) noexcept;

    /**
     * @brief Delete EEPROM stream.
     */
    ~Eeprom() noexcept override = default;

      /**
     * @brief Move memory from another EEPROM stream.
     *
     *        The other EEPROM stream is cleared once the move operation is completed.
     *
     * @param other Reference to EEPROM stream to move memory from.
     */
    Eeprom(Eeprom&& other) noexcept;

    /**
     * @brief Move content from another EEPROM stream.
     * 
     *        The other EEPROM stream is cleared once the move operation is completed.
     *
     * @param other Reference to EEPROM stream holding the data to move. 
     * 
     * @return Reference to this EEPROM stream.
     */
    Eeprom& operator=(Eeprom&& other) noexcept;

    /**
     * @brief Check whether the EEPROM stream is initialized.
     * 
     * @return True if the EEPROM stream is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicate whether the EEPROM stream is enabled.
     * 
     * @return True if the EEPROM stream is enabled, otherwise false.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of EEPROM stream.
     * 
     * @param enable Indicate whether to enable the EEPROM stream.
     */
    void setEnabled(const bool enable) noexcept override;

    Eeprom(const Eeprom&)            = delete; // No copy constructor.
    Eeprom& operator=(const Eeprom&) = delete; // No copy assignment.

private: 
    bool isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept override;
    void writeByte(const uint16_t address, const uint8_t data) const noexcept override;
    uint8_t readByte(const uint16_t address) const noexcept override;

    bool myEnabled; // Indicate whether the EEPROM stream is enabled.
};

} // namespace atmega328p
} // namespace driver
