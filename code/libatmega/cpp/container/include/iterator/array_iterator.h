/**
 * @brief Implementation of array iterators.
 * 
 * @note This file is included in <array.h> and shall not be included directly.
 */
#pragma once

namespace container
{

/**
 * @brief Implementation of mutable array iterators.
 *
 * @tparam T    The array type.
 * @tparam Size The array size. Must be greater than 0.
 */
template <typename T, size_t Size>
class Array<T, Size>::Iterator
{
    static_assert(Size > 0U, "Array size must be greater than 0!");

public:
    /**
     * @brief Create empty iterator.
     */
    explicit Iterator() noexcept
        : myData{nullptr} {}

    /**
     * @brief Create iterator pointing at given data.
     *
     * @param data Reference to data the iterator is set to point at.
     */
    Iterator(T& data) noexcept
        : myData{&data} {}

    /**
     * @brief Create iterator pointing at given data.
     *
     * @param data Pointer to data the iterator is set to point at.
     */
    Iterator(T* data) noexcept
        : myData{data} {}

    /**
     * @brief Delete iterator.
     */
    ~Iterator() noexcept = default;

    /**
     * @brief Create copy of another iterator.
     * 
     * @param other Reference to iterator to copy.
     */
    Iterator(const Iterator& other) noexcept
        : myData{other.myData} {}

    /**
     * @brief Create iterator overtaking resources from another iterator.
     * 
     *        The other iterator is set to null after the move operation is completed.
     * 
     * @param other Reference to iterator to move resources from.
     */
    Iterator(Iterator&& other) noexcept
        : myData{other.myData} { other.myData = nullptr; }

    /**
     * @brief Copy another iterator.
     * 
     * @param other Reference to iterator to copy.
     * 
     * @return Reference to this iterator.
     */
    Iterator& operator=(const Iterator& other) noexcept 
    { 
        if (this != &other) { myData = other.myData; }
        return *this;
    }

    /**
     * @brief Move resources from another iterator.
     * 
     *        The other iterator is set to null after the move operation is completed.
     * 
     * @param other Reference to iterator to move resources from.
     * 
     * @return Reference to this iterator.
     */
    Iterator& operator=(Iterator&& other) noexcept
    {
        if (this != &other)
        {
            myData       = other.myData;
            other.myData = nullptr;
        }
        return *this;
    }

    /**
     * @brief Increment the address the iterator is pointing at (prefix operator).
     * 
     * @return Reference to this iterator.
     */
    Iterator& operator++() noexcept 
    { 
        ++myData; 
        return *this;
    }

    /**
     * @brief Decrement the address the iterator is pointing at (prefix operator).
     * 
     * @return Reference to this iterator.
     */
    Iterator& operator--() noexcept 
    { 
        --myData; 
        return *this;
    }

    /**
     * @brief Increment the address the iterator is pointing at (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    Iterator operator++(int) noexcept 
    { 
        auto previous{*this};
        ++myData;
        return previous;
    }

    /**
     * @brief Decrement the address the iterator is pointing at (postfix operator).
     *
     * @return The previous state of this iterator.
     */
    Iterator operator--(int) noexcept 
    { 
        auto previous{*this};
        --myData; 
        return previous;
    }

    /**
     * @brief Increment the iterator given number of times.
     *
     * @param incrementCount The number of times the iterator will be incremented.
     */
    void operator+=(const size_t incrementCount) noexcept
    {
        for (size_t i{}; i < incrementCount; ++i) { ++myData; }
    }

    /**
     * @brief Decrement the iterator given number of times.
     *
     * @param decrementCount The number of times the iterator will be decremented.
     */
    void operator-=(const size_t decrementCount) noexcept
    {
        for (size_t i{}; i < decrementCount; ++i) { --myData; }
    }

    /**
     * @brief Check if the iterator and referenced other iterator point at the same address.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if the iterators point at the same address, otherwise false.
     */
    bool operator==(const Iterator& other) const noexcept { return myData == other.myData; }

    /**
     * @brief Check if the iterator and referenced other iterator point at different addresses.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if the iterators point at the different addresses, otherwise false.
     */
    bool operator!=(const Iterator& other) const noexcept { return myData != other.myData; }

    /**
     * @brief Check if the iterator points at an address that's higher than
     *        the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's higher than the address
     *         pointed to by referenced other iterator, otherwise false.
     */
    bool operator>(const Iterator& other) const noexcept { return myData > other.myData; }

    /**
     * @brief Check if the iterator points at an address that's lower than
     *        the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's lower than the address
     *         pointed to by referenced other iterator, otherwise false.
     */
    bool operator<(const Iterator& other) const noexcept { return myData < other.myData; }

    /**
     * @brief Check if the iterator points at an address that's higher or equal
     *        to the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's higher or equal to the
     *         address pointed to by referenced other iterator, otherwise false.
     */
    bool operator>=(const Iterator& other) const noexcept { return myData >= other.myData; }

    /**
     * @brief Check if the iterator points at an address that's lower or equal
     *        to the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's lower or equal to the
     *         address pointed to by referenced other iterator, otherwise false.
     */
    bool operator<=(const Iterator& other) const noexcept { return myData <= other.myData; }

    /**
     * @brief Get the value stored at the address the iterator is pointing at.
     *
     * @return Reference to the value at the address the iterator is pointing at.
     */
    T& operator*() noexcept { return *myData; }

