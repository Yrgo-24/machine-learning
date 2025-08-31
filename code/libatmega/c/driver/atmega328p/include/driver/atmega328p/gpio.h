/**
 * @brief GPIO implementation for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/gpio/interface.h"

/**
 * @brief Create a GPIO.
 * 
 * @param[in] pin PIN number on the ATmega328P.
 * @param[in] mode GPIO mode.
 * @param[in] callback Pointer to associated callback function (nullptr = none).
 * 
 * @return Pointer to the new GPIO, or nullptr if the device couldn't be created.
 */
GpioInterface* atmega328p_gpio_new(uint8_t pin, GpioMode mode, void (*callback)(void));

/**
 * @brief Delete the GPIO.
 *
 *        Release all resources allocated for the GPIO. Set the associated pointer to nullptr.
 *
 * @param[in] self Double pointer to the GPIO to delete. The pointer will be set to nullptr.
 */
void atmega328p_gpio_delete(GpioInterface** const self);
