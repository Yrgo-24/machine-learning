/**
 * @brief Generic system implementation for an MCU with configurable hardware devices.
 */
#pragma once

namespace driver
{
/** ADC (A/D converter) interface. */
class AdcInterface;

/** EEPROM (Electrically Erasable Programmable ROM) stream interface. */
class EepromInterface;

/** GPIO interface. */
class GpioInterface;

/** Serial transmission interface. */
class SerialInterface;

/** Timer interface. */
class TimerInterface;

/** Watchdog timer interface. */
class WatchdogInterface;
} // namespace driver

namespace target
{
/**
 * @brief Generic system for an MCU with configurable hardware devices.
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
class System final
{
public:
    /**
     * @brief Create a new system.
     *     
     * @param[in] led The LED to toggle.
     * @param[in] button Button used to toggle the toggle timer.
     * @param[in] debounceTimer Timer used to mitigate effects of contact bounces.
     * @param[in] toggleTimer Timer used to toggle the LED.
     * @param[in] serial Serial device used to print status messages.
     * @param[in] watchdog Watchdog timer that resets the program if it becomes unresponsive.
     * @param[in] eeprom EEPROM stream to write the status of the LED to EEPROM.
     * @param[in] adc ADC (currently unused).
     */
    explicit System(driver::GpioInterface& led, driver::GpioInterface& button, 
                    driver::TimerInterface& debounceTimer, driver::TimerInterface& toggleTimer,
                    driver::SerialInterface& serial, driver::WatchdogInterface& watchdog, 
                    driver::EepromInterface& eeprom, driver::AdcInterface& adc) noexcept;

    /**
     * @brief Delete system.
     */
    ~System() noexcept;

    /**
     * @brief Enable serial transmission.
     * 
     * @param[in] enable Indicate whether to enable serial transmission.
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

    /**
     * @brief Run the system as long as voltage is supplied.                                                               
     */
    void run() noexcept;

    System()                         = delete; // No default constructor.
    System(const System&)            = delete; // No copy constructor.
    System(System&&)                 = delete; // No move constructor.
    System& operator=(const System&) = delete; // No copy assignment.
    System& operator=(System&&)      = delete; // No move assignment.

private:
    void handleButtonPressed() noexcept;
    void checkLedStateInEeprom() noexcept;
    void writeLedStateToEeprom() noexcept;
    bool readLedStateFromEeprom() const noexcept;

    /** Reference to the LED to toggle. */
    driver::GpioInterface& myLed;

    /** Button used to toggle the toggle timer. */
    driver::GpioInterface& myButton;

    /** Debounce timer used to mitigate effects of contact bounces. */
    driver::TimerInterface& myDebounceTimer;

    /** Timer used to toggle the LED. */
    driver::TimerInterface& myToggleTimer;

    /** Serial device used to print status messages. */
    driver::SerialInterface& mySerial;

    /** Watchdog timer that resets the program if it becomes unresponsive. */
    driver::WatchdogInterface& myWatchdog;

    /** EEPROM stream to write the status of the LED to EEPROM. */
    driver::EepromInterface& myEeprom;

    /** A/D converter (currently unused). */
    driver::AdcInterface& myAdc;
};
} // namespace target