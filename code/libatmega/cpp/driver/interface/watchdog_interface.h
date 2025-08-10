/**
 * @brief Implementation of watchdog timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{

/**
 * @brief Class for implementation of watchdog timer interface.
 */
class WatchdogInterface
{
public:

    /**
     * @brief Delete watchdog timer.
     */
    virtual ~WatchdogInterface() noexcept = default;

    /**
     * @brief Check whether the watchdog timer is initialized.
     * 
     * @return True if the watchdog timer is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check whether the watchdog timer is enabled.
     * 
     * @return True if the watchdog timer is enabled, otherwise false.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Get the timeout of the watchdog timer.
     * 
     * @return The timeout of the watchdog timer in milliseconds.
     */
    virtual uint16_t timeoutMs() const noexcept = 0;

    /**
     * @brief Set enablement of watchdog timer.
     * 
     * @param enable Indicate whether to enable the watchdog timer.
     */
    virtual void setEnabled(const bool enable) noexcept = 0;

    /**
     * @brief Reset watchdog timer.
     */
    virtual void reset() noexcept = 0;
};

} // namespace driver