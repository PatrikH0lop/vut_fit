#include "mathematics.h"
#include <stdexcept>
#include <cmath>
#include <string>
#include "System.h"

double subtract(double a, double b)
{
    return a-b;
}

double divide(double a, double b)
{
    if (b==0)
    {
        throw (ParserException("Division by zero"));
    }
    return a/b;
}

double addition(double a, double b)
{
    return a+b;
}

double modulo(double a, double b)
{
    double intpart;
    if((std::modf(a, &intpart) != 0.0)&&(std::modf(b, &intpart) != 0.0)){
        throw (ParserException("Number is not integer"));
    }
    return static_cast<int>(a) % static_cast<int>(b);
}


double multiplication(double a, double b)
{
    return a*b;
}

double factorial(double a)
{
    double number = 1;
    double intpart;
    if(std::modf(a, &intpart) != 0.0){
        return std::tgamma(a + 1);
    }else{
        if(a>0){
            for (int i = (int)a; i>0; i--){
            number *= i;
            }

        }else{
             throw (ParserException("Number is not define"));
        }
    return number;
    }
}

double fibonacci(double n)
{
    double number = 0;
    if (n == 0){
        return 0;
    } else if (n == 1){
        return 1;
    }
    else if (n == 2){
        return 1;
    }else if(n>2){
    double intpart;
    if (std::modf(n, &intpart) != 0.0)
    {
        throw (ParserException("Exponent is not integer"));
    }else{
        int first = 1;
        int second = 1;

        for (int i = 2; i < n; i++){
            number = first + second;
            first = second;
            second = number;
        }
     }
    }
    else{
         throw (ParserException("number <1, ...>"));
    }

    return number;
}

double power(double a, double b)
{
    double intpart;
    if (std::modf(b, &intpart) != 0.0 || b<0)
    {
        throw (ParserException("Exponent is not integer"));
    }
    return pow(a,b);
}

double groot(double a, double b)
{
    double intpart;
    if (b==0)
    {
        throw (ParserException("Division by zero"));
    }
    if (a<0) {
        if (std::modf(b, &intpart) == 0.0 && ((long int)b)%2==0) {
            throw (ParserException("Not defined"));
        }
        return -pow(-a,1/b);
    }
    return pow(a, 1/b);
}
