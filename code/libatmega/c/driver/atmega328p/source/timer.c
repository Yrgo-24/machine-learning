/**
 * @brief Timer implementation details for ATmega328P.
 */
#include <stdbool.h>
#include <stdlib.h>

#include <avr/io.h>
#include <avr/interrupt.h>

#include "driver/atmega328p/config.h"
#include "driver/atmega328p/timer.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** Min timer duration (1 ms). */
#define TIMER_DURATION_MIN_MS 1UL

/** Max timer duration (four weeks). */
#define TIMER_DURATION_MAX_MS 2419200000UL

/** Max count value for the timers; reaching this value will generate an interrupt. */
#define TIMER_MAX_COUNT (uint16_t)(256)

/** Timer prescaler / clock divider. */
#define TIMER_PRESCALER 8U

/** Configuration bits for timer 0, setting interrupt frequency and normal mode. */
#define TIMER0_CONFIG_BITS (uint8_t)(1UL << CS01)

/** Configuration bits for timer 1, setting interrupt frequency and CTC mode. */
#define TIMER1_CONFIG_BITS (uint8_t)((1UL << CS11) | (1UL << WGM12))

/** Configuration bits for timer 2, setting interrupt frequency and normal mode. */
#define TIMER2_CONFIG_BITS (uint8_t)(1UL << CS21)

/*******************************************************************************
 * Forward declarations.
 ******************************************************************************/
/** Hardware structure. */
typedef struct TimerHardware TimerHardware;

/*******************************************************************************
 * Local type definitions.
 ******************************************************************************/
/**
 * @brief Enumeration representing available timer circuits.
 */
typedef enum TimerCircuit 
{
    TimerCircuit_0,     /** Timer circuit 0.  */
    TimerCircuit_1,     /** Timer circuit 1.  */
    TimerCircuit_2,     /** Timer circuit 2.  */
    TimerCircuit_Count, /** The number of available timer circuits. */
} TimerCircuit;

/**
 * @brief Timer implementation structure for ATmega328P.
 */
typedef struct Timer 
{
    /** Timer interface implementation. */
    TimerInterface itf;

    /** Hardware structure for read/write access. */
    TimerHardware* hw;

    /** Pointer to associated callback. */
    void (*callback)(void);

    /** Max count value (corresponds to the timer duration). */
    uint32_t maxCount;

    /** Timer counter. */
    volatile uint32_t counter;

    /** Timer mode. */
    TimerMode mode;
} Timer;

/**
 * @brief Timer hardware structure for read/write access.
 */
typedef struct TimerHardware 
{
    /** Mask register. */
    volatile uint8_t* mskReg;

    /** Mask bit. */
    uint8_t mskBit;

    /** Timer circuit. */
    TimerCircuit circuit;
} TimerHardware;

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const TimerVtable* const timerVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Timer registry (NULL = free slot). */
static Timer* myTimerRegistry[TimerCircuit_Count] = {NULL, NULL, NULL};

