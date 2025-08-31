/**
 * @brief Serial implementation for ATmega328P.
 */
#pragma once

#include "driver/serial/interface.h"

/**
 * @brief Get the singleton serial device instance for ATmega328P.
 * 
 * @return Pointer to the singleton serial device instance.
 */
SerialInterface* atmega328p_serial_getInstance(void);
