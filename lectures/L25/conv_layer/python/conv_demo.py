"""Simple convolutional layer demo."""
import random

# pylint: disable = consider-using-enumerate

# One-dimensional matrix.
Matrix1d = list[float]

# Two-dimensional matrix.
Matrix2d = list[Matrix1d]

def create_matrix2d(size: int) -> Matrix2d:
    """Create a two-dimensional matrix.

    Args:
        size: Size of the matrix.

    Returns:
        New matrix initialized with zeros.
    """
    matrix: Matrix2d = []
    for _ in range(size):
        row = [0.0] * size
        matrix.append(row)
    return matrix


def init_matrix2d(matrix: Matrix2d) -> None:
    """Initialize matrix with zeros.
    
    Args:
        matrix: The matrix to initialize.
    """
    for i in range(len(matrix)):
        for j in range(len(matrix)):
            matrix[i][j] = 0.0


def relu_output(num: float) -> float:
    """Get ReLU output based on the given input.
    
    Args:
        num: ReLU input.

    Returns: The corresponding ReLU output.
    """
    return num if num > 0 else 0


def relu_delta(num: float) -> float:
    """Get ReLU delta based on the given input.
    
    Args:
        num: ReLU input.

    Returns: The corresponding ReLU delta.
    """
    return 1.0 if num > 0 else 0


class ConvLayer:
    """Convolutional layer implementation."""
    # pylint: disable = too-many-instance-attributes

    def __init__(self, input_size: int, kernel_size: int) -> None:

        # Check the input parameters, throw an exception if invalid.
        if kernel_size == 0 or input_size < kernel_size:
            raise ValueError("Cannot create convolutional layer: invalid input arguments!")

        # Calculate the padded image size.
        pad_offset  = kernel_size // 2
        padded_size = input_size + 2 * pad_offset

        # Initialize the member variables.
        self.input_padded           = create_matrix2d(padded_size)
        self.input_gradients_padded = create_matrix2d(padded_size)
        self.input_gradients        = create_matrix2d(input_size)
        self.kernel                 = create_matrix2d(kernel_size)
        self.kernel_gradients       = create_matrix2d(kernel_size)
        self.output                 = create_matrix2d(input_size)
        self.bias                   = random.random()
        self.bias_gradient          = 0.0

        # Fill the kernel weights with random values in the range [0.0, 1.0].
        for ki in range(len(self.kernel)):
            for kj in range(len(self.kernel)):
                self.kernel[ki][kj] = random.random()

    def feedforward(self, input_data: Matrix2d) -> bool:
        """Perform feedforward operation.
        
        Args:
            input_data: Input data.

        Returns:
            True on success, false on failure.
        """
        # Check the input dimensions, return false on mismatch.
        if len(input_data) != len(self.output):
            return False

        # Pad the input, return false on failure.
        if not self._pad_input(input_data):
            return False

        # Perform feedforward - extract attributes out of the input data.
        for i in range(len(self.output)):
            for j in range(len(self.output)):
                num = self.bias
                for ki in range(len(self.kernel)):
                    for kj in range(len(self.kernel)):
                        num += self.input_padded[i + ki][j + kj] * self.kernel[ki][kj]
                self.output[i][j] = relu_output(num)
        # Return true to indicate success.
        return True

    def backpropagate(self, output_gradients: Matrix2d) -> bool:
        """Perform backpropagation.
        
        Args:
            output_gradients: Output gradients from the next layer.

        Returns:
            True on success, false on failure.
        """
        # Check the output dimensions, return false on mismatch.
        if len(output_gradients) != len(self.output):
            return False

        # Reinitialize the gradient matrices.
        init_matrix2d(self.input_gradients_padded)
        init_matrix2d(self.input_gradients)
        init_matrix2d(self.kernel_gradients)
        self.bias_gradient = 0.0

        # Perform backpropagation.
        for i in range(len(self.output)):
            # Check if the output gradient matrix is square, return false if not.
            if len(output_gradients[i]) != len(output_gradients):
                return False
            for j in range(len(self.output)):
                # Calculate the output gradient delta.
                delta = output_gradients[i][j] * relu_delta(self.output[i][j])
                self.bias_gradient += delta
                for ki in range(len(self.kernel)):
                    for kj in range(len(self.kernel)):
                        self.input_gradients_padded[i + ki][j + kj] += self.kernel[ki][kj] * delta
                        self.kernel_gradients[ki][kj] += self.input_padded[i + ki][j + kj] * delta
        # Extract the input gradients, then return true to indicate success.
        self._extract_input_gradients()
        return True

    def optimize(self, learning_rate: float) -> bool:
        """Perform optimization.
        
        Args:
            learning_rate: Learning rate used to adjust the trainable parameters.

        Returns:
            True on success, false on failure.
        """
        # Check the learning rate, return false if invalid.
        if learning_rate <= 0.0:
            return False

        # Optimize the traininable parameters.
        self.bias += self.bias_gradient * learning_rate
        for ki in range(len(self.kernel)):
            for kj in range(len(self.kernel)):
                self.kernel[ki][kj] += self.kernel_gradients[ki][kj] * learning_rate
        # Return true to indicate success.
        return True

    def _pad_input(self, input_data: Matrix2d) -> bool:
        # Fill the padded input matrix with zeros.
        init_matrix2d(self.input_padded)

        # Compute the pad offset.
        offset = len(self.kernel) // 2

        # Copy the input data to the padded input matrix.
        for i in range(len(input_data)):
            # Check if the input matrix is square, return false if not.
            if len(input_data[i]) != len(input_data):
                return False
            for j in range(len(input_data)):
                self.input_padded[i + offset][j + offset] = input_data[i][j]
        # Return true to indicate success.
        return True

    def _extract_input_gradients(self) -> None:
        # Compute the pad offset.
        offset = len(self.kernel) // 2

        # Pad the input gradients from the corresponding padded matrix.
        for i in range(len(self.output)):
            for j in range(len(self.output)):
                self.input_gradients[i][j] = self.input_gradients_padded[i + offset][j + offset]


def main() -> None:
    """Create and demonstrate a simple convolutional layer."""

    # Example 4x4 input matrix (could represent an image or feature map).
    input_data = [
        [1, 1, 1, 1],
        [1, 0, 0, 1],
        [1, 0, 0, 1],
        [1, 1, 1, 1],
    ]

    # Example output gradients (same shape as output, used for backpropagation demo).
    output_gradients = [
        [1, 1, 1, 1],
        [1, 1, 1, 1],
        [1, 1, 1, 1],
        [1, 1, 1, 1],
    ]

    # Create a convolutional layer: 4x4 input, 2x2 kernel.
    conv_layer = ConvLayer(4, 2)

    # Show the input matrix.
    print("Convolution input_data (2D):")
    for row in input_data:
        print("  ", " ".join(f"{num:.1f}" for num in row))

    # Perform feedforward (convolution).
    conv_layer.feedforward(input_data)
    print("\nConvolution output (2D):")
    for row in conv_layer.output:
        print("  ", " ".join(f"{num:.1f}" for num in row))

    # Show the output gradients.
    print("\nConvolution output gradients (2D):")
    for row in output_gradients:
        print("  ", " ".join(f"{num:.1f}" for num in row))

    # Perform backpropagation.
    conv_layer.backpropagate(output_gradients)
    print("\nInput gradients after backpropagation (2D):")
    for row in conv_layer.input_gradients:
        print("  ", " ".join(f"{num:.1f}" for num in row))


# Invoke the main function if this is the startup script.
if __name__ == "__main__":
    main()
