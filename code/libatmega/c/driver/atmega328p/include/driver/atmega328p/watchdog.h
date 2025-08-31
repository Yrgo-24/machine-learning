/**
 * @brief Watchdog timer implementation for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/watchdog/interface.h"

/**
 * @brief Enumeration of watchdog timeouts for ATmega328P.
 */
typedef enum
{
    ATMEGA328P_WATCHDOG_TIMEOUT_16_MS   = 16UL,   /** 16 ms.   */
    ATMEGA328P_WATCHDOG_TIMEOUT_32_MS   = 32UL,   /** 32 ms.   */
    ATMEGA328P_WATCHDOG_TIMEOUT_64_MS   = 64UL,   /** 64 ms.   */
    ATMEGA328P_WATCHDOG_TIMEOUT_128_MS  = 128UL,  /** 128 ms.  */
    ATMEGA328P_WATCHDOG_TIMEOUT_256_MS  = 256UL,  /** 256 ms.  */
    ATMEGA328P_WATCHDOG_TIMEOUT_512_MS  = 512UL,  /** 512 ms.  */
    ATMEGA328P_WATCHDOG_TIMEOUT_1024_MS = 1024UL, /** 1024 ms. */
    ATMEGA328P_WATCHDOG_TIMEOUT_2048_MS = 2048UL, /** 2048 ms. */
    ATMEGA328P_WATCHDOG_TIMEOUT_4096_MS = 4096UL, /** 4096 ms. */
    ATMEGA328P_WATCHDOG_TIMEOUT_8192_MS = 8192UL, /** 8192 ms. */
} atmega328p_WatchdogTimeout;

/**
 * @brief Get the singleton watchdog timer instance for ATmega328P.
 * 
 * @return Pointer to the singleton watchdog timer instance.
 */
WatchdogInterface* atmega328p_watchdog_getInstance();

/**
 * @brief Set the timeout of the watchdog timer.
 * 
 * @param[in] timeout_ms Watchdog timeout in milliseconds.
 *
 * @note The given timeout must be valid for the device; see enum `atmega328p_WatchdogTimeout`.
 * 
 * @return True if the given timeout was set, false otherwise.
 */
bool atmega328p_watchdog_setTimeout_ms(const uint16_t timeout_ms);
