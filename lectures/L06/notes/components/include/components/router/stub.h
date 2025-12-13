/**
 * @brief Router stub implementation for testing.
 */
#pragma once

#include <cstdint>
#include <vector>

#include "components/router/interface.h"

namespace components::sw
{
/** Switch interface. */
class Interface;
} // namespace components::sw

namespace components::router
{
/**
 * @brief Stub/test implementation of a router.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Create a new router stub.
     */
    Stub() noexcept;

    /**
     * @brief Delete the router.
     * 
     * @note This destructor is defined to link to the destructor of the subclass.
     */
    ~Stub() noexcept override;

    /**
     * @brief Get the router model.
     * 
     * @return The router model as a string (as text).
     */
    const char* model() const noexcept override;

    /**
     * @brief Get the router ID.
     * 
     * @return The router ID.
     */
    std::uint32_t id() const noexcept override;

    /**
     * @brief Connect to the given switch.
     * 
     * @param[in] sw Reference to the switch to connect to.
     * 
     * @return True if the switch was connected to, false if the switch was already connected.
     */
    bool connect(components::sw::Interface& sw) override;

    /**
     * @brief Disconnect the given switch.
     * 
     * @param[in] sw Reference to the switch from which to disconnect.
     * 
     * @return True if the switch was disconnected, false if the switch wasn't found.
     */
    bool disconnect(sw::Interface& sw) noexcept override;

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    bool findSwitch(sw::Interface& sw) noexcept;

    /** Switches connected to the router. */
    std::vector<sw::Interface*> mySwitches;

    /** Switch ID. */
    const std::uint32_t myId;
};
} // namespace components::router