/**
 * @brief Neural network implementation with stub dense layers.
 */
#include <iostream>

#include "ml/dense_layer/dense_layer.h"
#include "ml/neural_network/single_layer.h"

namespace
{
/**
 * @brief Round very small numbers to zero for cleaner output display.
 * 
 *        Eliminate floating-point precision errors that would otherwise display as 
 *        tiny non-zero values (e.g., 1.23e-15 instead of 0).
 * 
 * @param[in] number The number to potentially round to zero.
 *
 * @return 0.0 if the number is within the threshold range, otherwise the original number.
 */
constexpr double roundToZero(const double number) noexcept
{
    constexpr double threshold{1e-6};

    // Calculate absolute value to simplify the threshold comparison.
    const auto absVal{0.0 <= number ? number : -number};

    // Return 0 if the absolute value is smaller than the threshold.
    return threshold > absVal ? 0.0 : number;
}

/**
 * @brief Print numbers held by the given vector.
 * 
 * @param[in] numbers Vector holding the numbers to print.
 * @param[in] ostream Output stream to use (default = terminal print).
 */
void printNumbers(const std::vector<double>& numbers, std::ostream& ostream = std::cout)
{
    // Terminate the function if the vector is empty.
    if (numbers.empty()) { return; }

    // Get the address of the last element to not print a blank space after the last number.
    const auto end{&numbers[numbers.size() - 1U]};

    // Print each number on a single line, separated by a blank space.
    for (const auto& number : numbers)
    {
        ostream << roundToZero(number);
        if (end != &number) { ostream << " "; }
    }
}

/**
 * @brief Perform prediction with the given neural network.
 * 
 * @param[in] network Neural network to predict with.
 * @param[in] inputData Input data to predict with.
 * @param[in] ostream Output stream to use (default = terminal print).
 */
void predict(ml::neural_network::Interface& network,
             const std::vector<std::vector<double>>& inputData,
             std::ostream& ostream = std::cout)
{
    // Terminate the function if no input data is provided.
    if (inputData.empty())
    {
        ostream << "No input data!\n";
        return;
    }
    ostream << "--------------------------------------------------------------------------------\n";

    // Perform prediction with each input value, print the result in the terminal.
    for (const auto& input : inputData)
    {
        const auto prediction{network.predict(input)};
        ostream << "Input: ";
        printNumbers(input, ostream);
        ostream << ", predicted output: ";
        printNumbers(prediction, ostream);
        ostream << "\n";
    }
    ostream << "--------------------------------------------------------------------------------\n\n";
}
} // namespace

/**
 * @brief Train and predict with a simple neural network.
 * 
 * @return 0 on success, or -1 on failure.
 */
int main()
{
    // Implement the neural network parameters as compile-time constants.
    constexpr std::size_t inputCount{2U};
    constexpr std::size_t hiddenCount{3U};
    constexpr std::size_t outputCount{1U};

    // Implement the number of epochs to train and the learning rate as compile-time constants.
    constexpr std::size_t epochCount{1000};
    constexpr double learningRate{0.1};

    // Create training data vectors.
    const std::vector<std::vector<double>> trainInput{{0,0}, {0,1}, {1,0}, {1,1}};
    const std::vector<std::vector<double>> trainOutput{{0}, {1}, {1}, {0}};

    // Create the dense layers.
    ml::dense_layer::DenseLayer hiddenLayer{hiddenCount, inputCount};
    ml::dense_layer::DenseLayer outputLayer{outputCount, hiddenCount};

    // Create a single-layer neural network.
    ml::neural_network::SingleLayer network{hiddenLayer, outputLayer, trainInput, trainOutput};

    // Train the network, terminate the program with error code -1 on failure.
    if (!network.train(epochCount, learningRate))
    {
        std::cout << "Training failed!\n";
        return -1;
    }
    // Perform prediction with the network, then terminate the program.
    predict(network, trainInput);
    return 0;
}