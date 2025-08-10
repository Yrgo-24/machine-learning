/**
 * @brief Driver for the ATmega328P ADC (A/D converter).
 */
#pragma once

#include "adc_interface.h"
#include "utils.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Class for ATmega328P ADC driver.
 * 
 *        This class is non-copyable.
 */
class Adc : public AdcInterface
{
public:

    struct Pin;  // Pin aliases for analog pins.
    struct Port; // Port aliases for analog pins.

    /**
     * @brief Create new ADC.
     * 
     * @param enable Indicate whether to enable the ADC (default = true).
     */
    explicit Adc(const bool enable = true) noexcept;

    /**
     * @brief Delete ADC.
     */
    ~Adc() noexcept override = default;

    /**
     * @brief Move memory from another ADC.
     *
     *        The other ADC is cleared once the move operation is completed.
     *
     * @param other Reference to ADC to move memory from.
     */
    Adc(Adc&& other) noexcept;

    /**
     * @brief Move content from another ADC.
     * 
     *        The other ADC is cleared once the move operation is completed.
     *
     * @param other Reference to ADC holding the data to move. 
     * 
     * @return Reference to this ADC.
     */
    Adc& operator=(Adc&& other) noexcept;

    /**
     * @brief Get the resolution of the ADC.
     * 
     * @return The resolution of the ADC in bits.
     */
    uint8_t resolution() const noexcept override;

    /**
     * @brief Get the maximal input value of the ADC.
     * 
     * @return The maximum digital value of the ADC.
     */
    uint16_t maxValue() const noexcept override;

    /**
     * @brief Get the supply voltage of the ADC.
     * 
     * @return The supply voltage of the ADC in Volts.
     */
    double supplyVoltage() const noexcept override;

    /**
     * @brief Read input from given analog pin.
     * 
     * @param analogPin The analog pin from which to read.
     * 
     * @return The digital value corresponding to the input of the specified analog pin.
     */
    uint16_t read(const uint8_t analogPin) const noexcept override;

    /**
     * @brief Calculate duty cycle out of input from given analog pin.
     * 
     * @param analogPin The analog pin from which to read.
     * 
     * @return The duty cycle as a floating point value between 0.0 - 1.0.
     */
    double dutyCycle(const uint8_t analogPin) const noexcept override;

    /**
     * @brief Read input voltage from given analog pin.
     * 
     * @param analogPin The analog pin from which to read.
     * 
     * @return The input voltage in Volts.
     */
    double inputVoltage(const uint8_t analogPin) const noexcept override;
  
    /**
     * @brief Check whether the ADC is initialized.
     * 
     * @return True if the ADC is initialized, otherwise false.
     */
    bool isInitialized() const noexcept override;

    /**
     * @brief Indicate whether the ADC is enabled.
     * 
     * @return True if the ADC is enabled, otherwise false.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Set enablement of ADC.
     * 
     * @param enable Indicate whether to enable the ADC.
     */
    void setEnabled(const bool enable) noexcept override;

    Adc(const Adc&)            = delete; // No copy constructor.
    Adc& operator=(const Adc&) = delete; // No copy assignment.

private:

    void init() const noexcept;

    bool myEnabled; // Indicate whether the ADC is enabled.
};

/**
 * @brief Struct containing pin names for analog pins.
 */
struct Adc::Pin 
{
    static constexpr uint8_t A0{0U}; // Pin A0 = 0.
    static constexpr uint8_t A1{1U}; // Pin A1 = 1.
    static constexpr uint8_t A2{2U}; // Pin A2 = 2.
    static constexpr uint8_t A3{3U}; // Pin A3 = 3.
    static constexpr uint8_t A4{4U}; // Pin A4 = 4.
    static constexpr uint8_t A5{5U}; // Pin A5 = 5.
};

/**
 * @brief Struct containing port names for analog pins.
 */
struct Adc::Port
 {
    static constexpr uint8_t C0{14U}; // PORTC0 = pin 14.
    static constexpr uint8_t C1{15U}; // PORTC1 = pin 15.
    static constexpr uint8_t C2{16U}; // PORTC2 = pin 16.
    static constexpr uint8_t C3{17U}; // PORTC3 = pin 17.
    static constexpr uint8_t C4{18U}; // PORTC4 = pin 18.
    static constexpr uint8_t C5{19U}; // PORTC5 = pin 10.
};

} // namespace atmega328p
} // namespace driver