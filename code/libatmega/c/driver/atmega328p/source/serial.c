/**
 * @brief Serial implementation details for ATmega328P.
 */
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <avr/io.h>

#include "driver/atmega328p/config.h"
#include "driver/atmega328p/serial.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** Buffer size in bytes. */
#define UART_BUFFER_SIZE 100U

/** UART transmission enablement bit mask. */
#define UART_ENABLE_BIT_MASK (uint8_t)(1UL << TXEN0)

/** UART character size bit mask. */
#define UART_CHAR_SIZE_BIT_MASK (uint8_t)((1UL << UCSZ00) | (1UL << UCSZ01))

/** Baud rate configuration value. */
#define UART_BAUD_RATE_VALUE (uint16_t)(103U)

/** UART data register empty bit mask. */
#define UART_DATA_REG_EMPTY_BIT_MASK (uint8_t)(1UL << UDRE0)

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const SerialVtable* const serialVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton serial instance. */
static SerialInterface myInstance = {NULL};

/** UART buffer. */
static char myBuffer[UART_BUFFER_SIZE] = {'\0'};

/*******************************************************************************
 * Serial-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static inline bool serial_isTransmissionOngoing(void) 
{ 
    // Return true if a transmission is ongoing (data register is not empty).
    return 0U == (UCSR0A & (1UL << UDRE0));
}

// -----------------------------------------------------------------------------
static void serial_transmitByte(const char byte)
{
    // Wait until the UART buffer is empty.
    while (serial_isTransmissionOngoing()) {}

    // Put the new byte in the UART buffer.
    UDR0 = byte;
}

// -----------------------------------------------------------------------------
static void serial_init(void)
{
    // Only initialize the implementation once.
    if (!myInstance.vptr)
    {
        // Set the vtable pointer of the singleton instance.
        myInstance.vptr = serialVtable_ptr();

        // Enable UART transmission.
        UCSR0B = UART_ENABLE_BIT_MASK;

        // Set the character size to eight bits.
        UCSR0C = UART_CHAR_SIZE_BIT_MASK;

        // The baud rate is set via the configuration.
        // This call is made to ensure that the configuration has been initialized.
        (void) (atmega328p_config_getInstance());

        // Transmit starting characters to ensure the first printed output starts at the 
        // beginning of a new line in the terminal.
        serial_transmitByte('\r');
        serial_transmitByte('\n');
    }
}

// -----------------------------------------------------------------------------
static size_t serial_transmitBuffer(void)
{
    // Declare variable for storing the number of printed characters.
    size_t byteCount = 0U;
    
    // Transmit the content of the UART buffer byte by byte.
    for (; myBuffer[byteCount] && (UART_BUFFER_SIZE > byteCount); ++byteCount) 
    { 
        serial_transmitByte(myBuffer[byteCount]); 
    }

    // Clear the UART buffer once the transmission is complete.
    myBuffer[0U] = '\0';

    // Return the number of printed bytes.
    return byteCount;
}

// -----------------------------------------------------------------------------
static bool serial_isEnabled(const SerialInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Return true if UART transmission is enabled.
    return UCSR0B & UART_ENABLE_BIT_MASK;
}

// -----------------------------------------------------------------------------
static void serial_setEnabled(SerialInterface* const self, const bool enable)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Enable/disable UART as specified.
    UCSR0B = enable ? UART_ENABLE_BIT_MASK : 0x00U;
}

// -----------------------------------------------------------------------------
static size_t serial_printf(SerialInterface* const self, const char* const format, ...)
{
    // Terminate the function if the serial device isn't enabled.
    if (!serial_isEnabled(self)) { return 0U; }

    // Create a variadic arguments list.
    va_list args;

    // Initialize the variable argument list with the format string as the last fixed parameter.
    va_start(args, format);

    // Format the arguments according to the format string, store the result in the UART buffer.
    vsnprintf(myBuffer, sizeof(myBuffer), format, args);

    // Clean up the variable argument list.
    va_end(args);

    // Transmit data stored in the UART buffer, return the number of printed characters.
    return serial_transmitBuffer();
}

// -----------------------------------------------------------------------------
static inline void serial_delete(SerialInterface** const self)
{
    // Deletion is not valid in this implementation, since the singleton instance is static.
    // This function exists only to satisfy the interface.
    (void) (self);
}

/*******************************************************************************
 * Serial vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const SerialVtable* const serialVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden serial functions.
    static const SerialVtable myVtable =
    {
        .isEnabled  = serial_isEnabled,
        .setEnabled = serial_setEnabled,
        .printf     = serial_printf,
        .delete     = serial_delete,
    };
    // Return a pointer to the vtable for access through the serial interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) serial interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
SerialInterface* atmega328p_serial_getInstance(void) 
{ 
    // Initialize serial transmission (only done once).
    serial_init();

    // Return a pointer to the singleton serial device instance.
    return &myInstance; 
}
