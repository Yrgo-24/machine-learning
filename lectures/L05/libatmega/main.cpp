/**
 * @brief Simple driver implementation with C++ classes.
 */
#include "driver/button.h"
#include "driver/led.h"
#include "driver/serial.h"

namespace
{
/**
 * @brief Run the program.
 * 
 *        Blink the given LED whenever the given button is pressed.
 * 
 * @param[in] led Reference to the LED to blink.
 * @param[in] button Reference to button used to blink the LED.
 * @param[in] serial Serial device user to print data.
 * @param[in] blinkSpeed_ms The LED blink speed (default = 100 ms).
 */
void runProgram(driver::LedInterface& led, driver::ButtonInterface& button, 
                driver::SerialInterface& serial, const uint16_t blinkSpeed_ms = 100U)
{
    serial.printf("Starting the program!\n");
    serial.printf("Blinking the LED every %u ms when the button is pressed!\n", blinkSpeed_ms);

    // Create a variable to implement edge detection.
    bool lastInput{false};

    // Run the program continuously.
    while (1)
    {
        // Read the input from the button.
        const auto newInput{button.input()};

        // Log button events.
        if (newInput && !lastInput) { serial.printf("Button pressed, blinking the LED!\n"); }
        else if (!newInput && lastInput) { serial.printf("Button pressed, disabling the LED!\n"); }

        // Blink the LED if the button is pressed, independent of the edge.
        if (newInput) { led.blink(blinkSpeed_ms); }

        // Save the last input for next detection.
        lastInput = newInput;
    }
}
} // namespace

/**
 * @brief Initialize the hardware and run the program.
 */
int main()
{
    // Initialize an LED connected to pin 9.
    driver::Led led1{9U};

    // Initialize a button connected to pin 13.
    driver::Button button1{13U};

    // Run the program.
    runProgram(led1, button1, driver::Serial::getInstance());
}
