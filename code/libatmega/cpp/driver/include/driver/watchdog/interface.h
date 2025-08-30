/**
 * @brief Watchdog timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
/**
 * @brief Watchdog timer interface.
 */
class WatchdogInterface
{
public:
    /**
     * @brief Delete the watchdog timer.
     */
    virtual ~WatchdogInterface() noexcept = default;

    /**
     * @brief Check whether the watchdog timer is initialized.
     * 
     * @return True if the watchdog timer is initialized, false otherwise.
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief Check whether the watchdog timer is enabled.
     * 
     * @return True if the watchdog timer is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Set enablement of the watchdog timer.
     * 
     * @param[in] enable True to enable the watchdog timer, false otherwise.
     */
    virtual void setEnabled(const bool enable) = 0;

    /**
     * @brief Get the timeout of the watchdog timer.
     * 
     * @return The timeout of the watchdog timer in milliseconds.
     */
    virtual uint16_t timeoutMs() const = 0;

    /**
     * @brief Reset the watchdog timer.
     */
    virtual void reset() = 0;
};
} // namespace driver
