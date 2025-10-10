/**
 * @brief Dense layer implementation details.
 */
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <stdexcept>
#include <vector>

#include "ml/dense_layer/dense_layer.h"
#include "ml/types.h"

namespace ml::dense_layer
{
namespace
{
// -----------------------------------------------------------------------------
void initRandom() noexcept
{
    // Static variable to track whether random generator has been seeded.
    static bool initialized{false};

    // Terminate the function if already initialized to avoid reseeding.
    if (initialized) { return; }

    // Set up random numbers using the current time as a starting point.
    std::srand(std::time(nullptr));
    
    // Mark as initialized to prevent future reseeding.
    initialized = true;
}

// -----------------------------------------------------------------------------
double randomStartVal() noexcept
{
    // Generate random weight initialization value in range [0.0, 1.0].
    // Cast to double ensures floating-point division for precision.
    return static_cast<double>(std::rand()) / RAND_MAX;
}

// -----------------------------------------------------------------------------
double actFuncOutput(const ml::ActFunc actFunc, const double input) noexcept
{
    // Compute activation function output for the given input value.
    switch (actFunc)
    {
        case ml::ActFunc::Relu:
             // ReLU: f(x) = max(0, x) - return input if positive, zero otherwise.
             return 0.0 < input ? input : 0.0;
        case ml::ActFunc::Tanh:
             // Hyperbolic tangent: f(x) = tanh(x) - output range [-1, 1].
             return std::tanh(input);
        default:
            std::cout << "Invalid activation function!\n";
            return 0.0;
    }
}

// -----------------------------------------------------------------------------
double actFuncDelta(const ActFunc actFunc, const double input) noexcept
{
    // Calculate how much the activation function changes (needed for learning).
    switch (actFunc)
    {
        case ml::ActFunc::Relu:
             // ReLU derivative: f'(x) = 1 if x > 0, else 0.
             return 0.0 < input ? 1.0 : 0.0;
        case ml::ActFunc::Tanh:
             // Tanh derivative: f'(x) = 1 - tanh²(x).
             return 1.0 - std::tanh(input) * std::tanh(input);
        default:
            std::cout << "Invalid activation function!\n";
            return 0.0;
    }
}
} // namespace 

// -----------------------------------------------------------------------------
DenseLayer::DenseLayer(const std::size_t nodeCount, const std::size_t weightCount,
                       const ml::ActFunc actFunc)
    : myOutput(nodeCount, 0.0)
    , myError(nodeCount, 0.0)
    , myBias(nodeCount, 0.0)
    , myWeights(nodeCount, std::vector<double>(weightCount, 0.0))
    , myActFunc{actFunc}
{
    // Make sure we have at least 1 node and 1 weight per node.
    if ((0U == nodeCount) || (0U == weightCount))
    {
        throw std::invalid_argument(
            "Invalid dense layer parameters: nodeCount and weightCount must be > 0!");
    }

    // Initialize the random number generator (only done once).
    initRandom();

    // Initialize all biases and weights with random starting values.
    for (std::size_t i{}; i < nodeCount; ++i)
    {
        myBias[i] = randomStartVal();

        for (std::size_t j{}; j < weightCount; ++j)
        {
            myWeights[i][j] = randomStartVal();
        }
    }
}

// -----------------------------------------------------------------------------
std::size_t DenseLayer::nodeCount() const noexcept 
{
    // Return the number of nodes in this layer.
    return myOutput.size();
}

// -----------------------------------------------------------------------------
std::size_t DenseLayer::weightCount() const noexcept 
{
    // Return the number of weights per node (same for all nodes).
    return myWeights[0U].size();
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::output() const noexcept 
{
    // Return read-only access to layer's output values.
    return myOutput;
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::error() const noexcept 
{
    // Return read-only access to layer's error values.
    return myError;
}

// -----------------------------------------------------------------------------
const std::vector<double>& DenseLayer::bias() const noexcept 
{
    // Return read-only access to layer's bias values.
    return myBias;
}

// -----------------------------------------------------------------------------
const std::vector<std::vector<double>>& DenseLayer::weights() const noexcept 
{
    // Return read-only access to layer's weights.
    return myWeights;
}

// -----------------------------------------------------------------------------
bool DenseLayer::feedforward(const std::vector<double>& input) noexcept 
{
    // Validate that we have the correct number of inputs.
    if (input.size() != weightCount())
    {
        std::cout << "Input dimension mismatch: expected " << weightCount() 
                  << ", actual: " << input.size() << "!\n"; 
        return false;
    }

    // Compute the output value for each node in this layer.
    for (std::size_t i{}; i < nodeCount(); ++i)
    {
        // Start with the bias (like a starting point for each node).
        auto sum{myBias[i]};

        // Add up all the weighted inputs (input * weight for each connection).
        for (std::size_t j{}; j < weightCount(); ++j)
        {
            sum += input[j] * myWeights[i][j];
        }
        // Pass the sum through the activation function to get the final output.
        myOutput[i] = actFuncOutput(myActFunc, sum);
    }
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::backpropagate(const std::vector<double>& reference) noexcept 
{
    // Validate reference vector size matches number of output nodes.
    if (reference.size() != nodeCount())
    {
        std::cout << "Output dimension mismatch: expected " << nodeCount() 
                  << ", actual: " << reference.size() << "!\n"; 
        return false;
    }

    // Compute error gradients for each node (this is for the output layer).
    for (std::size_t i{}; i < nodeCount(); ++i)
    {
        // Calculate prediction error: target - actual output.
        const auto rawError{reference[i] - myOutput[i]};

        // Apply chain rule: multiply by activation function derivative.
        // This determines how much to adjust weights and biases.
        myError[i] = rawError * actFuncDelta(myActFunc, myOutput[i]);
    }
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::backpropagate(const Interface& nextLayer) noexcept 
{
    // Validate that the layers connect properly.
    if (nextLayer.weightCount() != nodeCount())
    {
        std::cout << "Layer dimension mismatch: expected " << nodeCount() 
                  << ", actual: " << nextLayer.weightCount() << "!\n"; 
        return false;
    }

    // Compute error gradients for each node (this is for hidden layers).
    for (std::size_t i{}; i < nodeCount(); ++i)
    {
        double weightedErrorSum{};

        // Accumulate weighted error contributions from the next layer.
        // Each connection propagates error back through its weight.
        for (std::size_t j{}; j < nextLayer.nodeCount(); ++j)
        {
            // Add error contribution: next_layer_error * connecting_weight.
            weightedErrorSum += nextLayer.error()[j] * nextLayer.weights()[j][i];
        }
        // Apply chain rule: multiply by activation function derivative.
        // This determines how much to adjust this node's weights and biases.
        myError[i] = weightedErrorSum * actFuncDelta(myActFunc, myOutput[i]);
    }
    return true;
}

// -----------------------------------------------------------------------------
bool DenseLayer::optimize(const std::vector<double>& input, const double learningRate) noexcept 
{
    // Validate learning rate and input dimensions.
    if (0.0 >= learningRate)
    {
        std::cout << "Invalid learning rate " << learningRate << "!\n";
        return false;
    }
    if (input.size() != weightCount())
    {
        std::cout << "Input dimension mismatch: expected " << weightCount() 
                  << ", actual: " << input.size() << "!\n";
        return false;
    }

    // Update parameters using gradient descent to minimize error.
    for (std::size_t i{}; i < nodeCount(); ++i)
    {
        // Update bias: bias += error * learning_rate.
        myBias[i] += myError[i] * learningRate;

        // Update weights: larger inputs contribute more to weight changes.
        for (std::size_t j{}; j < weightCount(); ++j)
        {
            // Update weight: weight += error * learning_rate * input_value.
            myWeights[i][j] += myError[i] * learningRate * input[j];
        }
    }
    // Return true to indicate success.
    return true;
}
} // namespace ml::dense_layer
