/**
 * @brief Router stub implementation details for testing.
 */
#include <vector>

#include "components/router/stub.h"
#include "components/sw/interface.h"

namespace components::router
{
namespace
{
// -----------------------------------------------------------------------------
std::uint32_t getUniqueId() noexcept
{
    // Create static local variable holding the next ID to provide.
    static std::uint32_t id{};

    // Return a unique id.
    return id++;
}
} // namespace

// -----------------------------------------------------------------------------
Stub::Stub() noexcept
    : mySwitches{}
    , myId{getUniqueId()}
{}

// -----------------------------------------------------------------------------
Stub::~Stub() noexcept
{
    // Disconnect all switches.
    for (auto& sw : mySwitches) 
    { 
        if (this == sw->router()) { sw->disconnectRouter(); }
    }
}

// -----------------------------------------------------------------------------
const char* Stub::model() const noexcept { return "RouterStub"; }

// -----------------------------------------------------------------------------
std::uint32_t Stub::id() const noexcept { return myId; }

// -----------------------------------------------------------------------------
bool Stub::connect(components::sw::Interface& sw)
{
    // Return false if the switch is already connected to the router.
    if (findSwitch(sw)) { return false; }
    // Connect to the switch and store it internally.
    mySwitches.push_back(&sw);
    sw.connectRouter(*this);

    // Enable the router, then return true to indicate success.
    sw.setEnabled(true);
    return true;
}

// -----------------------------------------------------------------------------
bool Stub::disconnect(sw::Interface& sw) noexcept
{
    // Iterate through the connected switches.
    for (auto it{mySwitches.begin()}; it != mySwitches.end(); ++it)
    {
        auto mySw{*it};

        // Disconnect to the given switch if found, return true to indicate success.
        if (mySw == &sw)
        {
            if (this == mySw->router()) { mySw->disconnectRouter(); }
            mySwitches.erase(it);
            return true;
        }
    }
    // Return false if the switch wasn't found.
    return false;
}

// -----------------------------------------------------------------------------
bool Stub::findSwitch(sw::Interface& sw) noexcept
{
    // Iterate through the connected switches, return true if found, false otherwise.
    for (const auto& mySw : mySwitches)
    {
        if (mySw == &sw) { return true; }
    }
    return false;
}
} // namespace components::router