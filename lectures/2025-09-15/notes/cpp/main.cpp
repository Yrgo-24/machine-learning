/**
 * @brief Minimalistic linear regression example.
 */
#include <cstdlib>
#include <ctime>
#include <iostream>
#include <vector>

namespace
{
/**
 * @brief Linear regression structure.
 */
struct LinReg
{
    const std::vector<double>& trainIn;  /** Input values for training (x). */
    const std::vector<double>& trainOut; /** Reference values for training (yref). */
    double bias;                         /** Bias value (m). */
    double weight;                       /** Weight value (k). */
};

/**
 * @brief Get the absolute value of the given number.
 * 
 * @param[in] num The number in question.
 * 
 * @return The absolute value of the given number.
 */
constexpr double absVal(const double num) noexcept { return 0.0 <= num ? num : -num; }

/**
 * @brief Get the number of complete training sets, i.e., the number of sets with an input value
 *        and an output value.
 * 
 * @param[in] in Input data sets.
 * @param[in] out Output/reference data sets.
 * 
 * @return The number of complete training sets.
 */
std::size_t trainSetCount(const std::vector<double>& in, const std::vector<double>& out) noexcept
{
    return in.size() <= out.size() ? in.size() : out.size();
}

/**
 * @brief Initialize the random generator (once only).
 */
void initRandom() noexcept
{
    // Create a static local variable, which indicates whether the generator has been initialized.
    // This line (with the initialization) is only run once.
    static auto initialized{false};

    // Terminate the function if the generator already has been initialized.
    if (initialized) { return; }

    // Initialize the generator, use the current time as seed (start of the random sequence).
    // Get the current time via std::time(nullptr).
    std::srand(std::time(nullptr));

    // Mark the random generator as initialized.
    initialized = true;
}

/**
 * @brief Get a random starting value for the linear regression parameters.
 * 
 * @return Random floating-point number between 0.0 - 1.0.
 */
double randomStartVal() noexcept
{
    // Divide std::rand() by RAND_MAX, cast RAND_MAX to double to ensure floating-point division.
    return std::rand() / static_cast<double>(RAND_MAX);
}

/**
 * @brief Shuffle the content of the given vector.
 * 
 * @param[in, out] data Reference to the vector to shuffle.
 */
void shuffle(std::vector<std::size_t>& data) noexcept
{
    // Shuffle the vector by swapping each element with a random element.
    for (std::size_t i{}; i < data.size(); ++i)
    {
        // Get a random index r (between 0-4 if we have five training sets).
        const auto r{std::rand() % data.size()};

        // Swap the elements at index i and r => make a copy of data[i].
        const auto temp{data[i]};

        // Copy data[r] to data[i] => now we have two instances of data[r] in the vector.
        data[i] = data[r];

        // Finally put the copy of the "old" data[i] to data[r] => we have swapped the elements.
        data[r] = temp;
    }
}

/**
 * @brief Predict with the given linear regression model.
 * 
 * @param[in] self Reference to the linear regression model.
 * @param[in] input Input value for which to predict (x).
 * 
 * @return The predicted value (ypred).
 */
double linRegPredict(const LinReg& self, const double input) noexcept
{
    return self.weight * input + self.bias;
}

/**
 * @brief Train the given linear regression model.
 * 
 * @param self Reference to the linear regression model to train.
 * @param epochCount The number of epochs to train the model.
 * @param learningRate The learning rate (default = 1 %).
 * 
 * @note Training stops when the model predicts within 1e-6 of the reference values, 
 *       or after the specified number of epochs.
 * 
 * @return True if the training was successful, false otherwise.
 */
bool linRegTrain(LinReg& self, const std::size_t epochCount, 
                 const double learningRate = 0.01)
{
    // Calculate the number of complete training sets.
    const auto setCount{trainSetCount(self.trainIn, self.trainOut)};

    // Return false if epochCount == 0, learningRate <= 0.0 or trainingSetCount == 0.
    if ((0U == epochCount) || (0.0 >= learningRate) || (0U == setCount)) { return false; }

    // Create a vector holding indexes of the training sets (for randomizing the training order).
    std::vector<std::size_t> trainIndex(setCount);
    
    // Add the indexes of the training sets (0, 1, 2... setCount - 1).
    for (std::size_t i{}; i < setCount; ++i) { trainIndex[i] = i; }

    // Train for `epochCount` epochs or until the model predicts within 1e-6 of the yref values.
    for (std::size_t epoch{}; epoch < epochCount; ++epoch)
    {
        double totalErr{};

        // Shuffle the content of trainIndex, i.e., randomize the order of the training sets.
        shuffle(trainIndex);

        // Iterate through the training sets one by one, use the random order from trainIndex.
        for (const auto& i : trainIndex)
        {
            // Get the values of the current training set.
            const auto x{self.trainIn[i]};
            const auto yref{self.trainOut[i]};

            // If x == 0.0, we just got our bias value, i.e., m = yref.
            if (0.0 == x) { self.bias = yref; }

            // Optimize the parameters as usual if x != 0.0.
            else
            {
                // Perform prediction and calculate the error/deviation.
                const auto ypred{linRegPredict(self, x)};
                const auto e{yref - ypred};
                
                // Modify the bias: m = m + e * LR.
                self.bias += e * learningRate;

                // Modify the weight: k = k + e * LR * x.
                self.weight += e * learningRate * x;

                // Add the absolute error value to the total.
                totalErr += absVal(e);
            }
        }
        // Calculate the average error of this epoch.
        const auto avgErr{totalErr / setCount};
        constexpr auto threshold{1.0 / 1000000};

        // Stop training if the average error is less than one in a million.
        if (threshold > avgErr)
        {
            std::cout << "Training finished after " << epoch << " epochs!\n";
            return true;
        }        
    }
    // Return true to indicate success.
    return true;
}
} // namespace

/**
 * @brief Train and predict with a simple linear prediction model.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{
    // Training parameters - adjust to get a feel how these affect the training.
    constexpr std::size_t epochCount{1000U};
    constexpr double learningRate{0.1};

    // Training sets to teach the model to predict y = 2x + 2.
    // Adjust these sets to adjust the model's behaviour.
    const std::vector<double> trainIn{0.0, 1.0, 2.0, 3.0, 4.0};
    const std::vector<double> trainOut{2.0, 4.0, 6.0, 8.0, 10.0};

    // Initialize the random generator before usage.
    initRandom();

    // Create a linear regression model with randomized weight and bias parameters.
    LinReg linReg{trainIn, trainOut, randomStartVal(), randomStartVal()};

    // Train the model, print the predicted values on success, log error on failure.
    if (linRegTrain(linReg, epochCount, learningRate))
    {
        for (const auto& x : trainIn)
        {
            std::cout << "x = " << x << ", ypred = " << linRegPredict(linReg, x) << "\n";
        }
    }
    else 
    { 
        std::cout << "Training failed!\n"; 
        return -1;
    }
    return 0;
}