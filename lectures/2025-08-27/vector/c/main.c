/**
 * @brief Int vector implementation in C.
 */
#include <stdio.h>

/**
 * @brief Structure holding car parameters.
 */
typedef struct car
{
    const char* brand;             // The car brand.
    const char* model;            // The car model.
    unsigned year_of_manufacture; // The year of manufacture.
} car_t;

/**
 * @brief Print information about the given car.
 */
void car_print(const car_t* self)
{
    printf("%s %s, manufactured %u\n", self->brand, self->model, 
        self->year_of_manufacture);
}

int main(void)
{
    car_t car1 = {"Volvo", "V70", 1991U};
    car_t car2 = {"BMW", "Z3", 2003U};

    car_print(&car1);
    car_print(&car2);
    return 0;
}