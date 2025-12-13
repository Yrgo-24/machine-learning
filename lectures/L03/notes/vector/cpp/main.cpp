/**
 * @brief Custom vector demonstration in C++.
 */
#include <iostream>    // For function std::cout.
#include <type_traits> // For std::is_arithmetic.

#include "vector.h"

namespace vector
{
/**
 * @brief Print the content of the given vector.
 * 
 * @tparam T The vector type. Must be of numerical type.
 * 
 * @param[in] data The vector to print.
 * @param[in] ostream The output stream to use (default = terminal print).
 */
template <typename T>
void print(const yrgo::Vector<T>& data, std::ostream& ostream = std::cout)
{
    // Generate a compiler error if the given vector isn't of numerical type.
    static_assert(std::is_arithmetic<T>::value, "Cannot print vectors of non-numerical types!");

    // Terminate the function if the vector is empty.
    if (data.empty()) { return; }

    ostream << "--------------------------------------------------------------------------------\n";

    // Print the vector content in the terminal
    for (const auto& i : data)
    {
        ostream << i << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

/**
 * @brief Demonstrate the usage of custom vectors in C++.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create vectors holding numbers.
    const yrgo::Vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const yrgo::Vector<double> v2{0.5, 1.5, 2.5, 3.5, 4.5};

    // Print the content of each vector in the terminal, then terminate the program.
    vector::print(v1);
    vector::print(v2);
    return 0;
}