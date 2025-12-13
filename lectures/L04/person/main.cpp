/**
 * @brief Example program demonstrating a simple class with an associated enum class.
 */
#include <fstream>
#include <iostream>

#include "person.h"

/**
 * @brief Print personal data in the terminal and to a file named `person.txt`.
 * 
 * @return 0 on termination of the program.
 */
int main()
{
    // Create objects holding personal data.
    Person person1{"Marie Nilsson", 42U, Gender::Female, "Juristgatan 17", "Lawyer"};
    Person person2{"Sven Andersson", 37U, Gender::Male, "Kunskapsgatan 4", "Teacher", false};
    
    // Print stored personal data in the terminal.
    person1.print();
    person2.print();

    // Open file `person.txt` for writing.
    std::ofstream ofstream{"person.txt"};

    // Write the personal data to `person.txt`, then terminate the program.
    person1.print(ofstream);
    person2.print(ofstream);
    return 0;
}