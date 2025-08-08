/**
 * @brief Demonstration of function templates with type traits in C++.
 */
#include <iostream>
#include <type_traits>
#include <vector>

namespace type_traits
{
/**
 * @brief Type trait for checking if specified type is of string type.
 * 
 * @tparam T The type to check.
 */
template <typename T>
struct is_string
{
    // Set to true if the given type is of string type, false otherwise.
    static const bool value{false};
};

/**
 * @brief Specialization of the type trait `is_string` for the type `const char*`.
 */
template <>
struct is_string<const char*>
{
    // Indicate that `const char*` is considered a string type.
    static const bool value{true};
};

/**
 * @brief Specialization of the type trait `is_string` for the type `std::string`.
 */
template <>
struct is_string<std::string>
{
    // Indicate that `std::string` is considered a string type.
    static const bool value{true};
};
} // namespace type_traits

namespace vector
{
/**
 * @brief Print numbers in the given vector. Each number is printed on a separate line.
 *
 * @tparam T The vector type.
 * 
 * @param[in] numbers Vector holding the numbers to print.
 * @param[in] ostream Output stream to use (default = terminal print).
 */
template <typename T>
void print(const std::vector<T>& data, std::ostream& ostream = std::cout)
{
    // Generate a compilation error if the vector type isn't of arithmetic or string type.
    static_assert(std::is_arithmetic<T>::value || type_traits::is_string<T>::value, 
        "Cannot print values other than numbers and text!");
    
    // Terminate the function if the vector is empty.
    if (data.empty()) { return; }

    // Print each number on a separate line.
    ostream << "--------------------------------------------------------------------------------\n";
    for (const auto& element : data)
    {
        ostream << element << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

/**
 * @brief Print content of vectors holding numbers and text via the vector::print function template.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create vectors holding numbers and text.
    const std::vector<int> v1{1, 2, 3};
    const std::vector<double> v2{0.5, 1.5, 2.5};
    const std::vector<const char*> v3{"C++", "programming", "is", "fun!"};
    
    // Print the content of each vector, then terminate the program.
    vector::print(v1);
    vector::print(v2);
    vector::print(v3);
    return 0;
}