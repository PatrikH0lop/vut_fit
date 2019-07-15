#ifndef MATHTESTS_HPP
#define MATHTESTS_HPP
#include <QApplication>

/**
 * \defgroup tests Testing
 * Classes to test functions of math library
 * @{
 */

/**
 * Testing class of function 'subtract'
 * @author PatrikHolop
 * @brief The Subtraction class
 */
class Subtraction : public QObject
{
    Q_OBJECT

public:
    /// Minuends
    std::vector<double> testNumbers1F;
    /// Subtrahends
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Test subracting on positive values
     * @brief subtractFloatPositives
     */
    void subtractFloatPositives();
    /**
     * Test subracting on negative values
     * @brief subtractFloatNegatives
     */
    void subtractFloatNegatives();
    /**
     * Test subracting on positive and negative values
     * @brief subtractFloatPositiveNegative
     */
    void subtractFloatPositiveNegative();
};

/**
 * Testing class of function 'divide'
 * @author PatrikHolop
 * @brief The Division class
 */
class Division : public QObject
{
    Q_OBJECT

public:
    /// Numerators
    std::vector<double> testNumbers1F;
    /// Denumerators
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Test dividing on positive values
     * @brief divideFloatPositives
     */
    void divideFloatPositives();
    /**
     * Test dividing on negative values
     * @brief divideFloatNegatives
     */
    void divideFloatNegatives();
    /**
     * Test dividing on positive and negative values
     * @brief divideFloatPositiveNegative
     */
    void divideFloatPositiveNegative();
};

/**
 * Testing class of function 'addition'
 * @author TomasChocholaty
 * @brief The Addition class
 */
class Addition : public QObject
{
    Q_OBJECT

public:
    std::vector<int> testNumbers1;
    std::vector<int> testNumbers2;
    std::vector<double> testNumbers1F;
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Test addition on positive values
     * @brief additionFloatPositives
     */
    void additionFloatPositives();
    /**
     * Test addition on negative values
     * @brief additionFloatNegatives
     */
    void additionFloatNegatives();
    /**
     * Test addition on positive and negative values
     * @brief additionFloatPositiveNegative
     */
    void additionFloatPositiveNegative();
};


/**
 * Testing class of function 'multiplication'
 * @author TomasChocholaty
 * @brief The Multiplication class
 */
class Multiplication : public QObject
{
    Q_OBJECT

public:
    std::vector<int> testNumbers1;
    std::vector<int> testNumbers2;
    std::vector<double> testNumbers1F;
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Test multiplication on positive values
     * @brief multiplicationFloatPositive
     */
    void multiplicationFloatPositives();
    /**
     * Test multiplication on negative values
     * @brief multiplicationFloatNegatives
     */
    void multiplicationFloatNegatives();
    /**
     * Test multiplication on positive and negative values
     * @brief multiplicationFloatPositiveNegative
     */
    void multiplicationFloatPositiveNegative();
    /**
     * Test multiplication on zero value
     * @brief multiplicationZero
     */
    void multiplicationZero();
};


/**
 * Testing class of function 'factorial'
 * @author TomasChocholaty
 * @brief The Factorial class
 */
class Factorial : public QObject
{
    Q_OBJECT

public:
    std::vector<int> testNumbers1;
    std::vector<int> testNumbers2;
    std::vector<double> testNumbers1F;
    std::vector<double> testNumbers2F;


private Q_SLOTS:
    /**
     * Test factorial on positive values
     * @brief factorialFloatPositive
     */
    void factorialFloatPositive();
    /**
     * Test factorial on negative values
     * @brief factorialFloatNegative
     */
    void factorialFloatNegative();
    /**
     * Test factorial on zero values
     * @brief factorialZero
     */
    void factorialZero();
    /**
     * Test factorial on integer values
     * @brief factorialInt
     */
    void factorialInt();
    /**
     * Test factorial on negative values
     * @brief factorialIntNegativ
     */
    void factorialIntNegative();
};


/**
 * Testing class of function 'fibonacci'
 * @author TomasChocholaty
 * @brief The Fibonacci class
 */
class Fibonacci : public QObject
{
    Q_OBJECT

public:
    std::vector<int> testNumbers1;
    std::vector<int> testNumbers2;
    std::vector<double> testNumbers1F;
    std::vector<double> testNumbers2F;


private Q_SLOTS:
    /**
     * Test fibonacci on negative values
     * @brief fibonacciNegative
     */
    void fibonacciNegative();
    /**
     * Test fibonacci on integer values
     * @brief fibonacciIntNegativ
     */
    void fibonacciInt();

};


/**
 * Testing class of function 'groot'
 * @author PatrikHolop
 * @brief The Root class
 */
class Root : public QObject
{
    Q_OBJECT

public:
    /// Numbers
    std::vector<double> testNumbers1F;
    /// Powers of root
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Test rooting on positive values
     * @brief rootFloatPositives
     */
    void rootFloatPositives();
    /**
     * Test rooting on negative values
     * @brief rootFloatNegatives
     */
    void rootFloatNegatives();
    /**
     * Test rooting on positive and negative values
     * @brief rootFloatPositiveNegative
     */
    void rootFloatPositiveNegative();
};

/**
 * Testing class of function 'power'
 * @author PatrikHolop
 * @brief The Power class
 */
class Power : public QObject
{
    Q_OBJECT

public:
    /// Numbers
    std::vector<double> testNumbers1F;
    /// Powers
    std::vector<double> testNumbers2F;

private Q_SLOTS:
    /**
     * Testing powers on positive values
     * @brief powerFloatPositives
     */
    void powerFloatPositives();
    /**
     * Testing powers on negative values
     * @brief powerFloatNegatives
     */
    void powerFloatNegatives();
    /**
     * Testing powers on positive and negative values
     * @brief powerFloatPositiveNegative
     */
    void powerFloatPositiveNegative();
};


/**
@}
*/

#endif // MATHTESTS_HPP
