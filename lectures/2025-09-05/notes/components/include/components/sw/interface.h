/**
 * @brief Switch interface.
 */
#pragma once

#include <cstdint>

namespace components::router
{
/** Router interface. */
class Interface;
} // namespace components::router

namespace components::sw
{
/**
 * @brief Switch interface.
 */
class Interface
{
public:
    /**
     * @brief Delete the switch.
     * 
     * @note This destructor is defined to link to the destructor of the subclass.
     */
    virtual ~Interface() noexcept = default;

    /**
     * @brief Get the switch model.
     * 
     * @return The switch model as a string (as text).
     */
    virtual const char* model() const = 0;

    /**
     * @brief Get the switch ID.
     * 
     * @return The switch ID.
     */
    virtual std::uint32_t id() const noexcept = 0;

    /**
     * @brief Check whether the switch is enabled.
     * 
     * @return True if the switch is enable, false otherwise.
     */
    virtual bool isEnabled() const = 0;

    /**
     * @brief Enable/disable the switch.
     * 
     * @param[in] enable True to enable the switch, false otherwise.
     */
    virtual void setEnabled(const bool enable) = 0;  

    /**
     * @brief Get the router the switch is connected to.
     * 
     * @return Pointer to the connected router, or nullptr if not connected.
     */
    virtual router::Interface* router() const  = 0;

    /**
     * @brief Connect the switch to the given router.
     * 
     * @param[in] router Reference to the router to connect to.
     */
    virtual void connectRouter(router::Interface& router) = 0;

    /**
     * @brief Disconnect the switch from its associated router.
     */
    virtual void disconnectRouter() = 0;
};
} // namespace components::sw