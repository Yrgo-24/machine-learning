/**
 * @brief Implementation details of miscellaneous utility functions.
 */
 #include "utils.h"

namespace utils
{
    
// -----------------------------------------------------------------------------
void delayS(const uint16_t& delayTimeS) noexcept
{
    for (uint16_t i{}; i < delayTimeS; ++i)
    {
        _delay_ms(1000U);
    }
}

// -----------------------------------------------------------------------------
void delayMs(const uint16_t& delayTimeMs) noexcept
{
    for (uint16_t i{}; i < delayTimeMs; ++i)
    {
        _delay_ms(1U);
    }
}

// -----------------------------------------------------------------------------
void delayUs(const uint16_t& delayTimeUs) noexcept
{
    for (uint16_t i{}; i < delayTimeUs; ++i)
    {
        _delay_us(1U);
    }
}

} // namespace utils

// -----------------------------------------------------------------------------
void operator delete(void*, unsigned int) noexcept {}

// -----------------------------------------------------------------------------
extern "C" void __cxa_pure_virtual() {}

// -----------------------------------------------------------------------------
extern "C" void __cxa_guard_acquire() {}

// -----------------------------------------------------------------------------
extern "C" void __cxa_guard_release() {}