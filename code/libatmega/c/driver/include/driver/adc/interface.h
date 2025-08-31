/**
 * @brief A/D converter interface implementation.
 */
#pragma once

#include <stdint.h>

/** ADC virtual table implementation. */
typedef struct AdcVtable AdcVtable;

/**
 * @brief ADC interface implementation.
 */
typedef struct AdcInterface 
{
    /** Pointer to the associated virtual table. */
    AdcVtable const *vptr;
} AdcInterface;

/**
 * @brief ADC virtual table implementation.
 */
typedef struct AdcVtable 
{
    /**
     * @brief Read the input of the given analog pin.
     * 
     * @param[in] self Reference to the ADC.
     * @param[in] pin The analog pin to read from.
     * 
     * @return The analog input as a value between 0.0-1.0, or -1.0 if the pin is invalid.
     */
    double (*read)(const AdcInterface* self, uint8_t pin);

    /**
     * @brief Delete the ADC.
     *
     *        Release all resources allocated for the ADC. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the ADC to delete. The pointer will be set to nullptr.
     */
    void (*delete)(AdcInterface** self);
} AdcVtable;
