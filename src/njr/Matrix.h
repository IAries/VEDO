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

#include <njr/DataType.h>
#include <iostream>
#include <cstdlib>
#include <string>

namespace njr
{

// Matrix is used to define cubic(NxN) or irregular (NxM) matrix.
class Matrix
{

public:

	Matrix();

	// Constructor is used to define the rows and columns of this matrix and allocate the memory space of the matrix
	Matrix(const _uint_t& rows, const _uint_t& columns);

	Matrix(const Matrix& matrix);

	virtual ~Matrix();

	// Reset the rows and columns of this matrix and allocate the memory space of the matrix
	void Resize(const _uint_t& rows, const _uint_t& columns);

	// Retrieves the number of rows  of this matrix
	inline _uint_t rows() const
	{
		return _rows;
	}

	// Retrieves the number of columns of this matrix
	inline _uint_t columns() const
	{
		return _cols;
	}

	// Assignment
	const Matrix& operator = (const Matrix& matrix);

	/******************************************************************************************
	 * Assignment
	 * 1. Sets all elements to be zero when the input argument is 0;
	 * 2. Sets all elements to be zero and matrix[i][j] equal the input argument when (i == j)
	 ******************************************************************************************/
	const Matrix& operator = (const _float_t& v);

	/**************************************************************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols) times B ( n1 rows , m1 cols); But, the operator
	 * will do noting and terminate the program when (m0 != n1)
	 **************************************************************************************************************************/
	Matrix operator * (const Matrix& matrix) const;

	/**************************************************************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols) adds B (n1 rows , m1 cols); But, the operator
	 * will do noting and terminate the program when (n0 != n1) or ( m0 != m1)
	 **************************************************************************************************************************/
	Matrix operator + (const Matrix& matrix) const;

	/**************************************************************************************************************************
	 * Retrieves the matrix that is the result of matrix A (n0 rows , m0 cols) reduces B ( n1 rows , m1 cols); But, the
	 * operator will do noting and terminate the program when (n0 != n1) or ( m0 != m1)
	 **************************************************************************************************************************/
	Matrix operator - (const Matrix& matrix) const;

	/********************************************************************************************************************
	 * Select columns from (this matrix)
	 * input value:
	 *    selected: For Recodes what columns you want;
	 *    len     : Length of selected
	 *
	 * For example: selected [] = (1,2,5,6), len = 4
	 *
	 * the member function will return a new matrix which has just 4 columns (1,2,5,6) of (this matrix);
	 *
	 * Note: if (len > cols ) or (the element in selected > cols), the matrix will do nothing and terminate the program.
	 ********************************************************************************************************************/
	Matrix Select(_uint_t* selected, _uint_t len) const;

	/********************************************************************
	 * Retrieves the type of the matrix,
	 * (0) 'VALUE'  when cols equals 1 and  rows equals 1;
	 * (1) 'VECTOR' cols or rows of the (matrix == 1) and (cols != rows)
	 * (2) 'CUBIC'  when (cols == rows) and (rows != 1)
	 * (3) 'IRREGULAR' all situation except for the above;
	 ********************************************************************/
   	const char* characteristic() const;

	// Transposes this matrix
	const Matrix& Transpose();

	// Retrieves element (r,c);
	_float_t Get(const _uint_t& r, const _uint_t& c) const;

	// Retrieves references of element (r,c)
	_float_t & operator() (const _uint_t& r, const _uint_t& c);

private:

	// _rows : the number of rows  of this matrix
	_uint_t _rows;

	// _cols: the number of columns of this matrix
	_uint_t _cols;

	// Matrix and matval : these two pointers are used to indicate all the elements of the matrix

	_float_t** matrix;

	_float_t*  matval;

};

}   // namespace njr



// output operator
std::ostream& operator << (std::ostream& os, const njr::Matrix& matrix);

#endif // _NJR_MATRIX_H
