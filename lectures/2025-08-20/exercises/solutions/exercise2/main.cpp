/**
 * @brief Solution for exercise 2, 2025-08-20.
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

/**
 * @brief Print the content of given vector.
 * 
 * @param[in] numbers Vector holding numbers.
 * @param[in] ostream Reference to the output stream to use (default = terminal print).
 */
void print(const std::vector<int>& numbers, std::ostream& ostream = std::cout)
{
    // Print an opening separator line.
    ostream << "--------------------------------------------------------------------------------\n";
    
    // Print each number in given vector on a separate line.
    for (const auto& number : numbers) { ostream << number << "\n"; }
    
    // Print a trailing separator line.
    ostream << "--------------------------------------------------------------------------------\n\n";
}
} // namespace vector

/**
 * @brief Remove the smallest integer held by a specific vector.
 * 
 * @return 0 upon termination of the program.
 */
int main()
{
    // Create a vector holding integers.
    std::vector<int> numbers{1, 2, 5, 4, 8, 9, 0, -3, -5, -2};

    // Get the smallest number in the vector.
    const auto smallest{vector::getSmallestInt(numbers)};

    // Iterate through the vector and remove the smallest number.
    // Note: Removing an element from the vector invalidates the iterator, hence the loop will
    // terminate immediately after removing the first occurrence of the smallest number.
    for (auto number{numbers.begin()}; number < numbers.end(); ++number) 
    {
        if (smallest == *number) { numbers.erase(number); }
    }
    // Print the vector content and terminate the program.
    vector::print(numbers);
    return 0;
}