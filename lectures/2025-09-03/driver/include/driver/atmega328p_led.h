/**
 * @brief LED implementation for ATmega328P.
 */
#pragma once

#include <cstdint>

#include "driver/led_interface.h"

namespace driver::atmega328p
{
/**
 * @brief LED implementation for ATmega328P.
 */
class Led final : public LedInterface
{
public:
    /**
     * @brief Create a new LED.
     * 
     * @param[in] pin The pin the LED is connected to.
     */
    explicit Led(const std::uint8_t pin) noexcept;

    /**
     * @brief Delete the LED.
     */
    ~Led() noexcept override;

    /**
     * @brief Get the pin the LED is connected to.
     * 
     * @return The pin the LED is connected to.
     */
    std::uint8_t pin() const noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True if the LED is to be enabled, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    Led()                      = delete; // No default constructor.
    Led(const Led&)            = delete; // No copy constructor.
    Led(Led&&)                 = delete; // No move constructor.
    Led& operator=(const Led&) = delete; // No copy assignment.
    Led& operator=(Led&&)      = delete; // No move assignment.

private:
    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::atmega328p