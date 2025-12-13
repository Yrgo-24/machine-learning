/**
 * @brief Router interface.
 */
#pragma once

#include <cstdint>

namespace components::sw
{
/** Switch interface. */
class Interface;
} // namespace components::sw

namespace components::router
{
/**
 * @brief Router interface.
 */
class Interface
{
public:
    /**
     * @brief Delete the router.
     * 
     * @note This destructor is defined to link to the destructor of the subclass.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the router model.
     * 
     * @return The router model as a string (as text).
     */
    virtual const char* model() const = 0;

    /**
     * @brief Get the router ID.
     * 
     * @return The router ID.
     */
    virtual std::uint32_t id() const noexcept = 0;

    /**
     * @brief Connect to the given switch.
     * 
     * @param[in] sw Reference to the switch to connect to.
     * 
     * @return True if the switch was connected to, false if the switch was already connected.
     */
    virtual bool connect(sw::Interface& sw) = 0;

    /**
     * @brief Disconnect the given switch.
     * 
     * @param[in] sw Reference to the switch from which to disconnect.
     * 
     * @return True if the switch was disconnected, false if the switch wasn't found.
     */
    virtual bool disconnect(sw::Interface& sw) = 0;
};
} // namespace components::router