/******************************************************************************
 * matrix.h  header file for matrix
 *
 * version   1.2
 * History:  Ver 1.0 Created 2001 Aug  by  C.T. Jitin Yang
 *           Ver 1.1 Modified 2003 Feb by  C.T. Jitin Yang
 *           Ver 1.2 Modifined 2003 July by C.T. Jitin Yang
 *
 *           For CAE summer traning course
 *           (C++ Programming Langurage)
 ******************************************************************************/

#ifndef _NJR_MATRIX_H
#define _NJR_MATRIX_H

#include <iostream>
#include <cstdlib>
#include <string>

namespace NJR
{

// Matrix is used to define cubic(NxN) or irregular (NxM) matrix.
class Matrix
{

public:

	Matrix();

	/**************************************************************************
	 * constructor is used to define the rows and columns of this matrix and
	 * allocate the memory space of the matrix
	 **************************************************************************/
	Matrix(const unsigned int& rows, const unsigned int& columns);

	//copy constructor
	Matrix(const Matrix& matrix);

	//destructor which used to free the memorey space of matrix
	virtual ~Matrix();

	/**************************************************************************
	 * reset the rows and columns of this matrix and allocate the memory space
	 * of the matrix
	 **************************************************************************/
	void Resize(const unsigned int& rows, const unsigned int& columns);

	// Retrieves the number of rows  of this matrix
	inline unsigned int rows() const
	{
		return _rows;
	};

	// Retrieves the number of columns of this matrix
	inline unsigned int columns() const
	{
		return _cols;
	};

	// Assignment
	const Matrix& operator = (const Matrix& matrix);

	/**************************************************************************
	 * Assignment
	 * 1. Sets all elements to be zero when the input argument is 0;
	 * 2. Sets all elements to be zero and matrix[i][j] equal the input
	 *    argument when (i == j)
	 **************************************************************************/
	const Matrix& operator = (const double& v);

	/**************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols )
	 * times B ( n1 rows , m1 cols); But, the operator will do noting and
	 * terminate the program when (m0 != n1)
	 **************************************************************************/
	Matrix operator * (const Matrix& matrix) const;

	/**************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols )
	 * adds B ( n1 rows , m1 cols); But, the operator will do noting and
	 * terminate the program when (n0 != n1) or ( m0 != m1)
	 **************************************************************************/
	Matrix operator + (const Matrix& matrix) const;

	/**************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols )
	 * reduces B ( n1 rows , m1 cols); But, the operator will do noting and
	 * terminate the program when (n0 != n1) or ( m0 != m1)
	 **************************************************************************/
	Matrix operator - (const Matrix& matrix) const;

	/**************************************************************************
	 * Select columns from (this matrix)
	 * input value:
	 *    selected: For Recodes what columns you want;
	 *    len     : Length of selected
	 *
	 * For example: selected [] = (1,2,5,6), len = 4
	 *
	 * the member function will return a new matrix which has just 4 columns
	 * (1,2,5,6) of (this matrix);
	 *
	 * Note: if (len > cols ) or (the element in selected > cols), the matrix
	 *       will do nothing and terminate the program.
	 **************************************************************************/
	Matrix Select(unsigned int* selected, unsigned int len) const;

	/**************************************************************************
	 * Retrieves the type of the matrix,
	 * (0) 'VALUE'  when cols equals 1 and  rows equals 1;
	 * (1) 'VECTOR' cols or rows of the (matrix == 1) and (cols != rows)
	 * (2) 'CUBIC'  when (cols == rows) and (rows != 1)
	 * (3) 'IRREGULAR' all situation except for the above;
	 **************************************************************************/
   	const char* characteristic() const;

	// Transposes this matrix
	const Matrix& Transpose();

	// Retrieves element (r,c);
	double Get(const unsigned int& r, const unsigned int& c) const;

	// Retrieves references of element (r,c)
	double & operator() (const unsigned int& r, const unsigned int& c);

private:

	// _rows : the number of rows  of this matrix
	unsigned int _rows;

	// _cols: the number of columns of this matrix
	unsigned int _cols;

	/**************************************************************************
	 * matrix and matval : these two pointers are used to indicate all the
	 * elements of the matrix
	 **************************************************************************/

	double** matrix;

	double*  matval;

};

};   // namespace NJR



// output operator
std::ostream& operator << (std::ostream& os, const NJR::Matrix& matrix);

#endif // _NJR_MATRIX_H
