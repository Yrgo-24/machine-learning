/**
 * @brief Button implementation details for ATmega328P.
 */
#include <stdint.h>

#include <avr/io.h>

#include "button.h"
#include "serial.h"

namespace driver
{
// -----------------------------------------------------------------------------
Button::Button(const uint8_t pin) noexcept
    : myPortReg{nullptr}
    , myPinReg{nullptr}
    , myPin{}
{
    // Pin 0 - 7 => the button is connected to I/O port D.
    if (7U >= pin)
    {
        myPortReg = &PORTD;
        myPinReg  = &PIND;
        myPin     = pin;
    }
    // Pin 8 - 13 => the button is connected to I/O port B.
    else if ((13U >= pin) && (8U <= pin))
    {
        myPortReg = &PORTB;
        myPinReg  = &PINB;
        myPin = pin - 8U;
    }
    // Print an error message if the pin number is invalid.
    else
    {
        // Halt execution if the pin number is invalid.
        Serial::getInstance().printf("Invalid pin number %u!\n", pin);
        Serial::getInstance().printf("Halting execution of the program!\n");
        while (1);
    }
    // Enable the internal pullup resistor by setting the corresponding bit in PORTx.
    *myPortReg |= (1UL << myPin);
    Serial::getInstance().printf("Button at pin %u initialized!\n", pin);
}

// -----------------------------------------------------------------------------
Button::~Button() noexcept
{
    // Disable the internal pullup resistor before deletion.
    *myPortReg &= ~(1UL << myPin);
}

// -----------------------------------------------------------------------------
bool Button::input() const noexcept
{
    // Read the input from PINx, return true if the button is pressed.
    return *myPinReg & (1UL << myPin);
}

// -----------------------------------------------------------------------------
uint8_t Button::pin() const noexcept 
{ 
    // For port B, add offset so that it's pins map to Arduino pins 8 – 13.
    if (&PORTB == myPortReg) { return myPin + 8U; }
    else { return myPin; }
}
} // namespace driver