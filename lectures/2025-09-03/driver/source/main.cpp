#include <chrono>
#include <thread>

#include "driver/atmega328p_led.h"
#include "driver/stm32_led.h"

using namespace driver;

namespace
{
// -----------------------------------------------------------------------------
void delayMs(const std::size_t delayTimeMs) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(delayTimeMs));
}

// -----------------------------------------------------------------------------
void blinkLed(LedInterface& led, const std::size_t delayTimeMs) noexcept
{
    led.toggle();
    delayMs(delayTimeMs);
    led.toggle();
    delayMs(delayTimeMs);
}
} // namespace

int main()
{
    atmega328p::Led led1{9U};
    stm32::Led led2{20U, true};

    for (std::size_t i{}; 10U > i; ++i) 
    { 
        blinkLed(led1, 1000U);
        blinkLed(led2, 500U); 
    }
    return 0;
}