#include <QtTest/QtTest>
#include "mathtests.hpp"
#include "mathematics.h"
#include <limits>

void Subtraction::subtractFloatPositives()
{
   testNumbers1F = {0.0, 5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {0.0, 5.6, 3.5, 1.8, 150.1};

   for (int i=0; i<testNumbers1F.size(); i++)
   {
       QCOMPARE(subtract(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]-testNumbers2F[i]);
   }
}

void Subtraction::subtractFloatNegatives()
{
    testNumbers1F = {-0.0, -5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {-0.0, -5, -3.5, -1.8, -150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(subtract(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]-testNumbers2F[i]);
    }
}

void Subtraction::subtractFloatPositiveNegative()
{
    testNumbers1F = {0.0, -5.5, -4.2, 2.4, -100.5};
    testNumbers2F = {-0.0, 5, 3.5, -1.8, 150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(subtract(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]-testNumbers2F[i]);
    }
}

void Division::divideFloatPositives()
{
   testNumbers1F = {5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {5.6, 3.5, 1.8, 150.1};

   for (int i=0; i<testNumbers1F.size(); i++)
   {
       QCOMPARE(divide(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]/testNumbers2F[i]);
   }

   QVERIFY_EXCEPTION_THROWN(divide(5.0,0.0), std::exception);
}

void Division::divideFloatNegatives()
{
    testNumbers1F = {-5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {-5, -3.5, -1.8, -150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(divide(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]/testNumbers2F[i]);
    }

    QVERIFY_EXCEPTION_THROWN(divide(-5.0,-0.0), std::exception);
}

void Division::divideFloatPositiveNegative()
{
    testNumbers1F = {-5.5, -4.2, 2.4, -100.5};
    testNumbers2F = {5, 3.5, -1.8, 150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(divide(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]/testNumbers2F[i]);
    }

    QVERIFY_EXCEPTION_THROWN(divide(-5.0,0.0), std::exception);
}



void Addition::additionFloatPositives()
{
   testNumbers1F = {0.0, 5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {0.0, 5.6, 3.5, 1.8, 150.1};

   for (int i=0; i<testNumbers1.size(); i++)
   {
       QCOMPARE(addition(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]+testNumbers2F[i]);
   }
}

void Addition::additionFloatNegatives()
{
    testNumbers1F = {-0.0, -5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {-0.0, -5, -3.5, -1.8, -150.1};

    for (int i=0; i<testNumbers1.size(); i++)
    {
        QCOMPARE(addition(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]+testNumbers2F[i]);
    }
}

void Addition::additionFloatPositiveNegative()
{
    testNumbers1F = {0.0, -5.5, -4.2, 2.4, -100.5};
    testNumbers2F = {-0.0, 5, 3.5, -1.8, 150.1};

    for (int i=0; i<testNumbers1.size(); i++)
    {
        QCOMPARE(addition(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]+testNumbers2F[i]);
    }
}




void Multiplication::multiplicationFloatPositives()
{
   testNumbers1F = {5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {5.6, 3.5, 1.8, 150.1};

   for (int i=0; i<testNumbers1F.size(); i++)
   {
       QCOMPARE(multiplication(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]*testNumbers2F[i]);
   }

}

void Multiplication::multiplicationFloatNegatives()
{
    testNumbers1F = {-5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {-5, -3.5, -1.8, -150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(multiplication(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]*testNumbers2F[i]);
    }

}

void Multiplication::multiplicationFloatPositiveNegative()
{
    testNumbers1F = {-5.5, -4.2, 2.4, -100.5};
    testNumbers2F = {5, 3.5, -1.8, 150.1};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(multiplication(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]*testNumbers2F[i]);
    }
}

void Multiplication::multiplicationZero()
{
    testNumbers1F = {0.0, -0.0, -0.0, 0.0};
    testNumbers2F = {0.0, -0.0, 0.0, -0.0};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(multiplication(testNumbers1F[i],testNumbers2F[i]), testNumbers1F[i]*testNumbers2F[i]);
    }
}



void Factorial::factorialZero()
{
   testNumbers1 = {0};

   for (int i=0; i<testNumbers1.size(); i++)
   {
       QVERIFY_EXCEPTION_THROWN(factorial(testNumbers1[i]), std::exception);
   }

}


void Factorial::factorialFloatPositive()
{
   testNumbers1F = {0.5, 5.8, 6.5};
   testNumbers2F = {0.88622692545275805,496.60607757369064, 1871.2543057977884};
   for (int i=0; i<testNumbers1.size(); i++)
   {
     QCOMPARE(factorial(testNumbers1F[i]), factorial(testNumbers2F[i]));
   }

}


void Factorial::factorialFloatNegative()
{
   testNumbers1F = {-0.5, -5.8, -6.5};

   for (int i=0; i<testNumbers1.size(); i++)
   {
       QVERIFY_EXCEPTION_THROWN(factorial(testNumbers1F[i]), std::exception);
   }

}
void Factorial::factorialIntNegative()
{
   testNumbers1 = {-5, -10, -20};

   for (int i=0; i<testNumbers1.size(); i++)
   {
       QVERIFY_EXCEPTION_THROWN(factorial(testNumbers1[i]), std::exception);
   }

}


void Factorial::factorialInt()
{
    testNumbers1F = {5, 7, 10, 12};
    testNumbers2F = {120, 5040, 3628800, 479001600};


    for (int i=0; i<testNumbers1.size(); i++)
    {
        QCOMPARE(factorial(testNumbers1F[i]), testNumbers2F[i]);
    }
}


void Fibonacci::fibonacciNegative()
{
    testNumbers1F = {0,-5,-10};



    for (int i=0; i<testNumbers1.size(); i++)
    {
        QVERIFY_EXCEPTION_THROWN(fibonacci(testNumbers1[i]), std::runtime_error);
    }
}


void Fibonacci::fibonacciInt()
{
    testNumbers1F = {5, 10, 12};
    testNumbers2F = {5, 55, 144};


    for (int i=0; i<testNumbers1.size(); i++)
    {
        QCOMPARE(fibonacci(testNumbers1F[i]), testNumbers2F[i]);
    }
}




void Power::powerFloatPositives()
{
   testNumbers1F = {1.0, 5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {1, 5, 3, 1, 150};

   for (int i=0; i<testNumbers1F.size(); i++)
   {
       QCOMPARE(power(testNumbers1F[i],testNumbers2F[i]), pow(testNumbers1F[i],testNumbers2F[i]));
   }

   QVERIFY_EXCEPTION_THROWN(power(5.0,0.5), std::exception);
}

void Power::powerFloatNegatives()
{
    testNumbers1F = {-0.0, -5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {5, 5, 3, 1, 150};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(power(testNumbers1F[i],testNumbers2F[i]), pow(testNumbers1F[i],testNumbers2F[i]));
    }
    QVERIFY_EXCEPTION_THROWN(power(-5.0,-0.5), std::exception);
}

void Power::powerFloatPositiveNegative()
{
    testNumbers1F = {0.0, -7.5, -4.2, 8.4, -100.5};
    testNumbers2F = {1, 5, 3, 1, 150};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(power(testNumbers1F[i],testNumbers2F[i]), pow(testNumbers1F[i],testNumbers2F[i]));
    }
    QVERIFY_EXCEPTION_THROWN(power(5.0,-0.5), std::exception);
}

void Root::rootFloatPositives()
{
   testNumbers1F = {5.5, 4.2, 2.4, 100.5};
   testNumbers2F = {5, 3, 1, 150};

   for (int i=0; i<testNumbers1F.size(); i++)
   {
       QCOMPARE(groot(testNumbers1F[i],testNumbers2F[i]), pow(testNumbers1F[i],1/testNumbers2F[i]));
   }
}

void Root::rootFloatNegatives()
{
    testNumbers1F = {-5.5, -4.2, -2.4, -100.5};
    testNumbers2F = {5, 3, 1, 150};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        QCOMPARE(groot(testNumbers1F[i],testNumbers2F[i]),-pow(-testNumbers1F[i],1/testNumbers2F[i]));
    }

    //QVERIFY_EXCEPTION_THROWN(groot(-5.0,0), std::runtime_error);
}

void Root::rootFloatPositiveNegative()
{
    testNumbers1F = {-5.5, -4.2, 2.4, -100.5};
    testNumbers2F = {5, 3, 1, 150};

    for (int i=0; i<testNumbers1F.size(); i++)
    {
        double r1;
        if (testNumbers1F[i]<0.0)
            r1 = -pow(-testNumbers1F[i],1/testNumbers2F[i]);
        else
            r1 = pow(testNumbers1F[i],1/testNumbers2F[i]);
        QCOMPARE(groot(testNumbers1F[i],testNumbers2F[i]),r1);
    }
    QVERIFY_EXCEPTION_THROWN(groot(5.0,0), std::exception);
}


//QTEST_APPLESS_MAIN(TestyTest)
//QTEST_MAIN(TestyTest)
