/**
 * @brief Serial interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/** Serial virtual table implementation. */
typedef struct SerialVtable SerialVtable;

/**
 * @brief Serial interface implementation.
 */
typedef struct SerialInterface {
    /** Pointer to the associated virtual table. */
    SerialVtable const *vptr;
} SerialInterface;

/**
 * @brief Serial virtual table implementation.
 */
typedef struct SerialVtable 
{
    /**
     * @brief Check if the serial device is enabled.
     * 
     * @param[in] self Reference to the serial device.
     * 
     * @return True if the serial device is enabled, otherwise false.
     */
    bool (*isEnabled)(const SerialInterface* self);

    /**
     * @brief Set the enablement of the serial device.
     * 
     * @param[in] self Reference to the serial device.
     * @param[in] enable True to enable the serial device, otherwise false.
     */
    void (*setEnabled)(SerialInterface* self, bool enable);

    /**
     * @brief Print formatted string in the terminal associated with the serial device.
     *
     *        If the formatted string contains format specifiers, the additional arguments 
     *        are formatted as inserted into the format string.
     *
     * @param[in] self Reference to the serial device.
     * @param[in] format Reference to string containing the text to print.
     * @param[in] ... Additional arguments to be formatted and inserted into the format string.
     *
     * @return The number of printed characters.
     */
    size_t (*printf)(SerialInterface* self, const char *format, ...);

    /**
     * @brief Delete the serial device.
     *
     *        Release all resources allocated for the device. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the device to delete. The pointer will be set to nullptr.
     */
    void (*delete)(SerialInterface** self);
} SerialVtable;
