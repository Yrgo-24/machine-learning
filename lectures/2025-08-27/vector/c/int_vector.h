/**
 * @brief Int vector implementation.
 */
#pragma once

#include <stddef.h> // For size_t.
/**
 * @brief Create a bool enumeration just for demonstration.
 */
typedef enum { false, true } bool;

/** Int vector structure. */
typedef struct int_vector int_vector_t;

/**
 * @brief Create a new empty vector.
 * 
 * @return Pointer to the new vector, or NULL if the vector couldn't be created.
 */
int_vector_t* int_vector_new(void);

/**
 * @brief Delete the vector and set the associated pointer to NULL.
 * 
 * @param[in] self Double pointer to the vector.
 */
void int_vector_delete(int_vector_t** self);

/**
 * @brief Get the data of the vector.
 * 
 * @param[in] self Pointer to the vector in question.
 * 
 * @return Pointer to the data of the vector.
 */
const int* int_vector_data(const int_vector_t* self);

/**
 * @brief Get the size of the vector.
 * 
 * @param[in] self Pointer to the vector in question.
 * 
 * @return The size of the vector.
 */
size_t int_vector_size(const int_vector_t* self);

/**
 * @brief Clear the vector.
 * 
 * @param[in] self Pointer to the vector to clear.
 */
void int_vector_clear(int_vector_t* self);

/**
 * @brief Push an element to the back of the vector.
 * 
 * @param[in] self Pointer to the vector in question.
 * @param[in] element The new element to push.
 */
bool int_vector_push_back(int_vector_t* self, const int element);