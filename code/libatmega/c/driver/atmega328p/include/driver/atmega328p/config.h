/**
 * @brief Configuration implementation for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/config/interface.h"

/**
 * @brief Get the singleton configuration instance for ATmega328P.
 * 
 * @return Pointer to the singleton configuration instance.
 */
ConfigInterface* atmega328p_config_getInstance(void);