/*******************************************************************************
 * Timer registry functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool timerRegistry_isFree(const TimerCircuit circuit)
{
    // Return true if the timer circuit is free.
    return NULL == myTimerRegistry[circuit];
}

// -----------------------------------------------------------------------------
static inline void timerRegistry_reserve(Timer* timer) 
{ 
    // Reserve the circuit by assigning storing the address of the timer in the timer registry.
    myTimerRegistry[timer->hw->circuit] = timer;
}

// -----------------------------------------------------------------------------
static inline void timerRegistry_free(const Timer* timer) 
{ 
    // Free the circuit by assigning a nullptr to the associated slot in the timer registry.
    myTimerRegistry[timer->hw->circuit] = NULL;
}

/*******************************************************************************
 * Timer mode-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool timerMode_isValid(const TimerMode mode)
{ 
    // Return true if the specified mode is of any of the supported types.
    return TimerMode_Count > mode;
}

/*******************************************************************************
 * Timer circuit-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool timerCircuit_isValid(const TimerCircuit circuit) 
{ 
    // Return true if the given circuit is a valid enum.
    return TimerCircuit_Count > circuit;
}

// -----------------------------------------------------------------------------
static TimerCircuit timerCircuit_nextFree(void)
{
    // Check status for each timer in the timer registry.
    for (TimerCircuit circuit = TimerCircuit_0; circuit < TimerCircuit_Count; ++circuit)
    {
        // Return circuit enum if it is free.
        if (timerRegistry_isFree(circuit)) { return circuit; }
    }
    // Return an invalid circuit enum if no timer is available.
    return TimerCircuit_Count;
}

// -----------------------------------------------------------------------------
static bool timerCircuit_isAvailable(void)
{
    // Return true if a timer circuit is available.
    return timerCircuit_isValid(timerCircuit_nextFree());
}

/*******************************************************************************
 * Hardware-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static bool timerHardware_isEnabled(const TimerHardware* const self)
{
    // Return true if the associated mask bit is set (timer is enabled)
    return *(self->mskReg) & (1UL << self->mskBit);
}

// -----------------------------------------------------------------------------
static void timerHardware_enable(TimerHardware* const self, const bool enable)
{
    // Check if the timer hardware is to be enabled or not.
    if (enable) 
    { 
        // Set the associated mask bit to enable interrupts if specified.
        *(self->mskReg) = (1UL << self->mskBit); 

        // Enable interrupts globally.
        asm("SEI");
    }
    // Clear the entire mask register to disable interrupts if specified.
    else { *(self->mskReg) = 0x00U; }
}

// -----------------------------------------------------------------------------
static void timerHardware_initCircuit0(TimerHardware* const self)
{
    // Initialize the hardware structure.
    self->mskReg  = &TIMSK0;
    self->mskBit  = TOIE0;
    self->circuit = TimerCircuit_0;

    // Set the interrupt period to 0.128 ms in normal mode.
    TCCR0B = TIMER0_CONFIG_BITS;
}

// -----------------------------------------------------------------------------
static void timerHardware_initCircuit1(TimerHardware* const self)
{
    // Initialize the hardware structure.
    self->mskReg  = &TIMSK1;
    self->mskBit  = OCIE1A;
    self->circuit = TimerCircuit_1;

    // Set the compare value to 256 for 8-bit operation in CTC mode (Clear Timer on Compare Match).
    OCR1A = TIMER_MAX_COUNT;

    // Configure Timer 1 for CTC mode with a 0.128 ms interrupt period; 
    // interrupt triggers the counter reaches 256.
    TCCR1B = TIMER1_CONFIG_BITS;
}

// -----------------------------------------------------------------------------
static void timerHardware_initCircuit2(TimerHardware* const self)
{
    // Initialize the hardware structure.
    self->mskReg  = &TIMSK2;
    self->mskBit  = TOIE2;
    self->circuit = TimerCircuit_2;

    // Set the interrupt period to 0.128 ms in normal mode.
    TCCR2B = TIMER2_CONFIG_BITS;
}

// -----------------------------------------------------------------------------
static void timerHardware_init(TimerHardware* const self, const TimerCircuit circuit)
{
    // Initialize the given circuit.
    switch (circuit)
    {
        case TimerCircuit_0:
        {
            timerHardware_initCircuit0(self);
            break;
        }
        case TimerCircuit_1:
        {
            timerHardware_initCircuit1(self);
            break;
        }
        case TimerCircuit_2:
        {
            timerHardware_initCircuit2(self);
            break;
        }
        default:
            return;
    }
    // Ensure that the timer isn't running inadvertently.
    timerHardware_enable(self, false);
}

// -----------------------------------------------------------------------------
static TimerHardware* timerHardware_new(void)
{
    // Get the next free circuit.
    TimerCircuit circuit = timerCircuit_nextFree();

    // Return a nullptr if no circuit is available.
    if (!timerCircuit_isValid(circuit)) { return NULL; }

    // Allocate memory for the hardware structure.
    TimerHardware* const self = (TimerHardware* )(malloc(sizeof(TimerHardware)));

    // Return a nullptr if memory couldn't be allocated.
    if (!self) { return NULL; }

    // Initialize the hardware.
    timerHardware_init(self, circuit);

    // Return the hardware structure after the initialization is finished.
    return self;
}

// -----------------------------------------------------------------------------
static void timerHardware_delete(TimerHardware* const self)
{
    // Terminate the function if no valid hardware has been specified.
    if (!self) { return; }

    // Disable interrupts for the given circuit by clearing the associated mask bit.
    timerHardware_enable(self, false);

    // Free resources allocated for the hardware.
    free(self);
}

/*******************************************************************************
 * Timer-specific functions.
 ******************************************************************************/
 // -----------------------------------------------------------------------------
static inline Timer* timer(TimerInterface* const self) 
{ 
    // Downcast to child structure Timer, return the corresponding pointer.
    return (Timer*)(self); 
}

