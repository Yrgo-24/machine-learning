/**
 * @brief Vector implementation.
 */
#pragma once

#include <cstdlib>  // For functions std::realloc and std::free.
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
     * @note This constructor is called upon creation without any input argument, e.g.
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
     * @note This constructor is called upon creation without a list of input arguments, e.g.
     *       yrgo::Vector<int> v1{1, 2, 3, 4, 5};
     * 
     * @tparam Args Parameter pack holding input arguments.
     * 
     * @param[in] args Reference to list holding input arguments.
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
     * 
     * @note This is the destructor, which is a function being invoke automatically before the
     *       given vector instance is deleted. For this reason, we can ensure that resources
     *       allocated for the vector are released before deletion, avoiding memory leaks.
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
     * @note This operator is used when use the [] operator, e.g. v1[i], where `i` is the index.
     * 
     * @return The element at the given index.
     */
    T& operator[](const std::size_t index)
    {
        return myData[index];
    }

    /**
     * @brief Overload of operator [], used when we write v1[i].
     *  
     * @note This operator is used when use the [] operator, e.g. v1[i], where `i` is the index.
     * 
     * @return The element at the given index.
     */
    const T& operator[](const std::size_t index) const
    {
        return myData[index];
    }

    /**
     * @brief Get the start of the vector.
     * 
     * @note This method, along with `end`, makes it possible to use range-based for loops
     *       with this vector, e.g. for (const auto& i : v1).
     * 
     * @return Pointer to the start of the vector.
     */
    T* begin() { return myData; }

    /**
     * @brief Get the start of the vector.
     * 
     * @note This method, along with `end`, makes it possible to use range-based for loops
     *       with this vector, e.g. for (const auto& i : v1).
     * 
     * @return Pointer to the start of the vector.
     */
    const T* begin() const { return myData; }

    /**
     * @brief Get the end of the vector.
     * 
     * @note This method, along with `begin`, makes it possible to use range-based for loops
     *       with this vector, e.g. for (const auto& i : v1).
     * 
     * @return Pointer to the end of the vector.
     */
    T* end() { return myData + mySize; }

    /**
     * @brief Get the end of the vector.
     * 
     * @note This method, along with `begin`, makes it possible to use range-based for loops
     *       with this vector, e.g. for (const auto& i : v1).
     * 
     * @return Pointer to the end of the vector.
     */
    const T* end() const { return myData + mySize; }

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
     * @brief Check if the vector is empty.
     * 
     * @return True if the vector is empty, otherwise false.
     */
    bool empty() const { return 0U == mySize; }

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

    /**
     * @brief Resize the vector.
     * 
     * @param[in] newSize The new size of the vector.
     * @param[in] startVal The initial value of potentially new elements. The default value of the
     *                     given type is used as default.
     * 
     * @return True if the vector was resized, false otherwise.
     */
    bool resize(const std::size_t newSize, const T& startVal = T{})
    {
        // Get the old size.
        const auto oldSize{mySize};

        // Create a copy of the current field with the new size.
        auto copy{static_cast<T*>(std::realloc(myData, sizeof(T) * newSize))};

        // Return false if we couldn't set the field to the new size.
        if (!copy) { return false; }

        // Update the internal vector parameters.
        myData = copy;
        mySize = newSize;

        // Set the initial value of potentially new elements.
        for (std::size_t i{oldSize}; i < newSize; ++i)
        {
            myData[i] = startVal;
        }
        // Return true to indicate success.
        return true;
    }

private:
    /** Pointer to the field holding data. */
    T* myData;

    /** The size of the vector in number of elements. */
    std::size_t mySize;
};
} // namespace