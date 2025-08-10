/**
 * @brief Implementation details of ATmega328P ADC driver.
 */
#include "adc.h"
#include "utils.h"

namespace driver 
{
namespace atmega328p
{
namespace 
{

/**
 * @brief Structure holding ATmega328P ADC parameters.
 */
struct AdcParam
{
    static constexpr uint8_t resolution{10U};   // Resolution in bits.
    static constexpr uint16_t maxValue{1023U};  // Max value of the ADC.
    static constexpr double supplyVoltage{5.0}; // Supply voltage in Volts.
    static constexpr uint8_t portOffset{14U};   // ADC port offset (pin [14:19] == port [A0:A5]).
};

// -----------------------------------------------------------------------------
constexpr bool isPinNumberValid(const uint8_t pin) noexcept
{
    return (pin >= Adc::Pin::A0 && pin <=  Adc::Pin::A5) 
        || (pin >=  Adc::Port::C0 && pin <=  Adc::Port::C5);
}

// -----------------------------------------------------------------------------
constexpr uint8_t isPinAdjustedForOffset(const uint8_t pin) noexcept
{
    return pin <=  Adc::Pin::A5 ? pin : pin - AdcParam::portOffset;
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
Adc::Adc(const bool enable) noexcept
    : myEnabled{enable}
{
    init();
}

// -----------------------------------------------------------------------------
Adc::Adc(Adc&& other) noexcept
    : myEnabled{other.myEnabled}
{
    other.myEnabled = false;
}

// -----------------------------------------------------------------------------
Adc& Adc::operator=(Adc&& other) noexcept
{
    if (&other != this)
    {
        myEnabled       = other.myEnabled;
        other.myEnabled = false;
    }
    return *this;
}

// -----------------------------------------------------------------------------
uint8_t Adc::resolution() const noexcept { return AdcParam::resolution; }

// -----------------------------------------------------------------------------
uint16_t Adc::maxValue() const noexcept { return AdcParam::maxValue; }

// -----------------------------------------------------------------------------
double Adc::supplyVoltage() const noexcept { return AdcParam::supplyVoltage; }

// -----------------------------------------------------------------------------
uint16_t Adc::read(const uint8_t analogPin) const noexcept
{ 
    return myEnabled ? adcValue(analogPin) : 0U;
}

// -----------------------------------------------------------------------------
double Adc::dutyCycle(const uint8_t analogPin) const noexcept
{
    return read(analogPin) / static_cast<double>(AdcParam::maxValue);
}

// -----------------------------------------------------------------------------
double Adc::inputVoltage(const uint8_t analogPin) const noexcept
{
    return dutyCycle(analogPin) * AdcParam::supplyVoltage;
}

// -----------------------------------------------------------------------------
bool Adc::isInitialized() const noexcept { return true; }

// -----------------------------------------------------------------------------
bool Adc::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Adc::setEnabled(const bool enable) noexcept { myEnabled = enable; }

// -----------------------------------------------------------------------------
void Adc::init() const noexcept { read(Pin::A0); }

} // namespace atmega328p
} // namespace driver
