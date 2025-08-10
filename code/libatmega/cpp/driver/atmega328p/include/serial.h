/**
 * @brief Driver for serial transmission via UART on ATmega328P.
 */
#pragma once

#include "utils.h"
#include "serial_interface.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for implementation of ATmega328P serial device driver.
 * 
 *        This class is non-copyable.
 */
class Serial : public SerialInterface
{
public:

    /**
     * @brief Create new serial device with a 9600 baud rate.
     * 
     * @param enable Indicate whether to enable serial transmission (default = true).
     */
    explicit Serial(const bool enable = true) noexcept;

    /**
     * @brief Delete serial device.
     */
    ~Serial() noexcept override = default;

    /**
     * @brief Move memory from another serial device.
     *
     *        The other device is cleared once the move operation is completed.
     *
     * @param other Reference to serial device to move memory from.
     */
    Serial(Serial&& other) noexcept;

    /**
     * @brief Move content from another serial device.
     * 
     *        The other device is cleared once the move operation is completed.
     *
     * @param other Reference to serial device holding the data to move. 
     * 
     * @return Reference to this serial device.
     */
    Serial& operator=(Serial&& other) noexcept;

    /**
     * @brief Check whether the serial device is initialized.
     * 
     * @return True if the device is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Check whether the serial device is enabled.
     * 
     * @return True if the serial device is enabled, otherwise false.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of serial device.
     * 
     * @param enable Indicate whether to enable the device.
     */
    void setEnabled(const bool enable) noexcept override;

    Serial(const Serial&)            = delete; // No copy constructor.
    Serial& operator=(const Serial&) = delete; // No copy assignment.

private:
    void print(const char* message) const noexcept override;

    bool myEnabled; // Indicate whether serial transmission is enabled.
};

} // namespace atmega328p
} // namespace driver