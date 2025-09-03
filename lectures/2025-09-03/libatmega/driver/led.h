/**
 * @brief LED driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "led_interface.h"

namespace driver
{
/**
 * @brief LED driver for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 */
class Led final : public LedInterface
{
public:
    /**
     * @brief Create a new LED.
     * 
     * @param[in] pin Arduino pin number (0 - 13) the LED is connected to.
     * @param[in] startVal Initial value of the LED (default = disabled).
     */
    explicit Led(const uint8_t pin, const bool startVal = false) noexcept;

    /**
     * @brief Delete the LED.
     */
    ~Led() noexcept override;

    /**
     * @brief Check whether the LED is enabled.
     * 
     * @return True if the LED is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the LED.
     * 
     * @param[in] enable True to enable the LED, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override;

    /**
     * @brief Toggle the LED.
     */
    void toggle() noexcept override;

    /**
     * @brief Blink the LED with the given blink period.
     * 
     * @param[in] blinkPeriod_ms The blink period in millisecond (0 - 65 seconds).
     */
    void blink(const uint16_t blinkPeriod_ms) noexcept override;

    /**
     * @brief Get the Arduino pin number the LED is connected to.
     * 
     * @return The Arduino pin number the LED is connected to.
     */
    uint8_t pin() const noexcept;

    Led(const Led&)            = delete; // No copy constructor.
    Led(Led&&)                 = delete; // No move constructor.
    Led& operator=(const Led&) = delete; // No copy assignment.
    Led& operator=(Led&&)      = delete; // No move assignment.

private:
    /** Pointer to the data direction register, e.g. DDRx. */
    volatile uint8_t* myDirReg;

    /** Pointer to the port register, e.g. PORTx. */
    volatile uint8_t* myPortReg;

    /** Pointer to the pin register, e.g. PINx. */
    volatile uint8_t* myPinReg;

    /** GPIO pin number on the associated port (0 - 7). */
    uint8_t myPin;
};
} // namespace driver