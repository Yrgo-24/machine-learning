/**
 * @brief Dense layer implementation details.
 */
#include <stdexcept>
#include <vector>

#include "ml/dense_layer/dense_layer.h"
#include "ml/types.h"

namespace ml::dense_layer
{
// -----------------------------------------------------------------------------
DenseLayer::DenseLayer(const std::size_t nodeCount, const std::size_t weightCount,
                       const ml::ActFunc actFunc)
    : myOutput(nodeCount, 0.0)
    , myError(nodeCount, 0.0)
    , myBias(nodeCount, 0.0)
    , myWeights(nodeCount, std::vector<double>(weightCount, 0.0))
    , myActFunc{actFunc}
{
    // Throw an exception if the node count or the weight count is invalid.
    if ((0U == nodeCount) || (0U == weightCount))
    {
        throw std::invalid_argument(
            "Invalid dense layer parameters: nodeCount and weightCount must be greater than 0!");
    }

    //! @todo Initialize the bias and weight values.
}

// -----------------------------------------------------------------------------
std::size_t DenseLayer::nodeCount() const noexcept 
{
    // The node count is equal to myOutput.size().
    return myOutput.size();
}

// -----------------------------------------------------------------------------
std::size_t DenseLayer::weightCount() const noexcept 
{
    // The weight count is equal for all nodes, we check the weight count of the first node.
    return myWeights[0U].size();
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::output() const noexcept 
{
    // Return a read-only reference to our output values.
    return myOutput;
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::error() const noexcept 
{
    // Return a read-only reference to our error values.
    return myError;
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::bias() const noexcept 
{
    // Return a read-only reference to our bias values.
    return myBias;
}

// -----------------------------------------------------------------------------
const std::vector<std::vector<double>>& DenseLayer::weights() const noexcept 
{
    // Return a read-only reference to our weights.
    return myWeights;
}

// -----------------------------------------------------------------------------
bool DenseLayer::feedforward(const std::vector<double>& input) noexcept 
{
    //! @todo Implement this method, return true temporarily.
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::backpropagate(const std::vector<double>& reference) noexcept 
{
    //! @todo Implement this method, return true temporarily.
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::backpropagate(const Interface& nextLayer) noexcept 
{
    //! @todo Implement this method, return true temporarily.
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::optimize(const std::vector<double>& input, const double learningRate) noexcept 
{
    //! @todo Implement this method, return true temporarily.
    return true;
}
} // namespace ml::dense_layer
