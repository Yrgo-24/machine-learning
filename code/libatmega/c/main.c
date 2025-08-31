/**
 * @brief System implementation.
 */
#include "driver/factory/interface.h"
#include "logic/interface.h"

// -----------------------------------------------------------------------------
int main(void)
{
     // Create the ATmega328P factory.
    FactoryInterface* const factory = factory_create(FactoryType_Atmega328p);

    // Create the logic for the main system, use revision 1.
    LogicInterface* const logic = logic_new(LogicRevision_Rev1, factory);

    // Run the main system if initialization succeeded.
    if (logic) { logic->vptr->run(logic); }

    // Return error code 1 if initialization failed.
    return 1;
}