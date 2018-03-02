//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     JMENO PRIJMENI <xlogin00@stud.fit.vutbr.cz>
// $Date:       $2017-01-04
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author JMENO PRIJMENI
 * 
 * @brief Implementace testu prace s maticemi.
 */

#include "gtest/gtest.h"
#include "white_box_code.h"

//============================================================================//
// ** ZDE DOPLNTE TESTY **
//
// Zde doplnte testy operaci nad maticemi. Cilem testovani je:
// 1. Dosahnout maximalniho pokryti kodu (white_box_code.cpp) testy.
// 2. Overit spravne chovani operaci nad maticemi v zavislosti na rozmerech 
//    matic.
//============================================================================//

class testMatrix : public ::testing::Test
{
    virtual void SetUp() {
        m1 = new Matrix(10,10);       
        for (int i = 0; i < 10; i++)
            for (int j = 0; j < 10; j++)
                m1->set(i,j,i+j);
    }
    protected:
        Matrix *m1;  
};

class testMatrixPointer : public ::testing::Test
{
    protected:
        Matrix *m1;  
        Matrix m2;
};

class testMatrixOperator : public ::testing::Test
{
    protected:
        Matrix *m1;  
        Matrix *m2;
        Matrix resultMatrix;
};

class testEquationSolving : public ::testing::Test
{
    virtual void SetUp() {        
        m1 = new Matrix(3,3);     
        m2 = new Matrix(2,2);
        m3 = new Matrix();
        m4 = new Matrix(4,4);
        m5 = new Matrix(1,3);
        wrongMatrix = new Matrix(1,4);
 		std::vector<std::vector<double> > v4x4 = {{1,-3,5,5},
                                                  {2,1,-6,5},
                                                  {3,-2,1,5},
                                              	  {3,-2,1,5}};
        std::vector<std::vector<double> > v3x3 = {{1,-3,5},
                                                  {2,1,-6},
                                                  {3,-2,1}};
        std::vector<std::vector<double> > v2x2 = {{1,1},
                                                  {1,-1}};
        std::vector<std::vector<double> > v1x3 = {{1,2,3}};                                                  
        m1->set(v3x3);
        m2->set(v2x2);
        m3->set(0,0,3);
        m4->set(v4x4);
        m5->set(v1x3);

        cooeficients = {-14,20,0};
        cooeficients2 = {2,4};
        cooeficients3 = {6};
        cooeficients4 = {1,2,3,4};
        cooeficients5 = {5};
        results = {1,0,-3};
    }
    protected:
        Matrix *m1;
        Matrix *m2;
        Matrix *m3;  
        Matrix *m4; 
        Matrix *m5;
        Matrix *wrongMatrix;
        std::vector<double> cooeficients;
        std::vector<double> cooeficients2;        
        std::vector<double> cooeficients3;
        std::vector<double> cooeficients4;        
        std::vector<double> cooeficients5;
        std::vector<double> results;
};

///////////////////////////////////////////////////////////////////////////

TEST_F(testMatrixPointer, creating_1x1)
{
    m1 = new Matrix;
    m1->set(0,0,10);
    EXPECT_EQ(m1->get(0,0), 10.0);    
    EXPECT_TRUE(std::isnan(m1->get(0,1)));
}

TEST_F(testMatrixPointer, creating_10x10)
{
    m1 = new Matrix(10,10);
    for (int i = 0; i<10; i++) {
        for (int j = 0; j<10; j++) {
            EXPECT_TRUE(m1->set(i,j,i+j));
        }
    }
    EXPECT_TRUE(std::isnan(m1->get(10,10)));
}

TEST_F(testMatrixPointer, creating_WrongMatrix)
{
    EXPECT_THROW(m2 = Matrix(0,0), std::runtime_error*);
}

///////////////////////////////////////////////////////////////////////////////

TEST_F(testMatrix, setValue)
{
    m1->set(0,0,50);  
    EXPECT_EQ(m1->get(0,0), 50);      
    m1->set(9,9,10);
    EXPECT_EQ(m1->get(9,9), 10);
    EXPECT_FALSE(m1->set(10,10,0));    
    EXPECT_TRUE(m1->set(5,5,0));
}

TEST_F(testMatrix, setVector)
{
    std::vector<std::vector<double> > vector1;
    std::vector<std::vector<double> > vector2;
    std::vector<std::vector<double> > vector3 = {{3,2,5},{8,5,5}};
    for (int i = 0; i<5; i++) {
        std::vector<double> temp;
        for (int j = 0; j<5; j++) {
            temp.push_back(j*i);
        }
        vector1.push_back(temp);
    }
    for (int i = 0; i<20; i++) {
        std::vector<double> temp;
        for (int j = 0; j<20; j++) {
            temp.push_back(j*i);
        }
        vector2.push_back(temp);
    }

    EXPECT_FALSE(m1->set(vector1));
    EXPECT_TRUE(m1->set(vector2));
    
    delete(m1);
    m1 = new Matrix(2,2);
    EXPECT_FALSE(m1->set(vector3));
    
}