    /**
     * @brief Get the value stored at the address the iterator is pointing at.
     *
     * @return Reference to the value at the address the iterator is pointing at.
     */
    const T& operator*() const noexcept { return *myData; }

private:
    T* myData; // Pointer to the data this iterator is referring to.
};

/**
 * @brief Implementation of constant array iterators.
 *
 * @tparam T    The array type.
 * @tparam Size The array size. Must be greater than 0.
 */
template <typename T, size_t Size>
class Array<T, Size>::ConstIterator
{
    static_assert(Size > 0U, "Array size must be greater than 0!");

public:
    /**
     * @brief Create empty iterator.
     */
    ConstIterator() noexcept
        : myData{nullptr} {}

    /**
     * @brief Create iterator pointing at given data.
     *
     * @param data Reference to data the iterator is set to point at.
     */
    ConstIterator(const T& data) noexcept
        : myData{&data} {}

    /**
     * @brief Create iterator pointing at given data.
     *
     * @param data Pointer to data the iterator is set to point at.
     */
    ConstIterator(const T* data) noexcept
        : myData{data} {}

    /**
     * @brief Delete iterator.
     */
    ~ConstIterator() noexcept = default;

    /**
     * @brief Create copy of another iterator.
     * 
     * @param other Reference to iterator to copy.
     */
    ConstIterator(const ConstIterator& other) noexcept
        : myData{other.myData} {}

    /**
     * @brief Create iterator overtaking resources from another iterator.
     * 
     *        The other iterator is set to null after the move operation is completed.
     * 
     * @param other Reference to iterator to move resources from.
     */
    ConstIterator(ConstIterator&& other) noexcept
        : myData{other.myData} { other.myData = nullptr; }

    /**
     * @brief Copy another iterator.
     * 
     * @param other Reference to iterator to copy.
     * 
     * @return Reference to this iterator.
     */
    ConstIterator& operator=(const ConstIterator& other) noexcept
    {
        if (this != &other) { myData = other.myData; }
        return *this;
    }

    /**
     * @brief Move resources from another iterator.
     * 
     *        The other iterator is set to null after the move operation is completed.
     * 
     * @param other Reference to iterator to move resources from.
     * 
     * @return Reference to this iterator.
     */
    ConstIterator& operator=(ConstIterator&& other) noexcept
    {
        if (this != &other)
        {
            myData       = other.myData;
            other.myData = nullptr;
        }
        return *this;
    }

    /**
     * @brief Increment the address the iterator is pointing at (prefix operator).
     * 
     * @return Reference to this iterator.
     */
    ConstIterator& operator++() noexcept
    {
        ++myData;
        return *this;
    }

    /**
     * @brief Decrement the address the iterator is pointing at (prefix operator).
     * 
     * @return Reference to this iterator.
     */
    ConstIterator& operator--() noexcept
    {
        --myData;
        return *this;
    }

    /**
     * @brief Increment the address the iterator is pointing at (postfix operator).
     * 
     * @return The previous state of this iterator.
     */
    ConstIterator operator++(int) noexcept
    {
        auto previous{*this};
        ++myData;
        return previous;
    }

    /**
     * @brief Decrement the address the iterator is pointing at (postfix operator).
     * 
     * @return The previous state of this iterator.
     */
    ConstIterator operator--(int) noexcept
    {
        auto previous{*this};
        --myData;
        return previous;
    }

    /**
     * @brief Increment the iterator given number of times.
     *
     * @param incrementCount The number of times the iterator will be incremented.
     */
    void operator+=(const size_t incrementCount) noexcept
    {
        for (size_t i{}; i < incrementCount; ++i) { ++myData; }
    }

    /**
     * @brief Decrement the iterator given number of times.
     *
     * @param decrementCount The number of times the iterator will be decremented.
     */
    void operator-=(const size_t decrementCount) noexcept
    {
        for (size_t i{}; i < decrementCount; ++i) { --myData; }
    }

    /**
     * @brief Check if the iterator and referenced other iterator point at the same address.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if the iterators point at the same address, otherwise false.
     */
    bool operator==(const ConstIterator& other) const noexcept { return myData == other.myData; }

    /**
     * @brief Check if the iterator and referenced other iterator point at different addresses.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if the iterators point at the different addresses, otherwise false.
     */
    bool operator!=(const ConstIterator& other) const noexcept { return myData != other.myData; }

    /**
     * @brief Check if the iterator points at an address that's higher than
     *        the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's higher than the address
     *         pointed to by referenced other iterator, otherwise false.
     */
    bool operator>(const ConstIterator& other) const noexcept { return myData > other.myData; }

    /**
     * @brief Check if the iterator points at an address that's lower than
     *        the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's lower than the address
     *         pointed to by referenced other iterator, otherwise false.
     */
    bool operator<(const ConstIterator& other) const noexcept { return myData < other.myData; }

    /**
     * @brief Check if the iterator points at an address that's higher or equal
     *        to the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's higher or equal to the
     *         address pointed to by referenced other iterator, otherwise false.
     */
    bool operator>=(const ConstIterator& other) const noexcept { return myData >= other.myData; }

    /**
     * @brief Check if the iterator points at an address that's lower or equal
     *        to the address pointed to by referenced other iterator.
     *
     * @param other Reference to other iterator.
     * 
     * @return True if iterator points at an address that's lower or equal to the
     *         address pointed to by referenced other iterator, otherwise false.
     */
    bool operator<=(const ConstIterator& other) const noexcept { return myData <= other.myData; }

    /**
     * @brief Get the value stored at the address the iterator is pointing at.
     *
     * @return Reference to the value at the address the iterator is pointing at.
     */
    const T& operator*() const noexcept { return *myData; }

private:
    const T* myData; // Pointer to the data this iterator is referring to.
};

} // namespace container
