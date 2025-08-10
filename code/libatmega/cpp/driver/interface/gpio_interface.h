/**
 * @brief Implementation of GPIO interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{

/**
 * @brief Class for implementation of GPIO interface.
 */
class GpioInterface
{
public:

    /** 
     * @brief Delete GPIO device.
     */
    virtual ~GpioInterface() noexcept = default;

    /**
     * @brief Check whether the device is initialized.
     * 
     *        An uninitialized device indicates that the specified PIN was unavailable or invalid
     *        when the device was created.
     * 
     * @return True if the device is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Read input of device.
     * 
     * @return True if the input is high, otherwise false.
     */
    virtual bool read() const noexcept = 0;

    /**
     * @brief Write output to device.
     * 
     * @param output The output value to write (true = high, false = low).
     */
    virtual void write(const bool output) noexcept = 0;

    /**
     * @brief Toggle device output.
     */
    virtual void toggle() noexcept = 0;

    /**
     * @brief Enable/disable pin change interrupt for device.
     * 
     * @param enable Indicate whether to enable pin change interrupt for the device.
     */
    virtual void enableInterrupt(const bool enable) noexcept = 0;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the device.
     * 
     * @param enable Indicate whether to enable pin change interrupt for the I/O port.
     */
    virtual void enableInterruptOnPort(const bool enable) noexcept = 0;
};

} // namespace driver