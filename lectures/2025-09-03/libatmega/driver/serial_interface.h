/**
 * @brief Serial driver interface.
 */
#pragma once

#include <stdint.h>
#include <stdio.h>

namespace driver 
{
/**
 * @brief Serial driver interface.
 */
class SerialInterface
{
public:
    /**
     * @brief Delete the serial device.
     */
    virtual ~SerialInterface() noexcept = default;

    /** 
     * @brief Get the baud rate of the serial device. 
     * 
     * @return The baud rate in bps (bits per second).
     */
    virtual uint32_t baudRate_bps() const = 0;

    /**
     * @brief Print formatted string in the serial terminal. 
     * 
     *        If the formatted string contains format specifiers, the additional arguments are 
     *        formatted and inserted into the format string.
     *
     * @tparam Args  Parameter pack containing an arbitrary number of arguments.
     *
     * @param[in] format Reference to string to print.
     * @param[in] args Parameter pack containing potential additional arguments.
     *
     * @return True if the string was printed, false otherwise.
     */
    template <typename... Args>
    bool printf(const char* format, const Args&... args) const noexcept;

private:
    /**
     * @brief Print the given string in the serial terminal.
     * 
     * @param[in] str The string to print.
     */
    virtual void print(const char* str) const = 0;
};

// -----------------------------------------------------------------------------
template <typename... Args>
bool SerialInterface::printf(const char* format, const Args&... args) const noexcept
{
    //! @note Function templates must be defined in the header file, since the compiler
    //!       needs the definition at compile-time.

    // Return false is the given format string is invalid.
    if (nullptr == format) { return false; }

    // Format and insert given additional arguments (if any).
    if (0U < sizeof...(args))
    {
        constexpr size_t length{101U};
        char buffer[length]{'\0'};
        (void) snprintf(buffer, length, format, args...);
        print(buffer);
    }
    // Print the string.
    else { print(format); }
    return true;
}
} // namespace driver