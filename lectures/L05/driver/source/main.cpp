/**
 * @brief Demonstration of interfaces for LEDs.
 */
#include <chrono>
#include <thread>

#include "driver/atmega328p_led.h"
#include "driver/stm32f103_led.h"

using namespace driver;

namespace
{
// -----------------------------------------------------------------------------
void delayMs(const std::size_t durationMs) noexcept
{
    std::this_thread::sleep_for(std::chrono::milliseconds(durationMs));
}

// -----------------------------------------------------------------------------
void blinkLed(LedInterface& led, const std::size_t blinkTimeMs) noexcept
{
    // Toggle the LED, then delay the calling thread.
    led.toggle();
    delayMs(blinkTimeMs);
    
    // Toggle the LED again, then delay the calling thread.
    led.toggle();
    delayMs(blinkTimeMs);
}
} // namespace

/**
 * @brief Blink LEDs connected to different microprocessors.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Connect LED1 at pin 9 of the ATmega328P processor.
    atmega328p::Led led1{9U};

    // Connect LED2 at pin 20 of the STM32F103 processor, enable the LED on startup.
    stm32f103::Led led2{20U, true};

    // Blink the LEDs ten times, then terminate the program.
    for (std::size_t i{}; i < 10U; ++i) 
    { 
        blinkLed(led1, 1000U);
        blinkLed(led2, 500U); 
    }
    return 0;
}