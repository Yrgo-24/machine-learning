/**
 * @brief Implementation of serial transmission interface.
 */
#pragma once

#include <stdio.h>

namespace driver 
{

/**
 * @brief Class for implementation of serial transmission interface.
 */
class SerialInterface
{
public:

    /**
     * @brief Delete serial device.
     */
    virtual ~SerialInterface() noexcept = default;

    /**
     * @brief Check whether the serial device is initialized.
     * 
     * @return True if the device is initialized, otherwise false.
     */
    virtual bool isInitialized() const noexcept = 0;

    /**
     * @brief Check whether the serial device is enabled.
     * 
     * @return True if the serial device is enabled, otherwise false.
     */
    virtual bool isEnabled() const noexcept = 0;

    /**
     * @brief Set enablement of serial device.
     * 
     * @param enable Indicate whether to enable the device.
     */
    virtual void setEnabled(const bool enable) noexcept = 0;

    /**
     * @brief Print formatted string in serial terminal. If the formatted string
     *        contains format specifiers, the additional arguments are formatted
     *        as inserted into the format string.
     *
     * @tparam Args  Parameter pack containing an arbitrary number of arguments.
     *
     * @param format Reference to string containing the text to print.
     * @param args   Parameter pack containing potential additional arguments.
     *
     * @return True if the string was printed, otherwise false.
     */
    template <typename... Args>
    bool printf(const char* format, const Args&... args) const noexcept;

private:
    virtual void print(const char* message) const noexcept = 0;
};

// -----------------------------------------------------------------------------
template <typename... Args>
bool SerialInterface::printf(const char* format, const Args&... args) const noexcept
{
    if (format == nullptr || !isEnabled()) { return false; }

    if (sizeof...(args) > 0U)
    {
        constexpr size_t length{101U};
        char buffer[length]{'\0'};
        snprintf(buffer, length, format, args...);
        print(buffer);
    }
    else { print(format); }
    return true;
}

} // namespace driver
