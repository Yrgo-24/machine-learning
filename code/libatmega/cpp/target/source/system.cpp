/**
 * @brief Implementation details of generic system implementation.
 */
#include <stdint.h>

#include "system.h"

#include "adc_interface.h"
#include "eeprom_interface.h"
#include "gpio_interface.h"
#include "serial_interface.h"
#include "timer_interface.h"
#include "watchdog_interface.h"

namespace target
{

/**
 * @brief Structure holding LED state parameters.
 */
namespace LedState
{
    static constexpr uint8_t address{0U}; // LED state address in EEPROM.
    static constexpr uint8_t enabled{1U}; // Enabled state value in EEPROM.
};

// -----------------------------------------------------------------------------
System::System(driver::GpioInterface& led,
               driver::GpioInterface& button,
               driver::TimerInterface& debounceTimer,
               driver::TimerInterface& toggleTimer,
               driver::SerialInterface& serial,
               driver::AdcInterface& adc,
               driver::WatchdogInterface& watchdog,
               driver::EepromInterface& eeprom) noexcept
    : myLed{led}
    , myButton{button}
    , myDebounceTimer{debounceTimer}
    , myToggleTimer{toggleTimer}
    , mySerial{serial}
    , myAdc{adc}
    , myWatchdog{watchdog}
    , myEeprom{eeprom}
{
    myButton.enableInterrupt(true);
    checkLedStateInEeprom();
}

// -----------------------------------------------------------------------------
System::~System() noexcept
{
    myLed.write(false);
    myButton.enableInterrupt(false);
    myDebounceTimer.stop();
    myToggleTimer.stop();
}

// -----------------------------------------------------------------------------
void System::enableSerialTransmission(const bool enable) noexcept
{
    mySerial.setEnabled(enable);
}

// -----------------------------------------------------------------------------
void System::handleButtonInterrupt() noexcept
{
    myButton.enableInterruptOnPort(false);
    myDebounceTimer.start();
    if (myButton.read()) { handleButtonPressed(); }
}

// -----------------------------------------------------------------------------
void System::handleDebounceTimerInterrupt() noexcept
{
    myDebounceTimer.stop();
    myButton.enableInterruptOnPort(true);
}

// -----------------------------------------------------------------------------
void System::handleToggleTimerInterrupt() noexcept 
{ 
    mySerial.printf("Toggling the LED!\n");
    myLed.toggle(); 
}

// -----------------------------------------------------------------------------
void System::run() noexcept
{
    mySerial.printf("Running the system!\n");
    
    while (1)
    {
        myWatchdog.reset();
    }
}

// -----------------------------------------------------------------------------
void System::handleButtonPressed() noexcept
{
    mySerial.printf("Button pressed!\n");
    myToggleTimer.toggle();
    writeLedStateToEeprom();

    if (myToggleTimer.isEnabled()) { mySerial.printf("Toggle timer enabled!\n"); }
    else
    {
        mySerial.printf("Toggle timer disabled!\n");
        myLed.write(false);
    }
}

// -----------------------------------------------------------------------------
void System::checkLedStateInEeprom() noexcept
{
    if (readLedStateFromEeprom())
    {
        myToggleTimer.start();;
        mySerial.printf("Toggle timer enabled!\n");
    }
}

// -----------------------------------------------------------------------------
void System::writeLedStateToEeprom() noexcept
{ 
    myEeprom.write(LedState::address, myToggleTimer.isEnabled());
}

// -----------------------------------------------------------------------------
bool System::readLedStateFromEeprom() const noexcept
{
    uint8_t state{};
    return myEeprom.read(LedState::address, state) ? state == LedState::enabled : false;
}

} // namespace target