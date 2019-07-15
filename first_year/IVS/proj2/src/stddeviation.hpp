#ifndef STDDEVIATION_HPP
#define STDDEVIATION_HPP
#include <vector>
/**
 * \defgroup stddev Standart deviation
 * Calculation of standart deviation
 * @{
 */

/**
 * Function reads number on input and calculates stddeviation
 * @author PatrikHolop
 * @brief reads input and calculate stddeviation
 * @pre Valid numbers on stdin
 * @return Standart deviation of numbers on input
 */
double calculateStddeviation();

/**
 * Function to calculate standart deviation of vector of numbers
 * @author PatrikHolop
 * @brief stddev
 * @pre First argument contains vector of floating number
 * @param numbers vector of numbers
 * @return Returns standart deviation
 */
double stddeviation(std::vector<double> numbers);

/**
@}
*/


#endif // STDDEVIATION_HPP