// -----------------------------------------------------------------------------
static inline const Timer* timerConst(const TimerInterface* const self)
{
    // Downcast to child structure Timer, return the corresponding pointer.
    return (const Timer*)(self);
}

// -----------------------------------------------------------------------------
static uint32_t timer_tickFrequencyHz(void)
{
    // Get singleton configuration instance to retrieve the CPU frequency.
    ConfigInterface* config = atmega328p_config_getInstance();

    // Return the timer frequency based on the set prescaler / clock divider.
    return config->vptr->cpuFrequencyHz(config) / TIMER_PRESCALER;
}

// -----------------------------------------------------------------------------
static inline double timer_interruptFrequencyHz(void)
{
    // Divide the set tick frequency with the number of ticks it takes to generate an interrupt.
    return timer_tickFrequencyHz() / (double)(TIMER_MAX_COUNT);
}

// -----------------------------------------------------------------------------
static double timer_interruptPeriod_ms(void)
{
    // Return the set interrupt period in ms, rounding to the nearest integer.
    return 1000UL / timer_interruptFrequencyHz();
}

// -----------------------------------------------------------------------------
static uint32_t timer_toMaxCount(const uint32_t duration_ms)
{
    // Convert the given duration to the corresponding max count, rounding to the nearest integer.
    return (uint32_t)(duration_ms / timer_interruptPeriod_ms() + 0.5);
}

// -----------------------------------------------------------------------------
static uint32_t timer_toDuration_ms(const uint32_t maxCount)
{
    // Convert the given max count to the corresponding duration, rounding to the nearest integer.
    return (uint32_t)(maxCount * timer_interruptPeriod_ms() + 0.5);
}

// -----------------------------------------------------------------------------
static inline bool timer_isDurationValid(const uint32_t duration_ms)
{
    // Return true if the given duration is within the allowed range.
    return (TIMER_DURATION_MIN_MS <= duration_ms) && (TIMER_DURATION_MAX_MS >= duration_ms);
}

// -----------------------------------------------------------------------------
static bool timer_checkParameters(const uint32_t duration_ms, const TimerMode mode, 
    void (*callback)(void))
{
    // Return true if the duration and mode are valid, and a non-null callback is specified.
    return timer_isDurationValid(duration_ms) && timerMode_isValid(mode) && callback;
}

// -----------------------------------------------------------------------------
static Timer* timer_new(const uint32_t duration_ms, const TimerMode mode, void (*callback)(void))
{
    // Allocate memory for the new timer structure.
    Timer* const self = (Timer*)(malloc(sizeof(Timer)));

    // Return a nullptr if memory couldn't be allocated.
    if (!self) { return NULL; }

    // Initialize the vtable pointer to give the interface access to the overridden functions.
    self->itf.vptr = timerVtable_ptr();

    // Initialize the hardware, point at the associated structure for hardware access.
    self->hw = timerHardware_new();

    // Set the callback function.
    self->callback = callback;

    // Convert given duration to the corresponding max count.
    self->maxCount = timer_toMaxCount(duration_ms);

    /** Initialize the timer counter. */
    self->counter = 0U;

    // Store the timer mode.
    self->mode = mode;

    // Reserve the given circuit in the timer registry.
    timerRegistry_reserve(self);

    // Return the newly allocated timer structure.
    return self;
}

// -----------------------------------------------------------------------------
static void timer_delete(Timer* const self)
{
    // Terminate the function is the timer is invalid.
    if (!self) { return; }

    // Free the given circuit in the timer registry.
    timerRegistry_free(self);

    // Disable and free the hardware allocated for the timer.
    timerHardware_delete(self->hw);
    self->hw = NULL;

    // Free the resources allocated for the device itself.
    free(self); 
}

// -----------------------------------------------------------------------------
static bool timer_hasTimedOut(Timer* const self)
{
    // Set the timeout flag to false.
    bool timeout = false;

    // Increment the timer and check if it has timed out.
    if (++self->counter >= self->maxCount)
    {
        // If true, clear the counter and set the timeout flag.
        self->counter = 0U;
        timeout       = true;

        // Disable the timer if it is a one-shot timer.
        if (TimerMode_OneShot == self->mode) { timerHardware_enable(self->hw, false); }
    }
    // Return true if the timer has timed out.
    return timeout;
}

