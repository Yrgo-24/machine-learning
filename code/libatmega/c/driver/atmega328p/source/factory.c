/**
 * @brief Factory implementation details for ATmega328P.
 */
#include "driver/atmega328p/adc.h"
#include "driver/atmega328p/config.h"
#include "driver/atmega328p/eeprom.h"
#include "driver/atmega328p/factory.h"
#include "driver/atmega328p/gpio.h"
#include "driver/atmega328p/serial.h"
#include "driver/atmega328p/timer.h"
#include "driver/atmega328p/watchdog.h"

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton factory instance. */
static FactoryInterface myInstance = {NULL};

/*******************************************************************************
 * Factory vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const FactoryVtable* const factoryVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden factory functions.
    static const FactoryVtable myVtable =
    {
        .adc_new      = atmega328p_adc_getInstance,
        .config_new   = atmega328p_config_getInstance,
        .eeprom_new   = atmega328p_eeprom_getInstance,
        .gpio_new     = atmega328p_gpio_new,
        .serial_new   = atmega328p_serial_getInstance,
        .timer_new    = atmega328p_timer_new,
        .watchdog_new = atmega328p_watchdog_getInstance,
    };
    // Return a pointer to the vtable for access through the factory interface.
    return &myVtable;
}

/*******************************************************************************
 * Factory-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline void factory_init(void)
{
    // Initialize the factory once.
    if (!myInstance.vptr) { myInstance.vptr = factoryVtable_ptr(); }
}

/*******************************************************************************
 * External (public) factory interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
FactoryInterface* atmega328p_factory_getInstance(void)
{
    // Initialize the factory (only done once).
    factory_init();

    // Return a pointer to the singleton factory instance.
    return &myInstance;
}
