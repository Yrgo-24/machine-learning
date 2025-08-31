/**
 * @brief ADC implementation for ATmega328P.
 */
#pragma once

#include "driver/adc/interface.h"

/**
 * @brief Get the singleton ADC instance for ATmega328P.
 * 
 * @return Pointer to the singleton ADC instance.
 */
AdcInterface* atmega328p_adc_getInstance(void);
