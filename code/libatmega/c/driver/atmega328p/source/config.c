/**
 * @brief Configuration implementation details for ATmega328P.
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include "driver/atmega328p/config.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** Default CPU frequency in Hz. */
#define CPU_FREQUENCY_DEFAULT_HZ 8000000UL

/** Minimum allowed CPU frequency in Hz. */
#define CPU_FREQUENCY_MIN_HZ 8000000UL

/** Maximum allowed CPU frequency in Hz. */
#define CPU_FREQUENCY_MAX_HZ 20000000UL

/** Default UART baud rate in bps (bits per seconds). */
#define UART_BAUD_DEFAULT_BPS 9600UL

/** Minimum allowed UART baud rate in bps (bits per seconds). */
#define UART_BAUD_MIN_BPS 4800UL

/** Maximum allowed UART baud rate in bps (bits per seconds). */
#define UART_BAUD_MAX_BPS 115220UL

/** Default baud rate configuration value (corresponds to 9600 bps). */
#define UART_BAUD_REG_DEFAULT (uint16_t)(103U)

/*******************************************************************************
 * Local type definitions.
 ******************************************************************************/
/**
 * @brief Configuration implementation structure for ATmega328P.
 */
typedef struct Config 
{
    /** Configuration interface implementation. */
    ConfigInterface itf;
    
    /** CPU frequency in Hz. */
    uint32_t cpuFrequencyHz;

    /** UART baud rate in bps. */
    uint32_t uartBaudRateBps;
} Config;

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const ConfigVtable* const configVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton configuration instance. */
static Config myInstance = 
{
    .itf             = {NULL},
    .cpuFrequencyHz  = CPU_FREQUENCY_DEFAULT_HZ,
    .uartBaudRateBps = UART_BAUD_DEFAULT_BPS,
};

/*******************************************************************************
 * Configuration-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static uint16_t config_calculateUartBaudRegValue(const uint32_t baudRateBps)
{
    // Calculate the 16-bit value to write to UBRR0 to set given baud rate.
    return (uint16_t)((myInstance.cpuFrequencyHz / (16U * baudRateBps)) - 1U);
}

// -----------------------------------------------------------------------------
static void config_init(void)
{
    // Only initialize the implementation once.
    if (!myInstance.itf.vptr)
    {
        // Set the vtable pointer of the singleton instance.
        myInstance.itf.vptr = configVtable_ptr();

        // Set the UART baud rate register to the default baud rate.
        UBRR0 = UART_BAUD_REG_DEFAULT;
    }
}

// -----------------------------------------------------------------------------
static inline bool config_isCpuFrequencyValid(const uint32_t frequencyHz)
{
    return (CPU_FREQUENCY_MIN_HZ <= frequencyHz) && (CPU_FREQUENCY_MAX_HZ >= frequencyHz);
}

// -----------------------------------------------------------------------------
static inline bool config_isUartBaudRateValid(const uint32_t baudRateBps)
{
    return (UART_BAUD_MIN_BPS <= baudRateBps) && (UART_BAUD_MAX_BPS >= baudRateBps);
}

// -----------------------------------------------------------------------------
static uint32_t config_cpuFrequencyHz(const ConfigInterface* const self)
{
    // Ignore the self argument; access the singleton instance directly.
    (void) (self);

    // Return the configured CPU frequency.
    return myInstance.cpuFrequencyHz;
}

// -----------------------------------------------------------------------------
static bool config_setCpuFrequencyHz(ConfigInterface* const self, const uint32_t frequencyHz)
{
    // Ignore the self argument; access the singleton instance directly.
    (void) (self);

    // Return false if the given CPU frequency is invalid.
    if (!config_isCpuFrequencyValid(frequencyHz)) { return false; }

    // Set the CPU frequency to given value.
    myInstance.cpuFrequencyHz = frequencyHz;

    // Return true to indicate that the CPU frequency has been set.
    return true;
}


// -----------------------------------------------------------------------------
static uint32_t config_uartBaudRateBps(const ConfigInterface* const self)
{
    // Ignore the self argument; access the singleton instance directly.
    (void) (self);

    // Return the configured UART baud rate.
    return myInstance.uartBaudRateBps;
}

// -----------------------------------------------------------------------------
static bool config_setUartBaudRateBps(ConfigInterface* const self, const uint32_t baudRateBps)
{
    // Ignore the self argument; access the singleton instance directly.
    (void) (self);

    // Return false if the given baud rate is invalid.
    if (!config_isUartBaudRateValid(baudRateBps)) { return false; }

    // Change the baud rate only if the requested value is different from the current one.
    if (baudRateBps != myInstance.uartBaudRateBps)
    {
        // Set the UART baud rate to given value.
        myInstance.uartBaudRateBps = baudRateBps;

        // Update the UART baud rate register accordingly.
        UBRR0 = config_calculateUartBaudRegValue(myInstance.uartBaudRateBps);
    }
    // Return true to indicate that the UART baud rate has been set.
    return true;
}

// -----------------------------------------------------------------------------
static inline void config_delete(ConfigInterface** const self)
{
    // Deletion is not valid in this implementation, since the singleton instance is static.
    // This function exists only to satisfy the interface.
    (void) (self);
}

/*******************************************************************************
 * Configuration vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const ConfigVtable* const configVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden configuration functions.
    static const ConfigVtable myVtable =
    {
        .cpuFrequencyHz     = config_cpuFrequencyHz,
        .setCpuFrequencyHz  = config_setCpuFrequencyHz,
        .uartBaudRateBps    = config_uartBaudRateBps,
        .setUartBaudRateBps = config_setUartBaudRateBps,
        .delete             = config_delete,
    };
    // Return a pointer to the vtable for access through the configuration interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) configuration interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
ConfigInterface* atmega328p_config_getInstance(void) 
{ 
    // Initialize default configuration (only done once).
    config_init();

    // Return a pointer to the singleton instance, cast to the corresponding interface.
    return (ConfigInterface*)(&myInstance); 
}
