/**
 * @brief Logic implementation details for the system, revision 1.
 */
#include <stdlib.h>

#include "driver/factory/interface.h"
#include "logic/rev1.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** CPU frequency in HZ. */
#define CPU_FREQUENCY_HZ 16000000UL

/** Digital pin connected to led1. */
#define LED1_PIN 8U

/** Digital pin connected to led2. */
#define LED2_PIN 9U

/** Digital pin connected to button1. */
#define BUTTON1_PIN 12U

/** Digital pin connected to button2. */
#define BUTTON2_PIN 13U

/** Debounce duration in milliseconds. */
#define DEBOUNCE_DURATION_MS 300U

/** Timer 1 duration in milliseconds. */
#define TIMER1_DURATION_MS 100U

/** Timer 2 duration in milliseconds. */
#define TIMER2_DURATION_MS 50U

/*******************************************************************************
 * Local type definitions.
 ******************************************************************************/
/**
 * @brief Concrete logic implementation structure for revision 1.
 */
typedef struct Logic
{
    /** Logic interface implementation. */
    LogicInterface itf;

    /** ADC implementation (currently unused). */
    AdcInterface* adc;

    /** Configuration implementation. */
    ConfigInterface* config;

    /** EEPROM stream. */
    EepromInterface* eeprom;

    /** GPIO devices. */
    GpioInterface *led1, *led2, *button1, *button2;

    /** Serial device (UART). */
    SerialInterface* serial;

    /** Hardware timers. */
    TimerInterface *debounceTimer, *timer1, *timer2;

    /** Watchdog timer. */
    WatchdogInterface* watchdog;
} Logic;

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const LogicVtable* const logicVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton instance of the revision 1 logic. */
static Logic myInstance = {
    .itf           = {NULL}, 
    .adc           = NULL,
    .config        = NULL, 
    .eeprom        = NULL,
    .led1          = NULL,
    .led2          = NULL,
    .button1       = NULL,
    .button2       = NULL,
    .serial        = NULL,
    .debounceTimer = NULL,
    .timer1        = NULL,
    .timer2        = NULL, 
    .watchdog      = NULL,
};

/** Pointer to the revision 1 logic instance. */
static Logic* const impl = &myInstance;

