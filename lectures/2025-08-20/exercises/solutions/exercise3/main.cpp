/**
 * @brief Solution for exercise 3, 2025-08-20.
 */
#include <fstream>
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

/**
 * @brief Erase the smallest integer held by specified vector.
 *        Potential duplicates of the smallest integer are also removed.
 * 
 * @param[in] numbers Vector holding numbers.
 */
void eraseSmallestInt(std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Get the smallest number in the vector.
    const auto smallest{vector::getSmallestInt(numbers)};

    // Run a loop continuously until all occurrences of the smallest number are removed.
    while (1)
    {
        // Search through the smallest number in the vector and erase each occurrence.
        for (auto number{numbers.begin()}; number < numbers.end(); ++number) 
        {
            if (smallest == *number) { numbers.erase(number); }
        }
        // Terminate the loop once no occurrence of the smallest number is present in the vector.
        if (smallest != vector::getSmallestInt(numbers)) { break; }
    }
}
} // namespace vector

/**
 * @brief Remove the smallest integer (including duplicates) in a given vector.
 *        The vector content is printed in the terminal and to a file named "numbers.txt".
 * 
 * @return 0 upon termination of the program.
 */
int main()
{
    // Create a vector holding numbers.
    std::vector<int> numbers{1, 2, -5, 5, 4, 8, 9, 0, -3, -5, -5, -5, -2};

    // Open the file `numbers.txt` for writing.
    std::ofstream ostream{"numbers.txt"};

    // Erase the smallest number in the vector.
    vector::eraseSmallestInt(numbers);

    // Print the numbers in the terminal.
    vector::print(numbers);

    // Write the numbers to `numbers.txt` and terminate the program.
    vector::print(numbers, ostream);
    return 0;
}