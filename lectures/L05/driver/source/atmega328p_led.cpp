/**
 * @brief LED implementation details for ATmega328P.
 */
#include <cstdint>
#include <iostream>

#include "driver/atmega328p_led.h"

namespace driver::atmega328p
{
// -----------------------------------------------------------------------------
Led::Led(const std::uint8_t pin) noexcept
    : myPin{pin}
    , myIsEnabled{false}
{
    // Insert code to initialize the hardware here.
    std::cout << "Initializing LED connected to pin " << static_cast<int>(myPin) 
              << " on processor ATmega328P!\n";
}

// -----------------------------------------------------------------------------
Led::~Led() noexcept 
{
    // Insert code to release allocated resources here.
    std::cout << "Deleting LED connected to pin " << static_cast<int>(myPin) 
              << " on processor ATmega328p!\n";
}

// -----------------------------------------------------------------------------
std::uint8_t Led::pin() const noexcept { return myPin; }

// -----------------------------------------------------------------------------
bool Led::isEnabled() const noexcept { return myIsEnabled; }

// -----------------------------------------------------------------------------
void Led::setEnabled(const bool enable) noexcept 
{
    // Insert code to enable the LED here.
    myIsEnabled = enable;
    const auto status{myIsEnabled ? "Enabling" : "Disabling"};
    std::cout << status << " LED connected to pin " << static_cast<int>(myPin) << "!\n";
}

// -----------------------------------------------------------------------------
void Led::toggle() noexcept { setEnabled(!myIsEnabled); }

} // namespace driver::atmega328p