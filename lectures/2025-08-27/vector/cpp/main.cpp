#include <iostream>

#include "vector.h"

int main()
{
    yrgo::Vector<int> v1{1, 2, 3, 4, 5, 6};
    std::cout << "Size: " << v1.size() << "\n";

    for (const auto& num : v1)
    {
        std::cout << num << "\n";
    }
}