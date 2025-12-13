/**
 * @brief Switch stub implementation for testing.
 */
#pragma once

#include <cstdint>

#include "components/sw/interface.h"

namespace components::sw
{
/**
 * @brief Switch stub implementation for testing.
 */
class Stub final : public Interface
{
public:
    /**
     * @brief Create a new stub switch.
     */
    Stub() noexcept;

    /**
     * @brief Delete the switch.
     */
    ~Stub() noexcept override;

    /**
     * @brief Get the switch model.
     * 
     * @return The switch model as a string (as text).
     */
    const char* model() const noexcept override;

    /**
     * @brief Get the switch ID.
     * 
     * @return The switch ID.
     */
    std::uint32_t id() const noexcept override;

    /**
     * @brief Check whether the switch is enabled.
     * 
     * @return True if the switch is enable, false otherwise.
     */
    bool isEnabled() const noexcept override;

    /**
     * @brief Enable/disable the switch.
     * 
     * @param[in] enable True to enable the switch, false otherwise.
     */
    void setEnabled(const bool enable) noexcept override;
    
    /**
     * @brief Get the router the switch is connected to.
     * 
     * @return Pointer to the connected router, or nullptr if not connected.
     */
    router::Interface* router() const noexcept override;

    /**
     * @brief Connect the switch to the given router.
     * 
     * @param[in] router Reference to the router to connect to.
     */
    void connectRouter(router::Interface& router) override;

    /**
     * @brief Disconnect the switch from its associated router.
     */
    void disconnectRouter() noexcept override;

    Stub(const Stub&)            = delete; // No copy constructor.
    Stub(Stub&&)                 = delete; // No move constructor.
    Stub& operator=(const Stub&) = delete; // No copy assignment.
    Stub& operator=(Stub&&)      = delete; // No move assignment.

private:
    /** Pointer to router the switch is connected to. */
    router::Interface* myRouter;

    /** Switch ID. */
    const std::uint32_t myId;

    /** Indicate whether the switch is enabled. */
    bool myEnabled;
};
} // namespace components::sw