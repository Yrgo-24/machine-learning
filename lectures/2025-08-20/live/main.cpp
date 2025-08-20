#include <iostream>
#include <vector>

/**
 * @brief Print content of the given vector.
 * 
 * @param[in] numbers The vector to print.
 */
void printVector(const std::vector<int>& numbers)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    std::cout << "----------------------------------------\n";

    // Print the numbers on separate lines.
    for (const auto& number : numbers)
    {
        std::cout << number << "\n";
    }
    std::cout << "----------------------------------------\n";
}

int main() 
{
    std::vector<int> v1{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> v2{};
    std::vector<int> v3{100, 200, 300};

    printVector(v1);
    printVector(v2);
    printVector(v3);
    return 0;
}