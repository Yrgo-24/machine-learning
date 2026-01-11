/** 
 * @brief Simple flatten layer demo.
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
void initMatrix(Matrix1d& matrix) noexcept
{
    // Fill the matrix with zeros.
    for (auto& num : matrix) { num = 0.0; }
}

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
void initMatrix(Matrix1d& matrix, const std::size_t size)
{
    // Resize the matrix if necessary, then fill the matrix with zeros.
    matrix.resize(size);
    initMatrix(matrix);
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
void printMatrix(const Matrix1d& matrix, const std::size_t precision = 1U,
                 std::ostream& ostream = std::cout) noexcept
{
    // Set the decimal precision.
    ostream << std::fixed << std::setprecision(precision) << "\t";

    const auto* lastNum{matrix.empty() ? nullptr : &matrix[matrix.size() - 1U]};

    // Print the matrix contents on a single row.
    for (const auto& num : matrix)
    {
        // Separate each number in the row with a comma.
        ostream << num;
        if (&num < lastNum) { ostream << ", "; }
    }
    ostream << "\n";
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
 * @brief Flatten layer structure.
 * 
 * @todo Implement this structure!
 */
struct FlattenLayer final
{
    /**
     * @brief Constructor.
     * 
     * @param[in] inputSize Input size. Must be greater than 0.
     */

    /**
     * @brief Flatten the input from 2D to 1D.
     * 
     * @param[in] input Matrix holding input data.
     * 
     * @return True on success, false on failure.
     */

     /**
     * @brief Unflatten the output gradients from 1D to 2D.
     * 
     * @param[in] outputGradients Matrix holding output gradients.
     * 
     * @return True on success, false on failure.
     */

    /** @todo Define member variables here! Keep them public and skip the 'my' prefix. */
};
} // namespace ml
} // namespace 

/**
 * @brief Create and demonstrate a simple flatten layer.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{
//! @todo Remove this header guard (and/or uncomment the compiler flags in the makefile) once the
//        implementation is finished.
#ifdef FLATTEN_LAYER_IMPLEMENTED
    // Example 4x4 input matrix (could represent an image or feature map).
    const Matrix2d input{{2, 1, 6, 1},
                         {3, 0, 4, 6},
                         {1, 2, 4, 5},
                         {3, 4, 7, 7}};

    // Example output gradients (same shape as flattened output, used for backpropagation demo).
    const Matrix1d outputGradients{1, 2, 3, 4, 8, 7, 6, 5, 0, 2, 4, 8, 9, 7, 5, 3};

    // Create a flatten layer: 4x4 input, produces 1x16 output.
    constexpr std::size_t inputSize{4U};
    FlattenLayer flattenLayer{inputSize};

    // Perform feedforward (flatten the input), print the result.
    std::cout << "Flattening input data (2D -> 1D):\n";
    printMatrix(input);
    flattenLayer.feedforward(input);
    std::cout << "Resulting flattened output (1D):\n";
    printMatrix(flattenLayer.output);

    // Perform backpropagation (unflatten the output), print the result.
    std::cout << "Applying backpropagation (1D -> 2D):\n";
    printMatrix(outputGradients);
    flattenLayer.backpropagate(outputGradients);
    std::cout << "Resulting unflattened input gradients (2D):\n";
    printMatrix(flattenLayer.inputGradients);
    return 0;

//! @todo Remove this header guard (and/or uncomment the compiler flags in the makefile) once the
//        implementation is finished.
#endif /** FLATTEN_LAYER_IMPLEMENTED */
}