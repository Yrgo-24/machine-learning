/**
 * @brief Implementation of callback arrays of arbitrary size.
 */
#pragma once

#include "array.h"

namespace container
{

/**
 * @brief Class for implementation of callback arrays.
 * 
 *        This class is non-copyable and non-movable.
 * 
 * @tparam Size The array size. Must be greater than 0.
 */
template <size_t Size>
class CallbackArray : public Array<void (*)(), Size>
{
public:
    /**
     * @brief Create empty callback array of given size.
     */
    explicit CallbackArray() noexcept;

    /**
     * @brief Delete callback array.
     */
    ~CallbackArray() noexcept;

    /**
     * @brief Add new callback routine at given index of callback array.
     *
     * @param callback Reference to the callback routine to add.
     * @param index    Storage index of callback routine.
     *
     * @return True if the callback routine was added, otherwise false.
     */
    bool add(void (*callback)(), const size_t index) noexcept;

     /**
     * @brief Remove callback routine at given index of callback array.
     *
     * @param index Storage index of callback routine.
     *
     * @return True if the callback routine was removed, otherwise false.
     */
    bool remove(const size_t index) noexcept;

     /**
     * @brief Remove given callback routine from the callback array.
     *
     * @param callback Reference to the callback routine to remove.
     *
     * @return True if the callback routine was removed, otherwise false.
     */
    bool remove(void (*callback)(), const size_t index) noexcept;

     /**
     * @brief Invoke callback at given index of callback array.
     *
     * @param index Storage index of callback routine to call.
     *
     * @return True if the callback routine was called, otherwise false.
     */
    bool invoke(const size_t index) noexcept;

    CallbackArray(const CallbackArray<Size>&)                  = delete; // No copy constructor.
    CallbackArray(CallbackArray<Size>&&)                       = delete; // No move constructor.
    CallbackArray<Size>& operator=(const CallbackArray<Size>&) = delete; // No copy assignment.
    CallbackArray<Size>& operator=(CallbackArray<Size>&&)      = delete; // No move assignment.

private:
    bool isIndexValid(const size_t index) const noexcept;
    bool isCallbackDefined(const size_t index) const noexcept;
};

} // namespace container

#include "impl/callback_array_impl.h"