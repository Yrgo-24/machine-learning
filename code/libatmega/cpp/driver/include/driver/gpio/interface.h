/**
 * @brief GPIO interface.
 */
#pragma once

#include <stdint.h>

namespace driver
{
/**
 * @brief GPIO interface.
 */
class GpioInterface
{
public:
    /** 
     * @brief Delete the GPIO.
     */
    virtual ~GpioInterface() noexcept = default;

    /**
     * @brief Check whether the GPIO is initialized.
     * 
     *        An uninitialized device indicates that the specified PIN was unavailable or invalid
     *        when the device was created.
     * 
     * @return True if the device is initialized, false otherwise.
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief Read input of the GPIO.
     * 
     * @return True if the input is high, false otherwise.
     */
    virtual bool read() const = 0;

    /**
     * @brief Write output to the GPIO.
     * 
     * @param[in] output The output value to write (true = high, false = low).
     */
    virtual void write(const bool output) = 0;

    /**
     * @brief Toggle the output of the GPIO.
     */
    virtual void toggle() = 0;

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     * 
     * @param[in] enable True to enable pin change interrupt for the GPIO, false otherwise.
     */
    virtual void enableInterrupt(const bool enable) = 0;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     * 
     * @param[in] enable True to enable pin change interrupt for the I/O port, false otherwise.
     */
    virtual void enableInterruptOnPort(const bool enable) = 0;
};
} // namespace driver
