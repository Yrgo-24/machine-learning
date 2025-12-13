/**
 * @brief Simple driver implementation with C++ classes.
 */
#include "driver/led.h"
#include "driver/serial.h"

namespace
{
/**
 * @brief Run the program.
 * 
 * @param[in] led Reference to led to blink.
 */
void runProgram(driver::Led& led)
{
    constexpr uint16_t blinkSpeed_ms{100U};

    driver::Serial::getInstance().printf("Running the program, blinking the LED every %u ms!\n", blinkSpeed_ms);

    // Run the program continuously.
    while (1)
    {
        led.blink(blinkSpeed_ms);
    }
}
} // namespace

/**
 * @brief Initialize the hardware and run the program.
 */
int main(void)
{
    // Initialize a LED connected to pin 9.
    driver::Led led1{9U};

    // Run the program.
    runProgram(led1);
}
