/**
 * @brief Simple convolutional layer demo.
 */
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace
{
/** One-dimensional matrix. */
using Matrix1d = std::vector<double>;

/** Two-dimensional matrix. */
using Matrix2d = std::vector<Matrix1d>;

/**
 * @brief Initialize given matrix with zeros.
 * 
 * @param[in] matrix The matrix to initialize.
 */
void initMatrix(Matrix2d& matrix) noexcept
{
    // Fill the matrix with zeros.
    for (auto& row : matrix)
    {
        for (auto& num : row) { num = 0.0; }
    }
}

/**
 * @brief Initialize matrix with zeros.
 * 
 * @param[in] matrix The matrix to initialize.
 * @param[in] size The desired size of the matrix.
 */
void initMatrix(Matrix2d& matrix, const std::size_t size)
{
    // Resize the matrix if necessary, then fill the matrix with zeros.
    matrix.resize(size, Matrix1d(size));
    initMatrix(matrix);
}

/**
 * @brief Check whether given matrix is square.
 * 
 * @param[in] matrix The matrix to check.
 * 
 * @return True if the matrix is square, false otherwise.
 */
bool isMatrixSquare(const Matrix2d& matrix) noexcept
{
    // Check the matrix row by row.
    for (const auto& row: matrix)
    {
        // Return false if the matrix isn't square.
        if (row.size() != matrix.size()) { return false; }
    }
    // Return true to indicate that the matrix is square.
    return true;
}

/**
 * @brief Print the contents of given matrix.
 * 
 * @param[in] matrix The matrix to print.
 * @param[in] precision Decimal precision (default = 1).
 * @param[in] ostream Output stream (default = terminal print).
 */
void printMatrix(const Matrix2d& matrix, const std::size_t precision = 1U,
                 std::ostream& ostream = std::cout) noexcept
{
    // Set the decimal precision.
    ostream << std::fixed << std::setprecision(precision);

    // Print the matrix contents row by row.
    for (const auto& row : matrix)
    {
        const auto* lastNum{row.empty() ? nullptr : &row[row.size() - 1U]};
        ostream << "\t";

        // Separate each number in the row with a comma.
        for (const auto& num : row)
        {
            ostream << num;
            if (&num < lastNum) { ostream << ", "; }
        }
        ostream << "\n";
    }
    ostream << "\n";
}

/**
 * @brief Generate a random starting value between 0.0 and 1.0.
 *
 * @return Random floating-point value in the range [0.0, 1.0].
 */
double randomStartVal() noexcept { return static_cast<double>(std::rand()) / RAND_MAX; }

/**
 * @brief ReLU activation function (output).
 *
 * @param[in] input Input value.
 * @return Output after ReLU activation.
 */
constexpr double reluOutput(const double input) noexcept { return 0.0 < input ? input : 0.0; }

/**
 * @brief ReLU activation function (derivative).
 *
 * @param[in] input Input value.
 * @return Derivative of ReLU at input.
 */
constexpr double reluDelta(const double input) noexcept { return 0.0 < input ? 1.0 : 0.0; }

namespace ml
{
/**
 * @brief Convolutional layer structure.
 */
struct ConvLayer final
{
    /**
     * @brief Constructor.
     * 
     * @param[in] inputSize Input size. Must be greater than 0.
     * @param[in] kernelSize Kernel size. Must be greater than 0 and smaller than the input size.
     */
    explicit ConvLayer(const std::size_t inputSize, const std::size_t kernelSize)
        : inputPadded{}
        , inputGradientsPadded{}
        , inputGradients{}
        , kernel{}
        , kernelGradients{}
        , output{}
        , bias{randomStartVal()}
        , biasGradient{}
    {
        // Check the input arguments, throw if invalid.
        if ((0U == inputSize) || (0U == kernelSize) || (inputSize < kernelSize))
        {
            throw std::invalid_argument(
                "Cannot create convolutional layer: invalid input arguments!");
        }
    
        // Calculate pad offset (how many zeros need to be added to each edge?).
        const std::size_t padOffset{kernelSize / 2U};

        // Calculate padded size (we need to add zeros both to the left and to the right).
        const std::size_t paddedSize{inputSize + 2U * padOffset};

        // Initialize matrices with zeros.
        initMatrix(inputPadded, paddedSize);
        initMatrix(inputGradientsPadded, paddedSize);
        initMatrix(inputGradients, inputSize);
        initMatrix(kernel, kernelSize);
        initMatrix(kernelGradients, kernelSize);
        initMatrix(output, inputSize);

        // Fill the kernel with randomized values in the range [0.0, 1.0].
        for (std::size_t ki{}; ki < kernel.size(); ++ki)
        {
            for (std::size_t kj{}; kj < kernel.size(); ++kj)
            {
                kernel[ki][kj] = randomStartVal();
            }
        }
    }
    
    /**
     * @brief Perform feedforward operation.
     * 
     * @param[in] input Matrix holding input data.
     * 
     * @return True on success, false on failure.
     */
    bool feedforward(const Matrix2d& input) noexcept
    {
        // Check the input matrix, return false on dimension mismatch.
        if ((input.size() != output.size()) || !isMatrixSquare(input)) { return false; }

        // Pad the input with zeros.
        padInput(input);

        // Run feedforward; accumulate bias and contributions from the input and the kernel.
        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                // Start by adding the bias value.
                auto sum{bias};

                // Iterate through the kernel and add the input * kernel values.
                for (std::size_t ki{}; ki < kernel.size(); ++ki)
                {
                    for (std::size_t kj{}; kj < kernel.size(); ++kj)
                    {
                        sum += inputPadded[i + ki][j + kj] * kernel[ki][kj];
                    }
                }

                // Pass the sum through the ReLU activation function, store as output.
                output[i][j] = reluOutput(sum);
            }
        }
        return true;
    }

    /**
     * @brief Perform backpropagation.
     * 
     * @param[in] outputGradients Matrix holding gradients from the next layer.
     * 
     * @return True on success, false on failure.
     */
    bool backpropagate(const Matrix2d& outputGradients) noexcept
    {
        // Check the output gradients matrix, return false on dimension mismatch.
        if ((outputGradients.size() != output.size()) || !isMatrixSquare(outputGradients))
        {
            return false;
        }

        // Reinitialize the gradients with zeros (to remove old values).
        // Else values from the previous backpropagation would still remain.
        initMatrix(inputGradientsPadded);
        initMatrix(inputGradients);
        initMatrix(kernelGradients);
        biasGradient = 0.0;

        // Iterate through the output gradients.
        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                // Calculate output derivate.
                const auto delta{outputGradients[i][j] * reluDelta(output[i][j])};

                // Accumulate the bias gradient by adding all output delta values.
                biasGradient += delta;

                // Iterate through the kernel.
                for (std::size_t ki{}; ki < kernel.size(); ++ki)
                {
                    for (std::size_t kj{}; kj < kernel.size(); ++kj)
                    {
                        kernelGradients[ki][kj] += inputPadded[i + ki][j + kj] * delta;
                        inputGradientsPadded[i + ki][j + kj] += kernel[ki][kj] * delta;
                    }
                }
            }
        }
        // Extract input gradients without zeros.
        extractInputGradients();
        return true;
    }

    /**
     * @brief Perform optimization.
     * 
     * @param[in] learningRate Learning rate to use. Must be in range (0.0, 1.0].
     * 
     * @return True on success, false on failure.
     */
    bool optimize(const double learningRate) noexcept
    {
        // Check the learning rate, return false if out of range.
        if ((0.0 >= learningRate) || (1.0 < learningRate)) { return false; }

        // Adjust the bias with the computed bias gradient, multiplied by the learning rate.
        // We subtract, since the gradients are computed in this manner, as opposed to what
        // we've used in dense layer.
        bias -= biasGradient * learningRate;

        // Adjust the kernel weights with the corresponding gradients and the learning rate.
        for (std::size_t ki{}; ki < kernel.size(); ++ki)
        {
            for (std::size_t kj{}; kj < kernel.size(); ++kj)
            {
                kernel[ki][kj] -= kernelGradients[ki][kj] * learningRate;
            }
        }
        return true;
    }

    /** Input matrix (padded with zeros). */
    Matrix2d inputPadded;

    /** Input gradient matrix (padded with zeros). */
    Matrix2d inputGradientsPadded;

    /** Input gradient matrix (without padding). */
    Matrix2d inputGradients;

    /** Kernel matrix (holding weights). */
    Matrix2d kernel;

    /** Kernel gradient matrix. */
    Matrix2d kernelGradients;

    /** Output matrix. */
    Matrix2d output;

    /** Bias value. */
    double bias;

    /** Bias gradient. */
    double biasGradient;

