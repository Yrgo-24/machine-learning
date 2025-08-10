/**
 * @brief Implementation of timer interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{

/**
 * @brief Class for implementation of timer interface.
 */
class TimerInterface
{
public:

    /**
     * @brief Delete timer.
     */
    virtual ~TimerInterface() noexcept = default;

    /**
     * @brief Check if the timer is initialized.
     * 
     *        An uninitialized timer indicates that no timer circuit
     *        was available when the timer was created.
     * 
     * @return True if the timer is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check if the timer is enabled.
     * 
     * @return True if the timer is enabled, otherwise false.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Check if the timer has elapsed.
     * 
     *        The timer restarts when it elapses.
     *
     * @return True if the counter has elapsed, otherwise false.
     */
    virtual bool hasElapsed() noexcept = 0;
 
    /**
     * @brief Get the elapse time of the timer.
     * 
     * @return The elapse time of the timer in milliseconds.
     */
    virtual uint32_t elapseTimeMs() const noexcept = 0;

    /**
     * @brief Set elapse time of the timer.
     * 
     * @param elapseTimeMs The new elapse time of the timer in milliseconds.
     */
    virtual void setElapseTimeMs(const uint32_t elapseTimeMs) noexcept = 0;

    /**
     * @brief Start timer.
     */
    virtual void start() noexcept = 0;

    /**
     * @brief Stop timer.
     */
    virtual void stop() noexcept = 0;

    /**
     * @brief Toggle timer.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Restart timer.
     */
    virtual void restart() noexcept = 0;
};

} // namespace driver