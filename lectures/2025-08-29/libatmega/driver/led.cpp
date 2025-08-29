/**
 * @brief LED driver implementation details for ATmega328P.
 */
#define F_CPU 16000000UL // 16 MHz CPU frequency (needed by <util/delay.h>).

#include <avr/io.h>     // For DDRB, PORTB.
#include <util/delay.h> // For _delay_ms.

#include "led.h"
#include "serial.h"

namespace
{
// -----------------------------------------------------------------------------
void delay_ms(const uint16_t delayTime_ms) noexcept
{
    // For x ms, run x laps * 1 ms via a for loop.
    for (uint16_t i{}; i < delayTime_ms; ++i)
    {
        _delay_ms(1U);
    }
}
} // namespace

namespace driver
{

// -----------------------------------------------------------------------------
Led::Led(const uint8_t pin, const bool startVal) noexcept
    : myDirReg{nullptr}
    , myPortReg{nullptr}
    , myPinReg{nullptr}
    , myPin{}
{
    // If the pin is between 0 - 7, the LED is connected to I/O port D.
    if (7U >= pin)
    {
        myDirReg  = &DDRD;
        myPortReg = &PORTD;
        myPinReg  = &PIND;
        myPin     = pin;
    }
    // Else if the pin is between 8 - 13, the LED is connected to I/O port D.
    else if ((8U <= pin) && (13U >= pin))
    {
        myDirReg  = &DDRB;
        myPortReg = &PORTB;
        myPinReg  = &PINB;
        myPin     = pin - 8U;
    }
    else 
    {
        // Halt execution if the pin number is invalid.
        Serial::getInstance().printf("Invalid pin number %u!\n", pin);
        Serial::getInstance().printf("Halting execution of the program!\n");
        while (1);
    }
    // Set the pin to output by setting the corresponding bit in DDRx.
    *myDirReg |= (1UL << myPin);
    Serial::getInstance().printf("Led at pin %u initialized!\n", pin);
}

// -----------------------------------------------------------------------------
Led::~Led() noexcept
{
    // Clear DDRx and PORTx before deleting the LED.
    *myDirReg &= ~(1UL << myPin);
    *myPortReg &= ~(1UL << myPin);
}

// -----------------------------------------------------------------------------
bool Led::isEnabled() const noexcept
{
    // Return true if the corresponding bit in PINx is set.
    return *myPinReg & (1UL << myPin);
}

// -----------------------------------------------------------------------------
void Led::setEnabled(const bool enable) noexcept
{
    if (enable)
    {
        // Enable the LED by setting the corresponding bit in PORTx.
        *myPortReg |= (1UL << myPin);
    }
    else
    {
        // Disable the LED by clearing the corresponding bit in PORTx.
        *myPortReg &= ~(1UL << myPin);
    }
}

// -----------------------------------------------------------------------------
void Led::toggle() noexcept
{
    // Toggle the LED by "writing" to PINx; the hardware will toggle the output.
        *myPinReg = (1UL << myPin);
}

// -----------------------------------------------------------------------------
void Led::blink(const uint16_t blinkPeriod_ms) noexcept
{
    // Toggle the LED and generate a delay twice.
    toggle();
    delay_ms(blinkPeriod_ms);
    toggle();
    delay_ms(blinkPeriod_ms);
}
} // namespace driver