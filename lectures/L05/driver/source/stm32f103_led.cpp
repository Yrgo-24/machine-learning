/**
 * @brief LED implementation details for STM32F103.
 */
#include <cstdint>
#include <iostream>

#include "driver/stm32f103_led.h"

namespace driver::stm32f103
{
// -----------------------------------------------------------------------------
Led::Led(const std::uint8_t pin, const bool startVal) noexcept
    : myPin{pin}
    , myIsEnabled{false}
{
    init(startVal);
}

// -----------------------------------------------------------------------------
Led::~Led() noexcept 
{
    // Insert code to release allocated resources here.
    std::cout << "Deleting LED connected to pin " << static_cast<int>(myPin) 
              << " on processor STM32F103!\n";
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

// -----------------------------------------------------------------------------
void Led::init(const bool startVal) noexcept
{
    // Insert code to initialize the hardware here.
    std::cout << "Initializing LED connected to pin " << static_cast<int>(myPin) 
              << " on processor STM32F103, starting value = " << startVal << "!\n";
    setEnabled(startVal);
}
} // namespace driver::stm32f103