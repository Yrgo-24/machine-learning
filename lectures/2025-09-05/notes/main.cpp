/**
 * @brief Demonstration of interfaces with stub classes for a network prototype.
 */
#include "components/router/stub.h"
#include "components/sw/stub.h"

using namespace components;

/**
 * @brief Run the application. 
 * 
 * @param[in] router Router used for the network.
 * @param[in] sw1 First switch.
 * @param[in] sw2 Second switch.
 */
void runApplication(router::Interface& router, sw::Interface& sw1, sw::Interface& sw2)
{
    // Connect the switches to the router.
    router.connect(sw1);
    router.connect(sw2);

    // Add additional code to run the application here.
}

/**
 * @brief Run application with a router and two switches.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create the network components.
    router::Stub router{};
    sw::Stub sw1{}, sw2{};

    // Run the application, then terminate the program.
    runApplication(router, sw1, sw2);
    return 0;
}