/**
 * @brief Contains miscellaneous utility functions for bit operations, 
 *        mathematical operations and generating delays.
 */
#pragma once

#ifndef F_CPU
#define F_CPU 16000000UL // Default CPU frequency measured in Hz.
#endif

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "type_traits.h"

namespace utils 
{

/**
 * @brief Block the calling thread for the given time measured in seconds.
 *
 * @param delayTimeS The time to block the thread in seconds.
 */
void delayS(const uint16_t& delayTimeS) noexcept;

/**
 * @brief Block the calling thread for the given time measured in milliseconds.
 *
 * @param delayTimeMs The time to block the thread in milliseconds.
 */
void delayMs(const uint16_t& delayTimeMs) noexcept;

/**
 * @brief Block the calling thread for the given time measured in microseconds.
 *
 * @param delayTimeUs The time to block the thread in microseconds.
 */
void delayUs(const uint16_t& delayTimeUs) noexcept;

/**
 * @brief Structure used for removing references in order to maintain value categories, 
 *        for instance between function calls.
 * 
 * @tparam T The value type.
 */
template <typename T>
struct RemoveReference
{
    typedef T type;
};

/**
 * @brief Maintain the value category of given object.
 *
 * @param object Reference to object whose value category is to be maintained.
 *
 * @return Given object with maintained value category.
 */
template <typename T>
constexpr T&& forward(typename RemoveReference<T>::type& object) noexcept;

/**
 * @brief Maintain the value category of given object.
 *
 * @param object Reference to object whose value category is to be maintained.
 *
 * @return Given object with maintained value category.
 */
template <typename T>
constexpr T&& forward(typename RemoveReference<T>::type&& object) noexcept;

/**
 * @brief Enable interrupts globally.
 */
inline void globalInterruptEnable() noexcept;

/**
 * @brief Disable interrupts globally.
 */
inline void globalInterruptDisable() noexcept;

/**
 * @brief Set bit in given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param reg Reference to the register to write to.
 * @param bit The bit to set.
 */
template <typename T = uint8_t>
constexpr void set(volatile T& reg, const uint8_t bit) noexcept;

/**
 * @brief Set bits in given register.
 *
 * @tparam T    The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 *
 * @param reg  Reference to the register to write to.
 * @param bit  The first bit to set.
 * @param bits The other bits to set.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void set(volatile T& reg, const uint8_t bit, const Bits&&... bits) noexcept;

/**
 * @brief Clear bit in given register.
 *
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param reg Reference to the register to write to.
 * @param bit The bit to clear.
 */
template <typename T = uint8_t>
constexpr void clear(volatile T& reg, const uint8_t bit) noexcept;

/**
 * @brief Clear bits in given register.
 *
 * @tparam T    The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param reg  Reference to the register to write to.
 * @param bit  The first bit to clear.
 * @param bits The other bits to clear.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void clear(volatile T& reg, const uint8_t bit, const Bits&&... bits) noexcept;

/**
 * @brief Toggle bit in given register.
 * 
 * @tparam T The register type. Must be of unsigned type.
 * 
 * @param reg Reference to the register to write to.
 * @param bit The bit to toggle.
 */
template <typename T = uint8_t>
constexpr void toggle(volatile T& reg, const uint8_t bit) noexcept;

/**
 * @brief Toggle bits in given register.
 *
 * @tparam T    The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param reg  Reference to the register to write to.
 * @param bit  The first bit to toggle.
 * @param bits The other bits to toggle.
 */
template <typename T = uint8_t, typename... Bits>
constexpr void toggle(volatile T& reg, const uint8_t bit, const Bits&&... bits) noexcept;

/**
 * @brief Read bit in given register.
 * 
 * @tparam T The register type. Must be of unsigned type.
 *
 * @param reg Reference to the register to read from.
 * @param bit The given bit to read.
 *
 * @return True if the bit is set, otherwise false.
 */
template <typename T = uint8_t>
constexpr bool read(const volatile T& reg, const uint8_t bit) noexcept;

/**
 * @brief Read bits in given register.
 *
 * @tparam T    The register type. Must be of unsigned type.
 * @tparam Bits Type for additional bits to set.
 * 
 * @param reg  Reference to the register to read from.
 * @param bit  The first bit to read.
 * @param bits The other bits to read.
 *
 * @return True if at least one of bits is set, otherwise false.
 */
template <typename T = uint8_t, typename... Bits>
constexpr bool read(const volatile T& reg, const uint8_t bit, const Bits&&... bits) noexcept;

/**
 * @brief Calculate the power out of given base and exponent, where power = base ^ exponent.
 *
 * @tparam T1 The base type. Must be arithmetic.
 * @tparam T2 The exponent type. Must be arithmetic.
 * @tparam T3 The power type. Must be arithmetic.
 * 
 * @param base     The base to use for the calculation.
 * @param exponent The exponent to use for the calculation.
 *
 * @return The power as a function of given base and exponent.
 */
template <typename T1 = double, typename T2 = double, typename T3 = double> 
T3 power(const T1 base, const T2 exponent) noexcept;

/**
 * @brief Round given number to nearest integer.
 *
 * @tparam T1 The type to round to. Must be integral.
 * @tparam T2 The type to round from. Must be arithmetic.
 * 
 * @param number The number to round.
 *
 * @return The corresponding rounded number.
 */
template <typename T1 = int32_t, typename T2 = double>
constexpr T1 round(const T2 value) noexcept;

/**
 * @brief Allocate a new object on the heap.
 *
 * @tparam T    The field type.
 * @tparam Args The types of arguments to pass to the constructor of T.
 * 
 * @param args The arguments to pass to the constructor of T.
 * 
 * @return A pointer to the new object.
 * 
 *         If the memory allocation fails, a nullptr is returned. 
 */
template <typename T, typename... Args>
inline T* newObject(Args&&... args) noexcept;

/**
 * @brief Allocate a new field on the heap.
 *
 * @tparam T The field type.
 * 
 * @param size The size of field in number of elements it can hold (default = 1).
 *
 * @return A pointer to the new object.
 * 
 *         If the memory allocation fails, a nullptr is returned. 
 */
template <typename T>
inline T* newMemory(const size_t size = 1U) noexcept;

/**
 * @brief Resize referenced heap allocated block via reallocation.
 *
 * @tparam T The block type.
 * 
 * @param block   The block to resize.
 * @param newSize The new size of allocated block, i.e. the number of
 *                elements it can hold after reallocation.
 *
 * @return A pointer to the resized block at success, else a null pointer.
 */
template <typename T>
inline T* reallocMemory(T* block, const size_t newSize) noexcept;

/**
 * @brief Delete heap allocated block via deallocation. 
 * 
 *        The pointer to the block is set to null after deallocation.
 * 
 * @tparam T The block type.
 *
 * @param block Reference to the block to delete.
 */
template <typename T>
inline void deleteMemory(T* &block) noexcept;

/**
 * @brief Move memory from given source to a copy. The copy will gain
 *        ownership of memory and the source will be emptied.
 *
 * @tparam T The type of source and the copy.
 * 
 * @param source Reference to the source whose memory is moved.
 *
 * @return The copy of source.
 */
template <typename T>
inline T move(T&& source) noexcept;

} // namespace utils

/**
 * @brief Overload of operator delete to satisfy the linker when using interfaces.
 */
void operator delete(void*, unsigned int) noexcept;

/**
 * @brief Definition of initialization error function to satisfy the linker when using interfaces.
 */
extern "C" void __cxa_pure_virtual();

extern "C" void __cxa_guard_acquire();

extern "C" void __cxa_guard_release();

#include "impl/utils_impl.h"