private:
    /**
     * @brief Pad input with zeros.
     * 
     * @param[in] input Input data.
     */
    void padInput(const Matrix2d& input) noexcept
    {
        // Compute the pad offset (the number of zeros in each direction).
        const std::size_t padOffset{kernel.size() / 2U};

        // Ensure that the padded input matrix is filled with zeros only.
        initMatrix(inputPadded);

        // Copy the input values to the corresponding padded matrix.
        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                inputPadded[i + padOffset][j + padOffset] = input[i][j];
            }
        }
    }

    /**
     * @brief Extract input gradients.
     */
    void extractInputGradients() noexcept
    {
        // Compute the pad offset (the number of zeros in each direction).
        const std::size_t padOffset{kernel.size() / 2U};

        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                inputGradients[i][j] = inputGradientsPadded[i + padOffset][j + padOffset];
            }
        }
    }
};
} // namespace ml
} // namespace

/**
 * @brief Create and demonstrate a simple convolutional layer.
 *
 * @return 0 on success, -1 on failure.
 */
int main()
{
    // Example 4x4 input matrix (could represent an image or feature map).
    const Matrix2d input{{1, 1, 1, 1},
                         {1, 0, 0, 1},
                         {1, 0, 0, 1},
                         {1, 1, 1, 1}};

    // Example output gradients (target output for demonstration).
    const Matrix2d outputGradients{{1, 1, 1, 1},
                                   {1, 1, 1, 1},
                                   {1, 1, 1, 1},
                                   {1, 1, 1, 1}};

    // Initialize the random generator with the current time as seed.
    std::srand(std::time(nullptr));

    // Create a convolutional layer: 4x4 input, 2x2 kernel.
    constexpr std::size_t inputSize{4U};
    constexpr std::size_t kernelSize{2U};
    ml::ConvLayer convLayer{inputSize, kernelSize};
    
    // Show the input matrix.
    std::cout << "Convolution input data (2D):\n";
    printMatrix(input);

    // Perform feedforward (convolution).
    convLayer.feedforward(input);
    std::cout << "Convolution output (2D):\n";
    printMatrix(convLayer.output);

    // Show the output gradients.
    std::cout << "Convolution output gradients (2D):\n";
    printMatrix(outputGradients);

    // Perform backpropagation.
    convLayer.backpropagate(outputGradients);
    std::cout << "Input gradients after backpropagation (2D):\n";
    printMatrix(convLayer.inputGradients);
    return 0;
}
