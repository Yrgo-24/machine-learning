/**
 * @brief Logic interface implementation details for the main system.
 */
#include <stddef.h>

#include "logic/interface.h"
#include "logic/rev1.h"

// -----------------------------------------------------------------------------
LogicInterface* logic_new(const LogicRevision revision, FactoryInterface* const factory)
{
    // Return a nullptr if the factory is invalid.
    if (!factory) { return NULL; }

    // Return a pointer to the specified logic revision, or a nullptr if the revision is invalid.
    switch (revision)
    {
        case LogicRevision_Rev1:
            return logic_rev1_new(factory);
        default:
            return NULL;
    }
}
