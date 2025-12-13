/**
 * @brief Minimalistic linear regression example.
 */
#include <stdbool.h>
#include <stdio.h>

/** 
 * @brief Training parameters - adjust to get a feel how these affect the training. 
 */
#define EPOCH_COUNT 1000U 
#define LEARNING_RATE 0.1
#define THRESHOLD (1.0 / 1000000)

/**
 * @brief Get the size of the given array.
 * 
 * @param[in] array The array in question.
 * 
 * @return The size of the array in number of elements.
 */
#define ARR_SIZE(array) (sizeof(array) / sizeof(array[0U]))

/**
 * @brief Get the number of complete training sets, i.e., the number of sets with an input value
 *        and an output value.
 * 
 * @param[in] in Input data sets.
 * @param[in] out Output/reference data sets.
 * 
 * @return The number of complete training sets.
 */
#define TRAIN_SET_COUNT(in, out) (ARR_SIZE(in) <= ARR_SIZE(out) ? ARR_SIZE(in) : ARR_SIZE(out))

/**
 * @brief Linear regression structure.
 */
typedef struct
{
    const double* const train_in;  /** Input values (x). */
    const double* const train_out; /** Reference values (yref). */
    const size_t set_count;        /** The number of complete training sets. */
    double bias;                   /** Bias value (m). */
    double weight;                 /** Weight value (k). */
} linreg_t;

/**
 * @brief Get the absolute value of the given number.
 * 
 * @param[in] num The number in question.
 * 
 * @return The absolute value of the given number.
 */
static inline double abs_val(const double num) { return 0.0 <= num ? num : -num; }

/**
 * @brief Predict with the given linear regression model.
 * 
 * @param[in] self Pointer to the linear regression model.
 * @param[in] input Input value for which to predict (x value).
 * 
 * @return The predicted value (ypred).
 */
static double linreg_predict(const linreg_t* self, const double input)
{
    return self->weight * input + self->bias;
}

/**
 * @brief Train the given linear regression model.
 * 
 * @param self Pointer to the linear regression model to train.
 * @param epoch_count The number of epochs to train the model.
 * @param learning_rate The learning rate.
 * 
 * @note Training stops when the model predicts within 1e-6 of the reference values, 
 *       or after the specified number of epochs.
 * 
 * @return True if the training was successful, false otherwise.
 */
bool linreg_train(linreg_t* self, const size_t epoch_count, const double learning_rate)
{
    // Return false if epoch_count == 0, learning_rate <= 0.0 or trainingSetCount == 0.
    if ((0U == epoch_count) || (0.0 >= learning_rate) || (0U == self->set_count)) { return false; }

    // Train for `epoch_count` epochs or until the model predicts within 1e-6 of the yref values.
    for (size_t epoch = 0U; epoch < epoch_count; ++epoch)
    {
        double total_err = 0.0;

        // Iterate through the training sets one by one.
        for (size_t i = 0U; i < self->set_count; ++i)
        {
            // Get the values of the current training set.
            const double x = self->train_in[i];
            const double yref = self->train_out[i];

            // If x == 0.0, we just got our bias value, i.e., m = yref.
            if (0.0 == x) { self->bias = yref; }

            // Optimize the parameters as usual if x != 0.0.
            else
            {
                // Perform prediction and calculate the error/deviation.
                const double ypred = linreg_predict(self, x);
                const double e = yref - ypred;
                
                // Modify the bias: m = m + e * LR.
                self->bias += e * learning_rate;

                // Modify the weight: k = k + e * LR * x.
                self->weight += e * learning_rate * x;

                // Add the absolute error value to the total.
                total_err += abs_val(e);
            }
        }
        // Calculate the average error of this epoch.
        const double avg_err = total_err / self->set_count;

        // Stop training if the average error is less than one in a million.
        if (THRESHOLD > avg_err)
        {
            printf("Training finished after %zu epochs!\n", epoch);
            return true;
        }        
    }
    // Return true to indicate success.
    return true;
}

/**
 * @brief Train and predict with a simple linear prediction model.
 * 
 * @return 0 on success, -1 on failure.
 */
int main()
{
    // Training sets to teach the model to predict y = 2x + 2.
    // Adjust these sets to train the model's behaviour.
    const double train_in[] = {0.0, 1.0, 2.0, 3.0, 4.0};
    const double train_out[] = {2.0, 4.0, 6.0, 8.0, 10.0};

    // Calculate the number of complete training sets.
    const size_t set_count = TRAIN_SET_COUNT(train_in, train_out);

    // Create a linear regression model, set the initial weight and bias parameters to 0.5.
    linreg_t linreg = {train_in, train_out, set_count, 0.5, 0.5};

    // Train the model, print the predicted values on success, log error on failure.
    if (linreg_train(&linreg, EPOCH_COUNT, LEARNING_RATE))
    {
        for (size_t i = 0U; i < set_count; ++i)
        {
            const double x = train_in[i];
            printf("x = %g, ypred = %g\n", x, linreg_predict(&linreg, x));
        }
    }
    else 
    { 
        printf("Training failed!\n"); 
        return -1;
    }
    return 0;
}