/**
 * @brief Serial driver for ATmega328P.
 */
#pragma once

#include "serial_interface.h"

namespace driver
{
/**
 * @brief Serial driver for ATmega328P.
 * 
 *        Use the singleton design pattern to ensure only one serial device instance exists,
 *        reflecting the hardware limitation of a single serial port on the MCU.
 */
class Serial final : public SerialInterface
{
public:
    /**
     * @brief Get the singleton serial instance.
     * 
     * @return Reference to the singleton serial instance.
     */
    static SerialInterface& getInstance() noexcept;

    /** 
     * @brief Get the baud rate of the serial device. 
     * 
     * @return The baud rate in bps (bits per second).
     */
    uint32_t baudRate_bps() const override;

    Serial(const Serial&)                      = delete; // No copy constructor.
    Serial(Serial&& other) noexcept            = delete; // No move constructor.
    Serial& operator=(const Serial&)           = delete; // No copy assignment.
    Serial& operator=(Serial&& other) noexcept = delete; // No move assignment.

private:
    /**
     * @brief Create new serial device.
     */
    Serial() noexcept;

    /**
     * @brief Delete the serial device.
     */
    ~Serial() noexcept override = default;

    /**
     * @brief Print the given string in the serial terminal.
     * 
     * @param[in] str The string to print.
     */
    void print(const char* str) const noexcept override;
};
} // namespace driver