/**
 * @brief Watchdog timer implementation details for ATmega328P.
 */
#include <avr/io.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "driver/atmega328p/watchdog.h"

/*******************************************************************************
 * Local macros.
 ******************************************************************************/
/** Default watchdog timeout. */
#define WATCHDOG_TIMEOUT_DEFAULT ATMEGA328P_WATCHDOG_TIMEOUT_1024_MS

/** Indicate invalid timeout. */
#define WATCHDOG_TIMEOUT_INVALID 0xFFU

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const WatchdogVtable* const watchdogVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton watchdog instance. */
static WatchdogInterface myInstance = {NULL};

/** Watchdog timeout in ms. */
static uint16_t myTimeout_ms = 0U;

/** Indicate whether the watchdog is enabled. */
static bool myEnabled = false;

/*******************************************************************************
 * Watchdog-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static uint8_t watchdog_timeoutValue(const atmega328p_WatchdogTimeout timeout)
{
    // Return the value that corresponds to the given timeout.
    switch (timeout)
    {
        case ATMEGA328P_WATCHDOG_TIMEOUT_16_MS:
            return 0U;
        case ATMEGA328P_WATCHDOG_TIMEOUT_32_MS:
            return (1U << WDP0);
        case ATMEGA328P_WATCHDOG_TIMEOUT_64_MS:
            return (1U << WDP1);
        case ATMEGA328P_WATCHDOG_TIMEOUT_128_MS:
            return (1U << WDP1) | (1U << WDP0);
        case ATMEGA328P_WATCHDOG_TIMEOUT_256_MS:
            return (1U << WDP2);
        case ATMEGA328P_WATCHDOG_TIMEOUT_512_MS:
            return (1U << WDP2) | (1U << WDP0);
        case ATMEGA328P_WATCHDOG_TIMEOUT_1024_MS:
            return (1U << WDP2) | (1U << WDP1);
        case ATMEGA328P_WATCHDOG_TIMEOUT_2048_MS:
            return (1U << WDP2) | (1U << WDP1) | (1U << WDP0);
        case ATMEGA328P_WATCHDOG_TIMEOUT_4096_MS:
            return (1U << WDP3);
        case ATMEGA328P_WATCHDOG_TIMEOUT_8192_MS:
            return (1U << WDP3) | (1U << WDP0);
        default:
            return WATCHDOG_TIMEOUT_INVALID;
    }
}

// -----------------------------------------------------------------------------
static void watchdog_reset(WatchdogInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Disable interrupts during the reset process.
    asm("CLI");

    // Reset the watchdog and clear the corresponding reset flag.
    asm("WDR");
    MCUSR &= ~(1UL << WDRF);

    // Re-enable interrupts once the reset process is complete.
    asm("SEI");
}

// -----------------------------------------------------------------------------
static bool watchdog_isEnabled(const WatchdogInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Return true if the watchdog is enabled, false otherwise.
    return myEnabled;
}

// -----------------------------------------------------------------------------
static void watchdog_setEnabled(WatchdogInterface* const self, const bool enable)
{
    // Reset the watchdog to prevent a timeout during the enablement update.
    watchdog_reset(self);

    // Update the enablement status, disable interrupts during the write sequence.
    asm("CLI");
    WDTCSR = ((1UL << WDCE) | (1UL << WDE));
    if (enable) { WDTCSR |= (1UL << WDE); }
    else { WDTCSR &= ~(1UL << WDE); }

    // Re-enable interrupts once the write sequence is complete.
    asm("SEI");

    // Update the enablement status.
    myEnabled = enable;
}

// -----------------------------------------------------------------------------
static uint16_t watchdog_timeout_ms(const WatchdogInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Return the watchdog timeout in ms.
    return myTimeout_ms;
}

// -----------------------------------------------------------------------------
static bool watchdog_setTimeout_ms(WatchdogInterface* const self, const uint16_t timeout_ms)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Calculate the timeout value before the timed write sequence.
    const uint8_t value = watchdog_timeoutValue(timeout_ms);

    // Return false if the timeout is invalid.
    if (WATCHDOG_TIMEOUT_INVALID == value) { return false; }

    // Update the watchdog timeout, disable interrupts during the write sequence.
    asm("CLI");
    WDTCSR |= (1U << WDCE) | (1U << WDE);
    WDTCSR = value;

    // Re-enable interrupts once the write sequence is complete.
    asm("SEI");

    // Store the new timeout and return true to indicate success.
    myTimeout_ms = timeout_ms;
    return true;
}

// -----------------------------------------------------------------------------
static void watchdog_delete(WatchdogInterface** const self)
{
    // Deletion is not valid in this implementation, since the singleton instance is static.
    // This function exists only to satisfy the interface.
    (void) (self);
}

// -----------------------------------------------------------------------------
static void watchdog_init(void)
{
    // Only initialize the implementation once.
    if (!myInstance.vptr)
    {
        // Set the vtable pointer of the singleton instance.
        myInstance.vptr = watchdogVtable_ptr();

        // Set the default timeout
        watchdog_setTimeout_ms(&myInstance, WATCHDOG_TIMEOUT_DEFAULT);
    }
}

/*******************************************************************************
 * Watchdog vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const WatchdogVtable* const watchdogVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden watchdog functions.
    static const WatchdogVtable myVtable =
    {
        .isEnabled     = watchdog_isEnabled,
        .setEnabled    = watchdog_setEnabled,
        .timeout_ms    = watchdog_timeout_ms,
        .setTimeout_ms = watchdog_setTimeout_ms,
        .reset         = watchdog_reset, 
        .delete        = watchdog_delete,
    };
    // Return a pointer to the vtable for access through the watchdog interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) watchdog interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
WatchdogInterface* atmega328p_watchdog_getInstance()
{
    // Initialize the watchdog transmission (only done once).
    watchdog_init();

    // Return a pointer to the singleton watchdog instance.
    return &myInstance; 
}

// -----------------------------------------------------------------------------
bool atmega328p_watchdog_setTimeout_ms(const uint16_t timeout_ms)
{
    return watchdog_setTimeout_ms(&myInstance, timeout_ms);
}