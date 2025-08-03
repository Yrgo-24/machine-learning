/**
 * @brief Solution for exercise 1, 2025-08-20.
 */
#include <iostream>
#include <vector>

namespace vector
{
/**
 * @brief Get the smallest integer in given vector.
 * 
 * @param[in] numbers Vector holding numbers.
 * 
 * @return The smallest integer in the vector, or 0 if the vector is empty.
 */
int getSmallestInt(const std::vector<int>& numbers)
{
    // Return 0 if the vector is empty.
    if (numbers.empty()) { return 0; }

    // Consider the first value of the vector to be the smallest at start.
    auto smallest{numbers[0U]};
    
    // Compare each value in the vector, store the smallest one.
    for (const auto& number : numbers)
    {
        if (number < smallest) { smallest = number; }
    }
    // Return the smallest value found.
    return smallest;
}
} // namespace vector

/**
 * @brief Test the `vector::getSmallestInt` function.
 * 
 * @return 0 upon termination of the program.
 */
int main()
{
    // Create three vectors holding different amounts of integers for the test.
    const std::vector<int> v1{3, 6, 4, 8, 6, 8, -2, -1}; 
    const std::vector<int> v2{1, 2, -1, -1, 0};          
    const std::vector<int> v3{};   

    // Print the smallest value found in each vector.
    std::cout << "Smallest integer in v1: " << vector::getSmallestInt(v1) << "\n";
    std::cout << "Smallest integer in v2: " << vector::getSmallestInt(v2) << "\n";
    std::cout << "Smallest integer in v3: " << vector::getSmallestInt(v3) << "\n";
    return 0;
}