/**
 * @brief LED implementation for STM32.
 */
#pragma once

#include <cstdint>

#include "driver/led_interface.h"

namespace driver::stm32
{
/**
 * @brief LED interface implementation.
 */
class Led final : public LedInterface
{
public:
    /**
     * @brief Create a new LED.
     * 
     * @param[in] pin The pin the LED is connected to.
     * @param[in] startVal Initial value of the LED (default = off).
     */
    explicit Led(const std::uint8_t pin, const bool startVal = false) noexcept;

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
    /**
     * @brief Set up the LED.
     */
    void setup(const bool startVal) noexcept;

    /** The pin the LED is connected to. */
    std::uint8_t myPin;

    /** Indicate whether the LED is enabled. */
    bool myIsEnabled;
};
} // namespace driver::stm32