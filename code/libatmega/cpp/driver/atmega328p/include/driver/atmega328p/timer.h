/**
 * @brief Timer driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/timer/interface.h"

namespace driver 
{
namespace atmega328p
{
/**
 * @brief Timer driver for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 *
 * @note Tree hardware timers Timer 0 - Timer 2 are available.
 */
class Timer final : public TimerInterface
{
public:
    /**
     * @brief Create a new timer with the given elapse time.
     *
     * @param[in] elapseTimeMs The elapse time of timer in milliseconds.
     * @param[in] callback Callback to invoke on timeout (default = none).
     * @param[in] startTimer Start the timer immediately (default = false).
     */
    explicit Timer(const uint32_t elapseTimeMs, void (*callback)() = nullptr,
                   const bool startTimer = false) noexcept;

    /**
     * @brief Delete the timer.
     */
    ~Timer() noexcept override;

    /**
     * @brief Check if the timer is initialized.
     * 
     *        An uninitialized timer indicates that no timer circuit was available when the 
     *        timer was created.
     * 
     * @return True if the timer is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the timer is enabled.
     *
     * @return True if the timer is enabled, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Check whether the timer has timed out.
     * 
     *        The timer will restart automatically on timeout.
     *
     * @return True if the timer has timed out, false otherwise.
     */
    bool hasTimedOut() noexcept override;

    /**
     * @brief Get the timeout of the timer.
     * 
     * @return The timeout in milliseconds.
     */
    uint32_t timeout_ms() const noexcept override;

    /**
     * @brief Set timeout of the timer.
     * 
     * @param[in] timeout_ms The new timeout in milliseconds.
     */
    void setTimeout_ms(const uint32_t timeout_ms) noexcept override;

    /**
     * @brief Start the timer.
     */
    void start() noexcept override;

    /**
     * @brief Stop the timer.
     */
    void stop() noexcept override;

    /**
     * @brief Toggle the timer.
     */
    void toggle() noexcept override;

    /**
     * @brief Restart the timer.
     */
    void restart() noexcept override;

    /**
     * @brief Add callback function for the timer.
     *
     * @param[in] callback Pointer to callback function.
     */
    void addCallback(void (*callback)()) const noexcept;

    /**
     * @brief Remove callback function for the timer.
     */
    void removeCallback() const noexcept;

    /**
     * @brief Increment the timer if it's enabled.
     *
     * @return True if the timer was incremented, false otherwise.
     */
    bool increment() noexcept;

    Timer()                        = delete; // No default constructor.
    Timer(const Timer&)            = delete; // No copy constructor.
    Timer(Timer&&)                 = delete; // No move constructor.
    Timer& operator=(const Timer&) = delete; // No copy assignment.
    Timer& operator=(Timer&&)      = delete; // No move assignment.

private:
    /** Timer hardware structure. */
    struct Hardware;

    /** Hardware structure associated with the timer. */
    Hardware* myHardware;

    /** Max value to count up to. */
    uint32_t myMaxCount;

    /** Indicate whether the timer is enabled. */
    bool myEnabled;
};
} // namespace atmega328p
} // namespace driver
