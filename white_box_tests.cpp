//======== Copyright (c) 2017, FIT VUT Brno, All rights reserved. ============//
//
// Purpose:     White Box - Tests suite
//
// $NoKeywords: $ivs_project_1 $white_box_code.cpp
// $Author:     DAVID MIHOLA <xmihol00@stud.fit.vutbr.cz>
// $Date:       $2020-20-02
//============================================================================//
/**
 * @file white_box_tests.cpp
 * @author DAVID MIHOLA
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

//I am using numbers without truncation error in most of the calculations in order to get precise results

//for debugging purposes
#define NDEBUG
#ifndef NDEBUG
#define MAT_PRINT(MAT, R, C) matrix_print(MAT, R, C);
void matrix_print(Matrix matrix, unsigned r, unsigned c)
{
    for (unsigned i = 0; i<r; i++)
    {
        for (unsigned j = 0; j<c; j++)
            printf("%f ", matrix.get(i ,j));
        putchar('\n');
    }
}
#else 
#define MAT_PRINT(MAT, R, C)
#endif

Matrix create_init_matrix(std::vector<std::vector<double>> vect)
{
    Matrix mat(vect.size(), vect[0].size());
    mat.set(vect);
    return mat;
}

class EmptyMatrix: public ::testing::Test
{
protected:
    Matrix matrix;
};

class NonEmptyMatrixMoreCols: public ::testing::Test
{
protected:
    Matrix matrix;
    virtual void SetUp()
    {
        matrix = {2, 3};
        matrix.set({{1.5, 3.0, 8.25}, {6.75, 2.0, 4.125}});
    }
};

class NonEmptyMatrixMoreRows: public ::testing::Test
{
protected:
    Matrix matrix;
    virtual void SetUp()
    {
        matrix = {3, 2};
        matrix.set({{2.25, 1.125}, {3.625, 4.0}, {5.75, 7.0}});
    }
};

class NonEmptyMatrixSquare: public ::testing::Test
{
protected:
    Matrix matrix;
    virtual void SetUp()
    {
        matrix = {3, 3};
        matrix.set({{3, 1, -1}, {1, 2, -5}, {-3, 5, 9}});
    }
};

class Matrix_1x1: public::testing::Test
{
protected:
    Matrix matrix;

    virtual void SetUp()
    {
        matrix.set({{8.25}});
    }
};

class Matrix_2x2: public::testing::Test
{
protected:
    Matrix matrix;
    Matrix singular;

    virtual void SetUp()
    {
        matrix = {2, 2};
        matrix.set({{4.0, 2.0}, {5.0, 3.0}});

        singular = {2, 2};
        singular.set({{2.0, 3.0}, {4.0, 6.0}});
    }
};

class Matrix_3x3: public::testing::Test
{
protected:
    Matrix matrix;
    Matrix singular;

    virtual void SetUp()
    {
        matrix = {3, 3};
        matrix.set({{3, 1, -1}, {1, 2, -5}, {3, 5, 2}});

        //row 1 and 3 are the same
        singular = {3, 3};
        singular.set({{2.5, 4.2, 5.8}, {1.2, 3.4, 3.3}, {2.5, 4.2, 5.8}});
    }
};

class Matrix_4x4: public::testing::Test
{
protected:
    Matrix matrix;
    Matrix singular;

    virtual void SetUp()
    {
        matrix = {4, 4};
        matrix.set({{2.5, 2.5, -3.75, 1.25}, {1.5, 3, 6, 3}, {-2.5, 2.5, -2.5, 2.5}, {2.25, -2.25, 4.5, -4.5}});

        //colums 1 and 4 are the same
        singular = {4, 4};
        singular.set({{2.6, 5.3, 7.4, 2.6}, {1.7, 8.4, 3.1, 1.7}, {0.5, 2.2, 5.8, 0.5}, {9.3, 6.7, 2.2, 9.3}});
    }
};

class Matrix_7x7: public::testing::Test
{
protected:
    Matrix matrix;
    Matrix singular;

    virtual void SetUp()
    {
        matrix = {7, 7};
        matrix.set({{4.0625, 3.5625, -7.5, -1.875, 7.75, -6.4375, 5}, {4.6875, -6.5, 8.375, 9, -7.0625, 3.875, -2.4375},
                    {5.125, -6.875, -8.625, 4.375, 2, -3.25, 7.5625}, {9.25, -4.375, -7.4375, -5.75, 3.125, 2, 4.5},
                    {3.5, -2.75, -9.4375, -4.125, 7.875, 1.625, 5.375}, {0.5625, 7.375, -8.0625, -7.5, 4.25, 5, 1.875},
                    {9.125, -2.0625, -4.25, -5.625, -3.5, 7, 6.375}});

        //third row is a sum of first and second
        singular = {7, 7};
        singular.set({{4.0625, 3.5625, -7.5, -1.875, 7.75, -6.4375, 5}, {4.6875, -6.5, 8.375, 9, -7.0625, 3.875, -2.4375},
                    {8.75, -2.9375, 0.875, 7.125, 0.6875, -2.5625, 2.5625}, {9.25, -4.375, -7.4375, -5.75, 3.125, 2, 4.5},
                    {3.5, -2.75, -9.4375, -4.125, 7.875, 1.625, 5.375}, {0.5625, 7.375, -8.0625, -7.5, 4.25, 5, 1.875},
                    {9.125, -2.0625, -4.25, -5.625, -3.5, 7, 6.375}});
    }
};

class Matrix_10x10: public::testing::Test
{
protected:
    Matrix matrix;
    Matrix singular;

    virtual void SetUp()
    {
        matrix = {10, 10};
        matrix.set({{0, 2, -5, 4, -2.5, 0, 4, -6, -4, 8},
                     {6.5, 0, -3, 9, 5, 4, -2, -5, 1.5, 1},
                     {-3, -1.5, 0, 4, -4, 4, 7.75, 9, -3, 3},
                     {-5, 5.5, -9, 0, 4, 4.5, -8.5, 6, -2.5, 0},
                     {-5, 2., -6, 5, 0, -4, 1.5, -3.5, 3, 2},
                     {0, -5.5, 7, 8, -6, 0, -2, 0, 9.5, 6},
                     {2.5, 4, -5, 5.5, 3, 5, 0, -7, 7.5, -6},
                     {-2.5, 0, 7, 2, 1, -1, 5.5, 0, -6, 3.5},
                     {0, -1, 1, 3, 0.25, -5, 2.5, -8, 0, 3},
                     {2, -3, 5, -3, -5.5, 7, 4, 5.5, -1, 0}});

        singular = {10, 10};
        //fifth column is 0
        singular.set({{4.25, 2.5, -5.125, 6, 0, 3.25, 4.75, -6.5, -9.625, 8},
                     {6.25, -8.5, -7.125, 9, 0, 4.75, -2.5, -5, 1.25, 7.375},
                     {-3.75, -1.5, 7.625, 4.5, 0, 4.5, 7.375, 9.25, -3.875, 9.875},
                     {-5.75, 5.125, -9.75, 3.75, 0, 4.125, -8.25, 6.625, -2.625, 5.125},
                     {-0.5, 8.5, -6.25, 0.625, 0, -4.75, 7.625, -3.625, 3.5, 9.125},
                     {4.875, -5.5, 7.25, 8.25, 0, 4.375, -2.625, 0.375, 8.25, 6.375},
                     {2.875, 7.875, -9.25, 5.5, 0, 5.750, 2.75, -7.25, 7.250, -6.5},
                     {-2.875, 8.75, 7.75, 0.125, 0, -1.75, 5.375, -7.625, -6.75, 7.750},
                     {9.375, -1.375, 1.125, 3.750, 0, -5.25, 2.625, -8.625, 8.875, 3.5},
                     {8.125, -3.625, 5.875, -3.625, 0, 7.125, 9.875, 5.375, -1.25, 5.125}});
    }
};

class MatrixDeterminantInterval_neg1to1: public::testing::Test
{
protected:
    Matrix matrix;

    virtual void SetUp()
    {
        matrix = {5, 5};
        matrix.set({{0.25, 1, -2, 0.5, 0.375}, {-0.625, 2, 0.25, 0.75, -1}, {0.75,0.625, 0, 1, 2},
	                {1, -2, 0.875, 0.125, 0.5}, {0.625, -1.25, 0.25, 0.125, 0}});
    }
};

class MatrixDeterminant_GT_IntMax: public::testing::Test
{
protected:
    Matrix matrix;

    virtual void SetUp()
    {
        matrix = {6, 6};
        matrix.set({{25, 10, 52, 87, 63, 13}, {6, 8, 7, 23, 51, 45}, {75, 35, 2, 87, 14, 62}, {5, 6, 9, 81, 21, 34},
                    {36, 24, 15, 14, 13, 56},{21, 24, 7, 8, 3, 27}});
    }
};

//In most cases, I am using this implementation for testing thrown exceptions \
instead of EXPECT/ASSERT_NO_THROWW/_ANY_THROW in order to be able to check, if the thrown exception was correct.
TEST_F(EmptyMatrix, GetSet)
{
    EXPECT_EQ(matrix.get(0, 0), 0.0);

    try 
    {
        matrix.get(1, 1);
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch (const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Pristup k indexu mimo matici");
    }

    EXPECT_EQ(matrix.get(0, 0), 0.0);

    EXPECT_FALSE(matrix.set(1, 1, 1.5));
    EXPECT_FALSE(matrix.set({{1.0, 2.0}, {3.0, 4.0}}));
    EXPECT_FALSE(matrix.set({{2.3, 2.4}}));
    EXPECT_FALSE(matrix.set({{2.3}, {2.4}}));

    EXPECT_TRUE(matrix.set(0, 0, 1.0));
    EXPECT_EQ(matrix.get(0, 0), 1.0);

    try 
    {
        matrix.get(1, 1);
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch (const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Pristup k indexu mimo matici");
    }

    EXPECT_TRUE(matrix.set({{2.0}}));
    EXPECT_EQ(matrix.get(0, 0), 2.0);

    try
    {
        matrix = {0, 0};       
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }    
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Minimalni velikost matice je 1x1");
    }
}

TEST_F(EmptyMatrix, Comparison)
{
    //3 possibilites of a noncomparable matrices
    //larger matrix
    try
    {
        matrix == (create_init_matrix({{2, 3}, {4.5, 5.5}}));
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi mit stejnou velikost.");
    }
    //matrix with too many columns
    try
    {
        matrix == (create_init_matrix({{4.5, 5.5}}));
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi mit stejnou velikost.");
    }
    //matrix with too many rows
    try
    {
        matrix == (create_init_matrix({{2}, {4.5}}));
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi mit stejnou velikost.");
    }

    EXPECT_FALSE(matrix == (create_init_matrix({{2.0}})));
}

TEST_F(EmptyMatrix, Addition)
{
    matrix.operator+(create_init_matrix({{5.2}}));
    EXPECT_EQ(matrix.get(0, 0), 0.0);

    EXPECT_EQ(matrix.operator+(create_init_matrix({{6.25}})), create_init_matrix({{6.25}}));

    try
    {
        matrix.operator+(create_init_matrix({{2, 1.5}, {4.2, 3.2}}));
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi mit stejnou velikost.");
    }
}

TEST_F(EmptyMatrix, Multiplication)
{
    try
    {
        matrix.operator*({2, 1});
        ASSERT_FALSE(true); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Prvni matice musi stejny pocet sloupcu jako druha radku.");
    }

    try
    {
        matrix.operator*({2, 2});
        ASSERT_FALSE(true); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Prvni matice musi stejny pocet sloupcu jako druha radku.");
    }

    //with square matrix
    EXPECT_EQ(matrix.operator*({1, 1}), create_init_matrix({{0}}));    
    EXPECT_EQ(matrix.operator*(create_init_matrix({{12.4}})), create_init_matrix({{0}}));

    //with non square matrix
    EXPECT_EQ(matrix.operator*({1, 3}), create_init_matrix({{0, 0, 0}}));    
    EXPECT_EQ(matrix.operator*(create_init_matrix({{12.4, 13.2, 2.3}})), create_init_matrix({{0, 0, 0}}));

    EXPECT_EQ(matrix.operator*(6.8), create_init_matrix({{0.0}}));    
}

TEST_F(EmptyMatrix, Equation)
{
    try
    {
        matrix.solveEquation({1.2});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    } 
    catch(const std::runtime_error *err)
    {
        
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }

    try
    {
        matrix.solveEquation({1.2, 3.4});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        
        EXPECT_EQ((std::string)err->what(), "Pocet prvku prave strany rovnice musi odpovidat poctu radku matice.");
    }
}

TEST_F(NonEmptyMatrixMoreCols, Comparison)
{
    EXPECT_FALSE(matrix.operator==(create_init_matrix({{1.5, 3.0, 8.25}, {6.7, 2.1, 4.125}})));
    EXPECT_TRUE(matrix.operator==(create_init_matrix({{1.5, 3.0, 8.25}, {6.75, 2.0, 4.125}})));
}

TEST_F(NonEmptyMatrixMoreCols, Adittion)
{
    EXPECT_EQ(matrix.operator+({2, 3}), matrix);
    EXPECT_EQ(matrix.operator+(create_init_matrix({{0.5, 1.4, 1.5}, {0.25, 1.25, 0.005}})), 
                               create_init_matrix({{2.0, 4.4, 9.75}, {7.0, 3.25, 4.130}}));
}

TEST_F(NonEmptyMatrixMoreCols, Multiplication)
{
    EXPECT_EQ(matrix.operator*({3, 2}), create_init_matrix({{0, 0}, {0, 0}}));    
    EXPECT_EQ(matrix.operator*(create_init_matrix({{2.0}, {4.0}, {6.0}})), create_init_matrix({{64.5}, {46.25}}));

    EXPECT_EQ(matrix.operator*(4.5), create_init_matrix({{1.5*4.5, 3.0*4.5, 8.25*4.5}, {6.75*4.5, 2.0*4.5, 4.125*4.5}}));  
}

TEST_F(NonEmptyMatrixMoreCols, Equation_WrongExceptionsWereThrown)
{
    try
    {
        matrix.solveEquation({5.0, 4.0, 2.0});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Pocet prvku prave strany rovnice musi odpovidat poctu radku matice.");
    }

    //the program will behave correctly, but the error message will be wrong

    try
    {
        matrix.solveEquation({4.0, 3.0});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi byt ctvercova.");
    }
}

TEST_F(NonEmptyMatrixMoreRows, Comparison)
{
    EXPECT_FALSE(matrix.operator==(create_init_matrix({{2.25, 1.125}, {3.625, 4.25}, {5.75, 7.0}})));
    EXPECT_TRUE(matrix.operator==(create_init_matrix({{2.25, 1.125}, {3.625, 4.0}, {5.75, 7.0}})));
}

TEST_F(NonEmptyMatrixMoreRows, Adittion)
{
    EXPECT_EQ(matrix.operator+({3, 2}), matrix);
    EXPECT_EQ(matrix.operator+(create_init_matrix({{0.75, 0.875}, {1.0, 1.25}, {2.125, 3.0}})), 
                               create_init_matrix({{3.0, 2.0}, {4.625, 5.25}, {7.875, 10.0}}));
}

TEST_F(NonEmptyMatrixMoreRows, Multiplication)
{
    EXPECT_EQ(matrix.operator*({2, 3}), create_init_matrix({{0, 0, 0}, {0, 0, 0}, {0, 0, 0}}));

    EXPECT_EQ(matrix * (create_init_matrix({{1.0, 4.0}, {2.0, 6.0}})), 
                              create_init_matrix({{4.5, 15.75}, {11.625, 38.5}, {19.75, 65}}));

    EXPECT_EQ(matrix * 3.25, create_init_matrix({{2.25*3.25, 1.125*3.25}, {3.625*3.25, 4.0*3.25}, {5.75*3.25, 7.0*3.25}}));
}

TEST_F(NonEmptyMatrixMoreRows, Equation_WrongExceptionsWereThrown)
{
    try
    {
        matrix.solveEquation({5.0, 4.0});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Pocet prvku prave strany rovnice musi odpovidat poctu radku matice.");
    }

    //the program will behave correctly, but the error message will be wrong

    try
    {
        matrix.solveEquation({4.0, 3.0, 2.3});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi byt ctvercova.");
    }
}

TEST_F(NonEmptyMatrixSquare, Comparison)
{
    EXPECT_FALSE(matrix == (create_init_matrix({{3, 1, -1}, {1, 2, -5}, {-3, 5, 9.2}})));
    EXPECT_TRUE(matrix == (create_init_matrix({{3, 1, -1}, {1, 2, -5}, {-3, 5, 9}})));
}

TEST_F(NonEmptyMatrixSquare, Addition)
{
    EXPECT_EQ(matrix.operator+({3, 3}), matrix);

    EXPECT_EQ(matrix + (create_init_matrix({{0.75, 0.125, 0.875}, {0.625, 1.0, 1.25}, {0.5, 2.125, 3.0}})), 
                               create_init_matrix({{3.75, 1.125, -0.125}, {1.625, 3.0, -3.75}, {-2.5, 7.125, 12.0}}));

    try
    {
        matrix + (create_init_matrix({{6, 5}, {4.2, 42}}));
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice musi mit stejnou velikost.");
    } 
}

TEST_F(NonEmptyMatrixSquare, Multiplication)
{
    EXPECT_EQ(matrix.operator*({3, 1}), create_init_matrix({{0}, {0}, {0}}));

    EXPECT_EQ(matrix * (create_init_matrix({{-2, 4, 3}, {6, -2, -4}, {2, -1 , 1}})), 
                              create_init_matrix({{-2, 11, 4}, {0, 5, -10}, {54, -31, -20}}));

    EXPECT_EQ(matrix * 2.625, 
              create_init_matrix({{3*2.625, 1*2.625, -1*2.625}, {1*2.625, 2*2.625, -5*2.625}, {-3*2.625, 5*2.625, 9*2.625}}));
}

TEST_F(Matrix_1x1, Equation)
{
    EXPECT_EQ(matrix.solveEquation({16.5}), (std::vector<double>){2});
}

TEST_F(Matrix_2x2, Equation)
{
    EXPECT_EQ(matrix.solveEquation({3, 6.5}), ((std::vector<double>){-2.0, 5.5}));

    try
    {
        singular.solveEquation({2.25, 3.5});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }
}

TEST_F(Matrix_3x3, Equation)
{
    EXPECT_EQ(matrix.solveEquation({3.5, 7.5, 4.5}), ((std::vector<double>){0.5, 1, -1}));

    try
    {
        singular.solveEquation({5, 4, 3});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }
}

TEST_F(Matrix_4x4, Equation)
{
    EXPECT_EQ(matrix.solveEquation({3.75, 7.5, 2.5, -9}), ((std::vector<double>){1, -1, 0, 3}));

    try
    {
        singular.solveEquation({-9.4, 2.3, -3.8, 8.8});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }
}

TEST_F(Matrix_7x7, Equation)
{
    EXPECT_EQ(matrix.solveEquation({5.6328125, 67.0859375, 21.0390625, -17.796875, 9.75, 8.359375, 19.8125}), 
                                   ((std::vector<double>){2.25, 4.5, 3.75, 8.625, 5, 7.125, 6.375}));

    try
    {
        singular.solveEquation({5.3, 4.2, 8, 5.125, 7.54, 8.1, 8.3});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }
}

TEST_F(Matrix_10x10, Equation)
{
    EXPECT_EQ(matrix.solveEquation({-48.875, 102.125, -98.25, -144.25, 37.75, 70.875, 67.125, 30.5625, 109.0625, -111.875}), 
                            ((std::vector<double>{2.25, -7.5, 4.5, 7.25, 9.75, -8.75, 1.5, -5, 3.25, -4.875})));

    try
    {
        singular.solveEquation({5.3, 4.2, 8, 5.125, 7.54, 8.1, 8.3, 4, 5.2, 5.3});
        ASSERT_TRUE(false); //for the case when exception wasn't thrown
    }
    catch(const std::runtime_error *err)
    {
        EXPECT_EQ((std::string)err->what(), "Matice je singularni.");
    }
}

TEST_F(MatrixDeterminantInterval_neg1to1, DeterminantIsFromInterval_Negative1to1)
{
    EXPECT_NO_THROW(matrix.solveEquation({0.2, 0.3, 0.4, 0.5, 0.6}));

    //determianant == 0.260284423828125 (as function determinanat() calculated), 
    //not 0.0 (caused by conversion double to int in function abs())
}

TEST_F(MatrixDeterminant_GT_IntMax, DeterminantIsGreaterThanIntMax)
{
    EXPECT_NO_THROW(matrix.solveEquation({8.5, 0.2, 2.6, 8.4, 1.5, 0.6}));

    //"determianant == -5873570494 (as function determinanat() calculated), 
    //abs(determinant) == -2147483648 (caused by int overflow in function abs())

}

/*** Konec souboru white_box_tests.cpp ***/