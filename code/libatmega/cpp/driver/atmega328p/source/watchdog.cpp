/**
 * @brief Watchdog timer driver implementation details for ATmega328P.
 */
#include <avr/io.h>

#include "utils/utils.h"
#include "driver/atmega328p/watchdog.h"

namespace driver 
{
namespace atmega328p
{
namespace 
{
/**
 * @brief Structure of ATmega328P watchdog timer parameters.
 */
struct WatchdogParam
{
    /** Default watchdog timeout (1024 ms). */
    static constexpr auto DefaultTimeout{Watchdog::Timeout::Duration1024ms};
};

// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const Watchdog::Timeout timeout) noexcept
{
    return static_cast<uint16_t>(Watchdog::Timeout::Invalid) > static_cast<uint16_t>(timeout);
}
    
// -----------------------------------------------------------------------------
uint8_t timeoutValue(const Watchdog::Timeout timeout) noexcept
{
    switch (timeout)
    {
        case Watchdog::Timeout::Duration16ms:
            return 0U;
        case Watchdog::Timeout::Duration32ms:
            return (1U << WDP0);
        case Watchdog::Timeout::Duration64ms:
            return (1U << WDP1);
        case Watchdog::Timeout::Duration128ms:
            return (1U << WDP1) | (1U << WDP0);
        case Watchdog::Timeout::Duration256ms:
            return (1U << WDP2);
        case Watchdog::Timeout::Duration512ms:
            return (1U << WDP2) | (1U << WDP0);
        case Watchdog::Timeout::Duration1024ms:
            return (1U << WDP2) | (1U << WDP1);
        case Watchdog::Timeout::Duration2048ms:
            return (1U << WDP2) | (1U << WDP1) | (1U << WDP0);
        case Watchdog::Timeout::Duration4096ms:
            return (1U << WDP3);
        case Watchdog::Timeout::Duration8192ms:
            return (1U << WDP3) | (1U << WDP0);
        default:
            return 0xFFU;
    }
}
} // namespace

// -----------------------------------------------------------------------------
WatchdogInterface& Watchdog::getInstance() noexcept
{
    // Create and initialize the singleton watchdog timer instance (once only).
    static Watchdog myInstance{};

    // Return a reference to the singleton watchdog instance, cast to the corresponding interface.
    return myInstance; 
}

// -----------------------------------------------------------------------------
bool Watchdog::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Watchdog::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Watchdog::setEnabled(const bool enable) noexcept
{
    // Reset the watchdog to prevent a timeout during the enablement update.
    reset();

    // Update the enablement status, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    utils::set(WDTCSR, WDCE, WDE);
    if (enable) { utils::set(WDTCSR, WDE); }
    else { utils::clear(WDTCSR, WDE); }

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
uint16_t Watchdog::timeoutMs() const noexcept { return static_cast<uint16_t>(myTimeout); }

// -----------------------------------------------------------------------------
void Watchdog::reset() noexcept 
{ 
    // Disable interrupts during the reset process.
    utils::globalInterruptDisable();

    // Reset the watchdog and clear the corresponding reset flag.
    asm("WDR");
    utils::clear(MCUSR, WDRF);

    // Re-enable interrupts once the reset process is complete.
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
bool Watchdog::setTimeout(const Timeout timeout) noexcept
{
    // Return false if the timeout is invalid.
    if (!isTimeoutValid(timeout)) { return false; }

    // Calculate the timeout value before the timed write sequence.
    const auto value{timeoutValue(timeout)};

    // Update the watchdog timeout, disable interrupts during the write sequence.
    utils::globalInterruptDisable();
    WDTCSR |= (1UL << WDCE) | (1UL << WDE);
    WDTCSR = value;
    utils::globalInterruptEnable();

    // Re-enable interrupts once the write sequence is complete.
    utils::globalInterruptEnable();

    // Store the new timeout and return true to indicate success.
    myTimeout = timeout;
    return true;
} 

// -----------------------------------------------------------------------------
Watchdog::Watchdog() noexcept
    : myTimeout{}
    , myEnabled{false}
{
    // Set the default timeout.
    setTimeout(WatchdogParam::DefaultTimeout);
}
} // namespace atmega328p
} // namespace driver
