/**
 * @brief Implementation details of ATmega328P watchdog driver.
 */
#include "utils.h"
#include "watchdog.h"

namespace driver 
{
namespace atmega328p
{
namespace 
{

/**
 * @param Structure holding ATmega328P Watchdog timer parameters.
 */
struct WatchdogParam
{
    static bool circuitReserved; // Indicate whether the only available circuit is reserved.
};

bool WatchdogParam::circuitReserved{false};

// -----------------------------------------------------------------------------
inline bool isCircuitReserved() noexcept { return WatchdogParam::circuitReserved; }

// -----------------------------------------------------------------------------
constexpr bool isTimeoutValid(const Watchdog::Timeout timeout) noexcept
{
    return timeout != Watchdog::Timeout::Invalid;
}
    
// -----------------------------------------------------------------------------
constexpr uint8_t timeoutValue(const Watchdog::Timeout timeout) noexcept
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
            return 0xFF;
    }
}

// -----------------------------------------------------------------------------
void resetWatchdogInHardware() noexcept { asm("WDR"); }

// -----------------------------------------------------------------------------
void clearWatchdogResetFlag() noexcept { utils::clear(MCUSR, WDRF); }

} // namespace

// -----------------------------------------------------------------------------
Watchdog::Watchdog(const Timeout timeout, const bool enable) noexcept
    : myTimeout{timeout}
    , myInitialized{init(myTimeout)}
    , myEnabled{false}
{
    if (myInitialized && enable) { setEnabled(enable); }
}

// -----------------------------------------------------------------------------
Watchdog::~Watchdog() noexcept { disable(); }

// -----------------------------------------------------------------------------
Watchdog::Watchdog(Watchdog&& other) noexcept
    : myTimeout{other.myTimeout}
    , myInitialized{other.myInitialized}
    , myEnabled{other.myEnabled}
{
    other.myTimeout     = Timeout::Invalid;
    other.myInitialized = false;
    other.myEnabled     = false;
}

// -----------------------------------------------------------------------------
Watchdog& Watchdog::operator=(Watchdog&& other) noexcept
{
    if (&other != this)
    {
        disable();
        myTimeout     = other.myTimeout;
        myEnabled     = other.myEnabled;
        myInitialized = other.myInitialized;

        other.myTimeout     = Timeout::Invalid;
        other.myEnabled     = false;
        other.myInitialized = false;
    }
    return *this;
}

// -----------------------------------------------------------------------------
bool Watchdog::isInitialized() const noexcept { return myInitialized; }

// -----------------------------------------------------------------------------
bool Watchdog::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
uint16_t Watchdog::timeoutMs() const noexcept { return static_cast<uint16_t>(myTimeout); }

// -----------------------------------------------------------------------------
void Watchdog::setEnabled(const bool enable) noexcept
{
    if (!myInitialized) { return; }
    reset();
    utils::globalInterruptDisable();
    utils::set(WDTCSR, WDCE, WDE);
    if (enable) { utils::set(WDTCSR, WDE); }
    else { utils::clear(WDTCSR, WDE); }
    utils::globalInterruptDisable();
}

// -----------------------------------------------------------------------------
void Watchdog::reset() noexcept 
{ 
    if (!myInitialized) { return; }
    utils::globalInterruptDisable();
    resetWatchdogInHardware();
    clearWatchdogResetFlag();
    utils::globalInterruptEnable();
}

// -----------------------------------------------------------------------------
bool Watchdog::init(const Timeout timeoutMs) noexcept
{
    if (isCircuitReserved() || !isTimeoutValid(timeoutMs)) { return false; }
    utils::globalInterruptDisable();
    utils::set(WDTCSR, WDCE, WDE);
    WDTCSR = timeoutValue(timeoutMs);
    utils::globalInterruptEnable();
    return true;
} 

// -----------------------------------------------------------------------------
void Watchdog::disable() noexcept
{
    if (myInitialized)
    {
        setEnabled(false);
        WatchdogParam::circuitReserved = false;
    }
}

} // namespace atmega328p
} // namespace driver
