/**
 * @brief Dense layer implementation.
 */
#pragma once

#include <vector>

#include "ml/dense_layer/interface.h"
#include "ml/types.h"

namespace ml::dense_layer
{
/**
 * @brief Dense layer implementation.
 */
class DenseLayer final : public Interface
{
public:
    /**
     * @brief Create a new dense layer.
     *
     * @param[in] nodeCount The number of nodes in the layer. Must exceed 0.
     * @param[in] weightCount The number of weights in the layer. Must exceed 0.
     * @param[in] actFunc The activation to use for this layer (default = ReLU).
     */
    explicit DenseLayer(const std::size_t nodeCount, const std::size_t weightCount,
                        const ml::ActFunc actFunc = ml::ActFunc::Relu);

    /**
     * @brief Delete the dense layer.
     */
    ~DenseLayer() noexcept override = default;

    /**
     * @brief Get the number of nodes in the dense layer.
     * 
     * @return The number of nodes in the dense layer.
     */
    std::size_t nodeCount() const noexcept override;

    /**
     * @brief Get the number of weights per node in the dense layer.
     * 
     * @return The number of weights per node in the dense layer.
     */
    std::size_t weightCount() const noexcept override;

    /**
     * @brief Get the output values of the dense layer.
     * 
     * @return Vector holding the output values of the dense layer.
     */
    const std::vector<double>& output() const noexcept override;

    /**
     * @brief Get the error values of the dense layer.
     * 
     * @return Vector holding the error values of the dense layer.
     */
    const std::vector<double>& error() const noexcept override;

    /**
     * @brief Get the bias values of the dense layer.
     * 
     * @return Vector holding the bias values of the dense layer.
     */
    const std::vector<double>& bias() const noexcept override;

    /**
     * @brief Get the weights of the dense layer.
     * 
     * @return Vector holding the weights of the dense layer.
     */
    const std::vector<std::vector<double>>& weights() const noexcept override;

    /**
     * @brief Perform feedforward with the given input.
     * 
     * @param[in] input Input values with which to perform feedforward.
     * 
     * @return True if feedforward was performed, or false on error.
     */
    bool feedforward(const std::vector<double>& input) noexcept override;

    /**
     * @brief Perform backpropagation with the given reference values.
     * 
     *        This method is appropriate for output layers only.
     * 
     * @param[in] reference Reference values with which to perform backpropagation.
     * 
     * @return True if backpropagation was performed, or false on error.
     */
    bool backpropagate(const std::vector<double>& reference) noexcept override;

    /**
     * @brief Perform backpropagation with the given next layer.
     * 
     *        This method is appropriate for hidden layers only.
     * 
     * @param[in] nextLayer The next consecutive layer.
     * 
     * @return True if backpropagation was performed, or false on error.
     */
    bool backpropagate(const Interface& nextLayer) noexcept override;

    /**
     * @brief Perform optimization with the given input.
     * 
     * @param[in] input Input values with which to perform optimization.
     * @param[in] learningRate Learning rate to use for optimization.
     * 
     * @return True if optimization was performed, or false on error.
     */
    bool optimize(const std::vector<double>& input, const double learningRate) noexcept override;

    DenseLayer()                             = delete; // No default constructor.
    DenseLayer(const DenseLayer&)            = delete; // No copy constructor.
    DenseLayer(DenseLayer&&)                 = delete; // No move constructor.
    DenseLayer& operator=(const DenseLayer&) = delete; // No copy assignment.
    DenseLayer& operator=(DenseLayer&&)      = delete; // No move assignment.

private:
    /** Vector holding the node outputs. */
    std::vector<double> myOutput;

    /** Vector holding the node errors. */
    std::vector<double> myError;

    /** Vector holding the node bias values. */
    std::vector<double> myBias;

    /** 2D vector holding the node weights: [i][j] => i = node index, j = weight index. */
    std::vector<std::vector<double>> myWeights;

    /** The activation function to use in this layer. */
    const ml::ActFunc myActFunc;
};
} // namespace ml::dense_layer