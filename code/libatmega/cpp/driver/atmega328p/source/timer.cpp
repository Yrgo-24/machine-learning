/**
 * @brief Implementation details of hardware timer driver.
 */
#include <avr/interrupt.h>

#include "container/array.h"
#include "driver/atmega328p/timer.h" 
#include "utils/callback_array.h"
#include "utils/utils.h"

namespace driver 
{
namespace atmega328p
{
/**
 * @brief Structure for implementation of timer hardware.
 */
struct Timer::Hardware 
{
    /** Hardware counter. */
	volatile uint32_t counter;

    /** Pointer to mask register. */
	volatile uint8_t* maskReg;

    /** Mask bit for timer interrupt. */
	uint8_t maskBit;

    /** Timer index. */
	uint8_t index;  

    static Hardware* reserve() noexcept;
	static void release(Hardware* hardware) noexcept;

private:
    static Hardware* init(const uint8_t timerIndex) noexcept;
}; 

namespace
{  
using container::CallbackArray;

/**
 * @brief Structure containing indexes for available timer circuits.
 */
struct TimerIndex
{
	/** Index for Timer 0. */
	static constexpr uint8_t timer0{0U};

	/** Index for Timer 1. */
	static constexpr uint8_t timer1{1U};

	/** Index for Timer 2. */
	static constexpr uint8_t timer2{2U};
};

/**
 * @brief Structure holding control bits of timer circuits.
 */
struct ControlBits 
{
	/** Control bits for Timer 0. */
	static constexpr uint8_t timer0{(1U << CS01)};

	/** Control bits for Timer 1. */
	static constexpr uint8_t timer1{(1U << CS11) | (1U << WGM12)};

	/** Control bits for Timer 2. */
	static constexpr uint8_t timer2{(1U << CS21)};
};

/**
 * @brief Structure holding ATmega328P timer parameter.
 */
struct TimerParam
{
	/** The number of available timer circuits. */
	static constexpr uint8_t circuitCount{3U};

	/** Max count for 16-bit timer Timer 1.  */
	static constexpr uint16_t timer1MaxCount{256U};  

	/** Time between each timer interrupt in ms. */
	static constexpr double interruptIntervalMs{0.128};

	/** Array holding pointers to TimerParam::timers. */
	static Timer* timers[circuitCount];

