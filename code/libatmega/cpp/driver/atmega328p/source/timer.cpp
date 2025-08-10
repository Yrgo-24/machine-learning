/**
 * @brief Implementation details of hardware timer driver.
 */
#include "array.h"
#include "callback_array.h"
#include "timer.h" 
#include "utils.h"

namespace driver 
{
namespace atmega328p
{

/**
 * @brief Structure for implementation of timer hardware.
 */
struct Timer::Hardware 
{
	volatile uint32_t counter; // Hardware counter.
	volatile uint8_t* maskReg; // Pointer to mask register.
	uint8_t maskBit;           // Mask bit for timer interrupt.
	uint8_t index;             // Timer index.    

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
	static constexpr uint8_t timer0{0U}; // Index for Timer 0.
	static constexpr uint8_t timer1{1U}; // Index for Timer 1.
	static constexpr uint8_t timer2{2U}; // Index for Timer 2.
};

/**
 * @brief Structure holding control bits of timer circuits.
 */
struct ControlBits 
{
	static constexpr uint8_t timer0{(1 << CS01)};                // Control bits for Timer 0.
	static constexpr uint8_t timer1{(1 << CS11) | (1 << WGM12)}; // Control bits for Timer 1.
	static constexpr uint8_t timer2{(1 << CS21)};                // Control bits for Timer 2.
};

/**
 * @brief Structure holding ATmega328P timer parameter.
 */
struct TimerParam
{
	static constexpr uint8_t circuitCount{3U};          // The number of available timer circuits.
	static constexpr uint16_t timer1MaxCount{256U};     // Max count for 16-bit timer Timer 1.   
	static constexpr double interruptIntervalMs{0.128}; // Time between each timer interrupt in ms.
	static Timer* timers[circuitCount];                 // Array holding pointers to TimerParam::timers.
	static CallbackArray<circuitCount> callbacks;       // Array holding pointers to callbacks.
};

Timer* TimerParam::TimerParam::timers[TimerParam::circuitCount]{};          
CallbackArray<TimerParam::circuitCount> TimerParam::callbacks{};

// -----------------------------------------------------------------------------
constexpr uint32_t maxCount(const uint32_t elapseTimeMs) noexcept
{
	return elapseTimeMs > 0U ? 
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

        if (timer->hasElapsed()) 
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
Timer::Timer(Timer&& other) noexcept
    : myHardware{other.myHardware}
    , myMaxCount{other.myMaxCount}
    , myEnabled{other.myEnabled}
{
    other.myHardware    = nullptr;
    other.myMaxCount    = 0U;
    other.myEnabled     = false;
}

// -----------------------------------------------------------------------------
Timer& Timer::operator=(Timer&& other) noexcept
{
    if (&other != this)
	{
		myHardware    = other.myHardware;
		myMaxCount    = other.myMaxCount;
		myEnabled     = other.myEnabled;

		other.myHardware    = nullptr;
		other.myMaxCount    = 0U;
		other.myEnabled     = false;
	}
    return *this;
}

// -----------------------------------------------------------------------------
bool Timer::isInitialized() const noexcept { return myHardware != nullptr; }

// -----------------------------------------------------------------------------
bool Timer::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
bool Timer::hasElapsed() noexcept
{
    if (!myEnabled || (myHardware->counter < myMaxCount)) { return false; } 
	else 
	{
	    myHardware->counter = 0U;
		return true;
	}
}

// -----------------------------------------------------------------------------
uint32_t Timer::elapseTimeMs() const noexcept
{
	return utils::round<uint32_t>(myMaxCount * TimerParam::interruptIntervalMs);
}

// -----------------------------------------------------------------------------
void Timer::setElapseTimeMs(const uint32_t elapseTimeMs) noexcept
{
    if (elapseTimeMs == 0U) { stop(); }
    myMaxCount = maxCount(elapseTimeMs);
}

// -----------------------------------------------------------------------------
void Timer::start() noexcept
{ 
	if (myMaxCount == 0U) { return; }
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
    for (uint8_t i = 0U; i < TimerParam::circuitCount; ++i)
	{
        if (TimerParam::timers[i] == nullptr) { return init(i); }
	}
	return nullptr;
}

// -----------------------------------------------------------------------------
void Timer::Hardware::release(Timer::Hardware* hardware) noexcept
{
	if (hardware == nullptr) { return; }
    *(hardware->maskReg) = 0U;

	switch (hardware->index)
	{
		case TimerIndex::timer0:
		    TCCR0B = 0U;
			break;
		case TimerIndex::timer1:
		    TCCR1B = 0U;
		    OCR1A  = 0U;
			break;
		case TimerIndex::timer2:
		    TCCR2B = 0U;
			break;
		default:
		    break;
	}
	utils::deleteMemory(hardware);
}

// -----------------------------------------------------------------------------
Timer::Hardware* Timer::Hardware::init(const uint8_t timerIndex) noexcept
{
    Hardware* hardware{utils::newMemory<Hardware>()};
	if (!hardware) { return nullptr; }

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