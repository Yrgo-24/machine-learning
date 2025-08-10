/**
 * @brief Demonstration of GPIO device drivers in C++:
 * 
 *        The following devices are used:
 *            - A button connected to pin 13 on the device toggles a timer.
 *            - The aforementioned timer toggles an LED every 100 ms when enabled.
 *            - Another timer reduces the effect of contact bounces after pushing the button.
 *            - A watchdog timer is used to restart the program if it gets stuck somewhere.
 */
#include "adc.h"
#include "eeprom.h"
#include "gpio.h"
#include "serial.h"
#include "system.h"
#include "timer.h"
#include "watchdog.h"

namespace
{

target::System* sys{nullptr}; // Pointer to system implementation.

// -----------------------------------------------------------------------------
void buttonCallback() noexcept { sys->handleButtonInterrupt(); }

// -----------------------------------------------------------------------------
void debounceTimerCallback() noexcept { sys->handleDebounceTimerInterrupt(); }

// -----------------------------------------------------------------------------
void toggleTimerCallback() noexcept { sys->handleToggleTimerInterrupt(); }

} // namespace


/**
 * @brief Initialize and run system on target MCU.
 * 
 * @return Success code 0 upon termination of the program (should never occur).
 */
int main()
{
    using driver::atmega328p::Adc;
    using driver::atmega328p::Eeprom;
    using driver::atmega328p::Gpio;
    using driver::atmega328p::Serial;
    using driver::atmega328p::Timer;
    using driver::atmega328p::Watchdog;

    Gpio led{8U, Gpio::Direction::Output};
    Gpio button{13U, Gpio::Direction::InputPullup, buttonCallback};
    Timer debounceTimer{300U, debounceTimerCallback};
    Timer toggleTimer{100U, toggleTimerCallback};
    Serial serial{};
    Adc adc{};
    Watchdog watchdog{Watchdog::Timeout::Duration1024ms};
    Eeprom eeprom{};

    target::System system{led, button, debounceTimer, toggleTimer, serial, adc, watchdog, eeprom};
    sys = &system;
    sys->run();
    return 0;
}
