/**
 * @brief Demonstration of vector usage and some I/O streams in C++.
 * 
 * \note The `vector::print` function is overloaded, i.e., more than one instance is present.
 *       This demonstrates that it's possible for functions to have the same name in C++, as long 
 *       as the input arguments are different. For this reason, there's no problem for the compiler 
 *       to know which of the functions to invoke for a given call.
 */
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace vector
{
/**
 * @brief Assign numbers to the given vector.
 * 
 * @param[in] number The vector for which to assign.
 * @param[in] size The desired vector size.
 * @param[in] startVal The starting value (default = 0).
 * @param[in] stepVal The step value (default = 1).
 */
void assign(std::vector<int>& numbers, const std::size_t size,  
            const int startVal = 0, const int stepVal = 1)
{
    // Create variable for assigning numbers.
    auto nextVal{startVal};

    // Resize the vector to the desired size.
    numbers.resize(size);

    // Iterate through the vector, assign the numbers.
    for (auto& num : numbers)
    {
        // Assign the next value to the vector.
        num = nextVal;

        // Update to the next value to assign.
        nextVal += stepVal;
    }
}

/**
 * @brief Print integers held by the given vector.
 * 
 * @param[in] numbers The vector to print.
 * @param[in] ostream The output stream to use (default = terminal print).
 */
void print(const std::vector<int>& numbers, std::ostream& ostream = std::cout)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    ostream << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (const auto& number : numbers)
    {
        ostream << number << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}

/**
 * @brief Print strings held by the given vector.
 * 
 * @param[in] text The vector to print.
 * @param[in] ostream The output stream to use (default = terminal print).
 */
void print(const std::vector<std::string>& text, std::ostream& ostream = std::cout)
{
    // Terminate the function if the vector is empty.
    if (text.empty()) { return; }

    ostream << "--------------------------------------------------------------------------------\n";

    // Print the numbers on separate lines.
    for (const auto& str: text)
    {
        ostream << str << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}


/**
 * @brief Read the content of the given file.
 * 
 * @param[in] filePath Path to the file to read.
 * @param[in, out] lines Vector holding lines of text retrieved from the file.
 * 
 * @return True on success, false on failure.
 */
bool readFile(const std::string& filePath, std::vector<std::string>& lines)
{
    // Try to open the file for reading, return false on failure.
    std::ifstream ifstream{filePath};
    if (!ifstream) { return false; }

    // Ensure that the given buffer is empty before reading.
    lines.clear();

    // Create string for retrieving each line.
    std::string line{};

    // Read each line in the file one by one.
    while (std::getline(ifstream, line))
    {
        // Push the current line to the back of the vector (unless the line is empty).
        if (!line.empty()) { lines.push_back(line); }
    }
    // Return true if at least one line was retrieved.
    return !lines.empty();
}
} // namespace vector

/**
 * @brief Demonstration of vector usage in C++.
 * 
 * @return 0 on termination of the program.
 */
int main() 
{
    // Implement the file path as a compile-time constant.
    constexpr auto filePath{"numbers.txt"};

    // Assign 20 values to a vector by invoking the `vector::assign` function.
    std::vector<int> v1{};

    // Assign 20 values to the vector, start value = 0, step value = 1.
    vector::assign(v1, 20U);

    // Print the vector content in the terminal.
    vector::print(v1);

    // Open file "numbers.txt" for printing.
    std::ofstream ofstream{filePath};

    // Write the vector content to `numbers.txt`.
    vector::print(v1, ofstream);

    // Close the file stream before trying to read the file.
    ofstream.close();

    // Create vector for retrieving the file content line by line.
    std::vector<std::string> retrievedLines{};
    
    // If any content was retrieved, print it in the terminal.
    if (vector::readFile(filePath, retrievedLines))
    {
        std::cout << "Data retrieved from the file " << filePath << ":\n";
        vector::print(retrievedLines);
    }
    // Terminate the program, return 0 to indicate success.
    return 0;
}