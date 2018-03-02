#include <vector>
#include "mathematics.h"

double stddeviation(std::vector<double> numbers)
{
    double average = 0;
    for (int i = 0; i<numbers.size(); i++)
    {
        average=addition(average, numbers[i]);
    }
    average=divide(average, numbers.size());

    double sum = 0;
    double partialsum = 0;
    for (int i = 0; i<numbers.size(); i++)
    {
        partialsum=subtract(power(numbers[i],2), multiplication(power(average,2), numbers.size()));
        sum=addition(sum, partialsum);
    }
    sum = multiplication(divide(1, subtract(numbers.size(), 1)), sum);
    double result = groot(sum, 2);

    return result;
}
