/**
 * @brief GPIO driver for ATmega328P.
 */
#pragma once

#include <stdint.h>

#include "driver/gpio/interface.h"

namespace driver 
{
namespace atmega328p
{
/**
 * @brief GPIO driver for ATmega328P.
 * 
 *        This class is non-copyable and non-movable.
 */
class Gpio final : public GpioInterface
{
public:
    /** Port aliases for GPIO pins. */
    struct Port;

    /** Enumeration of GPIO directions. */
    enum class Direction : uint8_t;

    /** Enumeration of I/O ports. */
    enum class IoPort : uint8_t;

    /**
     * @brief Create a new GPIO.
     *
     * @param[in] pin The pin number of the GPIO.
     * @param[in] direction The GPIO direction.
     * @param[in] callback Callback associated with the GPIO (default = none).
     */
    explicit Gpio(const uint8_t pin, const Direction direction, 
        void (*callback)() = nullptr) noexcept;

    /**
     * @brief Delete the GPIO.
     */
    ~Gpio() noexcept override;

    /**
     * @brief Get the pin number of the GPIO.
     *
     * @return The pin number of the GPIO.
     */
    uint8_t operator()() const noexcept;

    /**
     * @brief Get the pin number of the GPIO.
     *
     * @return The pin number of the GPIO.
     */
    uint8_t pin() const noexcept;

    /**
     * @brief Get I/O port associated with the GPIO.
     *
     * @return The I/O port associated with the GPIO as an enumerator of enum IoPort.
     */
    IoPort port() const noexcept;

    /**
     * @brief Check whether the GPIO is initialized.
     * 
     *        An uninitialized GPIO indicates that the specified PIN was unavailable or invalid
     *        when the GPIO was created.
     * 
     * @return True if the GPIO is initialized, false otherwise.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Read input of the GPIO.
     * 
     * @return True if the input is high, false otherwise.
     */
    bool read() const noexcept override;

    /**
     * @brief Write output to the GPIO.
     * 
     * @param[in] output The output value to write (true = high, false = low).
     */
    void write(const bool output) noexcept override;

    /**
     * @brief Toggle the output of the GPIO.
     *        
     * @note This operation is only supported for pins set to output.
     */
    void toggle() noexcept override;

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     * 
     * @param[in] enable True to enable pin change interrupt for the GPIO, false otherwise.
     */
    void enableInterrupt(const bool enable) noexcept override;

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     * 
     * @param[in] enable True to enable pin change interrupt for the I/O port, false otherwise.
     */
    void enableInterruptOnPort(const bool enable) noexcept override;

    /**
     * @brief Blink output of the GPIO with the given blink speed.
     *
     * @param[in] blinkSpeedMs The blink speed in milliseconds.
     * 
     * @note This operation is only supported for pins set to output.
     */
    void blink(const uint16_t& blinkSpeed_ms) noexcept;
    
    Gpio()                       = delete; // No default constructor.
    Gpio(const Gpio&)            = delete; // No copy constructor.
    Gpio(Gpio&&)                 = delete; // No move constructor.
    Gpio& operator=(const Gpio&) = delete; // No copy assignment.
    Gpio& operator=(Gpio&&)      = delete; // No move assignment.

private:
    /** GPIO hardware structure. */
    struct Hardware;

    static Hardware* reserve(const uint8_t pin, const Direction direction) noexcept;
    static Hardware* initHardware(const uint8_t pin) noexcept;

    void setDirection(const Direction direction) noexcept;
    void setCallback(void (*callback)()) const noexcept;

    /** Hardware structure for I/O port B. */
    static Hardware myHwPortB;
    
    /** Hardware structure for I/O port C. */
    static Hardware myHwPortC;

    /** Hardware structure for I/O port D. */
    static Hardware myHwPortD;

    /** Hardware structure associated with the GPIO. */
    Hardware* myHardware;

    /** Pin the GPIO is connected to. */
    uint8_t myPin;
};

/**
 * @brief Port aliases for GPIO pins.
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
 * @brief Enumeration of GPIO directions.
 */
enum class Gpio::Direction : uint8_t
{ 
    Input,       // Input without internal pull-up resistor enabled (tri-state).
    InputPullup, // Input with internal pull-up resistor enabled.
    Output,      // Output.
    Count,       // The number of data direction alternatives.
};

/**
 * @brief Enumeration of I/O ports.
*/
enum class Gpio::IoPort : uint8_t
{
    B,     // I/O port B.
    C,     // I/O port C.
    D,     // I/O port D.
    Count, // The number of I/O ports available.
};
} // namespace atmega328p
} // namespace driver
