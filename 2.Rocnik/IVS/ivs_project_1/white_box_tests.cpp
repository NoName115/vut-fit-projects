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

class SquareMatrix : public ::testing::Test
{
protected:
    virtual void SetUp()
    {
        int values[] = { 5, 15, 20, 13 };

        for (int i = 0; i < 4; ++i)
        {
        	matrix->set(0, i, values[i]);
        	matrix->set(1, i, values[i]);
        	matrix->set(2, i, values[i]);
        	matrix->set(3, i, values[i]);
        }
    }

    Matrix *matrix = new Matrix(4, 4);
};

class EmptySquareMatrix : public ::testing::Test
{
protected:
	Matrix *matrix = new Matrix(4, 4);
};

class MatrixForEquation : public ::testing::Test
{
protected:
	virtual void SetUp()
	{
		// x + 0y + z
		matrix->set(0, 0, 1);
		matrix->set(0, 2, 1);

		// 0x - 3y + z
		matrix->set(1, 1, -3);
		matrix->set(1, 2, 1);

		// 2x + y + 3z
		matrix->set(2, 0, 2);
		matrix->set(2, 1, 1);
		matrix->set(2, 2, 3);
	}

	Matrix *matrix = new Matrix(3, 3);
};

TEST_F(SquareMatrix, InitializeZeroMatrix)
{
	EXPECT_ANY_THROW(new Matrix(0, 1));
}

TEST_F(EmptySquareMatrix, SetValue)
{
	EXPECT_FALSE(matrix->set(5, 3, 5));
	EXPECT_FALSE(matrix->set(3, 5, 5));
	EXPECT_TRUE(matrix->set(2, 2, 2));
}

TEST_F(EmptySquareMatrix, SetVectorValues)
{
	std::vector<double> row = { 5, 10, 15, 20 };
	std::vector<std::vector< double > > wholeVector = { row, row, row, row };
	EXPECT_TRUE(matrix->set(wholeVector));

	wholeVector = { row, row, row };
	EXPECT_FALSE(matrix->set(wholeVector));
}

TEST_F(SquareMatrix, GetValue)
{
	EXPECT_TRUE(matrix->get(0, 0) == 5);
	matrix->get(5, 5);
}

TEST_F(SquareMatrix, operatorEqual)
{
	Matrix *newMatrix = new Matrix(1, 1);
	newMatrix->set(1, 1, 5);
	
	EXPECT_ANY_THROW(matrix->operator==((*newMatrix)));

	newMatrix = new Matrix(4, 4);
	for (int i = 0; i < 4; ++i)
	{
		newMatrix->set(0, i, 5);
		newMatrix->set(1, i, 10);
		newMatrix->set(2, i, 15);
		newMatrix->set(3, i, 20);
	}

	EXPECT_FALSE(matrix->operator==((*newMatrix)));
	EXPECT_TRUE(matrix->operator==((*matrix)));
}

TEST_F(SquareMatrix, operatorPlus)
{
	Matrix *newMatrix = new Matrix(2, 5);
	EXPECT_ANY_THROW(matrix->operator+((*newMatrix)));

	newMatrix = new Matrix(4, 4);
	for (int i = 0; i < 4; ++i)
	{
		newMatrix->set(0, i, 5);
		newMatrix->set(1, i, 10);
		newMatrix->set(2, i, 15);
		newMatrix->set(3, i, 20);
	}

	Matrix resultMatrix = matrix->operator+((*newMatrix));
	EXPECT_TRUE(resultMatrix.get(0, 0) == 10);
	EXPECT_TRUE(resultMatrix.get(1, 1) == 25);
	EXPECT_TRUE(resultMatrix.get(2, 2) == 35);
	EXPECT_TRUE(resultMatrix.get(3, 3) == 33);
}

TEST_F(SquareMatrix, operatorMultiplication)
{
	Matrix *newMatrix = new Matrix(2, 5);
	EXPECT_ANY_THROW(matrix->operator*((*newMatrix)));

	newMatrix = new Matrix(4, 4);
	for (int i = 0; i < 4; ++i)
	{
		newMatrix->set(0, i, 2);
		newMatrix->set(1, i, 8);
		newMatrix->set(2, i, 13);
		newMatrix->set(3, i, 27);
	}

	Matrix resultMatrix = matrix->operator*(5);
	EXPECT_TRUE(resultMatrix.get(0, 0) == 25);
	EXPECT_TRUE(resultMatrix.get(1, 1) == 75);
	EXPECT_TRUE(resultMatrix.get(2, 2) == 100);
	EXPECT_TRUE(resultMatrix.get(3, 3) == 65);

	resultMatrix = matrix->operator*((*newMatrix));

	for (int i = 0; i < 4; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			EXPECT_TRUE(resultMatrix.get(i, j) == 741);
		}
	}
}

TEST_F(MatrixForEquation, solveEquation)
{
	std::vector<double> b = { 6, 7 };
	// Zly pocet riadkov
	EXPECT_ANY_THROW(matrix->solveEquation(b));

	// Matica nieje stvorcova
	EXPECT_ANY_THROW((new Matrix(4, 2))->solveEquation(b));

	b.push_back(15);
	Matrix *singularMatrix = new Matrix(3, 3);

	for (int i = 0; i < 3; ++i)
	{
		singularMatrix->set(i, 0, 0);
		singularMatrix->set(i, 1, 1);
		singularMatrix->set(i, 2, 1);
	}
	EXPECT_ANY_THROW(singularMatrix->solveEquation(b));

	std::vector<double> result = matrix->solveEquation(b);
	EXPECT_TRUE(result.at(0) == 2);
	EXPECT_TRUE(result.at(1) == -1);
	EXPECT_TRUE(result.at(2) == 4);

	// Size == 1
	Matrix *sizeOneMatrix = new Matrix();
	sizeOneMatrix->set(0, 0, 1);
	std::vector<double> one = { 2 };
	sizeOneMatrix->solveEquation(one);

	// Size == 2
	Matrix *sizeTwoMatrix = new Matrix(2, 2);
	sizeTwoMatrix->set(0, 0, 1);
	sizeTwoMatrix->set(0, 1, 1);
	sizeTwoMatrix->set(1, 0, 1);
	sizeTwoMatrix->set(1, 1, -1);
	std::vector<double> two = { 2, 0 };
	sizeTwoMatrix->solveEquation(two);
}

/*** Konec souboru white_box_tests.cpp ***/
