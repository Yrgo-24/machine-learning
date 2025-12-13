/**
 * @brief Led implementation for miscellaneous LEDs.
 */
#pragma once

#include <stdint.h>

namespace driver
{
/**
 * @brief Led implementation for miscellaneous LEDs.
 */
class LedInterface
{
public:
    /**
     * @brief Delete the LED.
     */
    virtual ~LedInterface() noexcept = default;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True to enable the LED, false otherwise.
     */
    virtual void setEnabled(const bool enable) = 0;

    /**
     * @brief Toggle the LED.
     */
    virtual void toggle() = 0;

    /**
     * @brief Blink the LED with the given blink period.
     * 
     * @param[in] blinkPeriod_ms The blink period in millisecond (0 - 65 seconds).
     */
    virtual void blink(const uint16_t blinkPeriod_ms) = 0;
};
} // namespace driver