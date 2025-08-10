/**
 * @brief Watchdog timer driver for ATmega328P.
 */
#pragma once

#include "watchdog_interface.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for implementation of ATmega328P watchdog timer driver.
 * 
 *        This class is non-copyable.
 */
class Watchdog : public WatchdogInterface
{
public:

    enum class Timeout : uint16_t; // Enum class representing the available watchdog timer timeouts.

    /**
     * @brief Create new watchdog timer.
     * 
     * @param timeout The timeout of the watchdog timer.
     * @param enable  Indicate whether to enable the watchdog (default = true).
     */
    explicit Watchdog(const Timeout timeout, const bool enable = true) noexcept;

    /**
     * @brief Delete watchdog timer.
     */
    ~Watchdog() noexcept override;

    /**
     * @brief Move memory from another watchdog timer.
     *
     *        The other watchdog timer is cleared once the move operation is completed.
     *
     * @param other Reference to other watchdog timer to move memory from.
     */
    Watchdog(Watchdog&& other) noexcept;

    /**
     * @brief Move content from another watchdog timer.
     * 
     *        The other watchdog timer is cleared once the move operation is completed.
     *
     * @param other Reference to watchdog timer holding the data to move. 
     * 
     * @return Reference to this timer.
     */
     Watchdog& operator=(Watchdog&& other) noexcept;

    /**
     * @brief Check whether the watchdog timer is initialized.
     * 
     * @return True if the watchdog timer is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the watchdog timer is enabled.
     * 
     * @return True if the watchdog timer is enabled, otherwise false.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Get the timeout of the watchdog timer.
     * 
     * @return The timeout of the watchdog timer in milliseconds.
     */
    uint16_t timeoutMs() const noexcept override;

    /**
     * @brief Set enablement of watchdog timer.
     * 
     * @param enable Indicate whether to enable the watchdog timer.
     */
    void setEnabled(const bool enable) noexcept override;

    /**
     * @brief Reset watchdog timer.
     */
    void reset() noexcept override;

    Watchdog()                           = delete; // No default constructor.
    Watchdog(const Watchdog&)            = delete; // No copy constructor.
    Watchdog& operator=(const Watchdog&) = delete; // No copy assignment.

private:

    bool init(const Timeout timeoutMs) noexcept;
    void disable() noexcept;

    Timeout myTimeout;  // Watchdog timeout.
    bool myInitialized; // Indicate whether the watchdog is initialized.
    bool myEnabled;     // Indicate whether the watchdog is enabled.
};

/**
 * @brief Enum class representing the available watchdog timeout durations.
 */
enum class Watchdog::Timeout : uint16_t
{
    Duration16ms   = 16U,   // 16 ms.
    Duration32ms   = 32U,   // 32 ms.
    Duration64ms   = 64U,   // 64 ms.
    Duration128ms  = 128U,  // 128 ms.
    Duration256ms  = 256U,  // 256 ms.
    Duration512ms  = 512U,  // 512 ms.
    Duration1024ms = 1024U, // 1024 ms.
    Duration2048ms = 2048U, // 2048 ms.
    Duration4096ms = 4096U, // 4096 ms.
    Duration8192ms = 8192U, // 8192 ms.
    Invalid,                // Invalid timeout.
};

} // namespace atmega328p
} // namespace driver
