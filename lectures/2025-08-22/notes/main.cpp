/**
 * @brief Example of using function templates with type traits.
 */
#include <iostream>
#include <type_traits>
#include <vector>

namespace vector
{
/**
 * @brief Assign numbers to the given vector.
 * 
 * @tparam T The vector type. Must be of numerical type.
 * 
 * @param[in] data The vector for which to assign.
 * @param[in] size The desired size of the vector.
 * @param[in] startVal The value of the first element (default = 0).
 * @param[in] stepVal The step value (default = 1).
 */
template <typename T>
void assign(std::vector<T>& data, const std::size_t size, const T startVal = T{0}, 
            const T stepVal = T{1})
{
     // Generate a compiler error if the given vector isn't of numerical type.
    static_assert(std::is_arithmetic<T>::value, "Cannot print vectors of non-numerical types!");
    
    // Create variable holding the next value to assign.
    auto nextVal{stepVal};

    // Resize the vector to the desired size.
    data.resize(size);

    // Assign values to the vector, incrementing by stepVal each time.
    for (auto& num : data)
    {
        num = nextVal;
        nextVal += stepVal;
    }
}

/**
 * @brief Print the content of the given vector.
 * 
 * @tparam T The vector type. Must be of numerical type.
 * 
 * @param[in] data The vector to print.
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
    // Create and print a vector holding ten integers 0 - 9.
    std::vector<int> v1{};
    vector::assign(v1, 10U);
    vector::print(v1);

    // Create and print a vector holding five floating-point numbers 0.5 - 8.5.
    std::vector<double> v2{};
    vector::assign(v2, 5U, 0.5, 2.0);
    vector::print(v2);
    return 0;
}