TEST_F(testMatrix, getOk)
{
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) 
            EXPECT_EQ(m1->get(i,j),i+j);
}


TEST_F(testMatrix, getWrong)
{
	//EXPECT_TRUE(std::isnan(m1->get(-10,-10)));
	EXPECT_ANY_THROW(m1->get(-10,-10));    
    EXPECT_ANY_THROW(m1->get(100,10));
}


TEST_F(testMatrixOperator, operatorCompare)
{
    m1 = new Matrix(10,10);
    m2 = new Matrix(10,10);
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            m1->set(i,j,i+j);
            m2->set(i,j,i+j);
        }

    EXPECT_TRUE(*m1==*m2); 

     for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            m2->set(i,j,i*j);
        }

    EXPECT_FALSE(*m1==*m2);

    delete(m2);
    m2 = new Matrix(5,5);
    for (int i = 0; i<5; i++)
        for (int j = 0; j<5; j++) {
            m2->set(i,j,i-j);
        }
    EXPECT_THROW(*m1==*m2, std::runtime_error*);
}

TEST_F(testMatrixOperator, operatorAdd)
{
    m1 = new Matrix(10,10);
    m2 = new Matrix(10,10);
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            m1->set(i,j,i+j);
            m2->set(i,j,i+j);
        }
    resultMatrix = *m1+*m2;
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            EXPECT_EQ(resultMatrix.get(i,j),2*(i+j));
        }

    delete(m2);
    m2 = new Matrix(5,5);
    for (int i = 0; i<5; i++)
        for (int j = 0; j<5; j++) {
            m2->set(i,j,i-j);
        }

    EXPECT_THROW(resultMatrix = *m1+*m2, std::runtime_error*);
}

TEST_F(testMatrixOperator, operatorMultiplyScalar)
{
    m1 = new Matrix(10,10);
    m2 = new Matrix(10,10);
    
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            m1->set(i,j,i+j);
            m2->set(i,j,i+j);
        }
    resultMatrix = *m1*3;
    for (int i = 0; i<10; i++)
        for (int j = 0; j<10; j++) {
            EXPECT_EQ(resultMatrix.get(i,j),3*(i+j));
        }
}

TEST_F(testMatrixOperator, operatorMultiplyMatrix)
{
    m1 = new Matrix(2,3);    
    m2 = new Matrix(3,2);

    double counter = 1;
    for (int i = 0; i<2; i++) {
        for (int j = 0; j<3; j++) {
            m1->set(i,j,counter);
            counter++;
        }
    }
    counter = 1;
    for (int i = 0; i<3; i++) {
        for (int j = 0; j<2; j++) {
            m2->set(i,j,counter);
            counter++;
        }
    }

    Matrix *matrixExpected = new Matrix(2,2);
    matrixExpected->set(0,0,22);
    matrixExpected->set(0,1,28);
    matrixExpected->set(1,0,49);
    matrixExpected->set(1,1,64);

    resultMatrix = *m1*(*m2);
    EXPECT_TRUE(*matrixExpected==resultMatrix);

    delete(m2);
    m2 = new Matrix(5,5);
    EXPECT_THROW(*m1*(*m2), std::runtime_error*);
}


TEST_F(testEquationSolving, solveEquation3x3)
{
    std::vector<double> result = m1->solveEquation(cooeficients);
    for (int i = 0; i<results.size(); i++) {
        EXPECT_EQ(results[i],result[i]);
    }
}

TEST_F(testEquationSolving, solveEquation3x3Wrong)
{
    cooeficients.push_back(4);
    std::vector<double> result;
    EXPECT_THROW(result = m1->solveEquation(cooeficients), std::runtime_error* );
    EXPECT_THROW(result = wrongMatrix->solveEquation(cooeficients), std::runtime_error* );

    for (int i = 0; i<3; i++) {
        for (int j = 0; j<3; j++) {
            m1->set(i,j,0);
        }
    }    
    cooeficients.pop_back();
}

TEST_F(testEquationSolving, solveEquation2x2) {	
    std::vector<double> results2;
    results2 = m2->solveEquation(cooeficients2);
    EXPECT_EQ(results2[0],3);
    EXPECT_EQ(results2[1],-1);
}

TEST_F(testEquationSolving, solveEquation1x1) {
    std::vector<double> results1x1 = m3->solveEquation(cooeficients3);
    EXPECT_EQ(2, results1x1[0]);
}

TEST_F(testEquationSolving, solveEquation1x3) {    
	std::vector<double> results;
    EXPECT_THROW(results = m5->solveEquation(cooeficients5), std::runtime_error*);
}

TEST_F(testEquationSolving, solveDifferent) 
{   
    EXPECT_ANY_THROW(std::vector<double> results4x4 = m4->solveEquation(cooeficients4));
}

/*** Konec souboru white_box_tests.cpp ***/
