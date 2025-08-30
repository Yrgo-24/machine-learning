/**
 * @brief EEPROM stream for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/eeprom/interface.h"

namespace driver 
{
namespace atmega328p
{
/**
 * @brief EEPROM stream for ATmega328P.
 * 
 *        Use the singleton design pattern to ensure only one ADC instance exists,
 *        reflecting the hardware limitation of a single ADC on the MCU.
 */
class Eeprom final : public EepromInterface
{
public:
    /**
     * @brief Get the singleton EEPROM instance.
     * 
     * @return Reference to the singleton EEPROM instance.
     */
    static EepromInterface& getInstance() noexcept;

    /**
     * @brief Check whether the EEPROM stream is initialized.
     * 
     * @return True if the EEPROM stream is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicate whether the EEPROM stream is enabled.
     * 
     * @return True if the EEPROM stream is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of EEPROM stream.
     * 
     * @param[in] enable Indicate whether to enable the EEPROM stream.
     */
    void setEnabled(const bool enable) noexcept override;

    Eeprom(const Eeprom&)            = delete; // No copy constructor.
    Eeprom(Eeprom&&)                 = delete; // No move constructor.
    Eeprom& operator=(const Eeprom&) = delete; // No copy assignment.
    Eeprom& operator=(Eeprom&&)      = delete; // No move assignment.

private: 
    Eeprom() noexcept;
    ~Eeprom() noexcept override = default;
    bool isAddressValid(const uint16_t address, const uint8_t dataSize) const noexcept override;
    void writeByte(const uint16_t address, const uint8_t data) const noexcept override;
    uint8_t readByte(const uint16_t address) const noexcept override;

    /** Indicate whether the EEPROM stream is enabled. */
    bool myEnabled;
};
} // namespace atmega328p
} // namespace driver