// -----------------------------------------------------------------------------
static void timer_handleInterrupt(const TimerCircuit circuit)
{
    // Terminate the function is the given circuit is invalid.
    if (!timerCircuit_isValid(circuit)) { return; }

    // Get the associated timer circuit from the timer registry.
    Timer* const self = myTimerRegistry[circuit];

    // Terminate the function is the timer isn't present in the timer registry.
    if (!self) { return; }

    // Invoke the associated callback if the timer has timed out.
    if (timer_hasTimedOut(self)) { self->callback(); }
}

// -----------------------------------------------------------------------------
static uint8_t timer_id(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Return the timer ID (corresponds to the associated hardware timer).
    return (uint8_t)(impl->hw->circuit);
}

// -----------------------------------------------------------------------------
static uint32_t timer_duration_ms(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Return the calculated duration in milliseconds.
    return timer_toDuration_ms(impl->maxCount);
}

// -----------------------------------------------------------------------------
static TimerMode timer_mode(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Return the timer mode.
    return impl->mode;
}

// -----------------------------------------------------------------------------
static bool timer_isEnabled(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Return true if the timer is enabled.
    return timerHardware_isEnabled(impl->hw);
}

// -----------------------------------------------------------------------------
static void timer_start(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Enable the timer.
    timerHardware_enable(impl->hw, true);
}

// -----------------------------------------------------------------------------
static void timer_stop(const TimerInterface* const self, const bool reset)
{
    // Downcast to child structure Timer for accessing the hardware.
    Timer* impl = (Timer*)(timerConst(self));

    // Disable the timer.
    timerHardware_enable(impl->hw, false);

    // Reset the internal counter to zero if specified.
    if (reset) { impl->counter = 0U; }
}

// -----------------------------------------------------------------------------
static void timer_toggle(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    const Timer* impl = timerConst(self);

    // Determine the current state of the timer.
    const bool enabled = timerHardware_isEnabled(impl->hw);

    // Set the timer to the opposite state.
    timerHardware_enable(impl->hw, !enabled);
}

// -----------------------------------------------------------------------------
static void timer_restart(const TimerInterface* const self)
{
    // Downcast to child structure Timer for accessing the hardware.
    Timer* impl = (Timer*)(timerConst(self));

    // Reset the internal counter to zero.
    impl->counter = 0U;

    // Enable the timer.
    timerHardware_enable(impl->hw, true);
}

/*******************************************************************************
 * Timer vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const TimerVtable* const timerVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden timer functions.
    static const TimerVtable myVtable =
    {
        .id         = timer_id,
        .duration_ms = timer_duration_ms,
        .mode       = timer_mode,
        .isEnabled  = timer_isEnabled,
        .start      = timer_start,
        .stop       = timer_stop,
        .toggle     = timer_toggle,
        .restart    = timer_restart,
        .delete     = atmega328p_timer_delete,
    };
    // Return a pointer to the vtable for access through the timer interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) timer interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
TimerInterface* atmega328p_timer_new(const uint32_t duration_ms, const TimerMode mode, 
        void (*callback)(void))
{
    // Return a nullptr if the given parameters are invalid or no circuit is available.
    if (!timer_checkParameters(duration_ms, mode, callback) || !timerCircuit_isAvailable()) 
    { 
        return NULL; 
    }

    // Allocate memory for the new device.
    Timer* self = timer_new(duration_ms, mode, callback);

    // Return a pointer to new timer, cast to the corresponding interface.
    // A nullptr will be returned if memory couldn't be allocated.
    return (TimerInterface* )(self);
}

// -----------------------------------------------------------------------------
void atmega328p_timer_delete(TimerInterface** const self)
{
    // Terminate the function if the timer is invalid.
    if (!self || !(*self)) { return; }

    // Delete the timer and free allocated resources.
    timer_delete(timer(*self));

    // Set the associated pointer to null.
    *self = NULL;
}

/*******************************************************************************
 * Interrupt service routines.
 ******************************************************************************/
// -----------------------------------------------------------------------------
ISR(TIMER0_OVF_vect)
{
    // Handle Timer 0 overflow interrupt.
    timer_handleInterrupt(TimerCircuit_0);
}

// -----------------------------------------------------------------------------
ISR(TIMER1_COMPA_vect)
{
    // Handle Timer 1 compare match interrupt.
    timer_handleInterrupt(TimerCircuit_1);
}

// -----------------------------------------------------------------------------
ISR(TIMER2_OVF_vect)
{
    // Handle Timer 2 overflow interrupt.
    timer_handleInterrupt(TimerCircuit_2);
}
