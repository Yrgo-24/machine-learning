/**
 * @brief Timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
/**
 * @brief Timer interface.
 */
class TimerInterface
{
public:
    /**
     * @brief Delete the timer.
     */
    virtual ~TimerInterface() noexcept = default;

    /**
     * @brief Check if the timer is initialized.
     * 
     *        An uninitialized timer indicates that no timer circuit was available when the timer 
     *        was created.
     * 
     * @return True if the timer is initialized, false otherwise.
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief Check whether the timer is enabled.
     *
     * @return True if the timer is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Check whether the timer has timed out.
     * 
     *        The timer will restart automatically on timeout.
     *
     * @return True if the timer has timed out, false otherwise.
     */
    virtual bool hasTimedOut() = 0;

    /**
     * @brief Get the timeout of the timer.
     * 
     * @return The timeout in milliseconds.
     */
    virtual uint32_t timeout_ms() const = 0;

    /**
     * @brief Set timeout of the timer.
     * 
     * @param[in] timeout_ms The new timeout in milliseconds.
     */
    virtual void setTimeout_ms(const uint32_t timeout_ms) = 0;

    /**
     * @brief Start the timer.
     */
    virtual void start() = 0;

    /**
     * @brief Stop the timer.
     */
    virtual void stop() = 0;

    /**
     * @brief Toggle the timer.
     */
    virtual void toggle() = 0;

    /**
     * @brief Restart the timer.
     */
    virtual void restart() = 0;
};
} // namespace driver
