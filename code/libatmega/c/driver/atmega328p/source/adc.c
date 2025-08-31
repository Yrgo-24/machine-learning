/**
 * @brief ADC implementation details for ATmega328P.
 */
#include <stdbool.h>
#include <stddef.h>

#include <avr/io.h>

#include "driver/atmega328p/adc.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** The number of analog pins on the ATmega328P. */
#define ADC_PIN_COUNT 8U

/** Max value of an ADC conversion (corresponds to VCC). */
#define ADC_MAX_VALUE (uint16_t)(1023UL)

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const AdcVtable const* adcVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton ADC instance. */
static AdcInterface myInstance = {NULL};

/*******************************************************************************
 * ADC-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool adc_isPinValid(const uint8_t pin)
{
    // Return true if the given pin is a valid pin number of I/O port C.
    return ADC_PIN_COUNT > pin;
}

// -----------------------------------------------------------------------------
static inline void adc_startConversion(const uint8_t pin)
{
    // Use internal voltage, set specified pin to read from.
    ADMUX = ((1UL << REFS0) | pin);

    // Start conversion, set prescaler to get the most accurate approximation possible.
    ADCSRA = ((1UL << ADEN) | (1UL << ADSC) | (1UL << ADPS2) | (1UL << ADPS1) | (1UL << ADPS0));
}

// -----------------------------------------------------------------------------
static inline bool adc_isTransmissionOngoing(void) 
{ 
    // Return true if a transmission is ongoing (the ADC interrupt flag is not set).
    return (0U == (ADCSRA & (1UL << ADIF)));
}

// -----------------------------------------------------------------------------
static double adc_convert(const uint8_t pin)
{
    // Set up and start conversion.
    adc_startConversion(pin);

    // Wait until the conversion is finished.
    while (adc_isTransmissionOngoing()) {}

    // Return the result as a floating point value between 0.0-1.0.
    return ADC / (double)(ADC_MAX_VALUE);
}

// -----------------------------------------------------------------------------
static void adc_init(void)
{
    // Only initialize the implementation once.
    if (!myInstance.vptr)
    {
        // Set the vtable pointer of the singleton instance.
        myInstance.vptr = adcVtable_ptr();

        // Initialize the hardware with a dummy conversion.
        adc_convert(0U);
    }
}

// -----------------------------------------------------------------------------
static double adc_read(const AdcInterface* const self, const uint8_t pin)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Return -1.0 to indicate an error if the pin is invalid.
    if (!adc_isPinValid(pin)) { return -1.0; }

    // Read the input of the given pin, return the converted result as a floating-point value.
    return adc_convert(pin);
}

// -----------------------------------------------------------------------------
static inline void adc_delete(AdcInterface** const self)
{
    // Deletion is not valid in this implementation, since the singleton instance is static.
    // This function exists only to satisfy the interface.
    (void) (self);
}

/*******************************************************************************
 * ADC vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const AdcVtable const* adcVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden ADC functions.
    static const AdcVtable myVtable =
    {
        .read   = adc_read,
        .delete = adc_delete,
    };
    // Return a pointer to the vtable for access through the ADC interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) ADC interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
AdcInterface* atmega328p_adc_getInstance(void) 
{ 
    // Initialize ADC transmission (only done once).
    adc_init();

    // Return a pointer to the singleton ADC instance.
    return &myInstance; 
}
