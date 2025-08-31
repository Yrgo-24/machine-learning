/**
 * @brief Watchdog timer interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/** Watchdog virtual table implementation. */
typedef struct WatchdogVtable WatchdogVtable;

/**
 * @brief Watchdog interface implementation.
 */
typedef struct WatchdogInterface 
{
    /** Pointer to the associated virtual table. */
    WatchdogVtable const *vptr;
} WatchdogInterface;

/**
 * @brief Watchdog virtual table implementation.
 */
typedef struct WatchdogVtable 
{
    /**
     * @brief Check whether the watchdog timer is enabled.
     * 
     * @param[in] self Reference to the watchdog timer.
     *
     * @return True if the watchdog timer is enabled, false otherwise.
     */
    bool (*isEnabled)(const WatchdogInterface* self);

    /**
     * @brief Set enablement of the watchdog timer.
     * 
     * @param[in] self Reference to the watchdog timer.
     * @param[in] enable True to enable the watchdog timer, false otherwise.
     */
    void (*setEnabled)(WatchdogInterface* self, bool enable);

    /**
     * @brief Get the timeout of the watchdog timer.
     * 
     * @return The timeout of the watchdog timer in milliseconds.
     */
    uint16_t (*timeout_ms)(const WatchdogInterface* self);

    /**
     * @brief Set the timeout of the watchdog timer.
     * 
     * @param[in] self Reference to the watchdog timer.
     * @param[in] timeout_ms Watchdog timeout in milliseconds.
     *
     * @note The given timeout must be valid for the device in order for the change to take effect.
     * 
     * @return True if the given timeout was set, false otherwise.
     */
    bool (*setTimeout_ms)(WatchdogInterface* self, uint16_t timeout_ms);

    /**
     * @brief Reset the watchdog timer. 
     *
     * @param[in] self Reference to the watchdog timer.
     * 
     * @note This function must be called continuously to prevent a watchdog timeout.
     */
    void (*reset)(WatchdogInterface* self);

    /**
     * @brief Delete the watchdog timer.
     *
     *        Release all resources allocated for the device. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the device to delete. The pointer will be set to nullptr.
     */
    void (*delete)(WatchdogInterface** self);
} WatchdogVtable;