/*******************************************************************************
 * Internal helper functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static void logTimerState(const TimerInterface* const timer)
{
    // Get timer ID.
    const uint8_t id = timer->vptr->id(timer);

    // Create enablement string.
    const char *enableStr = timer->vptr->isEnabled(timer) ? "enabled" : "disabled";

    // Log the timer state.
    impl->serial->vptr->printf(impl->serial, "Timer with ID %u is now %s!\n", id, enableStr);
}

// -----------------------------------------------------------------------------
bool readTimerStateFromEeprom(TimerInterface* const timer)
{
    // Get the address from which to read.
    const uint8_t address = timer->vptr->id(timer);
    uint8_t state = 0U;

    // Perform read, return true if the stored value is 1.
    const bool result = impl->eeprom->vptr->read(impl->eeprom, address, &state, sizeof(state));
    return result ? (bool)(state) : false;
}

// -----------------------------------------------------------------------------
void writeTimerStateToEeprom(TimerInterface* const timer)
{ 
    // Get the timer state and the destination address.
    const uint8_t state   = (uint8_t)(timer->vptr->isEnabled(timer));
    const uint8_t address = timer->vptr->id(timer);

    // Write the timer data to EEPROM.
    (void) impl->eeprom->vptr->write(impl->eeprom, address, &state, sizeof(state));
}

// -----------------------------------------------------------------------------
void checkTimerStateInEeprom(TimerInterface* const timer)
{
    // Start the timer if it was enabled before the last power-off.
    if (readTimerStateFromEeprom(timer))
    {
        timer->vptr->start(timer);
        logTimerState(timer);
    }
}

// -----------------------------------------------------------------------------
void restoreTimerStates(void)
{
    impl->serial->vptr->printf(impl->serial, 
        "Reading EEPROM to restore the timer states to their last state before power-off!\n");
    checkTimerStateInEeprom(impl->timer1);
    checkTimerStateInEeprom(impl->timer2);
}

// -----------------------------------------------------------------------------
static void handlePressEvent(GpioInterface* const led, GpioInterface* const button, 
    TimerInterface* const timer)
{
    // Handle the interrupt if the button is pressed.
    if (button->vptr->read(button))
    {
        // Toggle the timer.
        timer->vptr->toggle(timer);

        // Immediately disable the LED if the timer is disabled.
        if (!timer->vptr->isEnabled(timer)) { led->vptr->write(led, false); }

        // Store the timer state in EEPROM.
        writeTimerStateToEeprom(timer);

        // Log the new timer state.
        logTimerState(timer);
    }
}

/*******************************************************************************
 * Internal handler functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static void buttonHandler(void)
{
    // Temporarily disable button interrupts to mitigate effects of debouncing.
    impl->button1->vptr->enableInterruptsOnPort(impl->button1, false);

    // Start the debounce timer to re-enable button interrupts after 300 ms.
    impl->debounceTimer->vptr->start(impl->debounceTimer);

    // Handle button1 press event.
    handlePressEvent(impl->led1, impl->button1, impl->timer1);

   // Handle button2 press event.
    handlePressEvent(impl->led2, impl->button2, impl->timer2);
}

// -----------------------------------------------------------------------------
static void debounceTimerHandler(void)
{
    // Enable button interrupts once the debounce timer elapses.
    impl->button1->vptr->enableInterruptsOnPort(impl->button1, true);
}

// -----------------------------------------------------------------------------
static void timer1Handler(void)
{
    // Toggle led1 whenever timer1 elapses.
    impl->led1->vptr->toggle(impl->led1);
}

// -----------------------------------------------------------------------------
static void timer2Handler(void)
{
    // Toggle led2 whenever timer2 elapses.
    impl->led2->vptr->toggle(impl->led2);
}

/*******************************************************************************
 * Logic-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static void logic_delete(void)
{
    // Set the vtable pointer to NULL.
    impl->itf.vptr = NULL;

    // Delete the ADC.
    impl->adc->vptr->delete(&(impl->adc));

    // Delete the configuration.
    impl->config->vptr->delete(&(impl->config));

    // Delete the EEPROM stream.
    impl->eeprom->vptr->delete(&(impl->eeprom));

    // Delete the GPIO devices.
    impl->led1->vptr->delete(&(impl->led1));
    impl->led2->vptr->delete(&(impl->led2));
    impl->button1->vptr->delete(&(impl->button1));
    impl->button2->vptr->delete(&(impl->button2));

    // Delete the serial instance.
    impl->serial->vptr->delete(&(impl->serial));

    // Delete the timers.
    impl->debounceTimer->vptr->delete(&(impl->debounceTimer));
    impl->timer1->vptr->delete(&(impl->timer1));
    impl->timer2->vptr->delete(&(impl->timer2));

    // Delete the watchdog.
    impl->watchdog->vptr->delete(&(impl->watchdog));
}

// -----------------------------------------------------------------------------
static bool logic_initAdc(FactoryInterface* const factory)
{
     // Initialize the A/D converter.
    impl->adc = factory->vptr->adc_new();

    // Return true if the ADC initialization succeeded.
    return impl->adc;
}

// -----------------------------------------------------------------------------
static bool logic_initConfig(FactoryInterface* const factory)
{
     // Initialize the configuration.
    impl->config = factory->vptr->config_new();

    // Set the CPU frequency if initialization succeeded.
    if (impl->config) { impl->config->vptr->setCpuFrequencyHz(impl->config, CPU_FREQUENCY_HZ); }

    // Return true if the configuration initialization succeeded.
    return impl->config;
}

// -----------------------------------------------------------------------------
static bool logic_initEeprom(FactoryInterface* const factory)
{
    // Initialize the EEPROM stream.
    impl->eeprom = factory->vptr->eeprom_new();

    // Return true if the EEPROM stream initialization succeeded.
    return impl->eeprom;
}

// -----------------------------------------------------------------------------
static bool logic_initGpio(FactoryInterface* const factory)
{
    // Initialize the GPIO instances.
    impl->led1    = factory->vptr->gpio_new(LED1_PIN, GpioMode_Output, NULL);
    impl->led2    = factory->vptr->gpio_new(LED2_PIN, GpioMode_Output, NULL);
    impl->button1 = factory->vptr->gpio_new(BUTTON1_PIN, GpioMode_Input, buttonHandler);
    impl->button2 = factory->vptr->gpio_new(BUTTON2_PIN, GpioMode_Input, NULL);

    // Check if GPIO initialization succeeded..
    const bool result = 
        impl->led1 && impl->led2 && impl->button1 && impl->button2;

    // Enable button interrupts if the GPIO initialization succeeded.
    if (result)
    {
        impl->button1->vptr->enableInterrupt(impl->button1, true);
        impl->button2->vptr->enableInterrupt(impl->button2, true);
    }
    // Return true if the GPIO initialization succeeded.
    return result;
}

// -----------------------------------------------------------------------------
static bool logic_initSerial(FactoryInterface* const factory)
{
    // Initialize the serial device.
    impl->serial = factory->vptr->serial_new();

    // Return true if the serial initialization succeeded.
    return impl->serial;
}

// -----------------------------------------------------------------------------
static bool logic_initTimers(FactoryInterface* const factory)
{
     // Initialize the hardware timers.
    impl->debounceTimer = factory->vptr->timer_new(
        DEBOUNCE_DURATION_MS, TimerMode_OneShot, debounceTimerHandler);
    impl->timer1 = factory->vptr->timer_new(TIMER1_DURATION_MS, TimerMode_Periodic, timer1Handler);
    impl->timer2 = factory->vptr->timer_new(TIMER2_DURATION_MS, TimerMode_Periodic, timer2Handler);

    // Return true if the timer initialization succeeded.
    return impl->debounceTimer && impl->timer1 && impl->timer2;
}

// -----------------------------------------------------------------------------
static bool logic_initWatchdog(FactoryInterface* const factory)
{
    // Initialize the watchdog.
    impl->watchdog = factory->vptr->watchdog_new();

    // Enable the watchdog if the initialization succeeded.
    if (impl->watchdog) { impl->watchdog->vptr->setEnabled(impl->watchdog, true); }
    
    // Return true if the serial initialization succeeded.
    return impl->watchdog;
}

// -----------------------------------------------------------------------------
static bool logic_init(FactoryInterface* const factory)
{
    // Delete previous implementation if it exists.
    if (impl->itf.vptr) { logic_delete(); }

    // Initialize the vtable pointer to give the interface access to the overridden functions.
    impl->itf.vptr = logicVtable_ptr();

    // Initialize the ADC, return false if initialization failed.
    if (!logic_initAdc(factory)) { return false; }

    // Initialize the configuration, return false if initialization failed.
    if (!logic_initConfig(factory)) { return false; }

    // Initialize the EEPROM stream, return false if initialization failed.
    if (!logic_initEeprom(factory)) { return false; }

    // Initialize the GPIO instances, return false if initialization failed.
    if (!logic_initGpio(factory)) { return false; }

    // Initialize the serial device, return false if initialization failed.
    if (!logic_initSerial(factory)) { return false; }

    // Initialize the hardware timers, return false if initialization failed.
    if (!logic_initTimers(factory)) { return false; }

    // Initialize the watchdog timer, return false if initialization failed.
    if (!logic_initWatchdog(factory)) { return false; }

    // Log successful system initialization and logic revision.
    impl->serial->vptr->printf(
            impl->serial, "System initialized with logic revision %d!\n", LogicRevision_Rev1);

    // Restore timer states by reading data stored in EEPROM.
    restoreTimerStates();

    // Return true to indicate successful initialization.
    return true;
}

/*******************************************************************************
 * Overridden logic interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static LogicRevision logic_rev1_revision(const LogicInterface* const self)
{
    // Return the logic revision as an enumerator.
    return LogicRevision_Rev1;
}

// -----------------------------------------------------------------------------
static void logic_rev1_run(LogicInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Keep the system running continuously.
    while (1) 
    {
        impl->watchdog->vptr->reset(impl->watchdog);
    }
}

// -----------------------------------------------------------------------------
static void logic_rev1_delete(LogicInterface** const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Delete the concrete implementation, free allocated resources.
    logic_delete();
}

/*******************************************************************************
 * Logic vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const LogicVtable* const logicVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden logic functions.
    static const LogicVtable myVtable =
    {
        .revision = logic_rev1_revision,
        .run      = logic_rev1_run,
        .delete   = logic_rev1_delete,
    };
    // Return a pointer to the vtable for access through the logic interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) logic interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
LogicInterface *logic_rev1_new(FactoryInterface* const factory)
{
    // Return nullptr is the factory is invalid.
    if (!factory) { return NULL; }

    // Initialize the implementation, delete allocated memory on failure.
    if (!logic_init(factory)) { logic_delete(); }
    
    // Return a pointer to the logic interface, or nullptr if the initialization failed.
    return &(impl->itf);
}