/**
 * @brief EEPROM implementation details for ATmega328P.
 */
#include <stdbool.h>
#include <stddef.h>

#include <avr/io.h>

#include "driver/atmega328p/eeprom.h"

/*******************************************************************************
 * Local macro definitions.
 ******************************************************************************/
/** The size of the EEPROM memory in bytes. */
#define EEPROM_SIZE_BYTES 1024U

/** Default value for each byte in EEPROM. */
#define EEPROM_DEFAULT_VALUE (uint8_t)(0XFFU)

/*******************************************************************************
 * Function declarations.
 ******************************************************************************/
static const EepromVtable* const eepromVtable_ptr(void);

/*******************************************************************************
 * Static instances used for the implementation.
 ******************************************************************************/
/** Singleton EEPROM instance. */
static EepromInterface myInstance = {NULL};

/*******************************************************************************
 * EEPROM-specific functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static void eeprom_init(void)
{
    // Only initialize the implementation once.
    if (!myInstance.vptr)
    {
        // Set the vtable pointer of the singleton instance.
        myInstance.vptr = eepromVtable_ptr();
    }
}

// -----------------------------------------------------------------------------
static inline bool eeprom_isAddressValid(const size_t address)
{
    // Return true if the given address is within the EEPROM memory region.
    return EEPROM_SIZE_BYTES > address;
}

// -----------------------------------------------------------------------------
static void eeprom_writeByte(const uint16_t address, const uint8_t data)
{
    // Wait until EEPROM is ready for another operation.
    while ((EECR & (1UL << EEPE)));
    
    // Set the destination address.
    EEAR = address;

    // Set the byte to write.
    EEDR = data;

    // Disable interrupts globally during the write sequence.
    asm("CLI");
    
    // Execute the EEPROM write sequence (must be done withing four clock cycles).
    EECR |= (1UL << EEMPE);
    EECR |= (1UL << EEPE);

    // Enable interrupts globally after the write sequence is complete.
    asm("SEI");
}

// -----------------------------------------------------------------------------
static uint8_t eeprom_readByte(const uint16_t address)
{
    // Wait until EEPROM is ready for another operation.
    while ((EECR & (1UL << EEPE)));
    
    // Set the address to read from.
    EEAR = address;

    // Perform EEPROM read.
    EECR |= (1UL << EERE);

    // Return the data retrieved from EEPROM.
    return EEDR;
}

// -----------------------------------------------------------------------------
static bool eeprom_write(EepromInterface* const self, const size_t address, 
    const void* const data, const size_t byteCount)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Return false if the given address is invalid.
    if (!eeprom_isAddressValid(address + byteCount)) { return false; }

    // Treat the data to write as an array of bytes.
    const uint8_t *buffer = (const uint8_t*)(data);

    // Iterate through the buffer byte by byte.
    for (size_t i = 0U; i < byteCount; ++i)
    {
        // Write each byte to EEPROM.
        eeprom_writeByte(address + i, buffer[i]);
    }
    // Return true to indicate success.
    return true;
}

// -----------------------------------------------------------------------------
static bool eeprom_read(const EepromInterface* const self, const size_t address, 
                        void* const buffer, const size_t byteCount)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

     // Return false if the given address is invalid.
    if (!eeprom_isAddressValid(address + byteCount)) { return false; }

    // Treat the buffer as an array of bytes.
    uint8_t* const data = (uint8_t*)(buffer);

    // Iterate through the EEPROM byte by byte.
    for (size_t i = 0U; i < byteCount; ++i)
    {
        // Read each byte from EEPROM.
        data[i] = eeprom_readByte(address + i);
    }
    // Return true to indicate success.
    return true;
}

// -----------------------------------------------------------------------------
static void eeprom_clear(EepromInterface* const self)
{
    // Suppress unused parameter warning for self, as it is not needed in this implementation.
    (void) (self);

    // Iterate through the entire EEPROM region.
    for (size_t address = 0U; address < EEPROM_SIZE_BYTES; ++address)
    {
        // Clear each byte in EEPROM.
        eeprom_writeByte(address, EEPROM_DEFAULT_VALUE);
    }
}

// -----------------------------------------------------------------------------
static inline void eeprom_delete(EepromInterface** const self)
{
    // Deletion is not valid in this implementation, since the singleton instance is static.
    // This function exists only to satisfy the interface.
    (void) (self);
}

/*******************************************************************************
 * EEPROM vtable-related functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
static const EepromVtable* const eepromVtable_ptr(void)
{
    // Create a static vtable holding pointers to the overridden EEPROM functions.
    static const EepromVtable myVtable =
    {
        .write  = eeprom_write,
        .read   = eeprom_read,
        .clear  = eeprom_clear,
        .delete = eeprom_delete,
    };
    // Return a pointer to the vtable for access through the EEPROM interface.
    return &myVtable;
}

/*******************************************************************************
 * External (public) EEPROM interface functions.
 ******************************************************************************/
// -----------------------------------------------------------------------------
EepromInterface* atmega328p_eeprom_getInstance(void) 
{ 
    // Initialize the EEPROM instance (only done once).
    eeprom_init();

    // Return a pointer to the singleton EEPROM instance.
    return &myInstance; 
}
