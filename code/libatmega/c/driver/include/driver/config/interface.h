/**
 * @brief Configuration interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/** Configuration virtual table implementation. */
typedef struct ConfigVtable ConfigVtable;

/**
 * @brief Configuration interface implementation.
 */
typedef struct ConfigInterface 
{
    /** Pointer to the associated virtual table. */
    ConfigVtable const *vptr;
} ConfigInterface;

/**
 * @brief Configuration virtual table implementation.
 */
typedef struct ConfigVtable 
{
    /**
     * @brief Get the CPU frequency.
     * 
     * @param[in] self Reference to the configuration.
     * 
     * @return The CPU frequency in Hz.
     */
    uint32_t (*cpuFrequencyHz)(const ConfigInterface* self);

    /**
     * @brief Set the CPU frequency.
     * 
     * @param[in] self Reference to the configuration.
     * @param[in] frequencyHz The CPU frequency in Hz.
     * 
     * @return True if the CPU frequency was set, false if the given frequency was invalid.
     */
    bool (*setCpuFrequencyHz)(ConfigInterface* self, uint32_t frequencyHz);

    /**
     * @brief Get the UART baud rate.
     * 
     * @param[in] self Reference to the configuration.
     * 
     * @return The UART baud rate in bps (bits per second);
     */
    uint32_t (*uartBaudRateBps)(const ConfigInterface* self);

    /**
     * @brief Set the UART baud rate.
     * 
     * @param[in] self Reference to the configuration.
     * @param[in] baudRateBps The UART baud rate in bps (bits per second).
     * 
     * @return True if the baud rate was set, false if the given baud rate was invalid.
     */
    bool (*setUartBaudRateBps)(ConfigInterface* self, uint32_t baudRateBps);

    /**
     * @brief Delete the configuration.
     *
     *        Release all resources allocated for the configuration. 
     *        
     *        Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the configuration to delete. 
     *                 The pointer will be set to nullptr.
     */
    void (*delete)(ConfigInterface** self);
} ConfigVtable;