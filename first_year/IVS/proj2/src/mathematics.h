#ifndef MATHEMATICS_H
#define MATHEMATICS_H

/**
 * \defgroup mathlibrary Math library
 * Functions for basic operations with numbers
 * @{
 */


/**
 * Simple function to subtract one number from another
 * @author PatrikHolop
 * @brief subtract
 * @pre Both arguments are valid floating numbers
 * @param n1 Minuend of operation
 * @param n2 Subtrahend of operation
 * @return Returns difference of 2 numbers
 */
double subtract(double n1, double n2);

/**
 * Simple function to addition two numbers
 * @author TomasChocholaty
 * @brief addition
 * @pre Both arguments are valid floating numbers
 * @param n1 First number
 * @param n2 Second number
 * @return Returns sum of two numbers
 */
double addition(double n1, double n2);

/**
 * Simple division to subtract one number from another
 * @author PatrikHolop
 * @brief divide
 * @pre Both arguments are valid floating numbers, n2 != 0
 * @param n1 Numerator of operation
 * @param n2 Denumerator of operation
 * @warning throws error if n2 = 0
 * @return Returns quotient of 2 numbers
 */
double divide(double n1, double n2);

/**
 * Simple function to devide remainder
 * @author TomasChocholaty
 * @brief modulo
 * @pre Both arguments are valid floating numbers
 * @param n1 Numerator of operation
 * @param n2 Denumerator of operation
 * @return Returns devide remainder
 */
double modulo(double n1, double n2);

/**
 * Simple function to multiplication two number
 * @author TomasChocholaty
 * @brief multiplication
 * @pre Both arguments are valid floating numbers
 * @param n1 First number
 * @param n2 Second number
 * @return Returns multiple of two numbers
 */
double multiplication(double n1, double n2);

/**
 * Function for factorial of number
 * @author TomasChocholaty
 * @brief factorial
 * @pre Both arguments are valid floating numbers
 * @param n1 number for factorial
 * @return Return result of factorial
 */
double factorial(double n1);

/**
 * Function fot calculation fibonacci number list
 * @author TomasChocholaty
 * @brief fibonacci
 * @pre Both arguments are valid floating numbers
 * @param n1 position of numver in sequence
 * @return Return number of fibonacci number list
 */
double fibonacci(double n1);

/**
 * Function to calculate general root of number
 * @author PatrikHolop
 * @brief groot
 * @pre First and second argument is valid floating number, second arg != 0
 * @param n1 Number
 * @param n2 Power of root
 * @warning throws error if n2 = 0
 * @return Returns general root of number
 */
double groot(double n1, double n2);

/**
 * Function to calculate power of number
 * @author PatrikHolop
 * @brief power
 * @pre First argument is valid floating number, second is integer
 * @param n1 Number to be powered
 * @param n2 Power
 * @warning throws error if n2 is not integer
 * @return Returns power of number
 */
double power(double n1, double n2);

/**
@}
*/

#endif // MATHEMATICS_H
