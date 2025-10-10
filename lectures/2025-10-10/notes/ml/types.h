/**
 * @brief Machine learning type definitions.
 */
#pragma once

namespace ml
{
/**
 * @brief Enumeration of activation functions.
 */
enum class ActFunc
{
    Relu, ///< ReLU (Rectified Linear Unit) => y = x if x > 0 else 0.
    Tanh, ///< Tanh (hyperbolic tangent)    => -1 <= y <= 1.
};
} // namespace ml
