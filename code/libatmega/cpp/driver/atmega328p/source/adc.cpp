/**
 * @brief ADC driver implementation details for the ATmega328P ADC (A/D converter).
 */
#include <avr/io.h>

#include "driver/atmega328p/adc.h"
#include "utils/utils.h"

namespace driver 
{
namespace atmega328p
{
namespace 
{
/**
 * @brief Structure of ATmega328P ADC parameters.
 */
struct AdcParam
{
    /** Resolution in bits. */
    static constexpr uint8_t Resolution{10U};

    /** Max value of the ADC (limited by the resolution). */
    static constexpr uint16_t MaxValue{1023U};

    /** Supply voltage in Volts. */
    static constexpr double SupplyVoltage{5.0};

    /** ADC port offset (pin [14:19] == port [A0:A5]). */
    static constexpr uint8_t PortOffset{14U};
};

// -----------------------------------------------------------------------------
constexpr bool isPinNumberValid(const uint8_t pin) noexcept
{
    return utils::inRange(pin, Adc::Pin::A0, Adc::Pin::A5) 
        || utils::inRange(pin, Adc::Port::C0, Adc::Port::C5);
}

// -----------------------------------------------------------------------------
constexpr uint8_t isPinAdjustedForOffset(const uint8_t pin) noexcept
{
    return Adc::Pin::A5 >= pin ? pin : pin - AdcParam::PortOffset;
}

// -----------------------------------------------------------------------------
inline uint16_t adcValue(const uint8_t pin) noexcept
{
    if (!isPinNumberValid(pin)) { return 0U; }
    ADMUX = (1U << REFS0) | isPinAdjustedForOffset(pin);
    utils::set(ADCSRA, ADEN, ADSC, ADPS0, ADPS1, ADPS2);
    while (!utils::read(ADCSRA, ADIF));
    utils::set(ADCSRA, ADIF);
    return ADC;
}
} // namespace 

// -----------------------------------------------------------------------------
AdcInterface& Adc::getInstance() noexcept
{ 
    // Create and initialize the singleton ADC instance (once only).
    static Adc myInstance{};

    // Return a reference to the singleton ADC instance, cast to the corresponding interface.
    return myInstance; 
}

// -----------------------------------------------------------------------------
uint8_t Adc::resolution() const noexcept { return AdcParam::Resolution; }

// -----------------------------------------------------------------------------
uint16_t Adc::maxValue() const noexcept { return AdcParam::MaxValue; }

// -----------------------------------------------------------------------------
double Adc::supplyVoltage() const noexcept { return AdcParam::SupplyVoltage; }

// -----------------------------------------------------------------------------
uint16_t Adc::read(const uint8_t analogPin) const noexcept
{ 
    return myEnabled ? adcValue(analogPin) : 0U;
}

// -----------------------------------------------------------------------------
double Adc::dutyCycle(const uint8_t analogPin) const noexcept
{
    return read(analogPin) / static_cast<double>(AdcParam::MaxValue);
}

// -----------------------------------------------------------------------------
double Adc::inputVoltage(const uint8_t analogPin) const noexcept
{
    return dutyCycle(analogPin) * AdcParam::SupplyVoltage;
}

// -----------------------------------------------------------------------------
bool Adc::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Adc::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Adc::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
Adc::Adc() noexcept
    : myEnabled{false}
{
    read(Pin::A0);
}
} // namespace atmega328p
} // namespace driver
