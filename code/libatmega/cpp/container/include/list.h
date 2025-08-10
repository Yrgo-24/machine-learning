/**
 * @brief Implementation of doubly linked lists of any type.
 */
#pragma once

#include <stddef.h>

namespace container 
{
/**
 * @brief Class for implementation of doubly linked lists.
 * 
 * @tparam T The list type.
 */
template <typename T>
class List 
{        
public:
    class Iterator;      // List iterator.
    class ConstIterator; // Constant list iterator.

    /**
     * @brief Create empty list.
     */
    explicit List() noexcept;

    /**
     * @brief Create list of given size initialized with given start value.
     *
     * @param size       Initial size of list.
     * @param startValue Initial value for each element (default = 0).
     */
     explicit List(const size_t size, const T& startValue = static_cast<T>(0)) noexcept;

    /**
     * @brief Create list initialized with values passed as input arguments.
     *
     * @tparam Values Parameter pack containing values.
     * 
     * @param values The values to add to the list.
     */
    template <typename... Values> 
    List(const Values&&... values) noexcept;

    /**
     * @brief Delete list.
     */
    ~List() noexcept;

    /**
     * @brief Create list as a copy of another list.
     *
     * @param other Reference to other list to copy from.
     */
    List(const List<T>& other) noexcept;

    /**
     * @brief Move memory from another list.
     * 
     *        The other list is emptied once the move operation is completed.
     *
     * @param other Reference to other list to move memory from.
     */
    List(List<T>&& other) noexcept;

     /**
     * @brief Copy the content of list to assigned list. 
     * 
     *        Previous values are cleared before copying.
     *
     * @param other Reference to list holding the data to copy.
     * 
     * @return Reference to this list.
     */
    List<T>& operator=(const List<T>& other) noexcept;

    /**
     * @brief Move the content from other list.
     * 
     *        Previous values are cleared before copying.
     * 
     *        The other list is emptied once the move operation is completed.
     *
     * @param other Reference to list holding the data to move. 
     * 
     * @return Reference to this list.
     */
    List<T>& operator=(List<T>&& other) noexcept;

    /**
     * @brief Add values from another list.
     *
     * @param other Reference to list holding the values to add.
     * 
     * @return Reference to this list.
     */
    List<T>& operator+=(const List<T>& other) noexcept;

    /**
     * @brief Push values to the back of list.
     *
     * @tparam ValueCount The number of values to add.
     * 
     * @param values Reference to the values to add.
     * 
     * @return Reference to this list.
     */
    template <size_t ValueCount>
    List<T>& operator+=(const T (&values)[ValueCount]) noexcept;

    /**
     * @brief Get reference to the value at given position in the list.
     *
     * @param iterator Reference to iterator pointing at the value to read.
     * 
     * @return A reference to the element at given position.
     */
    T& operator[](Iterator& iterator) noexcept;

    /**
     * @brief Get reference to the element at given index in the list.
     *
     * @param iterator Reference to iterator pointing at the value to read.
     * 
     * @return A reference to the element at given position.
     */
    const T& operator[] (ConstIterator& iterator) const noexcept;

    /**
     * @brief Get the size of list.
     *
     * @return The size of list in the number of element it holds.
     */
    size_t size() const noexcept;

    /**
     * @brief Clear the list.
     */
    void clear() noexcept;

    /**
     * @brief Check if the list is empty.
     *
     * @return True if the list is empty, otherwise false.
     */
    bool empty() const noexcept;

    /**
     * @brief Get the beginning of list.
     *
     * @return Iterator pointing at the beginning of list.
     */
    Iterator begin() noexcept;

    /**
     * @brief Get the beginning of list.
     *
     * @return Iterator pointing at the beginning of list.
     */
    ConstIterator begin() const noexcept;

    /**
     * @brief Get the end of list.
     *
     * @return Iterator pointing at the end of list.
     */
    Iterator end() noexcept;

     /**
     * @brief Get the end of list.
     *
     * @return Iterator pointing at the end of list.
     */
    ConstIterator end() const noexcept;

    /**
     * @brief Get the reverse beginning of list.
     *
     * @return Iterator pointing at the reverse beginning of list.
     */
    Iterator rbegin() noexcept;

    /**
     * @brief Get the reverse beginning of list.
     *
     * @return Iterator pointing at the reverse beginning of list.
     */
    ConstIterator rbegin() const noexcept;

    /**
     * @brief Get the reverse end of list.
     *
     * @return Iterator pointing at the reverse end of list.
     */
    Iterator rend() noexcept;

    /**
     * @brief Get the reverse end of list.
     *
     * @return Iterator pointing at the reverse end of list.
     */
    ConstIterator rend() const noexcept;

    /**
     * @brief Resize the list to given new size.
     *
     * @param newSize    The new size of list.
     * @param startValue Initial value for each new element (default = 0).
     * 
     * @return True if the list was resized, otherwise false.
     */
    bool resize(const size_t newSize, const T& startValue = static_cast<T>(0)) noexcept;

    /**
     * @brief Insert value at the front of list.
     *
     * @param value Reference to the value to add.
     * 
     * @return True if the value was added, otherwise false.
     */
    bool pushFront(const T& value) noexcept;

    /**
     * @brief Insert value at the back of list.
     *
     * @param value Reference to the value to add.
     * 
     * @return True if the value was added, otherwise false.
     */
    bool pushBack(const T& value) noexcept;

    /**
     * @brief Insert value at given position in the list.
     *
     * @param iterator Reference to iterator pointing at the location to place the new value.
     * @param value    Reference to the value to add.
     * 
     * @return True if the value was added, otherwise false.
     */
    bool insert(Iterator& iterator, const T& value) noexcept;

    /**
     * @brief Remove value at the front of list.
     */
    void popFront() noexcept;

    /**
     * @brief Remove value at the back of list.
     */
    void popBack() noexcept;
    
    /**
     * @brief Remove value at given position in the list.
     *
     * @param iterator Reference to iterator pointing at the value to remove.
     *
     * @return True if the value was removed, otherwise false.
     */
    bool remove(Iterator& iterator) noexcept;

protected:
    struct Node; // Node holding data stored in the list.

    bool copy(const List<T>& other) noexcept;

    template <size_t ValueCount>
    void assign(const T (&values)[ValueCount], const size_t offset = 0U) noexcept;

    template <size_t ValueCount>
    bool addValues(const T (&values)[ValueCount]) noexcept;

    void removeAllNodes() noexcept;

    Node* myFirst; // Pointer to the first node in the list.
    Node* myLast;  // Pointer to the last node in the list.
    size_t mySize; // The size of the list measured in number of nodes.
};

} // namespace container

#include "impl/list_impl.h"
#include "iterator/list_iterator.h"