/**
 * @brief Driver for ATmega328P hardware timers. 
 *
 * @note Three hardware timers Timer 0 - Timer 2 are available.
 */
#pragma once

#include "timer_interface.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for ATmega328P hardware timers.
 * 
 *        This class is non-copyable.
 */
class Timer : public TimerInterface
{
  public:

    /**
     * @brief Create new timer with given elapse time, provided that a timer circuit is available.
     *
     * @param elapseTimeMs The elapse time of timer in milliseconds.
     * @param callback     Function pointer to callback function (default = none).
     * @param startTimer   Start the timer immediately (default = false).
     */
    explicit Timer(const uint32_t elapseTimeMs, void (*callback)() = nullptr,
                   const bool startTimer = false) noexcept;

    /**
     * @brief Delete timer.
     */
    ~Timer() noexcept override; 

    /**
     * @brief Move memory from another timer.
     *
     *        The other timer is cleared once the move operation is completed.
     *
     * @param other Reference to other timer to move memory from.
     */
    Timer(Timer&& other) noexcept;

    /**
     * @brief Move content from another timer.
     * 
     *        The other timer is cleared once the move operation is completed.
     *
     * @param other Reference to timer holding the data to move. 
     * 
     * @return Reference to this timer.
     */
     Timer& operator=(Timer&& other) noexcept;

    /**
     * @brief Check if the timer is initialized.
     * 
     *        An uninitialized timer indicates that no timer circuit
     *        was available when the timer was created.
     * 
     * @return True if the timer is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicated if the timer is enabled.
     *
     * @return True if the timer is enabled, otherwise false.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Check if the timer has elapsed.
     * 
     *        The timer restarts when it elapses.
     *
     * @return True if the counter has elapsed, otherwise false.
     */
    bool hasElapsed() noexcept override;
    
    /**
     * @brief Get the elapse time of timer.
     *
     * @return The elapse time of timer measured in milliseconds.
     */
    uint32_t elapseTimeMs() const noexcept override;

    /**
     * @brief Start new elapse time of timer.
     *
     * @param elapseTimeMs The new elapse time measured in milliseconds.
     */
    void setElapseTimeMs(const uint32_t elapseTimeMs) noexcept override;

    /**
     * @brief Start timer.
     */
    void start() noexcept override;

    /**
     * @brief Stop timer.
     */
    void stop() noexcept override;

    /**
     * @brief Toggle timer.
     */
    void toggle() noexcept override;

    /**
     * @brief Restart timer.
     */
     void restart() noexcept override;

    /**
     * @brief Add callback function for timer.
     *
     * @param callback Pointer to callback function.
     */
    void addCallback(void (*callback)()) const noexcept;

    /**
     * @brief Remove callback function for timer.
     */
    void removeCallback() const noexcept;

    /**
     * @brief Increment the timer if it's enabled.
     *
     * @return True if the timer was incremented, otherwise false.
     */
    bool increment() noexcept;
 
    Timer()                        = delete; // No default constructor.
    Timer(const Timer&)            = delete; // No copy constructor.
    Timer& operator=(const Timer&) = delete; // No copy assignment.

private:

    struct Hardware; // Structure for implementation of timer hardware.

    Hardware* myHardware; // Pointer to timer hardware.
    uint32_t myMaxCount;  // Max value to count up to.
    bool myEnabled;       // Indicate whether the timer is enabled.
};

} // namespace atmega328p
} // namespace driver
