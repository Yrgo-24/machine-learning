/**
 * @brief Implementation of system with generic hardware.
 */
#pragma once

namespace driver
{
class AdcInterface;
class EepromInterface;
class GpioInterface;
class SerialInterface;
class TimerInterface;
class WatchdogInterface;
} // namespace driver

namespace target
{

/**
 * @brief Generic system implementation.
 * 
 *        The following devices are used:
 * 
 *            - A button connected toggles a timer.
 * 
 *            - The aforementioned timer toggles an LED every 100 ms when enabled.
 * 
 *            - Another timer reduces the effect of contact bounces after pushing the button.
 * 
 *            - A watchdog timer is used to restart the program if it gets stuck somewhere.
 *
 *            - The LED state is written to EEPROM upon every change. This value is evaluated upon startup.
 * 
 *        This class is non-copyable and non-movable.
 */
class System
{
public:

    /**
     * @brief Create new system.
     *     
     * @param led           Reference to LED.
     * @param button        Reference to button used to toggle the toggle timer.
     * @param debounceTimer Reference to timer used to mitigate effects of contact bounces.
     * @param toggleTimer   Reference to timer used to toggle the LED.
     * @param serial        Reference to serial device used to print status messages.
     * @param adc           Reference to ADC.
     * @param watchdog      Reference to watchdog timer.
     * @param eeprom        Reference to EEPROM stream.
     */
    System(driver::GpioInterface& led,
           driver::GpioInterface& button,
           driver::TimerInterface& debounceTimer,
           driver::TimerInterface& toggleTimer,
           driver::SerialInterface& serial,
           driver::AdcInterface& adc,
           driver::WatchdogInterface& watchdog,
           driver::EepromInterface& eeprom) noexcept;

    /**
     * @brief Delete system.
     */
    ~System() noexcept;

    /**
     * @brief Enable serial transmission.
     * 
     * @param enable Indicate whether to enable serial transmission.
     */
    void enableSerialTransmission(const bool enable) noexcept;

    /**
     * @brief Button interrupt handler.
     * 
     *        Toggle the timer whenever the button is pressed. 
     * 
     *        Pin change interrupts are disabled for 300 ms after a press to mitigate the effects 
     *        of contact bounce.
     */
    void handleButtonInterrupt() noexcept;

    /**
     * @brief Debounce timer interrupt handler.
     * 
     *        Enable pin change interrupts 300 ms after a press to mitigate the effects of contact bounce.
     */
    void handleDebounceTimerInterrupt() noexcept;

    /**
     * @brief Toggle timer interrupt handler.
     * 
     *        Toggle the LED every 100 ms when the associated timer is enabled.
     */
    void handleToggleTimerInterrupt() noexcept;

    System(const System&)            = delete; // No copy constructor.
    System(System&&)                 = delete; // No move constructor.
    System& operator=(const System&) = delete; // No copy assignment.
    System& operator=(System&&)      = delete; // No move assignment.

    /**
     * @brief Run the system as long as voltage is supplied.                                                               
     */
    void run() noexcept;

private:

    void handleButtonPressed() noexcept;
    void checkLedStateInEeprom() noexcept;
    void writeLedStateToEeprom() noexcept;
    bool readLedStateFromEeprom() const noexcept;

    driver::GpioInterface& myLed;            // Reference to LED.
    driver::GpioInterface& myButton;         // Reference to button for toggling the toggle timer.
    driver::TimerInterface& myDebounceTimer; // Reference to debounce timer.
    driver::TimerInterface& myToggleTimer;   // Reference to toggle timer for toggling the LED.
    driver::SerialInterface& mySerial;       // Reference to serial device used to print messages.
    driver::AdcInterface& myAdc;             // Reference to ADC.
    driver::WatchdogInterface& myWatchdog;   // Reference to watchdog timer.
    driver::EepromInterface& myEeprom;       // Reference to EEPROM stream.
};

} // namespace target