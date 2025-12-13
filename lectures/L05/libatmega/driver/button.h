/**
 * @brief Button implementation for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "button_interface.h"

namespace driver
{
/**
 * @brief Button implementation for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 */
class Button final : public ButtonInterface
{
public:
    /**
     * @brief Create a new button.
     * 
     * @param[in] pin Arduino pin number 0 - 13.
     */
    explicit Button(const uint8_t pin) noexcept;

    /**
     * @brief Delete the button.
     * 
     *        Deactivate the internal pullup resistor before deletion.
     */
    ~Button() noexcept override;

    /**
     * @brief Read the button input.
     * 
     * @return True if the button is pressed, false otherwise.
     */
    bool input() const noexcept override;

    /**
     * @brief Get the Arduino pin number the button is connected to.
     * 
     * @return The Arduino pin number the button is connected to.
     */
    uint8_t pin() const noexcept;

    Button()                         = delete; // No default constructor (we need a pin number).
    Button(const Button&)            = delete; // No copy constructor (we don't want copies).
    Button(Button&&)                 = delete; // No move constructor (we don't want to move memory).
    Button& operator=(const Button&) = delete; // No copy assignment.
    Button& operator=(Button&&)      = delete; // No move assignment.

private:
    /** Pointer to the port register, e.g. PORTx. */
    volatile uint8_t* myPortReg;

    /** Pointer to the pin register, e.g. PINx. */
    volatile uint8_t* myPinReg;

    /** GPIO pin number on the associated port (0 - 7). */
    uint8_t myPin;
};
} // namespace driver