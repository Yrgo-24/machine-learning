/**
 * @brief ADC (A/D converter) interface.
 */
#pragma once

#include <stddef.h>
#include <stdint.h>

namespace driver
{
/**
 * @brief ADC (A/D converter) interface.
 */
class AdcInterface
{
public:
    /**
     * @brief Delete the ADC.
     */
    virtual ~AdcInterface() noexcept = default;

    /**
     * @brief Get the resolution of the ADC.
     * 
     * @return The resolution of the ADC in bits.
     */
    virtual uint8_t resolution() const = 0;

    /**
     * @brief Get the maximal input value of the ADC.
     * 
     * @return The maximum digital value of the ADC.
     */
    virtual uint16_t maxValue() const = 0;

    /**
     * @brief Get the supply voltage of the ADC.
     * 
     * @return The supply voltage of the ADC in Volts.
     */
    virtual double supplyVoltage() const = 0;

    /**
     * @brief Read input from given analog pin.
     * 
     * @param[in] analogPin The analog pin from which to read.
     * 
     * @return The digital value corresponding to the input of the specified analog pin.
     */
    virtual uint16_t read(const uint8_t analogPin) const = 0;

    /**
     * @brief Calculate duty cycle out of input from given analog pin.
     * 
     * @param[in] analogPin The analog pin from which to read.
     * 
     * @return The duty cycle as a floating point value between 0.0 - 1.0.
     */
    virtual double dutyCycle(const uint8_t analogPin) const = 0;

    /**
     * @brief Read input voltage from given analog pin.
     * 
     * @param[in] analogPin The analog pin from which to read.
     * 
     * @return The input voltage in Volts.
     */
    virtual double inputVoltage(const uint8_t pin) const = 0;

    /**
     * @brief Check whether the ADC is initialized.
     * 
     * @return True if the ADC is initialized, false otherwise.
     */
    virtual bool isInitialized() const = 0;

    /**
     * @brief Indicate whether the ADC is enabled.
     * 
     * @return True if the ADC is enabled, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Set enablement of ADC.
     * 
     * @param[in] enable Indicate whether to enable the ADC.
     */
    virtual void setEnabled(const bool enable) = 0;
};
} // namespace driver
