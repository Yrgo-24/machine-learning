/**
 * @brief Driver for miscellaneous GPIO devices on ATmega328P.
 */
#pragma once

#include "utils.h"

#include "gpio_interface.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for generic ATmega328P GPIO driver.
 * 
 *        This class is non-copyable.
 */
class Gpio : public GpioInterface
{
public:

    struct Port;          // Port aliases for GPIO pins.
    enum class Direction; // Representation of GPIO direction.
    enum class IoPort;    // Representation of I/O ports.

    /**
     * @brief Create new GPIO device.
     *
     * @param pin       The PIN number of device.
     * @param direction The direction of device.
     * @param callback  Function pointer to callback associated with the device (default = none).
     */
    Gpio(const uint8_t pin, const Direction direction, void (*callback)() = nullptr) noexcept;

    /**
     * @brief Delete GPIO device.
     */
    ~Gpio() noexcept override;

    /**
     * @brief Move memory from another GPIO device.
     *
     *        The other device is cleared once the move operation is completed.
     *
     * @param other Reference to GPIO device to move memory from.
     */
    Gpio(Gpio&& other) noexcept;

    /**
     * @brief Move content from another GPIO device.
     * 
     *        The other device is cleared once the move operation is completed.
     *
     * @param other Reference to GPIO device holding the data to move. 
     * 
     * @return Reference to this GPIO device.
     */
    Gpio& operator=(Gpio&& other) noexcept;

    /**
     * @brief Get the pin number of the device.
     *
     * @return The pin number of device.
     */
    uint8_t operator()() noexcept;

    /**
     * @brief Get the pin number of the device.
     *
     * @return The pin number of device.
     */
     uint8_t pin() const noexcept;

    /**
     * @brief Get I/O port associated with the device.
     *
     * @return The I/O port associated with the device as an enumerator of enum IoPort.
     */
    IoPort port() const noexcept;

    /**
     * @brief Check whether the device is initialized.
     * 
     *        An uninitialized device indicates that the specified PIN was unavailable or invalid
     *        when the device was created.
     * 
     * @return True if the device is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Read input of device.
     * 
     * @return True if the input is high, otherwise false.
     */
    bool read() const noexcept override;

    /**
     * @brief Write output to device.
     * 
     * @param output The output value to write (true = high, false = low).
     */
    void write(const bool output) noexcept override;

    /**
     * @brief Toggle device output.
     *        
     * @note This operation is only permitted for pins set to output.
     */
    void toggle() noexcept override;

    /**
     * @brief Enable/disable pin change interrupt for device.
     * 
     * @param enable Indicate whether to enable pin change interrupt for the device.
     */
    void enableInterrupt(const bool enable) noexcept override;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the device.
     * 
     * @param enable Indicate whether to enable pin change interrupt for the I/O port.
     */
    void enableInterruptOnPort(const bool enable) noexcept override;

    /**
     * @brief Blink output of device with given blink speed.
     *
     * @param blinkSpeedMs Reference to variable holding the blink speed in milliseconds.
     * 
     * @note This operation is only permitted for pins set to output.
     */
    void blink(const uint16_t& blink_speed_ms) noexcept;
    
    Gpio()                       = delete; // No default constructor.
    Gpio(const Gpio&)            = delete; // No copy constructor.
    Gpio& operator=(const Gpio&) = delete; // No copy assignment.

  private:

    struct Hardware; // Structure for implementation of GPIO hardware.

    static Hardware* reserve(const uint8_t pin, const Direction direction) noexcept;
    static Hardware* initHardware(const uint8_t pin) noexcept;

    void setDirection(const Direction direction) noexcept;
    void setCallback(void (*callback)()) const noexcept;

    void disable() noexcept;

    static Hardware myHwPortB, myHwPortC, myHwPortD; // Pointer to GPIO hardware for each I/O port.

    Hardware* myHardware; // Pointer to GPIO hardware.
    uint8_t myPin;        // Pin the device is connected to.
};

/**
 * @brief Struct containing port aliases for each pin.
 * 
 *        - Pins 0 - 7 are associated with I/O port D.
 * 
 *        - Pins 8 - 13 are associated with I/O port B.
 * 
 *        - Pins 14 - 19 are associated with I/O port C.
 */
struct Gpio::Port 
{
    static constexpr uint8_t D0{0};  // PORTD0 = pin 0.
    static constexpr uint8_t D1{1};  // PORTD1 = pin 1.
    static constexpr uint8_t D2{2};  // PORTD2 = pin 2.
    static constexpr uint8_t D3{3};  // PORTD3 = pin 3.
    static constexpr uint8_t D4{4};  // PORTD4 = pin 4.
    static constexpr uint8_t D5{5};  // PORTD5 = pin 5.
    static constexpr uint8_t D6{6};  // PORTD6 = pin 6.
    static constexpr uint8_t D7{7};  // PORTD7 = pin 7.

    static constexpr uint8_t B0{8};  // PORTB0 = pin 8.
    static constexpr uint8_t B1{9};  // PORTB1 = pin 9.
    static constexpr uint8_t B2{10}; // PORTB2 = pin 10.
    static constexpr uint8_t B3{11}; // PORTB3 = pin 11.
    static constexpr uint8_t B4{12}; // PORTB4 = pin 12.
    static constexpr uint8_t B5{13}; // PORTB5 = pin 13.

    static constexpr uint8_t C0{14}; // PORTC0 = pin 14.
    static constexpr uint8_t C1{15}; // PORTC1 = pin 15.
    static constexpr uint8_t C2{16}; // PORTC2 = pin 16.
    static constexpr uint8_t C3{17}; // PORTC3 = pin 17.
    static constexpr uint8_t C4{18}; // PORTC4 = pin 18.
    static constexpr uint8_t C5{19}; // PORTC5 = pin 10.
};

/**
 * @brief Enumeration class representing the possible data directions for GPIO devices.
 */
enum class Gpio::Direction
{ 
    Input,       // Input without internal pull-up resistor enabled (tri-state).
    InputPullup, // Input with internal pull-up resistor enabled.
    Output,      // Output.
    Count,       // The number of data direction alternatives.
};

/**
 * @brief Enumeration class representing the I/O ports associated with the devices.
*/
enum class Gpio::IoPort 
{
    B = PCIE0, // I/O port B.
    C = PCIE1, // I/O port C.
    D = PCIE2, // I/O port D.
    Count,     // The number of I/O ports available.
};

} // namespace atmega328p
} // namespace driver