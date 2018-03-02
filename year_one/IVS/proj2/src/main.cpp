#include "mainwindow.h"
#include <QApplication>
#include <QtTest/qtest.h>
#include "mathtests.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    if (argc==2)
    {
        if (strcmp(argv[1], "-DT")==NULL)
        {
            Subtraction sTest;
            Division dTest;
            Addition aTest;
            Multiplication mTest;
            Factorial fTest;
            Fibonacci fiTest;
            Power pTest;
            Root rTest;
            QTest::qExec(&sTest);
            QTest::qExec(&dTest);
            QTest::qExec(&aTest);
            QTest::qExec(&mTest);
            QTest::qExec(&pTest);
            QTest::qExec(&rTest);
            QTest::qExec(&fTest);
            QTest::qExec(&fiTest);
        }
    }
    else
        return a.exec();
}
