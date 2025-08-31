/**
 * @brief Factory implementation for ATmega328P.
 */
#pragma once

#include "driver/factory/interface.h"

/**
 * @brief Get the singleton factory instance for ATmega328P.
 * 
 * @return Pointer to the singleton factory instance.
 */
FactoryInterface* atmega328p_factory_getInstance(void);
