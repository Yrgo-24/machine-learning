/**
 * @brief Simple max pooling layer demo.
 */
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
 * @brief Initialize matrix with zeros.
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

namespace ml
{
/**
 * @brief Max pooling layer structure.
 * 
 * @todo Implement this structure!
 */
struct MaxPoolLayer final
{
    /**
     * @brief Constructor.
     * 
     * @param[in] inputSize Input size. Must be greater than 0.
     * @param[in] poolSize Pool size. Must divide the input size.
     */
    explicit MaxPoolLayer(const std::size_t inputSize, const std::size_t poolSize)
        : input{}
        , inputGradients{}
        , output{}
    {
        // Check the input arguments, throw an exception if invalid.
        if ((0U == inputSize) || (0U == poolSize) || (0U != (inputSize % poolSize)))
        {
            throw std::invalid_argument(
                "Cannot create max pooling layer: invalid input arguments!");
        }
        
        // Compute the output size.
        const std::size_t outputSize{inputSize / poolSize};

        // Initialize the matrices.
        initMatrix(input, inputSize);
        initMatrix(inputGradients, inputSize);
        initMatrix(output, outputSize);
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
        if ((input.size() != this->input.size()) || !isMatrixSquare(input)) { return false; }

        // Calculate the pool size.
        const std::size_t poolSize{input.size() / output.size()};

        // Iterate through the image pool by pool, find and store the max value.
        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                // Get the input row and column.
                const std::size_t inRow{i * poolSize};
                const std::size_t inCol{j * poolSize};

                // Use the first value as max value, compare with the other values in the pool.
                double maxVal{input[inRow][inCol]};

                // Iterate through the pool.
                for (std::size_t pi{}; pi < poolSize; ++pi)
                {
                    for (std::size_t pj{}; pj < poolSize; ++pj)
                    {
                        // Get the value at the current cell.
                        const auto val{input[inRow + pi][inCol + pj]};

                        // Compare the value with the local max, store the bigger one.
                        if (val > maxVal) { maxVal = val; }
                    }
                }
                // Store the max value in the output matrix.
                output[i][j] = maxVal;
            }
        }
        // Store the input for backpropagation.
        this->input = input;

        // Return true to indicate success.
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
        // Check the output gradient matrix, return false on dimension mismatch.
        if ((outputGradients.size() != output.size()) || !isMatrixSquare(outputGradients))
        {
            return false;
        }

        // Calculate the pool size.
        const std::size_t poolSize{input.size() / output.size()};

        // Reinitialize input matrix with zeros (remove leftovers from previous backpropagation).
        initMatrix(inputGradients);
        
        // Locate the max value coordinates (row, col) and place the gradients there.
        for (std::size_t i{}; i < output.size(); ++i)
        {
            for (std::size_t j{}; j < output.size(); ++j)
            {
                // Compute the input row and column.
                const std::size_t inRow{i * poolSize};
                const std::size_t inCol{j * poolSize};

                // Get the max value for comparison.
                const auto maxVal{output[i][j]};

                // Variables holding the max coordinates (start with the first call of the pool).
                std::size_t maxRow{inRow};
                std::size_t maxCol{inCol};

                // Indicate whether the max value has been found.
                bool found{false};

                for (std::size_t pi{}; pi < poolSize; ++pi)
                {
                    for (std::size_t pj{}; pj < poolSize; ++pj)
                    {
                        // Get the value of the current cell.
                        const auto val{input[inRow + pi][inCol + pj]};

                        // If this is the max value, store the coordinates.
                        if (val == maxVal)
                        {
                            // Store the coordinates of the max value.
                            maxRow = inRow + pi;
                            maxCol = inCol + pj;

                            // Indicate that the value has been found and break the 'pj' loop.
                            found = true;
                            break;
                        }
                    }
                    // Break the 'pi' loop if the max value has been found.
                    if (found) { break; }
                }
                // Write the output gradient to the max value position.
                inputGradients[maxRow][maxCol] = outputGradients[i][j];
            }
        }
        // Return true to indicate success.
        return true;
    }

    /** Input matrix. */
    Matrix2d input;

    /** Input gradient matrix. */
    Matrix2d inputGradients;

    /** Output matrix. */
    Matrix2d output;
};
} // namespace ml
} // namespace 

/**
 * @brief Create and demonstrate a simple max pooling layer.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{   
    // Example 4x4 input matrix (could represent an image or feature map).
    const Matrix2d input{{2, 1, 6, 1},
                         {3, 0, 4, 6},
                         {1, 2, 4, 5},
                         {3, 4, 7, 7}};
        
    // Example output gradients (same shape as pooling output, used for backpropagation demo).
    const Matrix2d outputGradients{{1, 2},
                                   {3, 4}};

    // Create a max pooling layer: 4x4 input, 2x2 pooling regions, produces 2x2 output.
    constexpr std::size_t inputSize{4U};
    constexpr std::size_t poolSize{2U};
    ml::MaxPoolLayer poolLayer{inputSize, poolSize};
    
    // Show the input matrix.
    std::cout << "Pooling input data (2D):\n";
    printMatrix(input);

    // Perform feedforward (pooling).
    poolLayer.feedforward(input);
    std::cout << "Pooled output (2D):\n";
    printMatrix(poolLayer.output);

    // Show the output gradients.
    std::cout << "Pooling output gradients (2D):\n";
    printMatrix(outputGradients);

    // Perform backpropagation.
    poolLayer.backpropagate(outputGradients);
    std::cout << "Input gradients after backpropagation (2D):\n";
    printMatrix(poolLayer.inputGradients);
    return 0;
}
