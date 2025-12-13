/**
 * @brief Minimalistic linear regression example.
 */
#include <iostream>
#include <vector>

namespace
{
/**
 * @brief Linear regression structure.
 */
struct LinReg
{
    const std::vector<double>& trainIn;  /** Input values (x). */
    const std::vector<double>& trainOut; /** Reference values (yref). */
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
 * @brief Predict with the given linear regression model.
 * 
 * @param[in] self Reference to the linear regression model.
 * @param[in] input Input value for which to predict (x value).
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
                 const double learningRate = 0.01) noexcept
{
    // Calculate the number of complete training sets.
    const auto setCount{trainSetCount(self.trainIn, self.trainOut)};

    // Return false if epochCount == 0, learningRate <= 0.0 or trainingSetCount == 0.
    if ((0U == epochCount) || (0.0 >= learningRate) || (0U == setCount)) { return false; }

    // Train for `epochCount` epochs or until the model predicts within 1e-6 of the yref values.
    for (std::size_t epoch{}; epoch < epochCount; ++epoch)
    {
        double totalErr{};

        // Iterate through the training sets one by one.
        for (std::size_t i{}; i < setCount; ++i)
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
    // Adjust these sets to train the model's behaviour.
    const std::vector<double> trainIn{0.0, 1.0, 2.0, 3.0, 4.0};
    const std::vector<double> trainOut{2.0, 4.0, 6.0, 8.0, 10.0};

    // Create a linear regression model, set the initial weight and bias parameters to 0.5.
    LinReg linReg{trainIn, trainOut, 0.5, 0.5};

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