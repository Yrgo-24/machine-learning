/**
 * @brief Switch stub implementation details for testing.
 */
#include <iostream>
#include <sstream>
#include <string>

#include "components/router/interface.h"
#include "components/sw/stub.h"

namespace components::sw
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

// -----------------------------------------------------------------------------
std::string infoStr(const char* model, const std::uint32_t id)
{
    std::stringstream stream{};
    stream << model << " with ID " << id;
    return stream.str();
}
} // namespace

// -----------------------------------------------------------------------------
Stub::Stub() noexcept
    : myRouter{nullptr}
    , myId{getUniqueId()}
    , myEnabled{false}
{}

// -----------------------------------------------------------------------------
Stub::~Stub() noexcept { disconnectRouter(); }

// -----------------------------------------------------------------------------
const char* Stub::model() const noexcept { return "SwitchStub"; }

// -----------------------------------------------------------------------------
std::uint32_t Stub::id() const noexcept { return myId; }

// -----------------------------------------------------------------------------
bool Stub::isEnabled() const noexcept { return myEnabled; }

// -----------------------------------------------------------------------------
void Stub::setEnabled(const bool enable) noexcept
{
    myEnabled = enable;
    const auto action{myEnabled ? "enabled" : "disabling"};
    std::cout << infoStr(model(), id()) << " " << action << "!\n";
}

// -----------------------------------------------------------------------------
router::Interface* Stub::router() const noexcept { return myRouter; }

// -----------------------------------------------------------------------------
void Stub::connectRouter(router::Interface& router)
{
    // Connect to the given router.
    myRouter = &router;
    myRouter->connect(*this);
    std::cout << infoStr(model(), id()) << " is now connected to " 
        << infoStr(myRouter->model(), myRouter->id()) << "!\n";
}

// -----------------------------------------------------------------------------
void Stub::disconnectRouter() noexcept
{
    // Disconnect the associated router (if it exists).
    if (nullptr != myRouter)
    {
        std::cout << "Disconnecting " << infoStr(model(), id()) << " from " 
            << infoStr(myRouter->model(), myRouter->id()) << "!\n";
        auto router{myRouter};
        myRouter = nullptr;
        router->disconnect(*this);
    }
    else { std::cout << "No router is connected! to " << infoStr(model(), id()) << "!\n"; }
}
} // namespace components::sw