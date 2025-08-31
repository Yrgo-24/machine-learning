/**
 * @brief EEPROM implementation for ATmega328P.
 */
#pragma once

#include "driver/eeprom/interface.h"

/**
 * @brief Get the singleton EEPROM instance for ATmega328P.
 * 
 * @return Pointer to the singleton EEPROM instance.
 */
EepromInterface* atmega328p_eeprom_getInstance(void);
