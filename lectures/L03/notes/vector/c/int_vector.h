/**
 * @brief Int vector implementation.
 */
#pragma once

#include <stddef.h> // For data type size_t.

/**
 * @brief Implementation of data type bool as an enumeration.
 * 
 * @note This is just done for demonstration, please use <stdbool.h> in practice.
 */
typedef enum { false, true } bool;

/** 
 * @brief Int vector structure. 
 * 
 * @note This is a forward declaration. The implementation is hidden in the corresponding C file
 *       to keep the implementation details hidden. For this reason, we can only use int vector
 *       pointers, since `struct int_vector` is "incomplete" in this scope.
 */
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
 * @note A double pointer is used so that the actual pointer used is set to NULL; if a regular
 *       pointer is used, a copy of the pointer is passed. We would be able to delete the vector
 *       per se, but the pointer would still refer to the location the vector was located.
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
 * @brief Check if the vector is empty.
 * 
 * @param[in] self Pointer to the vector.
 * 
 * @return True if the vector is empty, false otherwise.
 */
bool int_vector_empty(const int_vector_t* self);

/**
 * @brief Push an element to the back of the vector.
 * 
 * @param[in] self Pointer to the vector in question.
 * @param[in] element The new element to push.
 */
bool int_vector_push_back(int_vector_t* self, const int element);