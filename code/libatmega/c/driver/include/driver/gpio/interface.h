/**
 * @brief GPIO interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stdint.h>

/** GPIO virtual table implementation. */
typedef struct GpioVtable GpioVtable;

/**
 * @brief GPIO interface implementation.
 */
typedef struct GpioInterface 
{
    /** Pointer to the associated virtual table. */
    GpioVtable const *vptr;
} GpioInterface;

/**
 * @brief Enumeration representing supported GPIO modes.
 */
typedef enum GpioMode 
{
    GpioMode_Input,       /** GPIO input. */
    GpioMode_InputPullup, /** GPIO input with an internal pullup resistor enabled. */
    GpioMode_Output,      /** GPIO output. */
    GpioMode_Count,       /** The number of supported GPIO modes. */
} GpioMode;

/**
 * @brief GPIO virtual table implementation.
 */
typedef struct GpioVtable {
    /**
     * @brief Get the pin the GPIO is connected to.
     * 
     * @param[in] self Reference to the GPIO.
     * 
     * @return The pin the GPIO is connected to.
     */
    uint8_t (*pin)(const GpioInterface* self);

    /**
     * @brief Get the GPIO mode.
     * 
     * @param[in] self Reference to the GPIO.
     * 
     * @return The GPIO mode as an enumerator.
     */
    GpioMode (*mode)(const GpioInterface* self);

    /**
     * @brief Read the input of the GPIO.
     * 
     * @param[in] self Reference to the GPIO.
     * 
     * @return True if the input is high, otherwise false.
     */
    bool (*read)(const GpioInterface* self);

    /**
     * @brief Write value to the GPIO.
     * 
     * @param[in] self Reference to the GPIO.
     * @param[in] value The value to write (true = high, false = low).
     */
    void (*write)(const GpioInterface* self, bool value);

    /**
     * @brief Toggle the output of the GPIO.
     * 
     * @param[in] self Reference to the GPIO.
     */
    void (*toggle)(const GpioInterface* self);

    /**
     * @brief Enable/disable pin change interrupt for the GPIO.
     * 
     * @param[in] self Reference to the GPIO.
     * @param[in] enable Indicate whether to enable pin change interrupt for the device.
     */
    void (*enableInterrupt)(const GpioInterface* self, bool enable);

    /**
     * @brief Enable pin change interrupt for I/O port associated with the GPIO.
     * 
     * @param[in] self Reference to the GPIO.
     * @param enable Indicate whether to enable pin change interrupt for the I/O port.
     */
    void (*enableInterruptsOnPort)(const GpioInterface* self, bool enable);

    /**
     * @brief Delete the GPIO.
     *
     *        Release all resources allocated for the GPIO. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the GPIO to delete. The pointer will be set to nullptr.
     */
    void (*delete)(GpioInterface** self);
} GpioVtable;
