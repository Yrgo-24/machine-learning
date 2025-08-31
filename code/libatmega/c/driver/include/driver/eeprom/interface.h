/**
 * @brief EEPROM (Electrically Erasable Programmable Read-Only Memory) interface implementation.
 */
#pragma once

#include <stdbool.h>
#include <stddef.h>

/** EEPROM virtual table implementation. */
typedef struct EepromVtable EepromVtable;

/**
 * @brief EEPROM interface implementation.
 */
typedef struct EepromInterface 
{
    /** Pointer to the associated virtual table. */
    EepromVtable const *vptr;
} EepromInterface;

/**
 * @brief EEPROM virtual table implementation.
 */
typedef struct EepromVtable 
{
    /**
     * @brief Write data to EEPROM.
     * 
     * @param[in] self Reference to the EEPROM.
     * @param[in] address The address at which to write the first byte.
     * @param[in] data Reference to the data to write. Can be an array or a variable.
     * @param[in] byteCount The number of bytes to write.
     * 
     * @return True if the data was written to EEPROM, otherwise false.
     */
    bool (*write)(EepromInterface* self, size_t address, const void *data, size_t byteCount);

    /**
     * @brief Read data from EEPROM.
     * 
     * @param[in] self Reference to the EEPROM.
     * @param[in] address The address at which to read the first byte.
     * @param[out] buffer Pointer to the buffer where the read data will be stored. 
     *                    Can be an array or a variable.
     * @param[in] byteCount The number of bytes to read.
     * 
     * @return True if data was read from EEPROM, otherwise false.
     */
    bool (*read)(const EepromInterface* self, size_t address, void *buffer, size_t byteCount);

    /**
     * @brief Clear the entire EEPROM memory region.
     * 
     * @param[in] self Reference to the EEPROM.
     */
    void (*clear)(EepromInterface* self);

    /**
     * @brief Delete the EEPROM.
     *
     *        Release all resources allocated for the EEPROM. Set the associated pointer to nullptr.
     *
     * @param[in] self Double pointer to the EEPROM to delete. The pointer will be set to nullptr.
     */
    void (*delete)(EepromInterface* *self);
} EepromVtable;