	/** Array holding pointers to callbacks. */
	static CallbackArray<circuitCount> callbacks;
};

/** Array holding pointers to TimerParam::timers. */
Timer* TimerParam::TimerParam::timers[TimerParam::circuitCount]{};  

/** Array holding pointers to callbacks. */
CallbackArray<TimerParam::circuitCount> TimerParam::callbacks{};

// -----------------------------------------------------------------------------
constexpr uint32_t maxCount(const uint32_t elapseTimeMs) noexcept
{
	return 0U < elapseTimeMs ? 
        utils::round<uint32_t>(elapseTimeMs / TimerParam::interruptIntervalMs) : 0U;
}

// -----------------------------------------------------------------------------
void invokeCallback(const uint8_t timerIndex) noexcept
{
	if (timerIndex >= TimerParam::circuitCount) { return; }
    auto timer{TimerParam::timers[timerIndex]};

    if (timer)
    {
        timer->increment();

        if (timer->hasTimedOut()) 
		{ 
			TimerParam::callbacks.invoke(timerIndex); 
		}
    }
}
} // namespace

// -----------------------------------------------------------------------------
Timer::Timer(const uint32_t elapseTimeMs, void (*callback)(), const bool startTimer) noexcept
    : myHardware{Hardware::reserve()}
	, myMaxCount{maxCount(elapseTimeMs)}
	, myEnabled{false}
{
    if (!myHardware) { return; }
	TimerParam::timers[myHardware->index] = this;
	addCallback(callback);
	if (startTimer) { start(); }
}

// -----------------------------------------------------------------------------
Timer::~Timer() noexcept 
{ 
	removeCallback();
	TimerParam::timers[myHardware->index] = nullptr;
	Hardware::release(myHardware); 
}

// -----------------------------------------------------------------------------
bool Timer::isInitialized() const noexcept { return nullptr != myHardware; }

// -----------------------------------------------------------------------------
bool Timer::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
bool Timer::hasTimedOut() noexcept
{
    if (!myEnabled || (myHardware->counter < myMaxCount)) { return false; } 
	else 
	{
	    myHardware->counter = 0U;
		return true;
	}
}

// -----------------------------------------------------------------------------
uint32_t Timer::timeout_ms() const noexcept
{
	return utils::round<uint32_t>(myMaxCount * TimerParam::interruptIntervalMs);
}

// -----------------------------------------------------------------------------
void Timer::setTimeout_ms(const uint32_t timeout_ms) noexcept
{
    if (0U == timeout_ms) { stop(); }
    myMaxCount = maxCount(timeout_ms);
}

// -----------------------------------------------------------------------------
void Timer::start() noexcept
{ 
	if (0U == myMaxCount) { return; }
    utils::globalInterruptEnable();
	utils::set(*(myHardware->maskReg), myHardware->maskBit);
	myEnabled = true;
}

// -----------------------------------------------------------------------------
void Timer::stop() noexcept
{ 
    *(myHardware->maskReg) = 0U;
	myEnabled              = false; 
}

// -----------------------------------------------------------------------------
void Timer::toggle() noexcept 
{ 
	if (myEnabled) { stop(); }
	else { start(); }
}

// -----------------------------------------------------------------------------
void Timer::restart() noexcept
{
    myHardware->counter = 0U;
    start();
}

// -----------------------------------------------------------------------------
void Timer::addCallback(void (*callback)()) const noexcept
{ 
    TimerParam::callbacks.add(callback, myHardware->index);
}

// -----------------------------------------------------------------------------
void Timer::removeCallback() const noexcept
{
	TimerParam::callbacks.remove(myHardware->index);
}

// -----------------------------------------------------------------------------
bool Timer::increment() noexcept
{
	if (!myEnabled) { return false; }
	myHardware->counter++; 
	return true;
}

// -----------------------------------------------------------------------------
Timer::Hardware* Timer::Hardware::reserve() noexcept
{
	// Reserve a timer circuit if any is available, otherwise return a nullptr.
    for (uint8_t i{}; i < TimerParam::circuitCount; ++i)
	{
        if (nullptr == TimerParam::timers[i]) { return init(i); }
	}
	return nullptr;
}

// -----------------------------------------------------------------------------
void Timer::Hardware::release(Timer::Hardware* hardware) noexcept
{
	// Reset the associated hardware timer.
	if (hardware == nullptr) { return; }
    *(hardware->maskReg) = 0U;

	switch (hardware->index)
	{
		case TimerIndex::timer0:
		    TCCR0B = 0U;
			break;
		case TimerIndex::timer1:
		    TCCR1B = 0U;
		    OCR1A = 0U;
			break;
		case TimerIndex::timer2:
		    TCCR2B = 0U;
			break;
		default:
		    break;
	}
	// Release allocated resources.
	utils::deleteMemory(hardware);
}

// -----------------------------------------------------------------------------
Timer::Hardware* Timer::Hardware::init(const uint8_t timerIndex) noexcept
{
	// Allocate memory for the new timer hardware, return false is memory allocation failed.
    auto hardware{utils::newMemory<Hardware>()};
	if (!hardware) { return nullptr; }

	// Set the structure to refer to the corresponding timer circuit.
	switch (timerIndex)
	{
		case TimerIndex::timer0:
            hardware->maskReg = &TIMSK0;
            hardware->maskBit = TOIE0;
			TCCR0B            = ControlBits::timer0;
			break;
		case TimerIndex::timer1:
            hardware->maskReg = &TIMSK1;
            hardware->maskBit = OCIE1A;
			TCCR1B            = ControlBits::timer1;
		    OCR1A             = TimerParam::timer1MaxCount;
			break;
		case TimerIndex::timer2:
		    hardware->maskReg = &TIMSK2;
            hardware->maskBit = TOIE2;
			TCCR2B            = ControlBits::timer2;
			break;
		default:
		    utils::deleteMemory(hardware);
			return nullptr;
	}
	// Return the initialized circuit.
    hardware->counter = 0U;
	hardware->index   = timerIndex;
	return hardware;
}

// -----------------------------------------------------------------------------
ISR (TIMER0_OVF_vect) { invokeCallback(TimerIndex::timer0); }

// -----------------------------------------------------------------------------
ISR (TIMER1_COMPA_vect) { invokeCallback(TimerIndex::timer1); }

// -----------------------------------------------------------------------------
ISR (TIMER2_OVF_vect) { invokeCallback(TimerIndex::timer2); }

} // namespace atmega328p
} // namespace driver
