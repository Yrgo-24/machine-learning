/**
 * @brief Factory interface implementation details.
 */
#include <stddef.h>

#include "driver/atmega328p/factory.h"
#include "driver/factory/interface.h"

// -----------------------------------------------------------------------------
FactoryInterface* factory_create(const FactoryType type)
{
    // Check the given factory type, return a pointer to the corresponding factory instance.
    // Return a nullptr if an invalid factory type is specified.
    switch (type)
    {
        case FactoryType_Atmega328p:
            return atmega328p_factory_getInstance();
        default:
            return NULL;
    }
}