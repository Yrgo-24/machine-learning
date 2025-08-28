/**
 * @brief Int vector implementation details.
 */
#include <stdlib.h> // For functions malloc, realloc and free.
#include <stdio.h>  // For function printf.

#include "int_vector.h"

/**
 * @brief Int vector structure.
 */
typedef struct int_vector
{
    int* data;   // Pointer to dynamic field holding numbers.
    size_t size; // Size of the vector in number of elements.
} int_vector_t;

// -----------------------------------------------------------------------------
int_vector_t* int_vector_new(void)
{
    // Allocate memory for the new vector.
    int_vector_t* self = (int_vector_t*)(malloc(sizeof(int_vector_t)));

    // Return NULL if the vector couldn't be created.
    if (!self) { return NULL; }

    // Initialize the vector members (data and size).
    self->data = NULL;
    self->size = 0U;

    printf("New vector created!\n");

    // Return a pointer to the vector.
    return self;
}

// -----------------------------------------------------------------------------
void int_vector_delete(int_vector_t** self)
{
    // If the pointer if actually pointer at something, delete the memory.
    if (self && *self)
    {
        printf("Delete the vector!\n");

        // Empty the vector (without deleting the vector itself).
        int_vector_clear(*self);

        // Free the memory allocated for the vector instance.
        free(*self);

        // Set the associated pointer to NULL.
        *self = NULL;
    }
}

// -----------------------------------------------------------------------------
const int* int_vector_data(const int_vector_t* self)
{
    return self->data;
}

// -----------------------------------------------------------------------------
size_t int_vector_size(const int_vector_t* self)
{
    return self->size;
}

// -----------------------------------------------------------------------------
void int_vector_clear(int_vector_t* self)
{
    printf("Free resources allocated for the vector!\n");

    // Free allocated memory and set the pointer to NULL.
    free(self->data);

    // Reinitialize the member, the vector is now empty.
    self->data = NULL;
    self->size = 0U;
}

// -----------------------------------------------------------------------------
bool int_vector_empty(const int_vector_t* self)
{
    // Return true if the vector contains no elements.
    return 0U == self->size;
}

// -----------------------------------------------------------------------------
bool int_vector_push_back(int_vector_t* self, const int element)
{
    // Create a copy of the current field with space for one more element.
    int* copy = (int*)(realloc(self->data, sizeof(int) * (self->size + 1U)));

    // Return false if we couldn't enlarge the field.
    if (!copy) { return false; }

    // Add the new element to the back of the new larger field.
    // Also increment the number of elements in the field (from 3 - 4 for example).
    copy[self->size++] = element;

    // Set our data pointer to point at the new larger field.
    self->data = copy;

    // Return true to indicate success.
    return true;
}