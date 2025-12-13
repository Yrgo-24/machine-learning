/**
 * @brief Int vector demonstration in C.
 */
#include <stdio.h> // For function printf.

#include "int_vector.h"

/**
 * @brief Demonstrate the usage of an int vector in C.
 * 
 * @return 0 on termination of the program.
 */
int main(void)
{
    // Create a new vector.
    int_vector_t* v1 = int_vector_new();

    // Push ten integers to the back or the vector.
    for (int i = 0; i < 10; ++i)
    {
        int_vector_push_back(v1, i);
    }

    // Print the vector content.
    for (size_t i = 0U; i < int_vector_size(v1); ++i)
    {
        printf("%d\n", int_vector_data(v1)[i]);
    }

    // Free allocated memory to avoid memory leaks, then terminate the program.
    int_vector_delete(&v1);
    return 0;
}