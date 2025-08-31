/**
 * @brief Logic implementation for the main system, revision 1.
 */
#pragma once

#include "logic/interface.h"

/**
 * @brief Create logic implementation for the main system.
 * 
 * @param[in] factory Factory to use for creation.
 * 
 * @return Pointer to the new logic implementation, or nullptr if the logic couldn't be created.
 */
LogicInterface* logic_rev1_new(FactoryInterface* factory);