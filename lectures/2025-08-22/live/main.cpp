/**
 * @brief Example of using function templates with type traits.
 */
#include <iostream>
#include <type_traits>
#include <vector>

namespace vector
{
/**
 * @brief Print the content of the given vector.
 * 
 * @tparam T The vector type. Must be of numerical type.
 * 
 * @param[in] data The vector in question.
 * @param[in] ostream The output stream to use (default = terminal print).
 */
template <typename T>
void print(const std::vector<T>& data, std::ostream& ostream = std::cout)
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
 * @brief Print content of vectors using the `vector::print` function template.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create a vector holding ten integers 0 - 9.
    std::vector<int> v1{0, 1, 2, 3, 4, 5, 6, 7, 8, 9};

    // Create a vector holding five floating-point numbers.
    std::vector<double> v2{0.5, 2.5, 4.5, 6.5, 8.5};

    // Print the content of each vector.
    vector::print(v1);
    vector::print(v2);
    return 0;
}