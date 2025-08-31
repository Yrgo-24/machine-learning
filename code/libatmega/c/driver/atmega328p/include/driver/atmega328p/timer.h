/**
 * @brief Timer implementation for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/timer/interface.h"

/**
 * @brief Create a timer.
 * 
 * @param[in] duration_ms Timer duration in milliseconds.
 * @param[in] mode Timer mode.
 * @param[in] callback Pointer to associated callback function.
 * 
 * @return Pointer to the new timer, or nullptr if the timer couldn't be created.
 */
TimerInterface* atmega328p_timer_new(uint32_t duration_ms, TimerMode mode, void (*callback)(void));

/**
 * @brief Delete the timer.
 *
 *        Release all resources allocated for the timer. Set the associated pointer to nullptr.
 *
 * @param[in] self Double pointer to the timer to delete. The pointer will be set to nullptr.
 */
void atmega328p_timer_delete(TimerInterface** const self);
