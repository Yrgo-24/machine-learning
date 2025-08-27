/**
 * @brief Vector implementation.
 */
#pragma once

#include <cstdlib>  // For realloc and free.
#include <iostream> // For std::cout.

namespace yrgo
{
/**
 * @brief Vector implementation.
 * 
 * @tparam T The vector type.
 */
template <typename T>
class Vector
{
public:
    /** 
     * @brief Create a new empty vector.
     * 
     * @note This constructor is called upon creation without any input argument.
     *       yrgo::Vector<int> v1{};
    */
    Vector()
         : myData{nullptr}
         , mySize{0U}
    {
        std::cout << "New empty vector created!\n";
    }

    /**
     * @brief Create a new vector holding the given parameters.
     * 
     * @param[in] args The args to add.
     */
    template <typename...Args>
    Vector(const Args&... args)
        : Vector{}
    {
        // Push all given arguments.
        for (const auto arg : {args...})
        {
            pushBack(arg);
        }
    }

    /**
     * @brief Delete the vector.
     */
    ~Vector()
    {
        std::cout << "Deleting the vector!\n";

        // Clear the vector.
        clear();
    }

    /**
     * @brief Overload of operator [], used when we write v1[i].
     * 
     * @return The element at the given index.
     */
    T& operator[](const std::size_t index)
    {
        return myData[index];
    }

    T* begin() { return myData; }
    T* end() { return myData + mySize; }

    /**
     * @brief Get the data of the vector.
     * 
     * @return Pointer to the associated field.
     */
    const T* data() const { return myData; }

    /**
     * @brief Get the size of the vector.
     */
    std::size_t size() const { return mySize; }

    /**
     * @brief Check if the vector is empty.
     * 
     * @return True if the vector is empty, otherwise false.
     */
    bool empty() const { return 0U == mySize; }

    /**
     * @brief Clear the vector.
     */
    void clear() 
    {
        std::cout << "Clear the vector!\n";

        // Free allocated resources.
        std::free(myData);
        myData = nullptr;
        mySize = 0U;
    }

    /**
     * @brief Push an element to the back or the vector.
     * 
     * @param[in] element Reference to the element to add.
     * 
     * @return True if the element was added, false otherwise.
     */
    bool pushBack(const T& element)
    {
        // Create a copy of the current field with space for one more element.
        auto copy{static_cast<T*>(std::realloc(myData, sizeof(T) * (mySize + 1U)))};

        // Return false if we couldn't enlarge the field.
        if (!copy) { return false; }

        // Add the new element to the back of the new larger field.
        // Also increment the number of elements in the field (from 3 - 4 for example).
        copy[mySize++] = element;

        // Set our data pointer to point at the new larger field.
        myData = copy;

        // Return true to indicate success.
        return true;
    }


// Everything in the private segment is internal and cannot be used outside the class.
private:
    T* myData;          // Pointer to the field holding data.
    std::size_t mySize; // The size of the vector in number of elements.
};
} // namespace